
#include "game_server/server/extension/welfare/actor_message_handler.h"

#include <boost/bind.hpp>

#include "entity/resource_types.h"
#include "gateway_server/protocol/gateway_welfare_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/welfare/facede_request.h"
#include "game_server/server/extension/welfare/achieve_actor.h"
#include "game_server/server/extension/welfare/configure.h"
#include "game_server/server/extension/welfare/load_task_configure.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "global/distance_calculator.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

ActorMessageHandler::ActorMessageHandler() {}
ActorMessageHandler::~ActorMessageHandler() {}

bool ActorMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACHIEVE_AWARD_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageAward, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACHIEVE_CHANGE_TITLE_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageAlterTitle, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_CHECKINS_OPT_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageCheckins, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ONLINE_AWARD_ITEM_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageOnlineAward, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ONLINE_AWARD_RESOURCE_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageResourceAward, this, _1, _2, _3));

  return true;
}

void ActorMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACHIEVE_AWARD_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACHIEVE_CHANGE_TITLE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_CHECKINS_OPT_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ONLINE_AWARD_ITEM_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ONLINE_AWARD_RESOURCE_REQUEST);
}

void ActorMessageHandler::OnMessageCheckins(core::uint64 id, 
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 获取游戏玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    LOG_ERROR("游戏玩家(%lu)对象没有找到", id);
    return ;
  }

  // 获取成就对象
  WelfareActor *welfare = WelfareActorManager::GetInstance()->Get(id);
  if(welfare == NULL) {
    LOG_ERROR("玩家成就对象(%lu)没有找到", id);
    return ;
  }

  CheckinsActor *actor = welfare->GetCheckins();

  // 消息解析
  gateway::protocol::MessageCheckinsOptRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("反序列化消息失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_CHECKINS_OPT_REQUEST from(%lu, %s), days_(%d),type_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.days_, message.type_);

  if(message.type_ == gateway::protocol::CheckinsOptType::DAILY_CHECKINS) {
    // 每日签到
    if(actor->CheckinsDays(message.days_) == false) {
      LOG_ERROR("签到失败(%d)", message.days_);
      return ;
    }
  } else if(message.type_ == gateway::protocol::CheckinsOptType::GENERAL_CHECKINS) {
    // 领取普通玩家的奖励
    if(actor->GetMonthCheckinsNum() < message.days_) {
      LOG_ERROR("签到次数不足，不能领取奖励");
      return ;
    }
    const CheckinsCell *cell = Configure::GetInstance()->GetCell(message.days_);
    if(cell == NULL) {
      LOG_ERROR("配置没有找到(%d)", message.days_);
      return ;
    }
    if(actor->CheckGeneralCheckins(cell->sort_) == true) {
      LOG_ERROR("奖励(%d)已经领取过", cell->sort_);
      return ;
    }
    // 领取奖励
    if(FacedeRequest::GetInstance()->Awards(id, cell->award_, 5) == false) {
      LOG_ERROR("加入失败");
      return ;
    }
    actor->SetGeneralCheckins(cell->sort_);

  } else if(message.type_ == gateway::protocol::CheckinsOptType::VIP_CHECKINS) {
    // 领取VIP签到玩家的奖励
    if(actor->GetMonthCheckinsNum() < message.days_) {
      LOG_ERROR("签到次数不足，不能领取奖励");
      return ;
    }
    if(game_actor->GetAttribute(entity::ActorAoiFields::VIP) == (core::int32)entity::VipType::INVALID) {
      LOG_ERROR("不是VIP，无法领取VIP奖励");
      return ;
    }
    const CheckinsCell *cell = Configure::GetInstance()->GetCell(message.days_);
    if(cell == NULL) {
      LOG_ERROR("配置没有找到(%d)", message.days_);
      return ;
    }
    if(actor->CheckVipCheckins(cell->sort_) == true) {
      LOG_ERROR("奖励(%d)已经领取过", cell->sort_);
      return ;
    }

    // 领取奖励
    if(FacedeRequest::GetInstance()->Awards(id, cell->vip_award_, 5) == false) {
      LOG_ERROR("加入失败");
      return ;
    }
    actor->SetVipCheckins(cell->sort_);

  } else if(message.type_ == gateway::protocol::CheckinsOptType::DAILY_CHECKINS_AWARD) {
    // 领取每日签到的奖励
    if(actor->CheckDailyCheckins(actor->GetDaysOfMonth()) == false) {
      LOG_ERROR("尚未签到(%d)，不能领取奖励", actor->GetDaysOfMonth());
      return ;
    }
    if(actor->CheckCurrentCheckinsAward() == true) {
      LOG_ERROR("奖励已经被领取");
      return ;
    }

    core::int32 dayofweek = actor->GetDaysOfWeek();
    const DailyCheckinsCell *cell = Configure::GetInstance()->GetWeekCell(dayofweek);
    if(cell == NULL) {
      LOG_ERROR("配置文件没有找到[%d]", dayofweek);
      return ;
    }
    if(FacedeRequest::GetInstance()->Awards(id, cell->award_, 5) == false) {
      LOG_ERROR("奖励加入失败");
      return ;
    }
    // 加入奖励
    actor->SetCurrentCheckinsAward(true);
  } else {
    LOG_ERROR("参数(%d)错误", message.type_);
    return ;
  }

  gateway::protocol::MessageCheckinsOptResponse response;
  response.__set_type_(message.type_);
  response.__set_days_(message.days_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_CHECKINS_OPT_RESPONSE);
}

