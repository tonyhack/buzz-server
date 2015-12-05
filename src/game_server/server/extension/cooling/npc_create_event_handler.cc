//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-22 14:34:43.
// File name: npc_create_event_handler.cc
//
// Description:
// Define class NpcCreateEventHandler
//

#include "game_server/server/extension/cooling/npc_create_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/cooling/cooling_role.h"
#include "game_server/server/extension/cooling/cooling_role_manager.h"
#include "game_server/server/extension/cooling/cooling_role_pool.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "global/channel.h"

namespace game {

namespace server {

namespace cooling {

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
  CoolingRole *role = CoolingRoleManager::GetInstance()->GetNpc(event->id_);
  if(role) {
    global::LogError("%s:%d (%s) Npc(%lu) cooling扩展已存在",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 分配 CoolingRole 对象
  role = CoolingRolePool::GetInstance()->Allocate();
  if(role == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) 分配 CoolingRole 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 初始化
  if(role->Initialize(npc, CoolingRole::CoolingVector()) == false) {
    CoolingRolePool::GetInstance()->Deallocate(role);
    global::LogError("%s:%d (%s) Npc(%lu) 初始化 CoolingRole 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 加入 CoolingRole 管理器
  if(CoolingRoleManager::GetInstance()->AddNpc(role) == false) {
    role->Finalize();
    CoolingRolePool::GetInstance()->Deallocate(role);
    global::LogError("%s:%d (%s) Npc(%lu) CoolingRole 加入管理器失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // Cooling扩展设置为加载成功
  npc->GetLoadProgress().LoadExtensionFinish(ExtensionType::COOLING);
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

  // 管理器中删除 CoolingRole 角色
  CoolingRole *role = CoolingRoleManager::GetInstance()->RemoveNpc(event->id_);
  if(role == NULL) {
    global::LogError("%s:%d (%s) Npc(%lu) 移除cooling角色对象失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 回收对象
  role->Finalize();
  CoolingRolePool::GetInstance()->Deallocate(role);
}

}  // namespace cooling

}  // namespace server

}  // namespace game

