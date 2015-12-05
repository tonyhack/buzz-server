//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-26 20:03:19.
// File name: ai_event_handler.cc
//
// Description:
//
#include "game_server/server/extension/ai/ai_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension/ai/ai_actor.h"
#include "game_server/server/extension/ai/ai_actor_manager.h"
#include "game_server/server/extension/ai/ai_actor_pool.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

AiEventHandler::AiEventHandler() {}
AiEventHandler::~AiEventHandler() {}

bool AiEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_actor_login_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AiEventHandler::OnLoginEvent,
        this, _1, _2, _3, _4));

  // Follow 所有玩家登出事件
  this->conn_actor_logout_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AiEventHandler::OnLogoutEvent,
        this, _1, _2, _3, _4));

  return true;
}

void AiEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_login_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_logout_);
}

void AiEventHandler::OnLoginEvent(core::uint64 channel, core::uint8 channel_type,
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
    global::LogError("%s:%d (%s) 玩家(%lu) ai 扩展登陆失败，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }
 
  // 检查玩家 AI 对象
  if(AiActorManager::GetInstance()->Get(event->id_) != NULL) {
     LOG_ERROR("玩家对象已经存在(%lu)", event->id_);
     return ;
  }

  // 分配AI对象
  AiActor *ai_actor = AiActorPool::GetInstance()->Allocate();
  if(ai_actor == NULL) {
    LOG_ERROR("AI 对象分配失败(%lu)", event->id_);
    return ;
  }

  if(ai_actor->Initialize(actor) == false) {
    LOG_ERROR("AI 对象初始化失败(%lu)", event->id_);
    AiActorPool::GetInstance()->Deallocate(ai_actor);
    return ;
  }

  // 加入到管理器中
  if(AiActorManager::GetInstance()->Add(ai_actor) == false) {
    LOG_ERROR("加入 AI 管理器 失败(%lu)", event->id_);
    ai_actor->Finalize();
    AiActorPool::GetInstance()->Deallocate(ai_actor);
    return ;
  }
  
  LOG_DEBUG("AI 模块加载成功");
 //发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  load_finish.__set_extension_type_(ExtensionType::AI);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));
}

void AiEventHandler::OnLogoutEvent(core::uint64 channel, core::uint8 channel_type,
   const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家离开 AI 失败，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  AiActor *ai_actor = AiActorManager::GetInstance()->Get(event->id_);
  if(ai_actor == NULL) {
    LOG_ERROR("玩家退出时，AI 玩家出错 (%lu)", event->id_);
    return ;
  }

  ai_actor->Finalize();
  if(AiActorManager::GetInstance()->Remove(event->id_) == false) {
    LOG_ERROR("玩家退出时，AI 玩家出错 (%lu)", event->id_);
    return ;
  }
  // 交还内存
  AiActorPool::GetInstance()->Deallocate(ai_actor);
}

} // namespace ai

} // namespace server

} // namepsace game