void ActorMessageHandler::OnMessageOnlineAward(core::uint64 id, 
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 获取游戏玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    LOG_ERROR("游戏玩家(%lu)对象没有找到", id);
    return ;
  }

  // 获取成就对象
  WelfareActor *welfare = WelfareActorManager::GetInstance()->Get(id);
  if(welfare == NULL) {
    LOG_ERROR("玩家成就对象(%lu)没有找到", id);
    return ;
  }

  CheckinsActor *actor = welfare->GetCheckins();

  // 消息解析
  gateway::protocol::MessageOnlineAwardItemRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("反序列化消息失败");
    return ;
  }

  LOG_INFO("Receive MESSAGE_ONLINE_AWARD_ITEM_REQUEST from actor(%lu, %s), time_(%lu)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.time_);

  // 获取配置文件
  core::int32 award_pos = Configure::GetInstance()->GetOnlineToPos(message.time_);
  if(award_pos == -1) {
    LOG_ERROR("配置(%d)没有找到", message.time_);
    return ;
  }

  // 验证在线时间
  if(actor->GetDailyTime() < message.time_) {
    LOG_ERROR("每日在线时间没有超过领奖时间(%d)", message.time_);
    return ;
  }

  // 验证是否已经领取过奖励
  if(actor->CheckOnlineAwardItem(award_pos) == true) {
    LOG_ERROR("每日在线奖励已经领取过(%d)", message.time_);
    return ;
  }

  // 随机出道具
  OnlineCell * award = Configure::GetInstance()->RandCell(message.time_);
  if(award == NULL) {
    LOG_ERROR("随机道具出错(%d)", message.time_);
    return ;
  }
  if(award->rand_result_ >= 5 || award->rand_result_ < 0) {
    LOG_ERROR("随机出错误的结果(%d)", award->rand_result_);
    return ;
  }
  if(FacedeRequest::GetInstance()->Awards(id, &(award->award_[award->rand_result_]), 1) == false) {
    LOG_ERROR("加入奖励失败");
    return ;
  }

  actor->SetOnlineAwardItem(award->id_, award->rand_result_ + 1);
  // 添加奖励并且设置已经领取过
  gateway::protocol::MessageOnlineAwardItemResponse response;
  response.__set_time_(message.time_);
  response.__set_pos_(award->rand_result_ + 1);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ONLINE_AWARD_ITEM_RESPONSE);
}

void ActorMessageHandler::OnMessageResourceAward(core::uint64 id,  
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 获取游戏玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    LOG_ERROR("游戏玩家(%lu)对象没有找到", id);
    return ;
  }

  // 获取成就对象
  WelfareActor *welfare = WelfareActorManager::GetInstance()->Get(id);
  if(welfare == NULL) {
    LOG_ERROR("玩家成就对象(%lu)没有找到", id);
    return ;
  }
  CheckinsActor *actor = welfare->GetCheckins();

  // 消息解析
  gateway::protocol::MessageOnlineAwardResourceRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("反序列化 MessageOnlineAwardResourceRequest 失败");
    return ;
  }

  LOG_INFO("Receive MESSAGE_ONLINE_AWARD_ITEM_RESOURCE_REQUEST from actor(%lu, %s)",
      game_actor->GetActorID(), game_actor->GetName().c_str());

  if(actor->PickOnlineResourceAward() == false) {
    LOG_ERROR("上月的奖励已经领取过");
    return ;
  }

  // 领取资源奖励
  gateway::protocol::MessageOnlineAwardResourceResponse response;
  response.__set_result_(true);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ONLINE_AWARD_ITEM_RESOURCE_RESPONSE);
}

