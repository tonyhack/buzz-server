//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 02:02:10.
// File name: transport_event_handler.cc
//
// Description:
// Define class TransportEventHandler.
//

#include "game_server/server/extension/transport/transport_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_server.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_transport_event_types.h"
#include "game_server/server/extension/transport/facade_request.h"
#include "game_server/server/extension/transport/transport_actor.h"
#include "game_server/server/extension/transport/transport_actor_manager.h"
#include "game_server/server/extension/transport/transport_actor_pool.h"
#include "game_server/server/extension/transport/transport_destory_checker.h"
#include "game_server/server/extension/transport/transport_npc.h"
#include "game_server/server/extension/transport/transport_npc_manager.h"
#include "game_server/server/extension/transport/transport_npc_pool.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_transport_protocol_types.h"
#include "global/common_functions.h"
#include "global/distance_calculator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace transport {

TransportEventHandler::TransportEventHandler() {}
TransportEventHandler::~TransportEventHandler() {}

bool TransportEventHandler::Initialize() {
  this->conn_actor_login_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&TransportEventHandler::OnEventActorLogin,
        this, _1, _2, _3, _4));

  this->conn_actor_login_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&TransportEventHandler::OnEventActorLogout,
        this, _1, _2, _3, _4));

  this->conn_scene_jump_map_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_JUMP_MAP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&TransportEventHandler::OnEventSceneJumpMap,
        this, _1, _2, _3, _4));

  this->conn_scene_role_change_location_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&TransportEventHandler::OnEventSceneRoleChangeLocation,
        this, _1, _2, _3, _4));

  this->conn_role_killed_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_KILLED, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&TransportEventHandler::OnEventRoleKilled,
        this, _1, _2, _3, _4));

  this->conn_scene_broadcast_create_role_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_BROADCAST_CREATE_ROLE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SCENE, boost::bind(&TransportEventHandler::OnEventSceneBroadcastCreateRole,
        this, _1, _2, _3, _4));

  this->conn_scene_create_aoi_roles_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_CREATE_AOI_ROLES, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SCENE, boost::bind(&TransportEventHandler::OnEventSceneCreateAoiRoles,
        this, _1, _2, _3, _4));

  return true;
}

void TransportEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_login_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_logout_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_scene_jump_map_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_scene_role_change_location_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_role_killed_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_scene_broadcast_create_role_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_scene_create_aoi_roles_);
}

void TransportEventHandler::OnEventActorLogin(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if (actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->id_);
    return;
  }

  TransportActor *transport_actor = TransportActorPool::GetInstance()->Allocate();
  if (transport_actor == NULL) {
    LOG_ERROR("分配 TransportActor(%lu) 失败", event->id_);
    return;
  }

  if (transport_actor->Initialize(actor) == false) {
    LOG_ERROR("初始化 TransportActor(%lu) 失败", event->id_);
    return;
  }

  if (TransportActorManager::GetInstance()->Add(transport_actor) == false) {
    LOG_ERROR("TransportActor(%lu) 加入 TransportActorManager 失败");
    return;
  }

  core::uint64 transport_npc_id =
    TransportActorManager::GetInstance()->GetTransport(actor->GetID());
  TransportNpc *transport_npc =
    TransportNpcManager::GetInstance()->Get(transport_npc_id);
  if (transport_npc != NULL) {
    transport_actor->SetTransportNpc(transport_npc_id, false);
    transport_npc->Regain();
    // TransportDestoryChecker::GetInstance()->Remove(transport_npc_id);
  }

  LOG_INFO("玩家(%lu) 护送扩展加载成功 transport_npc_id=%lu", event->id_, transport_npc_id);

  gateway::protocol::MessageTransportInitialize initialize;
  initialize.__set_npc_id_(global::ToString(transport_actor->GetTransportNpc()));
  transport_actor->SendMessage(initialize,
    gateway::protocol::MessageType::MESSAGE_TRANSPORT_INITIALIZE);

  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  load_finish.__set_extension_type_(ExtensionType::TRANSPORT);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));
}

