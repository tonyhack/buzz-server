//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-22 14:18:56.
// File name: npc_create_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/buff/npc_create_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "game_server/server/extension/buff/buff_role_pool.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "global/channel.h"

namespace game {

namespace server {

namespace buff {

NpcCreateEventHandler::NpcCreateEventHandler() {}
NpcCreateEventHandler::~NpcCreateEventHandler() {}

bool NpcCreateEventHandler::Initialize() {
  // Follow 所有 npc 创建完成事件
  this->conn_create_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_NPC_CREATE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(
        &NpcCreateEventHandler::OnEventCreate, this, _1, _2, _3, _4));

  this->conn_destory_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_NPC_DESTROY, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(
        &NpcCreateEventHandler::OnEventDestory, this, _1, _2, _3, _4));

  return true;
}

void NpcCreateEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_create_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_destory_);
}

void NpcCreateEventHandler::OnEventCreate(core::uint64 channel,
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

  // 是否存在
  BuffRole *role = BuffRoleManager::GetInstance()->GetNpc(event->id_);
  if(role) {
    global::LogError("%s:%d (%s) Npc(%lu) buff扩展已存在 BuffRoleManager",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 分配buff角色对象
  role = BuffRolePool::GetInstance()->Allocate();
  if(role == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) 分配 BuffRole 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 初始化buff角色
  if(role->Initialize(npc) == false) {
    BuffRolePool::GetInstance()->Deallocate(role);
    global::LogError("%s:%d (%s) Npc(%lu) 初始化 BuffRole 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 加入buff角色对象管理器
  if(BuffRoleManager::GetInstance()->AddNpc(role) == false) {
    role->Finalize();
    BuffRolePool::GetInstance()->Deallocate(role);
    global::LogError("%s:%d (%s) Npc(%lu) 加入 BuffRole 到管理器失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // BUFF扩展设置为加载成功
  npc->GetLoadProgress().LoadExtensionFinish(ExtensionType::BUFF);
}

void NpcCreateEventHandler::OnEventDestory(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventNpcDestory) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventNpcDestory *event = (event::EventNpcDestory *)message;

  // 管理器中删除 buff角色对象
  BuffRole *role = BuffRoleManager::GetInstance()->RemoveNpc(event->id_);
  if(role == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) 移除buff角色对象失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 回收对象
  role->Finalize();
  BuffRolePool::GetInstance()->Deallocate(role);
}

}  // namespace buff

}  // namespace server

}  // namespace game

