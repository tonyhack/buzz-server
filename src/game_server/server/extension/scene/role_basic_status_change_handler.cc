//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-16 15:22:26.
// File name: role_basic_status_change_handler.cc
//
// Description:
// Define class RoleBasicStatusChangeHandler.
//
#include "game_server/server/extension/scene/role_basic_status_change_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/extension/scene/scene_extension.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"
#include "game_server/server/extension/scene/scene_npc_pool.h"
#include "global/channel.h"
#include "global/game_misc.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

RoleBasicStatusChangeHandler::RoleBasicStatusChangeHandler() {}
RoleBasicStatusChangeHandler::~RoleBasicStatusChangeHandler() {}

bool RoleBasicStatusChangeHandler::Initialize() {
  // Follow 所有玩家状态转换事件
  this->conn_actor_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BASIC_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&RoleBasicStatusChangeHandler::OnActorEvent,
      this, _1, _2, _3, _4));
  // Follow 所有NPC状态转换事件
  this->conn_npc_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BASIC_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&RoleBasicStatusChangeHandler::OnNpcEvent,
      this, _1, _2, _3, _4));
  return true;
}

void RoleBasicStatusChangeHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_);
}

void RoleBasicStatusChangeHandler::OnActorEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBasicStatusChange) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventBasicStatusChange *event = (event::EventBasicStatusChange *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  // 玩家对象
  SceneActor *actor =
    scene::SceneExtension::GetInstance()->GetSceneActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到场景玩家对象(%d)",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  if(event->from_ != entity::RoleBasicStatus::STATUS_DIE &&
      event->to_ == entity::RoleBasicStatus::STATUS_DIE) {
    actor->OnDeath();
  }

  if(event->from_ == entity::RoleBasicStatus::STATUS_ZAZEN &&
      event->to_ != entity::RoleBasicStatus::STATUS_ZAZEN) {
    ZazenActor &zazen = actor->GetZazen();
    zazen.StopTime();
  }
}

void RoleBasicStatusChangeHandler::OnNpcEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBasicStatusChange) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventBasicStatusChange *event = (event::EventBasicStatusChange *)message;

  if(event->type_ != entity::EntityType::TYPE_NPC) {
    return ;
  }

  // NPC对象
  SceneNpc *scene_npc = SceneNpcManager::GetInstance()->Get(event->id_);
  if(scene_npc == NULL) {
    global::LogError("%s:%d (%s) 获取 SceneNpc 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  Scene *scene = scene_npc->GetScene();
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 获取 SceneNpc 所在场景失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  GameNpc *npc = scene_npc->GetGameNpc();
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 获取 GameNpc 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // Npc 模板表配置
  const NpcConfigure &configure = game::server::Configure::GetInstance()->GetNpcConfigure();
  const NpcCell *cell = configure.GetNpc(npc->GetTemplateID());
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 配置中找不到npc(%u)",
        __FILE__, __LINE__, __FUNCTION__, npc->GetTemplateID());
    return ;
  }

  if(event->from_ != entity::RoleBasicStatus::STATUS_DIE &&
      event->to_ == entity::RoleBasicStatus::STATUS_DIE) {
    // 如果需要重生
    if(scene_npc->CheckNeedReborn()) {
      core::uint x = 0, y = 0;
      scene_npc->GetBornPosition(x, y);
      // 召唤NPC
      scene->GetEntityBuilder().SummonNpc(npc->GetTemplateID(),
          x, y, 0, 0, scene_npc->GetBornDir(), scene_npc->GetRebornSecs());
    }
  }

  scene->GetEntityBuilder().DisappearNpc(event->id_, cell->disappear_time_);
}

}  // namespace scene

}  // namespace server

}  // namespace game