void TransportEventHandler::OnEventActorLogout(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventActorLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  TransportActor *transport_actor =
    TransportActorManager::GetInstance()->Remove(event->id_);
  if (transport_actor == NULL) {
    LOG_ERROR("获取 TransportActor(%lu) 失败", event->id_);
    return;
  }

  core::uint64 transport_npc_id = transport_actor->GetTransportNpc();
  if (transport_npc_id != 0) {
    TransportNpc *transport_npc =
      TransportNpcManager::GetInstance()->Get(transport_npc_id);
    if (transport_npc) {
      transport_npc->Lose();
    }
    // TransportDestoryChecker::GetInstance()->Add(transport_npc_id);
  }

  transport_actor->Finalize();
  TransportActorPool::GetInstance()->Deallocate(transport_actor);
}

void TransportEventHandler::OnEventRoleKilled(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventRoleKilled) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)message;

  if (event->type_ != entity::EntityType::TYPE_NPC ||
      event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return;
  }

  TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(event->id_);
  if (transport_npc == NULL) {
    return;
  }
  core::uint64 transport_master_actor_id = transport_npc->GetMasterActor();
  TransportActor *transport_actor =
    TransportActorManager::GetInstance()->Get(transport_npc->GetMasterActor());
  if (transport_actor) {
    transport_actor->SetTransportNpc(0);
    // 护送失败，发事件
    event::EventTransportFailure event2;
    event2.__set_actor_(transport_actor->GetID());
    event2.__set_rob_actor_(event->attacker_id_);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_TRANSPORT_FAILURE, transport_actor->GetID(),
        event::ChannelType::CHANNEL_ACTOR, &event2,
        sizeof(event::EventTransportFailure));
  }
  transport_npc->Finalize();
  TransportNpcManager::GetInstance()->Remove(event->id_);
  TransportNpcPool::GetInstance()->Deallocate(transport_npc);
  TransportActorManager::GetInstance()->RemoveTransport(transport_master_actor_id);
  TransportDestoryChecker::GetInstance()->Remove(event->id_);
}

void TransportEventHandler::OnEventSceneJumpMap(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventSceneJumpMap) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSceneJumpMap *event = (event::EventSceneJumpMap *)message;

  if (event->type_ != entity::EntityType::TYPE_ACTOR) {
    return;
  }

  TransportActor *transport_actor = TransportActorManager::GetInstance()->Get(event->id_);
  if (transport_actor == NULL) {
    LOG_ERROR("获取 TransportActor(%lu) 失败", event->id_);
    return;
  }

  core::uint64 transport_npc_id = transport_actor->GetTransportNpc();
  if (transport_npc_id == 0) {
    return;
  }

  TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(transport_npc_id);
  if (transport_npc == NULL) {
    LOG_ERROR("获取 TransportNpc(%lu) 失败", transport_npc_id);
    return;
  }

  if (transport_npc->GetLost() == true) {
    return;
  }

  if (FacadeRequest::GetInstance()->JumpScene(entity::EntityType::TYPE_NPC, transport_npc_id,
        event->dest_scene_, event->dest_x_, event->dest_y_) == false) {
    LOG_ERROR("TransportNpc(%lu) 跳转场景失败", transport_npc_id);
    return;
  }
}

