//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-12 18:25:28.
// File name: playing_event_handler.cc
//
// Description:
//
#include "game_server/server/extension/task/playing_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_item_event_types.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

PlayingEventHandler::PlayingEventHandler() {}
PlayingEventHandler::~PlayingEventHandler() {}

bool PlayingEventHandler::Initialize() {
  // Follow 关注道具使用事件
  this->conn_finish_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_PLAYING_ACTOR_FINISH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&PlayingEventHandler::OnFinishEvent,
        this, _1, _2, _3, _4));
 return true;
}

void PlayingEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_finish_);
}

void PlayingEventHandler::OnFinishEvent(core::uint64 channel, core::uint8 channel_type,
        const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventPlayingActorFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventPlayingActorFinish *event = (event::EventPlayingActorFinish *)message;
  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_PLAYING_ACTOR_FINISH, message, size);
}

}  // namespace task

}  // namespace server

}  // namespace game

