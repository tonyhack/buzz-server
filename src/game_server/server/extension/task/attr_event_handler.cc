//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-11 23:58:46.
// File name: attr_event_handler.cc
//
// Description:
//
#include "game_server/server/extension/task/attr_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_attr_event_types.h"
#include "global/logging.h"
#include "entity/constants_constants.h"

namespace game {

namespace server {

namespace task {

AttrEventHandler::AttrEventHandler() {}
AttrEventHandler::~AttrEventHandler() {}

bool AttrEventHandler::Initialize() {
  // Follow 关注精灵培养事件
  this->conn_spirit_feed_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ATTR_SPIRIT_FEED, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AttrEventHandler::OnSpiritFeedEvent,
        this, _1, _2, _3, _4));
  // Follow 关注星级提升事件
  this->conn_constellation_up_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ATTR_CONSTELLATION_UP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,boost::bind(
        &AttrEventHandler::OnEventConstellationUpGrade, this, _1, _2, _3, _4));
  return true;
}

void AttrEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_spirit_feed_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_constellation_up_);
}

void AttrEventHandler::OnSpiritFeedEvent(core::uint64 channel, core::uint8 channel_type,
        const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventAttrSpiritFeed) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventAttrSpiritFeed *event = (event::EventAttrSpiritFeed *)message;
  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_ATTR_SPIRIT_FEED, message, size);
}

void AttrEventHandler::OnEventConstellationUpGrade(
    core::uint64 channel, core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventAttrConstellationUpgrade) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventAttrConstellationUpgrade *event = (event::EventAttrConstellationUpgrade *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_ATTR_CONSTELLATION_UP, message, size);

}

}  // namespace task

}  // namespace server

}  // namespace game