void TransportEventHandler::OnEventSceneRoleChangeLocation(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventSceneRoleChangeLocation) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSceneRoleChangeLocation *event =
    (event::EventSceneRoleChangeLocation *)message;

  if (event->type_ != entity::EntityType::TYPE_ACTOR) {
    return;
  }

  TransportActor *transport_actor = TransportActorManager::GetInstance()->Get(event->id_);
  if (transport_actor == NULL) {
    LOG_ERROR("获取 TransportActor(%lu) 失败", event->id_);
    return;
  }

  core::uint64 transport_npc_id = transport_actor->GetTransportNpc();
  if (transport_npc_id == 0) {
    return;
  }

  TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(transport_npc_id);
  if (transport_npc == NULL) {
    LOG_ERROR("获取 TransportNpc(%lu) 失败", transport_npc_id);
    return;
  }

  GameActor *game_actor = transport_actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->id_);
    return;
  }

  GameNpc *game_npc = transport_npc->GetNpc();
  if (game_npc == NULL) {
    LOG_ERROR("获取 GameNpc(%lu) 失败", transport_npc_id);
    return;
  }

  if (game_actor->GetScene() == game_npc->GetScene()) {
    const NpcCell *cell =
      game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(game_npc->GetTemplateID());
    if(cell == NULL) {
      LOG_ERROR("获取 NpcCell(%u) 失败", game_npc->GetTemplateID());
      return;
    }

    core::int32 distance = global::DistanceCalculator::GetInstance()->Distance(
        game_actor->GetPosx(), game_actor->GetPosy(),
        game_npc->GetPosx(), game_npc->GetPosy());
    if (transport_npc->GetLost() == false && distance > cell->attack_distance_) {
      transport_npc->Lose();
    } else if (transport_npc->GetLost() == true && distance <= cell->attack_distance_) {
      transport_npc->Regain();
    }
  } else if (transport_npc->GetLost() == false) {
    transport_npc->Lose();
  }
}

void TransportEventHandler::OnEventSceneBroadcastCreateRole(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventSceneBroadcastCreateRole) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSceneBroadcastCreateRole *event =
    (event::EventSceneBroadcastCreateRole *)message;

  if (event->actors_.empty() == true) {
    return;
  }

  if (event->type_ != entity::EntityType::TYPE_NPC) {
    return;
  }

  TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(event->id_);
  if (transport_npc != NULL) {
    gateway::protocol::MessageTransportAoiSynchronize message;
    gateway::protocol::TransportAoiData aoi;
    aoi.__set_npc_id_(global::ToString(event->id_));
    aoi.__set_master_name_(transport_npc->GetMasterName());
    message.aoi_data_.push_back(aoi);
    GameServerSingleton::GetInstance().MulticastMessage(message,
        gateway::protocol::MessageType::MESSAGE_TRANSPORT_AOI_SYNCHRONIZE,
        (core::uint64 *)&event->actors_[0], event->actors_.size());
  }
}

void TransportEventHandler::OnEventSceneCreateAoiRoles(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventSceneCreateAoiRoles) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSceneCreateAoiRoles *event =
    (event::EventSceneCreateAoiRoles *)message;

  if (event->roles_.empty() == true) {
    return;
  }

  TransportActor *transport_actor = TransportActorManager::GetInstance()->Get(event->actor_);
  if (transport_actor == NULL) {
    LOG_ERROR("获取 TransportActor(%lu) 失败");
    return;
  }

  gateway::protocol::MessageTransportAoiSynchronize synchronize;
  gateway::protocol::TransportAoiData aoi;

  for (size_t pos = 0; pos < event->roles_.size(); ++pos) {
    if (event->roles_[pos].type_ != entity::EntityType::TYPE_NPC) {
      continue;
    }
    TransportNpc *transport_npc = TransportNpcManager::GetInstance()->Get(event->roles_[pos].id_);
    if (transport_npc) {
      aoi.__set_npc_id_(global::ToString(event->roles_[pos].id_));
      aoi.__set_master_name_(transport_npc->GetMasterName());
      synchronize.aoi_data_.push_back(aoi);
      if(synchronize.aoi_data_.size() >= kMaxCreateSize_) {
        transport_actor->SendMessage(synchronize,
            gateway::protocol::MessageType::MESSAGE_TRANSPORT_AOI_SYNCHRONIZE);
        synchronize.aoi_data_.clear();
      }
    }
  }

  transport_actor->SendMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_TRANSPORT_AOI_SYNCHRONIZE);
}

}  // namespace transport

}  // namespace server

}  // namespace game

