//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 14:46:36.
// File name: playing_message_handler.cc
//
// Description:
// Define PlayingMessageHandler.
//

#include "game_server/server/extension/playing/playing_message_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/error_code.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/extension/playing/playing.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/playing_page.h"
#include "game_server/server/extension/playing/playing_manager.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "global/configure/playing_configure.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/protocol/session_protocol_types.h"

namespace game {

namespace server {

namespace playing {

PlayingMessageHandler::PlayingMessageHandler() {}
PlayingMessageHandler::~PlayingMessageHandler() {}

bool PlayingMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_CREATE_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingCreateRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_COMPLETE_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingCompleteRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_LEAVE_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingLeaveRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_AWARD_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingAwardRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_AUTO_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingAutoRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_AUTO_FINISH_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingAutoFinishRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_PAID_AWARD_REQUEST,
      boost::bind(&PlayingMessageHandler::OnMessagePlayingPaidAwardRequest,
        this, _1, _2, _3));

  return true;
}

void PlayingMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_CREATE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_COMPLETE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_LEAVE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_AWARD_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_AUTO_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_AUTO_FINISH_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_PLAYING_PAID_AWARD_REQUEST);
}

void PlayingMessageHandler::OnMessagePlayingCreateRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("游戏玩家(%lu)没有找到", id);
    return ;
  }

  gateway::protocol::MessagePlayingCreateRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_CREATE_REQUEST from actor(%lu, %s), template_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), request.template_);

  if (actor->CheckFunctionalityOpen(request.template_) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  if(actor->GetPlaying() != 0) {
    LOG_ERROR("玩家 playingActor(%lu) 还处于副本中", id);
    return ;
  }

  // 判断金钱、次数等
  if(actor->CheckCreatePlaying(request.template_) == true) {
    // 创建副本请求到会话服务器
    session::protocol::MessagePlayingCreateRequest session_request;
    session_request.__set_actor_(id);
    session_request.__set_template_(request.template_);
    SessionChannel::SendMessage(session_request,
        session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_REQUEST);
  } else {
    gateway::protocol::MessagePlayingCreateResponse response;
    response.__set_result_(false);
    response.__set_template_(request.template_);
    actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_PLAYING_CREATE_RESPONSE);
  }
}

void PlayingMessageHandler::OnMessagePlayingCompleteRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("游戏玩家(%lu)没有找到", id);
    return ;
  }

  gateway::protocol::MessagePlayingCompleteRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_COMPLETE_REQUEST from actor(%lu, %s)",
      game_actor->GetActorID(), game_actor->GetName().c_str());

  if(actor->GetPlaying() == 0) {
    LOG_ERROR("玩家 PlayingActor(%lu) 处于非副本状态", id);
    return ;
  }

  Playing *playing = PlayingManager::GetInstance()->Get(actor->GetPlaying());
  if(playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", actor->GetPlaying());
    return ;
  }

  const global::configure::PlayingCell *playing_cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        playing->GetTemplateID());
  if(playing_cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", playing->GetTemplateID());
    return ;
  }

  if (actor->CheckFunctionalityOpen(playing_cell->camp_type_) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  // 发送副本玩家请求完成事件
  event::EventPlayingActorRequestComplete event;
  event.__set_actor_(id);
  event.__set_playing_(playing->GetID());
  event.__set_template_(playing->GetTemplateID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_PLAYING_ACTOR_REQUEST_COMPLETE, id,
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

  // 回应
  gateway::protocol::MessagePlayingCompleteResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_COMPLETE_RESPONSE);

  LOG_DEBUG("收到 MessagePlayingCompleteRequest");
}

void PlayingMessageHandler::OnMessagePlayingLeaveRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  gateway::protocol::MessagePlayingLeaveRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_LEAVE_REQUEST from actor(%lu, %s)",
      game_actor->GetActorID(), game_actor->GetName().c_str());

  gateway::protocol::MessagePlayingLeaveResponse response;
  response.__set_result_(true);

  if(actor->GetPlaying() == 0) {
    actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_PLAYING_LEAVE_RESPONSE);
    LOG_ERROR("玩家 PlayingActor(%lu) 不在副本状态中");
    return;
  }

  Playing *playing = PlayingManager::GetInstance()->Get(actor->GetPlaying());
  if(playing == NULL) {
    actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_PLAYING_LEAVE_RESPONSE);
    LOG_ERROR("获取 Playing(%lu) 失败", actor->GetPlaying());
    return;
  }

  const global::configure::PlayingCell *playing_cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        playing->GetTemplateID());
  if(playing_cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", playing->GetTemplateID());
    return;
  }

  if (actor->CheckFunctionalityOpen(playing_cell->camp_type_) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  request::RequestSceneJump request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(game_actor->GetID());
  if(game_actor->GetPreMap() != 0) {
    request.__set_map_(game_actor->GetPreMap());
    request.__set_x_(game_actor->GetPrePosx());
    request.__set_y_(game_actor->GetPrePosy());
  } else {
    // TODO: 由于没有阵营系统，这里取的都是第0个
    playing_cell->GetExitPosition(0, request.map_, request.x_, request.y_);
  }
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_JUMP,
      &request, sizeof(request)) != 0) {
    LOG_ERROR("请求跳转地图(%lu) 失败", request.map_);
    return ;
  }

  LOG_DEBUG("收到 MessagePlayingLeaveRequest");

  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_LEAVE_RESPONSE);
}

