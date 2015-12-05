//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-03 10:24:05.
// File name: npc_create_destory_event_handler.cc
//
// Description:
// Define class NpcCreateDestoryEventHandler.
//

#include "game_server/server/extension/python/npc_create_destory_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/python/python_npc.h"
#include "game_server/server/extension/python/python_npc_manager.h"
#include "game_server/server/extension/python/python_npc_pool.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "global/channel.h"

namespace game {

namespace server {

namespace python {

NpcCreateDestoryEventHandler::NpcCreateDestoryEventHandler() {}
NpcCreateDestoryEventHandler::~NpcCreateDestoryEventHandler() {}

bool NpcCreateDestoryEventHandler::Initialize() {
  // Follow 所有 npc 创建事件
  this->connection_create_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_NPC_CREATE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(
        &NpcCreateDestoryEventHandler::OnCreateEvent, this, _1, _2, _3, _4));
  this->connection_destory_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_NPC_DESTROY, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(
        &NpcCreateDestoryEventHandler::OnDestoryEvent, this, _1, _2, _3, _4));

  return true;
}

void NpcCreateDestoryEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_create_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_destory_);
}

void NpcCreateDestoryEventHandler::OnCreateEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventNpcCreate) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventNpcCreate *event = (event::EventNpcCreate *)message;

  // 从 NPC 管理器中获取 NPC 对象
  GameNpc *npc = GameNpcManager::GetInstance()->Get(event->id_);
  if(npc == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) 获取 GameNpc 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // NPC 对象是否已存在于管理器中
  PythonNpcPtr python_npc = PythonNpcManager::GetInstance()->Get(event->id_);
  if(python_npc.get() != NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) Python扩展已存在",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 从对象池中分配一个 PythonNpc 对象
  python_npc = PythonNpcPool::GetInstance()->Allocate();
  if(python_npc.get() == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) 分配 PythonNpcPtr 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 初始化 PythonNpc 对象
  if(python_npc->Initialize(npc) == false) {
    PythonNpcPool::GetInstance()->Deallocate(python_npc);
    global::LogError("%s:%d (%s) Npc(%lu) 初始化 PythonNpcPtr 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 加入 PythonNpc 对象
  if(PythonNpcManager::GetInstance()->Add(python_npc) == false) {
    python_npc->Finalize();
    PythonNpcPool::GetInstance()->Deallocate(python_npc);
    global::LogError("%s:%d (%s) Npc(%lu) 加入 PythonNpc 到管理器失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // Python 扩展设置为加载成功
  npc->GetLoadProgress().LoadExtensionFinish(ExtensionType::PYTHON);
}

void NpcCreateDestoryEventHandler::OnDestoryEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventNpcDestory) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventNpcDestory *event = (event::EventNpcDestory *)message;

  // 从管理器中删除
  PythonNpcPtr python_npc = PythonNpcManager::GetInstance()->Remove(event->id_);
  if(python_npc.get() == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) 管理器中删除 PythonNpc 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 放回对象池
  python_npc->Finalize();
  PythonNpcPool::GetInstance()->Deallocate(python_npc);
}

}  // namespace python

}  // namespace server

}  // namespace game


