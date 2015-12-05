//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-31 19:46:16.
// File name: extension_load_event_handler.cc
//
// Description:
// Define class ExtensionLoadEventHandler.
//

#include "game_server/server/extension/effect/extension_load_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/effect/effect_role.h"
#include "game_server/server/extension/effect/effect_role_manager.h"
#include "game_server/server/extension/effect/effect_role_pool.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "global/channel.h"

namespace game {

namespace server {

namespace effect {

ExtensionLoadEventHandler::ExtensionLoadEventHandler() {}
ExtensionLoadEventHandler::~ExtensionLoadEventHandler() {}

bool ExtensionLoadEventHandler::Initialize() {
  // Follow 所有 npc 创建完成事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ExtensionLoadEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ExtensionLoadEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ExtensionLoadEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventActorExtensionLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 或 channel_type 错误");
    return;
  }

  event::EventActorExtensionLoadFinish *event = (event::EventActorExtensionLoadFinish *)message;

  if (event->extension_type_ == ExtensionType::SOUL) {
    // 从玩家管理器中获取玩家对象
    GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
    if(actor == NULL) {
      LOG_ERROR("玩家(%lu) 效果扩展登陆失败，获取 GameActor 失败", event->id_);
      return;
    }

    // 效果玩家对象是否已存在管理器中
    EffectRole *role = EffectRoleManager::GetInstance()->GetActor(event->id_);
    if(role) {
      global::LogError("%s:%d (%s) 玩家(%lu) 效果扩展已存在 EffectRoleManager",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }

    // 从对象池分配一个效果玩家对象
    role = EffectRolePool::GetInstance()->Allocate();
    if(role == NULL) {
      global::LogError("%s:%d (%s) 玩家(%lu) 分配 EffectRole 失败",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }

    // 初始化效果玩家
    if(role->Initialize(actor) == false) {
      EffectRolePool::GetInstance()->Deallocate(role);
      global::LogError("%s:%d (%s) 玩家(%lu) 初始化 EffectRole 失败",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }

    // 加入效果玩家管理器
    if(EffectRoleManager::GetInstance()->AddActor(role) == false) {
      role->Finalize();
      EffectRolePool::GetInstance()->Deallocate(role);
      global::LogError("%s:%d (%s) 玩家(%lu) 加入 EffectRole 到管理器失败",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }

    // 发送扩展加载成功事件
    event::EventActorExtensionLoadFinish load_finish;
    load_finish.__set_id_(event->id_);
    load_finish.__set_extension_type_(ExtensionType::EFFECT);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
        event::ChannelType::CHANNEL_ACTOR, &load_finish,
        sizeof(event::EventActorExtensionLoadFinish));

    global::LogDebug("玩家(%lu, %s) 效果扩展加载成功",
        actor->GetID(), actor->GetName().c_str());
  }
}

}  // namespace effect

}  // namespace server

}  // namespace game