void PlayingMessageHandler::OnMessagePlayingAwardRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  gateway::protocol::MessagePlayingAwardRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_AWARD_REQUEST from actor(%lu, %s), playing_template_(%d), playing_group_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), request.playing_template_, request.playing_group_);

  gateway::protocol::MessagePlayingAwardResponse response;
  response.__set_result_(true);

  if (request.__isset.playing_template_ == true && request.playing_template_ > 0) {
    if (actor->CheckFunctionalityOpen(request.playing_template_) == false) {
      LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
      return;
    }
    if (actor->AwardPlaying(request.playing_template_) == false) {
      response.__set_result_(false);
    }
  } else if (request.__isset.playing_group_ == true && request.playing_group_ > 0) {
    if (actor->AwardPlayingGroup(request.playing_group_) == false) {
      response.__set_result_(false);
    }
  } else {
    response.__set_result_(false);
  }

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_AWARD_RESPONSE);

  LOG_DEBUG("收到 MessagePlayingAwardRequest");
}

void PlayingMessageHandler::OnMessagePlayingAutoRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  gateway::protocol::MessagePlayingAutoRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_AUTO_REQUEST from actor(%lu, %s), playing_template_(%d), playing_group_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), request.playing_template_, request.playing_group_);

  gateway::protocol::MessagePlayingAutoResponse response;

  if (request.__isset.playing_template_ == true && request.playing_template_ > 0) {
    if (actor->CheckFunctionalityOpen(request.playing_template_) == false) {
      LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
      return;
    }
    PlayingPage *page = actor->GetPage(request.playing_template_);
    if (page == NULL) {
      LOG_ERROR("获取 玩家(%lu) PlayingPage(%d) 失败", id, request.playing_template_);
      return;
    }
    response.__set_result_(actor->StartAutoPlaying(request.playing_template_));
    response.__set_playing_template_(request.playing_template_);
  } else if (request.__isset.playing_group_ == true && request.playing_group_ > 0) {
    const global::configure::VipCell *vip_cell =
      VIP_CONF()->GetCell((entity::VipType::type)game_actor->GetAttribute(entity::ActorAoiFields::VIP),
          entity::VipPrivilegeType::PLAYING_GROUP_AUTO, request.playing_group_);
    if (vip_cell == NULL || vip_cell->value2_ == 0) {
      LOG_ERROR("玩家(%lu) 没有开通VIP一键自动出征");
      return;
    }
    response.__set_result_(actor->StartAutoPlayingGroup(request.playing_group_));
    response.__set_playing_group_(request.playing_group_);
  } else {
    LOG_WARNING("请求消息错误");
    return;
  }

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_AUTO_RESPONSE);
}

void PlayingMessageHandler::OnMessagePlayingAutoFinishRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  gateway::protocol::MessagePlayingAutoFinishRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_AUTO_FINISH_REQUEST from actor(%lu, %s), playing_template_(%d), playing_group_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), request.playing_template_, request.playing_group_);

  gateway::protocol::MessagePlayingAutoResponse response;

  if (request.__isset.playing_template_ == true && request.playing_template_ > 0) {
    if (actor->CheckFunctionalityOpen(request.playing_template_) == false) {
      LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
      return;
    }
    PlayingPage *page = actor->GetPage(request.playing_template_);
    if (page == NULL) {
      LOG_ERROR("获取 玩家(%lu) PlayingPage(%d) 失败", id, request.playing_template_);
      return;
    }
    response.__set_result_(actor->FinishAutoPlaying(request.playing_template_));
    response.__set_playing_template_(request.playing_template_);
  } else if (request.__isset.playing_group_ == true && request.playing_group_ > 0) {
    response.__set_result_(actor->FinishAutoPlayingGroup(request.playing_group_));
    response.__set_playing_group_(request.playing_group_);
  } else {
    LOG_WARNING("请求消息错误");
    return;
  }

  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_AUTO_FINISH_RESPONSE);
}

