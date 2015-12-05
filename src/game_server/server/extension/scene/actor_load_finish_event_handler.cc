//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-09 18:05:03.
// File name: actor_load_finish_event_handler.cc
//
// Description:
// Define class ActorLoadFinishEventHandler.
//

#include "game_server/server/extension/scene/actor_load_finish_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/extension/scene/scene_extension.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "global/channel.h"
#include "global/game_misc.h"

namespace game {

namespace server {

namespace scene {

ActorLoadFinishEventHandler::ActorLoadFinishEventHandler() {}
ActorLoadFinishEventHandler::~ActorLoadFinishEventHandler() {}

bool ActorLoadFinishEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOAD_FINISH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLoadFinishEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLoadFinishEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLoadFinishEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorLoadFinish *event = (event::EventActorLoadFinish *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 登陆失败，无法从 GameActorManager 获取 GameActor",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 从场景管理器中获取场景
  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    scene = SceneManager::GetInstance()->GetScene(actor->GetPreMap());
    if(scene == NULL) {
      global::LogError("%s:%d (%s) 玩家(%lu) 登陆失败，无法获取 scene(%d) 和 pre_map(%d)",
          __FILE__, __LINE__, __FUNCTION__, event->id_,
          (core::uint32)actor->GetScene(), actor->GetPreMap());
      return ;
    }
    actor->SetScene(actor->GetPreMap());
    actor->SetPosition(actor->GetPrePosx(), actor->GetPrePosy());
  }

  if(scene->CheckWalkBlock(actor->GetPosx(), actor->GetPosy()) == true) {
    core::uint32 x = actor->GetPosx();
    core::uint32 y = actor->GetPosy();
    if(scene->GetNearWalkPosition(x, y) == false) {
      // 获取本场景配置
      const ServerMapConfigure *server_map_configure =
        Configure::GetInstance()->GetServerMapConfigure();
      if(server_map_configure == NULL) {
        global::LogError("%s:%d (%s) 找不到配置 ServerMapConfigure",
            __FILE__, __LINE__, __FUNCTION__);
        return ;
      }
      const MapInfo *map_info = server_map_configure->GetMapInfo(
          scene->GetMapID());
      if(map_info == NULL) {
        LOG_ERROR("场景[%d]配置没有找到", scene->GetMapID());
        return ;
      }
      x = map_info->default_area_.x_;
      y = map_info->default_area_.y_;
      if(scene->CheckWalkBlock(x, y) == true) {
        global::LogError("%s:%d (%s) 玩家(%lu) 寻找坐标失败",
            __FILE__, __LINE__, __FUNCTION__, event->id_);
        return ;
      }
    }
    actor->SetPosition(x, y);
  }

  // 登陆场景
  if(scene->AddEntity(actor) == false) {
    global::LogError("%s:%d (%s) 玩家(%lu) 登陆失败，加入场景失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 如果玩家进入场景时是死亡状态，复活玩家
  if(actor->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
    // 玩家对象
    SceneActor *scene_actor =
      scene::SceneExtension::GetInstance()->GetSceneActor(event->id_);
    if(scene_actor == NULL) {
      global::LogError("%s:%d (%s) 找不到场景玩家对象(%d)",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }
    scene_actor->Revive(true);
  }

  global::LogDebug("玩家(%lu, %s) 载入场景成功",
      actor->GetID(), actor->GetName().c_str());
}

}  // namespace scene

}  // namespace server

}  // namespace game


