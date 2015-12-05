//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-19 19:37:56.
// File name: role_killed_event_handler.cc
//
// Description:
// Define class RoleKilledEventHandler.
//

#include "game_server/server/extension/scene/role_killed_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"
#include "game_server/server/extension/scene/scene_extension.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/session_channel.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace scene {

RoleKilledEventHandler::RoleKilledEventHandler() {}
RoleKilledEventHandler::~RoleKilledEventHandler() {}

bool RoleKilledEventHandler::Initialize() {
  // Follow 所有被杀事件
  this->conn_actor_killed_ = 
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_ROLE_KILLED, coresh::Communicator::GetChannelAll(),
        event::ChannelType::CHANNEL_ACTOR, boost::bind(&RoleKilledEventHandler::OnActorEvent,
          this, _1, _2, _3, _4));
  this->conn_npc_killed_ = 
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_ROLE_KILLED, coresh::Communicator::GetChannelAll(),
        event::ChannelType::CHANNEL_NPC, boost::bind(&RoleKilledEventHandler::OnNpcEvent,
          this, _1, _2, _3, _4));

  return true;
}

void RoleKilledEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_killed_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_killed_);
}

void RoleKilledEventHandler::OnActorEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleKilled) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  event::EventRoleKilled *event = (event::EventRoleKilled*)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return;
  }

  // 从场景玩家管理器中获取玩家对象
  SceneActor *actor = SceneExtension::GetInstance()->GetSceneActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到场景玩家对象(%lu)",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  if(event->attacker_type_ == entity::EntityType::TYPE_ACTOR) {
    SceneActor *attacker_actor =
      SceneExtension::GetInstance()->GetSceneActor(event->attacker_id_);
    if(attacker_actor == NULL) {
      global::LogError("%s:%d (%s) 找不到场景攻击玩家对象(%lu)",
          __FILE__, __LINE__, __FUNCTION__, event->attacker_id_);
      return ;
    }
    // 处理被杀死
    actor->AfterKilled(attacker_actor);
  } else if(event->attacker_type_ == entity::EntityType::TYPE_NPC) {
    // NPC对象
    SceneNpc *attacker_npc = SceneNpcManager::GetInstance()->Get(event->attacker_id_);
    if(attacker_npc == NULL) {
      global::LogError("%s:%d (%s) 获取 SceneNpc 对象失败",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    actor->AfterKilledByNpc(attacker_npc); 
  }
}

void RoleKilledEventHandler::OnNpcEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleKilled) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled*)message;

  if(event->type_ != entity::EntityType::TYPE_NPC) {
    return ;
  }

  // 从场景玩家管理器中获取玩家对象
  SceneNpc *npc = SceneNpcManager::GetInstance()->Get(event->id_);
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 找不到场景npc对象(%lu)",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  } 
  Scene *scene = npc->GetScene();
  if(scene == NULL) {
    LOG_ERROR("场景没有找到");
    return ;
  }

  GameRole *killer = NULL;
  if(event->attacker_type_ == entity::EntityType::TYPE_ACTOR) {
    killer = GameActorManager::GetInstance()->GetActor(event->attacker_id_);
    if(killer != NULL && scene->GetWorldBossID() == (core::uint64)event->id_) {
      GameActor *actor = (GameActor *)killer;
      session::protocol::MessageSceneWorldBossKilledNotice message;
      message.__set_killer_(actor->GetName());
      message.__set_id_(scene->GetSceneID());
      SessionChannel::SendMessage(message,
          session::protocol::GameMessageType::MESSAGE_SCENE_WORLD_BOSS_KILLED_NOTICE);
      scene->SetWorldBossID(0);
      LOG_DEBUG("通知session 世界boss被杀死");
    }

    // 获取场景玩家对象，处理连斩
    SceneActor *scene_actor = SceneExtension::GetInstance()->GetSceneActor(event->attacker_id_);
    if(scene_actor == NULL) {
      LOG_ERROR("玩家(%lu)没有找到", event->attacker_id_);
      return ;
    }
    scene_actor->KilledNpc(npc);
  } else if(event->attacker_type_ == entity::EntityType::TYPE_NPC) {
    killer = GameNpcManager::GetInstance()->Get(event->attacker_id_);
  }

  if(killer != NULL) {
    npc->OnEvent(event::EventType::EVENT_ROLE_KILLED, message, size);
    npc->DropItems(killer);
    npc->LoseExp(killer);
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game