void PlayingMessageHandler::OnMessagePlayingPaidAwardRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  gateway::protocol::MessagePlayingPaidAwardRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_PLAYING_PAID_AWARD_REQUEST from actor(%lu, %s), playing_template_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), request.playing_template_);

  PlayingPage *page = actor->GetPage(request.playing_template_);
  if (page == NULL) {
    LOG_ERROR("获取 玩家(%lu) PlayingPage(%d) 失败", id, request.playing_template_);
    return;
  }

  const global::configure::PlayingCell *playing_cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        request.playing_template_);
  if (playing_cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", request.playing_template_);
    return;
  }

  if (actor->CheckFunctionalityOpen(playing_cell->camp_type_) == false) {
    LOG_ERROR("PlayingActor(%lu) CheckFunctionalityOpen 失败", id);
    return;
  }

  if (playing_cell->paid_awards_.empty() == true) {
    LOG_WARNING("副本(%d) 没有 paid_awards_", request.playing_template_);
    return;
  }

  core::int32 vip_paid_award_count = 0;
  const global::configure::VipCell *vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)game_actor->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::PLAYING_PERIOD_PAID_AWARD_COUNT, request.playing_template_);
  if (vip_cell != NULL) {
    vip_paid_award_count = vip_cell->value2_;
  }

  entity::PlayingResultField &result = page->GetResult();
  if (playing_cell->paid_award_period_count_ + vip_paid_award_count <= result.paid_award_count_) {
    LOG_WARNING("副本(%d) 购买奖励周期次数已满 vip次数(%d)", request.playing_template_, vip_paid_award_count);
    return;
  }

  core::int32 vip_period_count = 0;
  const global::configure::VipCell *vip_period_count_cell =
    VIP_CONF()->GetCell((entity::VipType::type)game_actor->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::PLAYING_PERIOD_COUNT, request.playing_template_);
  if (vip_period_count_cell) {
    vip_period_count = vip_period_count_cell->value2_;
  }

  entity::PlayingRecordField &record = page->GetRecord();
  if (playing_cell->period_count_ + vip_period_count < record.period_count_) {
    LOG_WARNING("副本(%d) 次数未完成", request.playing_template_);
    return;
  }

  core::int32 spend_freedom_dollars =
    result.paid_award_count_ * playing_cell->paid_award_spend_freedom_dollars_;
  core::int32 freedom_dollars =
    game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);
  if (freedom_dollars < spend_freedom_dollars) {
    LOG_WARNING("玩家(%lu) FREEDOM_DOLLARS(%d) 不足(%d)",
        id, freedom_dollars, spend_freedom_dollars);
    return;
  }

  game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS,
      0 - spend_freedom_dollars);

  // 道具
  request::RequestAddItems request_add_items;
  request_add_items.__set_actor_id_(id);
  entity::AddItemFields field;
  global::configure::PlayingCell::AwardVector::const_iterator iterator =
    playing_cell->paid_awards_.begin();
  for (; iterator != playing_cell->paid_awards_.end(); ++iterator) {
    if (iterator->type_ == entity::ResourceType::ITEM) {
      field.__set_item_template_(iterator->id_);
      field.__set_number_(iterator->number_);
      field.__set_bind_(true);
      request_add_items.items_.push_back(field);
    }
  }
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEMS,
        &request_add_items, sizeof(request_add_items)) != 0) {
    LOG_ERROR("请求失败 REQUEST_ADD_ITEMS");
    return;
  }
  if(request_add_items.result_ != request::AddItemResult::SUCCESS) {
    if(request_add_items.result_ == request::AddItemResult::PACKET_FULL) {
      LOG_ERROR("增加道具失败，背包已满");
      ErrorCode::Send(game_actor, entity::ErrorType::PACKET_FULL);
      return;
    } else {
      LOG_ERROR("增加道具失败，错误未知");
      return;
    }
  }

  // 资源
  iterator = playing_cell->paid_awards_.begin();
  for (; iterator != playing_cell->paid_awards_.end(); ++iterator) {
    if (iterator->type_ == entity::ResourceType::RESOURCE) {
      game_actor->AddResource((entity::ResourceID::type)iterator->id_, iterator->number_);
    }
  }

  // 次数增加
  result.paid_award_count_ += 1;

  page->Synchronize();

  gateway::protocol::MessagePlayingPaidAwardResponse response;
  response.__set_result_(true);
  response.__set_playing_template_(request.playing_template_);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_PLAYING_PAID_AWARD_RESPONSE);

  LOG_DEBUG("收到 MessagePlayingPaidAwardRequest");
}

}  // namespace playing

}  // namespace server

}  // namespace game

