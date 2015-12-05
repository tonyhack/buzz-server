//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 11:54:08.
// File name: npc_create_event_handler.cc
//
// Description:
// Define class NpcCreateEventHandler.
//

#include "game_server/server/extension/skill/npc_create_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/skill/skill.h"
#include "game_server/server/extension/skill/skill_pool.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/extension/skill/skill_role_pool.h"

namespace game {

namespace server {

namespace skill {

NpcCreateEventHandler::NpcCreateEventHandler() {}
NpcCreateEventHandler::~NpcCreateEventHandler() {}

bool NpcCreateEventHandler::Initialize() {
  // Follow 所有 npc 创建完成事件
  this->connection_create_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_NPC_CREATE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&NpcCreateEventHandler::OnCreateEvent,
        this, _1, _2, _3, _4));

  // Follow 所有 npc 销毁事件
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
    global::LogError("%s:%d (%s) NPC(%lu) 无法从 GameNpcManager 获取 GameNpc",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // NPC配置
  const NpcCell *npc_cell =
    game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
        npc->GetTemplateID());
  if(npc_cell == NULL) {
    global::LogError("%s:%d (%s) NPC(%lu) 获取 NPC(%u) 配置失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_, npc->GetTemplateID());
    return ;
  }

  // 技能npc是否存在于管理器中
  SkillRole *skill_npc = SkillRoleManager::GetInstance()->GetNpc(event->id_);
  if(skill_npc != NULL) {
    global::LogError("%s:%d (%s) NPC(%lu) 技能扩展已存在 SkillRoleManager",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 分配技能NPC对象
  skill_npc = SkillRolePool::GetInstance()->Allocate();
  if(skill_npc == NULL) {
    global::LogError("%s:%d (%s) NPC(%lu) 分配 SkillNpc 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 初始化技能NPC对象
  if(skill_npc->Initialize(npc) == false) {
    SkillRolePool::GetInstance()->Deallocate(skill_npc);
    global::LogError("%s:%d (%s) NPC(%lu) 初始化 SkillRole 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 加入技能角色管理器
  if(SkillRoleManager::GetInstance()->Add(skill_npc) == false) {
    skill_npc->Finalize();
    SkillRolePool::GetInstance()->Deallocate(skill_npc);
    global::LogError("%s:%d (%s) NPC(%lu) 初始化 SkillRole 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // NPC技能
  std::vector<core::int32>::const_iterator iterator = npc_cell->skills_.begin();
  for(; iterator != npc_cell->skills_.end(); ++iterator) {
    Skill *skill = SkillPool::GetInstance()->Allocate();
    if(skill == NULL) {
      global::LogError("%s:%d (%s) NPC(%lu) 获取技能(%u)失败，无法分配对象",
          __FILE__, __LINE__, __FUNCTION__, event->id_, *iterator);
      continue ;
    }
    if(skill->Initialize(skill_npc) == false) {
      SkillPool::GetInstance()->Deallocate(skill);
      global::LogError("%s:%d (%s) NPC(%lu) 初始化技能(%u)失败",
          __FILE__, __LINE__, __FUNCTION__, event->id_, *iterator);
      continue ;
    }
    skill->SetID(*iterator);
    skill->SetLevel(1);
    if(skill_npc->AddSkill(skill) == false) {
      skill->Finalize();
      SkillPool::GetInstance()->Deallocate(skill);
      global::LogError("%s:%d (%s) NPC(%lu) 初始化技能(%u)失败",
          __FILE__, __LINE__, __FUNCTION__, event->id_, *iterator);
      continue ;
    }
    skill_npc->SkillStand(*iterator);
  }

  // 场景扩展设置为加载成功
  npc->GetLoadProgress().LoadExtensionFinish(ExtensionType::SKILL);
}

void NpcCreateEventHandler::OnDestoryEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventNpcDestory) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventNpcCreate *event = (event::EventNpcCreate *)message;

  // 从管理器中删除
  SkillRole *npc = SkillRoleManager::GetInstance()->RemoveNpc(event->id_);
  if(npc == NULL) {
    global::LogError("%s:%d (%s) NPC(%lu) 管理器中删除 SkillRole 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 放回对象池
  npc->Finalize();
  SkillRolePool::GetInstance()->Deallocate(npc);
}

}  // namespace skill

}  // namespace server

}  // namespace game

