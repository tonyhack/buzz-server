//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 15:42:51.
// File name: actor_login_event_handler.cc
//
// Description:
// Define class ActorLoginEventHandler.
//

#include "game_server/server/extension/python/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/python/python_actor.h"
#include "game_server/server/extension/python/python_actor_manager.h"
#include "game_server/server/extension/python/python_actor_pool.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace python {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_actor_login_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_LOGIN,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR, 
        boost::bind(&ActorLoginEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_login_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) Python扩展登陆失败，获取 GameActor失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 玩家对象是否已存在于管理器中
  PythonActorPtr python_actor = PythonActorManager::GetInstance()->Get(event->id_);
  if(python_actor.get() != NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) Python扩展已存在 PythonActorManager",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 从对象池中分配一个Python玩家对象
  python_actor = PythonActorPool::GetInstance()->Allocate();
  if(python_actor.get() == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 分配 PythonActorPtr 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 初始化 Python 对象
  if(python_actor->Initialize(actor) == false) {
    PythonActorPool::GetInstance()->Deallocate(python_actor);
    global::LogError("%s:%d (%s) 玩家(%lu) 初始化 PythonActor 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 加入 Python 对象
  if(PythonActorManager::GetInstance()->Add(python_actor) == false) {
    python_actor->Finalize();
    PythonActorPool::GetInstance()->Deallocate(python_actor);
    global::LogError("%s:%d (%s) 玩家(%lu) 加入 PythonActor 到管理器失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  load_finish.__set_extension_type_(ExtensionType::PYTHON);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  global::LogDebug("玩家(%lu, %s) Python扩展加载成功",
      actor->GetID(), actor->GetName().c_str());
}

}  // namespace python

}  // namespace server

}  // namespace game

