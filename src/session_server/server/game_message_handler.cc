//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-26 10:58:31.
// File name: game_message_handler.cc
//
// Description:
// Define class GameMessageHandler.
//

#include "session_server/server/game_message_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/functionality_types.h"
#include "global/global_packet.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/error_code.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/session_actor_pool.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"

namespace session {

namespace server {

GameMessageHandler::GameMessageHandler() {}
GameMessageHandler::~GameMessageHandler() {}

bool GameMessageHandler::Initialize() {
  this->SetMessageHandler(protocol::GameMessageType::MESSAGE_ACTOR_LOGIN_RESPONSE,
      boost::bind(&GameMessageHandler::OnMessageActorLoginResponse, this, _1, _2, _3));

  this->SetMessageHandler(protocol::GameMessageType::MESSAGE_ACTOR_BATCH_LOGIN_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageActorBatchLogin, this, _1, _2, _3));

  this->SetMessageHandler(protocol::GameMessageType::MESSAGE_ACTOR_LEVEL_SYNCHRONIZE,
      boost::bind(&GameMessageHandler::OnMessageActorLevelSynchonize, this, _1, _2, _3));

  this->SetMessageHandler(protocol::GameMessageType::MESSAGE_ACTOR_VOCATION_SYNCHRONIZE,
      boost::bind(&GameMessageHandler::OnMessageActorVocationSynchronzie, this, _1, _2, _3));

  this->SetMessageHandler(protocol::GameMessageType::MESSAGE_ACTOR_FUNCTIONALITY_STATE_SYCHRONIZE,
      boost::bind(&GameMessageHandler::OnMessageActorFunctionalityStateSynchronize, this, _1, _2, _3));

  this->SetMessageHandler(protocol::GameMessageType::MESSAGE_ACTOR_FIGHT_SCORE_SYCHRONIZE,
      boost::bind(&GameMessageHandler::OnMessageActorFightScoreSychronize, this, _1, _2, _3));

  this->SetMessageHandler(protocol::GameMessageType::MESSAGE_ACTOR_COMPARE_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageActorCompareRequest, this, _1, _2, _3));

  this->SetMessageHandler(protocol::GameMessageType::MESSAGE_ACTOR_VIP_SYNCHRONIZE,
      boost::bind(&GameMessageHandler::OnMessageActorVipSynchronize, this, _1, _2, _3));

  return true;
}

void GameMessageHandler::Finalize() {
  this->ResetMessageHandler(
      protocol::GameMessageType::MESSAGE_ACTOR_LOGIN_RESPONSE);

  this->ResetMessageHandler(
      protocol::GameMessageType::MESSAGE_ACTOR_BATCH_LOGIN_REQUEST);

  this->ResetMessageHandler(
      protocol::GameMessageType::MESSAGE_ACTOR_LEVEL_SYNCHRONIZE);

  this->ResetMessageHandler(
      protocol::GameMessageType::MESSAGE_ACTOR_VOCATION_SYNCHRONIZE);

  this->ResetMessageHandler(
      protocol::GameMessageType::MESSAGE_ACTOR_FUNCTIONALITY_STATE_SYCHRONIZE);

  this->ResetMessageHandler(
      protocol::GameMessageType::MESSAGE_ACTOR_FIGHT_SCORE_SYCHRONIZE);

  this->ResetMessageHandler(
      protocol::GameMessageType::MESSAGE_ACTOR_COMPARE_REQUEST);

  this->ResetMessageHandler(
      protocol::GameMessageType::MESSAGE_ACTOR_VIP_SYNCHRONIZE);
}

void GameMessageHandler::OnMessage(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) Terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::ThriftPacket *thrift = terminal->GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageGameServer message2;
  if(thrift->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(message2.type_ < protocol::GameMessageType::MESSAGE_MIN ||
        message2.type_ >= protocol::GameMessageType::MESSAGE_MAX ||
      this->handler_[message2.type_].empty() == true) {
    global::LogError("%s:%d (%s) Error message type[%d]",
        __FILE__, __LINE__, __FUNCTION__, message2.type_);
    return ;
  }
  this->handler_[message2.type_](terminal, message2.message_.data(),
      message2.message_.size());
}

