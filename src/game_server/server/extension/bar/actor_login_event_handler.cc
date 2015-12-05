//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-03 20:16:17.
// File name: actor_login_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/bar/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/bar/bar_actor.h"
#include "game_server/server/extension/bar/bar_actor_manager.h"
#include "game_server/server/extension/bar/bar_actor_pool.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"

namespace game {

namespace server {

namespace bar {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_actor_login_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLoginEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_login_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 读条玩家对象是否已存在于管理器中
  BarActor *actor = BarActorManager::GetInstance()->Remove(event->id_);
  if(actor != NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 移动读条玩家对象失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 玩家对象是否存在
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(game_actor == NULL) {
    LOG_ERROR("玩家不存在[%lu]", event->id_);
    return ;
  }

  // 申请玩家读条对象
  actor = BarActorPool::GetInstance()->Allocate();
  if(actor == NULL) {
    LOG_ERROR("为读条玩家申请内存失败[%lu]", event->id_);
    return ;
  }

  // 初始化玩家进度条
  if(actor->Initialize(game_actor) == false) {
    LOG_ERROR("初始化玩家读条数据失败");
    BarActorPool::GetInstance()->Deallocate(actor);
    return ;
  }

  // 加入读条玩家 
  if(BarActorManager::GetInstance()->Add(actor) == false) {
    LOG_ERROR("加入读条玩家失败");
    BarActorPool::GetInstance()->Deallocate(actor);
    return ;
  }

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  load_finish.__set_extension_type_(ExtensionType::BAR);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));
  LOG_DEBUG("玩家(%lu) bar扩展加载成功", event->id_);
}

}  // namespace bar

}  // namespace server

}  // namespace game

