//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-07 03:43:34.
// File name: scene_event_handler.cc
//
// Description:
//
#include "game_server/server/extension/task/scene_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_scene_event_types.h"

namespace game {

namespace server {

namespace task {

SceneEventHandler::SceneEventHandler() {}
SceneEventHandler::~SceneEventHandler() {}

bool SceneEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_gather_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_SCENE_GATHER,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&SceneEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void SceneEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_gather_);
}

void SceneEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneGather) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventSceneGather *event = (event::EventSceneGather *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_SCENE_GATHER, message, size);
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }
}

}  // namespace task

}  // namespace server

}  // namespace game