void GameMessageHandler::OnMessageActorLoginResponse(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) Terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::ThriftPacket *thrift = terminal->GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  protocol::GameMessageActorLoginResponse response;
  if(thrift->Deserialize(response, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(response.actor_id_);
  if(actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", response.actor_id_);
    return ;
  }

  // 玩家跨 GameServer 服务器跳转场景完成
  if(response.jump_scene_ == true) {
    if(response.logined_ == false) {
      SessionActor *actor = SessionActorManager::GetInstance()->GetActor(response.actor_id_);
      if(actor) {
        actor->Kickout();
      }
      return ;
    }
    event::EventActorJumpScene event;
    event.__set_actor_(response.actor_id_);
    event.__set_cross_game_server_(true);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_JUMP_SCENE, response.actor_id_,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
    LOG_INFO("actor[%lu] jump game server complete.", response.actor_id_);
  } else {
    actor->OnGameLoadFinish();
  }
}

void GameMessageHandler::OnMessageActorBatchLogin(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(terminal == NULL) {
    LOG_ERROR("Terminal is null.");
    return ;
  }

  global::ThriftPacket *thrift = terminal->GetThrift();
  if(thrift == NULL) {
    LOG_ERROR("Thrift is null.");
    return ;
  }

  // 消息解析
  protocol::MessageActorBatchLoginRequest request;
  if(thrift->Deserialize(request, message, size) == false) {
    LOG_ERROR("Deserialize message failed.");
    return ;
  }

  size_t pos = 0;
  for(; pos < request.actors_.size(); ++pos) {
    SessionActor *actor = SessionActorManager::GetInstance()->GetActor(
        request.actors_[pos].id_);
    if(actor != NULL) {
      continue;
    }
    actor = SessionActorPool::GetInstance()->Allocate();
    if(actor == NULL) {
      LOG_ERROR("分配玩家对象(%lu)失败", request.actors_[pos].id_);
      continue;
    }

    actor->SetActorID(request.actors_[pos].id_);

    if(actor->Initialize() == false) {
      LOG_ERROR("初始化玩家对象(%lu)失败", request.actors_[pos].id_);
      SessionActorPool::GetInstance()->Deallocate(actor);
      continue;
    }

    // 新增 BriefActor
    BriefActor brief_actor(request.actors_[pos].id_,
        request.actors_[pos].name_,
        request.actors_[pos].level_,
        request.actors_[pos].vocation_,
        request.actors_[pos].gender_,
        request.actors_[pos].map_,
        request.actors_[pos].functionality_state_,
        request.actors_[pos].axe_fight_score_,
        request.actors_[pos].shoot_fight_score_,
        request.actors_[pos].magic_fight_score_,
        request.actors_[pos].last_logout_time_,
        request.actors_[pos].vip_type_,
        request.actors_[pos].vip_expire_time_);
    SessionActorManager::GetInstance()->AddBriefActor(brief_actor);
    // 设置 BriefActor
    actor->SetBriefActor(
        SessionActorManager::GetInstance()->GetBriefActor(request.actors_[pos].id_));
    // 设置服务器
    actor->SetGateway(request.actors_[pos].gateway_);
    actor->SetGameServer(terminal->GetSignature().id_);

    if(SessionActorManager::GetInstance()->AddActor(actor) == false) {
      LOG_ERROR("玩家对象(%lu)加入管理器失败", request.actors_[pos].id_);
      actor->Finalize();
      SessionActorPool::GetInstance()->Deallocate(actor);
      continue;
    }
    // 广播玩家登陆事件
    event::EventActorLogin event;
    event.__set_id_(actor->GetActorID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_LOGIN, actor->GetActorID(),
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

    if(actor->GetLoadProgress().LoadFinish() == true) {
      actor->OnSessionLoadFinish();
    }
  }
}

void GameMessageHandler::OnMessageActorLevelSynchonize(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(NULL == terminal || NULL == message || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  global::ThriftPacket *thrift = terminal->GetThrift();
  if(NULL == thrift) {
    LOG_ERROR("Thrift is null.");
    return;
  }

  // 消息解析
  session::protocol::MessageActorLevelSynchronize sync;
  if(thrift->Deserialize(sync, message, size) == false) {
    LOG_ERROR("Deserialize MessageActorLevelSynchronize failed.");
    return;
  }

  // 更新 BriefActor 的玩家等级
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(sync.actor_id_);
  if(NULL == brief_actor) {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", sync.actor_id_);
    return;
  }
  brief_actor->SetLevel(sync.new_level_);

  // 发送升级事件
  event::EventActorLevelUp event;
  event.__set_actor_id_(sync.actor_id_);
  event.__set_old_level_(sync.old_level_);
  event.__set_new_level_(sync.new_level_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_LEVEL_UP, sync.actor_id_,
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void GameMessageHandler::OnMessageActorVocationSynchronzie(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(NULL == terminal || NULL == message || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  global::ThriftPacket *thrift = terminal->GetThrift();
  if(NULL == thrift) {
    LOG_ERROR("Thrift is null.");
    return;
  }

  // 消息解析
  session::protocol::MessageActorVocationSynchronzie sync;
  if(thrift->Deserialize(sync, message, size) == false) {
    LOG_ERROR("Deserialize MessageActorVocationSynchronzie failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ACTOR_VOCATION_SYNCHRONIZE from game_server, "
           "actor_id_=[%lu], src_vocation_=[%d], dst_vocation_=[%d].",
           sync.actor_id_, sync.src_vocation_, sync.dst_vocation_);

  // 更新 BriefActor 的玩家职业
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(sync.actor_id_);
  if(NULL == brief_actor) {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", sync.actor_id_);
    return;
  }
  brief_actor->SetVocation(sync.dst_vocation_);

  // 发送切换职业事件
  event::EventActorChangeVocation event;
  event.__set_actor_id_(sync.actor_id_);
  event.__set_src_vocation_(sync.src_vocation_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_CHANGE_VOCATION, sync.actor_id_,
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void GameMessageHandler::OnMessageActorFunctionalityStateSynchronize(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(NULL == terminal || NULL == message || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  global::ThriftPacket *thrift = terminal->GetThrift();
  if(NULL == thrift) {
    LOG_ERROR("Thrift is null.");
    return;
  }

  // 消息解析
  session::protocol::MessageActorFunctionalityStateSynchronize sync;
  if(thrift->Deserialize(sync, message, size) == false) {
    LOG_ERROR("Deserialize MessageActorFunctionalityStateSynchronize failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ACTOR_FUNCTIONALITY_STATE_SYCHRONIZE from game_server, "
           "actor_id_=[%lu], type_=[%d]",
           sync.actor_id_, sync.type_);

  if(sync.type_ < entity::FunctionalityType::MIN ||
      sync.type_ >= entity::FunctionalityType::MAX) {
    LOG_ERROR("sync.type_ is invalid.");
    return;
  }

  // 开启系统功能
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(sync.actor_id_);
  if(NULL == brief_actor) {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", sync.actor_id_);
    return;
  }
  brief_actor->SetFunctionalityState(sync.type_, sync.state_);
}

void GameMessageHandler::OnMessageActorFightScoreSychronize(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(NULL == terminal || NULL == message || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  global::ThriftPacket *thrift = terminal->GetThrift();
  if(NULL == thrift) {
    LOG_ERROR("Thrift is null.");
    return;
  }

  // 消息解析
  session::protocol::MessageActorFightScoreSychronize sync;
  if(thrift->Deserialize(sync, message, size) == false) {
    LOG_ERROR("Deserialize MessageActorFightScoreSychronize failed.");
    return;
  }

  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(sync.actor_id_);
  if(NULL == brief_actor) {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", sync.actor_id_);
    return;
  }

  event::EventActorChangeFightScore event;
  event.__set_actor_id_(sync.actor_id_);

  if(sync.__isset.axe_fight_score_) {
    if(brief_actor->GetFightScore(entity::FightScoreType::AXE) != sync.axe_fight_score_) {
      event.__set_old_axe_fight_score_(brief_actor->GetFightScore(entity::FightScoreType::AXE));
      event.__set_new_axe_fight_score_(sync.axe_fight_score_);
      brief_actor->SetFightScore(entity::FightScoreType::AXE, sync.axe_fight_score_);
    }
  }
  if(sync.__isset.shoot_fight_score_) {
    if(brief_actor->GetFightScore(entity::FightScoreType::SHOOT) != sync.shoot_fight_score_) {
      event.__set_old_shoot_fight_score_(brief_actor->GetFightScore(entity::FightScoreType::SHOOT));
      event.__set_new_shoot_fight_score_(sync.shoot_fight_score_);
      brief_actor->SetFightScore(entity::FightScoreType::SHOOT, sync.shoot_fight_score_);
    }
  }
  if(sync.__isset.magic_fight_score_) {
    if(brief_actor->GetFightScore(entity::FightScoreType::MAGIC) != sync.magic_fight_score_) {
      event.__set_old_magic_fight_score_(brief_actor->GetFightScore(entity::FightScoreType::MAGIC));
      event.__set_new_magic_fight_score_(sync.magic_fight_score_);
      brief_actor->SetFightScore(entity::FightScoreType::MAGIC, sync.magic_fight_score_);
    }
  }

  // 发送战斗力变更事件
  if (event.__isset.old_axe_fight_score_ ||
      event.__isset.old_shoot_fight_score_ ||
      event.__isset.old_magic_fight_score_) {
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_CHANGE_FIGHT_SCORE, sync.actor_id_,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
}

void GameMessageHandler::OnMessageActorCompareRequest(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(NULL == terminal || NULL == message || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  session::protocol::MessageActorCompareRequest request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, message, size) == false) {
    LOG_ERROR("Deserialize MessageEquipCompareRequest failed.");
    return;
  }

  SessionActor *actor =
    SessionActorManager::GetInstance()->GetActor(request.actor_);
  if(actor == NULL) {
    LOG_ERROR("Get SessionActor(%lu) failed.", request.actor_);
    return;
  }

  SessionActor *compare_actor =
    SessionActorManager::GetInstance()->GetActor(request.compare_actor_);
  if(compare_actor == NULL) {
    ErrorCode::Send(actor, entity::ErrorType::ACTOR_OFFLINE);
    return;
  }

  // 发送到被比较玩家所在 GameServer
  session::protocol::MessageActorCompareSynchronize synchronize;
  synchronize.__set_actor_(request.actor_);
  synchronize.__set_compare_actor_(request.compare_actor_);
  actor->SendGameMessage(synchronize,
      session::protocol::GameMessageType::MESSAGE_ACTOR_COMPARE_SYNCHRONIZE);
}

void GameMessageHandler::OnMessageActorVipSynchronize(SessionTerminal *terminal,
    const char *message, size_t size) {
  if(NULL == terminal || NULL == message || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  session::protocol::MessageActorVipSynchronize msg;
  if(global::GlobalPacket::GetPacket()->Deserialize(msg, message, size) == false) {
    LOG_ERROR("Deserialize MessageEquipCompareRequest failed.");
    return;
  }

  SessionActor *actor =
    SessionActorManager::GetInstance()->GetActor(msg.actor_);
  if(actor == NULL) {
    LOG_ERROR("Get SessionActor(%lu) failed.", msg.actor_);
    return;
  }

  BriefActor *brief_actor = actor->GetBriefActor();
  if(brief_actor == NULL) {
    LOG_ERROR("Get BriefActor(%lu) failed.", msg.actor_);
    return;
  }

  brief_actor->SetVipType(msg.vip_type_);
  brief_actor->SetVipExpireTime(msg.viP_expire_time_);
}

}  // namespace server

}  // namespace session