void ActorMessageHandler::OnMessageAward(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 获取游戏玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    LOG_ERROR("游戏玩家(%lu)对象没有找到", id);
    return ;
  }

  // 获取成就对象
  WelfareActor *welfare = WelfareActorManager::GetInstance()->Get(id);
  if(welfare == NULL) {
    LOG_ERROR("玩家成就对象(%lu)没有找到", id);
    return ;
  }

  AchieveActor *achieve = welfare->GetAchieve();

  // 消息解析
  gateway::protocol::MessageAchieveAwardRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    return ;
  }

  LOG_INFO("Recevie MESSAGE_ACHIEVE_AWARD_REQUEST from(%lu, %s), id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.id_);

  // 获取配置
  const AchieveTaskCell *cell = Configure::GetInstance()->GetAchieveCell(message.id_);
  if(cell == NULL) {
    LOG_ERROR("成就(%d)配置没有找到", message.id_);
    return ;
  }

  if(achieve->CheckFinishAchieve(message.id_) == false) {
    LOG_ERROR("成就(%d)未完成", message.id_);
    return ;
  }

  // 添加奖励
  if(cell->award_.type_ == entity::ResourceType::RESOURCE) {
    game_actor->AddResource((entity::ResourceID::type)cell->award_.id_, cell->award_.number_);
  } else if(cell->award_.type_ == entity::ResourceType::ITEM){
    // 请求增奖励的道具
    if(FacedeRequest::GetInstance()->AddItem(id, cell->award_.id_,
          cell->award_.number_, cell->award_.bind_ != 0) == false) {
    }
  } else {
  }

  if(achieve->FinishAchieve(message.id_) == false) {
    LOG_ERROR("完成成就失败");
    return ;
  }

  gateway::protocol::MessageAchieveAwardResponse response;
  response.__set_id_(message.id_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ACHIEVE_AWARD_RESPONSE);
}

void ActorMessageHandler::OnMessageAlterTitle(core::uint64 id, const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 获取游戏玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    LOG_ERROR("游戏玩家(%lu)对象没有找到", id);
    return ;
  }

  // 获取成就对象
  WelfareActor *welfare = WelfareActorManager::GetInstance()->Get(id);
  if(welfare == NULL) {
    LOG_ERROR("玩家成就对象(%lu)没有找到", id);
    return ;
  }

  AchieveActor *achieve = welfare->GetAchieve();

  // 消息解析
  gateway::protocol::MessageAchieveChangeTitleRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    return ;
  }

  LOG_INFO("Recevie MESSAGE_ACHIEVE_CHANGE_TITLE_REQUEST from(%lu, %s), id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.id_);

  if(message.id_ != 0) {
    // 获取配置
    const AchieveTaskCell *cell = Configure::GetInstance()->GetAchieveCell(message.id_);
    if(cell == NULL) {
      LOG_ERROR("成就(%d)配置没有找到", message.id_);
      return ;
    }

    if(achieve->CheckFinishExist(message.id_) == false) {
      LOG_ERROR("此成就尚未达成");
      return ;
    }
    achieve->SetTitleID(cell->id_);
  } else {
    achieve->SetTitleID(0);
  }

  gateway::protocol::MessageAchieveChangeTitleResponse response;
  response.__set_id_(message.id_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ACHIEVE_CHANGE_TITLE_RESPONSE);

  // 广播周围玩家
  gateway::protocol::MessageWelfareTitleBroadCast msg;
  gateway::protocol::MessageTitleFiled field;
  field.__set_actor_id_(global::ToString(id));
  field.__set_title_id_(achieve->GetTitleID());
  msg.fields_.push_back(field);
  game_actor->SendMessageAround(msg,
      gateway::protocol::MessageType::MESSAGE_WELFARE_TITLE_BROADCAST, true);

}

}  // namespace welfare

}  // namespace server

}  // namespace game

