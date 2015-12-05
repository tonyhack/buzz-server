//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-22 14:06:49.
// File name: npc_create_event_handler.cc
//
// Description:
// Define class NpcCreateEventHandler.
//

#include "game_server/server/extension/scene/npc_create_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"
#include "game_server/server/extension/scene/scene_npc_pool.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "global/channel.h"

namespace game {

namespace server {

namespace scene {

NpcCreateEventHandler::NpcCreateEventHandler() {}
NpcCreateEventHandler::~NpcCreateEventHandler() {}

bool NpcCreateEventHandler::Initialize() {
  // Follow 所有 npc 创建事件
  this->connection_create_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_NPC_CREATE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&NpcCreateEventHandler::OnCreateEvent,
        this, _1, _2, _3, _4));
  this->connection_destory_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_NPC_DESTROY, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&NpcCreateEventHandler::OnDestoryEvent,
        this, _1, _2, _3, _4));

  return true;
}

void NpcCreateEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_create_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_destory_);
}

void NpcCreateEventHandler::OnCreateEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventNpcCreate) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventNpcCreate *event = (event::EventNpcCreate *)message;

  // 从 npc 管理器中获取 npc 对象
  GameNpc *npc = GameNpcManager::GetInstance()->Get(event->id_);
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 无法从 GameNpcManager 获取 GameNpc",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  SceneNpc *scene_npc = SceneNpcPool::GetInstance()->Allocate();
  if(scene_npc == NULL) {
    global::LogError("%s:%d (%s) 分配 SceneNpc 对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(scene_npc->Initialize(npc, event->reborn_, event->reborn_secs_) == false) {
    global::LogError("%s:%d (%s) 初始化 SceneNpc 对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(SceneNpcManager::GetInstance()->Add(scene_npc) == false) {
    global::LogError("%s:%d (%s) SceneNpc 对象 加入管理器失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 场景扩展设置为加载成功
  npc->GetLoadProgress().LoadExtensionFinish(ExtensionType::SCENE);
}

void NpcCreateEventHandler::OnDestoryEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventNpcDestory) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventNpcDestory *event = (event::EventNpcDestory *)message;

  SceneNpc *scene_npc = SceneNpcManager::GetInstance()->Remove(event->id_);
  if(scene_npc == NULL) {
    global::LogError("%s:%d (%s) 无法从 SceneNpcManager 中删除 SceneNpc",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  scene_npc->Finalize();
  SceneNpcPool::GetInstance()->Deallocate(scene_npc);
}

}  // namespace scene

}  // namespace server

}  // namespace game

