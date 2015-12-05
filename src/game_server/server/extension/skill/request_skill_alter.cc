//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-07 11:18:26.
// File name: request_skill_alter.cc
//
// Description:
// Define class RequestSkillAlter.
//

#include "game_server/server/extension/skill/request_skill_alter.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/skill_configure.h"
#include "game_server/server/extension/skill/skill_pool.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace skill {

RequestSkillAlter::RequestSkillAlter() {}
RequestSkillAlter::~RequestSkillAlter() {}

bool RequestSkillAlter::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_ADD_SKILL,
      boost::bind(&RequestSkillAlter::OnRequestAddSkill, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_REMOVE_SKILL,
      boost::bind(&RequestSkillAlter::OnRequestRemoveSkill, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_UPGRADE_SKILL,
      boost::bind(&RequestSkillAlter::OnRequestUpgradeSkill, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SKILL_INCREASE_ATTRIBUTE,
      boost::bind(&RequestSkillAlter::OnRequestSkillIncreaseAttribute, this, _1, _2));

  return true;
}

void RequestSkillAlter::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_ADD_SKILL);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_REMOVE_SKILL);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_UPGRADE_SKILL);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SKILL_INCREASE_ATTRIBUTE);
}

int RequestSkillAlter::OnRequestAddSkill(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestAddSkill) != size) {
    global::LogError("%s:%d (%s) 请求增加技能时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestAddSkill *request = (request::RequestAddSkill *)message;

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(request->skill_id_,
      request->level_);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 请求增加技能时，技能(%u, %u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->skill_id_, request->level_);
    return -1;
  }

  // 获取 SkillRole 对象
  SkillRole *role = NULL;

  if(request->type_ == entity::EntityType::TYPE_ACTOR) {
    if((cell->master_type_ & MasterType::ACTOR) == 0) {
      request->__set_result_(SkillResultType::ERROR_SKILL_MASTER);
      return 0;
    }
    role = SkillRoleManager::GetInstance()->GetActor(request->id_);
  } else if(request->type_ == entity::EntityType::TYPE_NPC) {
    if((cell->master_type_ & MasterType::NPC) == 0) {
      request->__set_result_(SkillResultType::ERROR_SKILL_MASTER);
      return 0;
    }
    role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
  } else {
    return -1;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求增加技能时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  Skill *skill = role->GetSkill(request->skill_id_);
  if(skill != NULL) {
    skill->SetLevel(request->level_);
    request->__set_result_(SkillResultType::SUCCESS);
  } else {
    skill = SkillPool::GetInstance()->Allocate();
    if(skill == NULL) {
      global::LogError("%s:%d (%s) 分配技能对象失败",
          __FILE__, __LINE__, __FUNCTION__);
      return -1;
    }
    if(skill->Initialize(role) == false) {
      LOG_ERROR("初始化 Skill 失败");
      return -1;
    }
    skill->SetID(request->skill_id_);
    skill->SetLevel(request->level_);
    if(role->AddSkill(skill) == false) {
      SkillPool::GetInstance()->Deallocate(skill);
      global::LogError("%s:%d (%s) 请求 Role(%d, %lu) 增加技能(%u,%u)失败",
          __FILE__, __LINE__, __FUNCTION__, request->type_,
          request->id_, request->skill_id_, request->level_);
      return -1;
    }
  }

  // 立即可用
  if(request->stand_) {
    role->SkillStand(skill->GetID());
  }

  request->__set_result_(SkillResultType::SUCCESS);

  return 0;
}

int RequestSkillAlter::OnRequestRemoveSkill(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestRemoveSkill) != size) {
    global::LogError("%s:%d (%s) 请求删除技能时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestRemoveSkill *request = (request::RequestRemoveSkill *)message;

  // 获取 SkillRole 对象
  SkillRole *role = NULL;

  switch(request->type_) {
    case entity::EntityType::TYPE_ACTOR:
      role = SkillRoleManager::GetInstance()->GetActor(request->id_);
      break;
    case entity::EntityType::TYPE_NPC:
      role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
      break;
    default:
      break;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求删除技能时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  // 删除技能
  Skill *skill = role->RemoveSkill(request->skill_id_);
  if(skill == NULL) {
    request->__set_result_(SkillResultType::ERROR_SKILL_LEVEL);
    return 0;
  }

  // 回收技能对象
  skill->Finalize();
  SkillPool::GetInstance()->Deallocate(skill);

  request->__set_result_(SkillResultType::SUCCESS);

  return 0;
}

int RequestSkillAlter::OnRequestUpgradeSkill(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestUpgradeSkill) != size) {
    global::LogError("%s:%d (%s) 请求升级技能时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestUpgradeSkill *request = (request::RequestUpgradeSkill *)message;

  // 获取 SkillRole 对象
  SkillRole *role = NULL;

  switch(request->type_) {
    case entity::EntityType::TYPE_ACTOR:
      role = SkillRoleManager::GetInstance()->GetActor(request->id_);
      break;
    case entity::EntityType::TYPE_NPC:
      role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
      break;
    default:
      break;
  }

  // 检测是否存在
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求升级技能时，Role(%d, %lu) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->type_, request->id_);
    return -1;
  }

  // 获取技能
  Skill *skill = role->GetSkill(request->skill_id_);
  if(skill == NULL) {
    request->__set_result_(SkillResultType::ERROR_SKILL_LEVEL);
    return 0;
  }

  // 获取下一级的技能
  core::uint32 level = skill->GetLevel() + request->upgrade_;

  // 获取技能配置
  const SkillCell *cell = Configure::GetInstance()->GetSkill(request->skill_id_,
      level);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 请求升级技能时，技能(%u, %u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, request->skill_id_, level);
    return -1;
  }

  // 设置技能等级
  skill->SetLevel(level);

  request->__set_result_(SkillResultType::SUCCESS);
  
  return 0;
}

int RequestSkillAlter::OnRequestSkillIncreaseAttribute(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSkillIncreaseAttribute) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSkillIncreaseAttribute *request =
    (request::RequestSkillIncreaseAttribute *)message;

  // 获取 SkillRole 对象
  SkillRole *role = NULL;

  switch(request->type_) {
    case entity::EntityType::TYPE_ACTOR:
      role = SkillRoleManager::GetInstance()->GetActor(request->id_);
      break;
    case entity::EntityType::TYPE_NPC:
      role = SkillRoleManager::GetInstance()->GetNpc(request->id_);
      break;
    default:
      break;
  }

  // 检测是否存在
  if(role == NULL) {
    LOG_ERROR("获取 SkillRole(%d, %lu) 失败", request->type_, request->id_);
    return -1;
  }

  // 获取技能
  Skill *skill = role->GetSkill(request->skill_id_);
  if(skill == NULL) {
    LOG_ERROR("获取 SkillRole(%d, %lu) 技能(%d) 失败",
        request->type_, request->id_, request->skill_id_);
    return -1;
  }

  skill->SetAttribute(request->attribute_,
      skill->GetAttribute(request->attribute_) + request->increment_);

  return 0;
}

}  // namespace skill

}  // namespace server

}  // namespace game

