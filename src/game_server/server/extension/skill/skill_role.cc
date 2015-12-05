//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-31 16:47:47.
// File name: skill_role.cc
//
// Description:
// Define class SkillRole.
//

#include "game_server/server/extension/skill/skill_role.h"

#include "entity/actor_types.h"
#include "entity/npc_types.h"
#include "entity/role_types.h"
#include "entity/camp_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/facade_request.h"
#include "game_server/server/extension/skill/skill_pool.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/request/game_cooling_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/distance_calculator.h"
#include "global/types.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace skill {

SkillRole::SkillRole() : role_(NULL), current_status_(NULL) {}
SkillRole::~SkillRole() {}

bool SkillRole::Initialize(GameRole *role) {
  if(role == NULL) {
    global::LogError("%s:%d (%s) 参数 role 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->role_ = role;

  if(this->fighting_status_.Initialize(this) == false) {
    LOG_ERROR("初始化 FightingStatus 失败");
    return false;
  }

  // 初始化各个状态
  if(this->idle_status_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) 初始化 SkillIdleStatus 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->boot_status_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) 初始化 SkillBootStatus 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->launch_status_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) 初始化 SkillLaunchStatus 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->finish_status_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) 初始化 SkillFinishStatus 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->GotoStatus(SkillStatusType::IDLE);

  this->SetSkillForm(entity::SkillFormType::COMMON);

  return true;
}

void SkillRole::Finalize() {
  if(this->current_status_) {
    this->current_status_->SetLeaving(true);
    this->current_status_->OnLeave();
    this->current_status_->SetLeaving(false);
  }
  this->current_status_ = NULL;

  this->idle_status_.Finalize();
  this->boot_status_.Finalize();
  this->launch_status_.Finalize();
  this->finish_status_.Finalize();

  this->fighting_status_.Finalize();

  SkillMap::iterator iterator = this->skills_.begin();
  for(; iterator != this->skills_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      SkillPool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->skills_.clear();

  this->role_ = NULL;
}

entity::EntityType::type SkillRole::GetType() const {
  if(this->role_ == NULL) {
    return entity::EntityType::TYPE_MAX;
  }
  return this->role_->GetType();
}

core::uint64 SkillRole::GetID() const {
  if(this->role_ == NULL) {
    return entity::EntityType::TYPE_MAX;
  }
  return this->role_->GetID();
}

Skill *SkillRole::GetSkill(core::uint32 skill_id) {
  SkillMap::iterator iterator = this->skills_.find(skill_id);
  if(iterator != this->skills_.end()) {
    return iterator->second;
  }

  return NULL;
}

const Skill *SkillRole::GetSkill(core::uint32 skill_id) const {
  SkillMap::const_iterator iterator = this->skills_.find(skill_id);
  if(iterator != this->skills_.end()) {
    return iterator->second;
  }

  return NULL;
}

bool SkillRole::AddSkill(Skill *skill) {
  if(skill == NULL) {
    global::LogError("%s:%d (%s) 参数 skill 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  SkillMap::iterator iterator = this->skills_.find(skill->GetID());
  if(iterator != this->skills_.end()) {
    global::LogError("%s:%d (%s) 增加技能失败，该技能(%u)已存在",
        __FILE__, __LINE__, __FUNCTION__, skill->GetID());
    return false;
  }

  this->skills_.insert(std::make_pair(skill->GetID(), skill));

  return true;
}

Skill *SkillRole::RemoveSkill(core::uint32 skill_id) {
  Skill *skill = NULL;

  SkillMap::iterator iterator = this->skills_.find(skill_id);
  if(iterator != this->skills_.end()) {
    skill = iterator->second;
    this->skills_.erase(iterator);
    this->SkillSit(skill_id);
  }

  return skill;
}

bool SkillRole::SkillStand(core::uint32 id) {
  Skill *skill = GetSkill(id);
  if(skill == NULL) {
    return false;
  }

  SkillVector::iterator iterator = this->stand_skills_.begin();
  for(; iterator != this->stand_skills_.end(); ++iterator) {
    if(*iterator == id) {
      return true;
    }
  }

  this->stand_skills_.push_back(id);
  return true;
}

bool SkillRole::SkillSit(core::uint32 id) {
  SkillVector::iterator iterator = this->stand_skills_.begin();
  for(; iterator != this->stand_skills_.end(); ++iterator) {
    if(*iterator == id) {
      this->stand_skills_.erase(iterator);
      Skill *skill = this->GetSkill(*iterator);
      if(skill) {
        // 技能由待用状态切换到收回状态时，打断BUFF
        const SkillCell *cell = Configure::GetInstance()->GetSkill(id, skill->GetLevel());
        if(cell) {
          std::vector<core::int32>::const_iterator iterator =
            cell->sit_break_buffs_.begin();
          for(; iterator != cell->sit_break_buffs_.end(); ++iterator) {
            if(FacadeRequest::GetInstance()->RemoveBuff(this->GetType(),
                  this->GetID(), *iterator) == false) {
              global::LogError("%s:%d (%s) 技能收回时，打断BUFF(%u) 失败",
                  __FILE__, __LINE__, __FUNCTION__, *iterator);
              continue;
            }
          }
        }
      }
      break;
    }
  }

  return true;
}

Skill *SkillRole::GetTempSkill(core::uint32 id) {
  SkillMap::iterator iterator = this->temp_skills_.find(id);
  if(iterator != this->temp_skills_.end()) {
    return iterator->second;
  }

  return NULL;
}

bool SkillRole::AddTempSkill(core::uint32 id, core::uint32 level) {
  const SkillCell *cell = Configure::GetInstance()->GetSkill(id, level);
  if(cell == NULL) {
    LOG_ERROR("请求增加技能时，技能(%u, %u) 不存在", id, level);
    return -1;
  }

  if(this->temp_skills_.find(id) != this->temp_skills_.end()) {
    return false;
  }

  Skill *skill = SkillPool::GetInstance()->Allocate();
  if(skill == NULL) {
    LOG_ERROR("分配 Skill对象 失败");
    return false;
  }

  if (skill->Initialize(this) == false) {
    LOG_ERROR("初始化 Skill对象 失败");
    return false;
  }

  skill->SetID(id);
  skill->SetLevel(level);
  this->temp_skills_.insert(std::make_pair(skill->GetID(), skill));

  return true;
}

bool SkillRole::RemoveTempSkill(core::uint32 id) {
  SkillMap::iterator iterator = this->temp_skills_.find(id);
  if(iterator == this->temp_skills_.end()) {
    return false;
  }

  Skill *skill = iterator->second;
  if(skill == NULL) {
    return false;
  }

  this->temp_skills_.erase(iterator);

  skill->Finalize();
  SkillPool::GetInstance()->Deallocate(skill);

  return true;
}

bool SkillRole::CheckSkillReady(core::uint32 id) const {
  if(this->GetSkillForm() == entity::SkillFormType::COMMON) {
    return this->CheckSkillStand(id);
  } else if(this->GetSkillForm() == entity::SkillFormType::TEMP) {
    return this->CheckSkillTemp(id);
  } else {
    return false;
  }
}

bool SkillRole::CheckSkillStand(core::uint32 id) const {
  SkillVector::const_iterator iterator = this->stand_skills_.begin();
  for(; iterator != this->stand_skills_.end(); ++iterator) {
    if(*iterator == id) {
      return true;
    }
  }

  return false;
}

bool SkillRole::CheckSkillTemp(core::uint32 id) const {
  return this->temp_skills_.find(id) != this->temp_skills_.end();
}

bool SkillRole::CheckBuffCondition(const SkillCell *cell) const {
  if (cell == NULL || this->role_ == NULL) {
    return false;
  }

  if (cell->buff_status_ == 0) {
    return true;
  }

  return FacadeRequest::GetInstance()->CheckBuffExist(this->GetType(),
      this->GetID(), cell->buff_status_);
}

bool SkillRole::CheckSoulCondition(const SkillCell *cell) const {
  if(cell == NULL || this->role_ == NULL) {
    return false;
  }

  if(cell->soul_status_ == SoulStatusType::ANY) {
    return true;
  }

  // 获取幻化状态
  core::int32 common_status =
    this->role_->GetAttribute(entity::RoleAoiFields::COMMON_STATUS);

  // 幻化状态
  if(cell->soul_status_ == SoulStatusType::SOUL) {
    return (common_status & entity::RoleCommonStatus::SOUL) != 0;
  }

  // 非幻化状态
  if(cell->soul_status_ == SoulStatusType::NOT_SOUL) {
    return (common_status & entity::RoleCommonStatus::SOUL) == 0;
  }

  return true;
}

void SkillRole::GotoStatus(SkillStatusType::type type) {
  if(this->current_status_ && this->current_status_->CheckLeaving() == false) {
    this->current_status_->SetLeaving(true);
    this->current_status_->OnLeave();
    this->current_status_->SetLeaving(false);
  }

  switch(type) {
    case SkillStatusType::IDLE:
      this->current_status_ = &this->idle_status_;
      break;
    case SkillStatusType::BOOT:
      this->current_status_ = &this->boot_status_;
      break;
    case SkillStatusType::LAUNCH:
      this->current_status_ = &this->launch_status_;
      break;
    case SkillStatusType::FLIGHT:
      this->current_status_ = this->current_status_;
      break;
    case SkillStatusType::FINISH:
      this->current_status_ = &this->finish_status_;
      break;
    default:
      break;
  }

  this->current_status_->OnEnter();
}

core::int32 SkillRole::GetSkillDistance(const SkillCell *cell) const {
  if(cell == NULL || this->role_ == NULL) {
    return core::kint32max;
  }

  const SkillContext &context = this->GetSkillContext();

  // 碰撞类按方向走，不需要检测距离
  if(cell->type_ == SkillType::COLLISION &&
      cell->target_type_ == TargetType::NONE) {
    return 0;
  }

  core::uint32 src_x = 0, src_y = 0, dest_x = 0, dest_y = 0;
  // 得到源点
  this->role_->GetPosition(src_x, src_y);

  if(cell->target_type_ == TargetType::ENTITY) {
    // 得到目标实体对象
    SkillRole *dest_role = SkillRoleManager::GetInstance()->Get(
        context.target_type_, context.target_id_);
    if(dest_role == NULL) {
      return core::kint32max;
    }
    // 获取目标点坐标
    GameRole *dest_game_role = dest_role->GetGameRole();
    if(dest_game_role == NULL) {
      return core::kint32max;
    }
    // 同场景判断
    if(this->role_->GetScene() != dest_game_role->GetScene()) {
      return core::kint32max;
    }
    dest_game_role->GetPosition(dest_x, dest_y);
  } else if(cell->target_type_ == TargetType::POSITION ||
      cell->target_type_ == TargetType::POSITION_SELF) {
    // 得到目标点坐标
    dest_x = context.x_;
    dest_y = context.y_;
  } else {
    return core::kint32max;
  }

  return global::DistanceCalculator::GetInstance()->Distance(
      src_x, src_y, dest_x, dest_y);
}

SkillResultType::type SkillRole::CheckTarget(const SkillCell *cell) const {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  if(cell->target_type_ == TargetType::ENTITY && 
      this->current_skill_.target_type_ == entity::EntityType::TYPE_NPC) {
    GameNpc *npc = GameNpcManager::GetInstance()->Get(this->current_skill_.target_id_);
    if(npc == NULL) {
      return SkillResultType::ERROR_ARGS;
    }
    if(npc->GetNpcType() != entity::NpcKind::MONSTER &&
        npc->GetNpcType() != entity::NpcKind::TRANSPORT) {
      return SkillResultType::ERROR_TARGET;
    }
  }

  return SkillResultType::SUCCESS;
}

SkillResultType::type SkillRole::CheckDistance(const SkillCell *cell) const {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  const Skill *skill = this->GetSkill(cell->id_);
  if (skill == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 得到与目标之间的距离
  core::int32 distance = this->GetSkillDistance(cell);

  if(distance == core::kint32max) {
    return SkillResultType::ERROR_UNKNOWN;
  }

  core::int32 distance_add =
    skill->GetAttribute(entity::SkillVarFields::DISTANCE_ADD_VALUE);

  if(distance > cell->distance_ + distance_add) {
    return SkillResultType::ERROR_DISTANCE;
  }

  return SkillResultType::SUCCESS;
}

SkillResultType::type SkillRole::CheckSingDistance(const SkillCell *cell) const {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  const Skill *skill = this->GetSkill(cell->id_);
  if (skill == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 得到与目标之间的距离
  core::int32 distance = this->GetSkillDistance(cell);

  if(distance == core::kint32max) {
    return SkillResultType::ERROR_UNKNOWN;
  }

  core::int32 distance_add =
    skill->GetAttribute(entity::SkillVarFields::DISTANCE_ADD_VALUE);

  if(distance > cell->sing_distance_ + distance_add) {
    return SkillResultType::ERROR_DISTANCE;
  }

  return SkillResultType::SUCCESS;
}

SkillResultType::type SkillRole::CheckCooling(const SkillCell *cell) const {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }
  request::RequestCheckInCooling check_cooling;
  check_cooling.__set_type_(this->role_->GetType());
  check_cooling.__set_id_(this->role_->GetID());
  check_cooling.__set_cooling_id_(cell->cooling_);
  int ret = ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_CHECK_IN_COOLING,
      &check_cooling, sizeof(check_cooling));
  if(ret != 0) {
    global::LogError("%s:%d (%s) 请求 REQUEST_CHECK_IN_COOLING 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return SkillResultType::ERROR_COOLING;
  }
  if(check_cooling.in_cooling_ == true) {
    return SkillResultType::ERROR_COOLING;
  }

  return SkillResultType::SUCCESS;
}

SkillResultType::type SkillRole::CheckConsume(const SkillCell *cell) const {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // NPC不消耗
  if(this->GetType() == entity::EntityType::TYPE_NPC) {
    return SkillResultType::SUCCESS;
  }

  const Skill *skill = this->GetSkill(cell->id_);
  if (skill == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // HP判断
  core::int32 cur_hp = this->role_->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
  core::int32 max_hp = this->role_->GetAttribute(entity::RoleAoiFields::MAX_HP);
  core::int32 spend_hp_per = cell->spend_hp_per_ +
    skill->GetAttribute(entity::SkillVarFields::HP_SPEND_ADD_PERCENT);
  if (spend_hp_per < 0) {
    return SkillResultType::ERROR_ARGS;
  }
  core::int32 spend_hp_value = cell->spend_hp_ +
    skill->GetAttribute(entity::SkillVarFields::HP_SPEND_ADD_VALUE);
  core::int32 need_hp = max_hp * spend_hp_per / global::kPercentDenom + spend_hp_value;
  if(need_hp > cur_hp) {
    return SkillResultType::ERROR_HP;
  }

  // MP判断
  core::int32 cur_mp = this->role_->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
  core::int32 max_mp = this->role_->GetAttribute(entity::RoleAoiFields::MAX_MP);
  core::int32 spend_mp_per = cell->spend_mp_per_ +
    skill->GetAttribute(entity::SkillVarFields::MP_SPEND_ADD_PERCENT);
  if (spend_mp_per < 0) {
    return SkillResultType::ERROR_ARGS;
  }
  core::int32 spend_mp_value = cell->spend_mp_ +
    skill->GetAttribute(entity::SkillVarFields::MP_SPEND_ADD_VALUE);
  core::int32 need_mp = max_mp * spend_mp_per / global::kPercentDenom + spend_mp_value;
  if(need_mp > cur_mp) {
    return SkillResultType::ERROR_MP;
  }

  // TODO: 物品消耗的判断

  return SkillResultType::SUCCESS;
}

SkillResultType::type SkillRole::CheckCondition(const SkillCell *cell) {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  SkillResultType::type result;
  if((result = this->CheckStatusCondition(cell)) != SkillResultType::SUCCESS) {
    return result;
  }
  if((result = this->CheckCampCondition(cell)) != SkillResultType::SUCCESS) {
    return result;
  }
  if((result = this->CheckSkillCondition(cell)) != SkillResultType::SUCCESS) {
    return result;
  }
  if((result = this->CheckPKCondition(cell)) != SkillResultType::SUCCESS) {
    return result;
  }

  return SkillResultType::SUCCESS;
}

SkillResultType::type SkillRole::SkillCooling(const SkillCell *cell,
    core::int64 add_value) {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }
  request::RequestStartCooling request;
  request.__set_type_(this->role_->GetType());
  request.__set_id_(this->role_->GetID());
  request.__set_cooling_id_(cell->cooling_);
  request.__set_cooling_add_value_(add_value);
  if(ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_START_COOLING,
      &request, sizeof(request)) != 0) {
    global::LogError("%s:%d (%s) 请求 REQUEST_START_COOLING 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return SkillResultType::ERROR_COOLING;
  }

  return SkillResultType::SUCCESS;
}

SkillResultType::type SkillRole::SkillConsume(const SkillCell *cell) {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // NPC不消耗
  if(this->GetType() == entity::EntityType::TYPE_NPC) {
    return SkillResultType::SUCCESS;
  }

  Skill *skill = this->GetSkill(this->current_skill_.id_);
  if(skill == NULL) {
    LOG_ERROR("技能[%d]没有找到", this->current_skill_.id_);
    return SkillResultType::ERROR_SKILL_ID;
  }

  // HP判断
  core::int32 cur_hp = this->role_->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
  core::int32 max_hp = this->role_->GetAttribute(entity::RoleAoiFields::MAX_HP);
  core::int32 spend_hp_per = cell->spend_hp_per_ +
    skill->GetAttribute(entity::SkillVarFields::HP_SPEND_ADD_PERCENT);
  if (spend_hp_per < 0) {
    return SkillResultType::ERROR_ARGS;
  }
  core::int32 spend_hp_value = cell->spend_hp_ +
    skill->GetAttribute(entity::SkillVarFields::HP_SPEND_ADD_VALUE);
  core::int32 need_hp = max_hp * spend_hp_per / global::kPercentDenom + spend_hp_value;
  if(need_hp > 0) {
    cur_hp -= need_hp;
    this->role_->SetAttribute(entity::RoleAoiFields::CURRENT_HP, cur_hp);
  }

  // MP判断
  core::int32 cur_mp = this->role_->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
  core::int32 max_mp = this->role_->GetAttribute(entity::RoleAoiFields::MAX_MP);
  core::int32 spend_mp_per = cell->spend_mp_per_ +
    skill->GetAttribute(entity::SkillVarFields::MP_SPEND_ADD_PERCENT);
  if (spend_mp_per < 0) {
    return SkillResultType::ERROR_ARGS;
  }
  core::int32 spend_mp_value = cell->spend_mp_ +
    skill->GetAttribute(entity::SkillVarFields::MP_SPEND_ADD_VALUE);
  core::int32 need_mp = max_mp * spend_mp_per / global::kPercentDenom + spend_mp_value;
  if(need_mp > 0) {
    cur_mp -= need_mp;
    this->role_->SetAttribute(entity::RoleAoiFields::CURRENT_MP, cur_mp);
  }

  // TODO: 物品消耗的判断

  return SkillResultType::SUCCESS;
}

void SkillRole::AddGlobalDebuff(core::uint32 buff, core::uint32 odds,
    core::uint32 cooling, entity::BuffTargetType::type target_type) {
  if(buff == 0) {
    return ;
  }
  this->global_debuffs_.push_back(GlobalBuff(buff, odds, cooling, target_type));
}

void SkillRole::RemoveGlobalDebuff(core::uint32 buff, core::uint32 odds,
    core::uint32 cooling) {
  if(buff == 0) {
    return ;
  }
  DebuffVector::iterator iterator = this->global_debuffs_.begin();
  for(; iterator != this->global_debuffs_.end(); ++iterator) {
    if(iterator->buff_ == buff && iterator->odds_ == odds &&
        iterator->cooling_ == cooling) {
      this->global_debuffs_.erase(iterator);
      return ;
    }
  }
}

SkillResultType::type SkillRole::CheckCampCondition(const SkillCell *cell) {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 非实体直接返回成功
  if(cell->target_type_ != TargetType::ENTITY) {
    return SkillResultType::SUCCESS;
  }

  SkillRole *taget_role = SkillRoleManager::GetInstance()->Get(
      this->current_skill_.target_type_, this->current_skill_.target_id_);
  if(taget_role == NULL) {
    LOG_ERROR("对象没有找到[%lu]", this->current_skill_.target_id_);
    return SkillResultType::ERROR_ARGS; 
  }

  // 得到玩家阵营关系
  GameRole *game_taget_role = taget_role->GetGameRole();
  if(game_taget_role == NULL) {
    LOG_ERROR("对象没有找到[%lu]", this->current_skill_.target_id_);
    return SkillResultType::ERROR_ARGS;
  }
  GameRole *game_src_role = this->GetGameRole();
  if(game_src_role == NULL) {
    LOG_ERROR("对象没有找到[%lu]", this->current_skill_.target_id_);
    return SkillResultType::ERROR_ARGS;
  }
  entity::CampRelationType::type camp_relation = game::server::Configure::GetInstance()->GetCampRelation(
      game_taget_role->GetAttribute(entity::RoleAoiFields::CAMP),
      game_src_role->GetAttribute(entity::RoleAoiFields::CAMP));
  if(camp_relation <= entity::CampRelationType::MIN || camp_relation >= entity::CampRelationType::MAX) {
    LOG_ERROR("阵营配置没有找到,target[%d],src[%d]",
        game_src_role->GetAttribute(entity::RoleAoiFields::CAMP),
        game_taget_role->GetAttribute(entity::RoleAoiFields::CAMP));
    return SkillResultType::ERROR_ARGS;
  }

  // 找到技能配置
  Skill *skill = this->GetSkill(this->current_skill_.id_);
  if(skill == NULL) {
    LOG_ERROR("技能[%d]没有找到", this->current_skill_.id_);
    return SkillResultType::ERROR_SKILL_ID;
  }
  const SkillCell *skill_cell = Configure::GetInstance()->GetSkill(
      skill->GetID(), skill->GetLevel());
  if(skill_cell == NULL) {
    LOG_ERROR("技能配置没有找到id[%d]level[%d]", 
        skill->GetID(), skill->GetLevel());
    return SkillResultType::ERROR_SKILL_ID;
  }

  // 阵营类型
  if((1 << (camp_relation - 1)) & skill_cell->camp_type_) {
    return SkillResultType::SUCCESS;
  }

  return SkillResultType::ERROR_TARGET;
}

SkillResultType::type SkillRole::CheckSkillCondition(const SkillCell *cell) {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 非实体直接返回成功
  if(cell->target_type_ != TargetType::ENTITY) {
    return SkillResultType::SUCCESS;
  }

  // 技能判断
  if(cell->cast_type_ & AffectType::SELF) {
    if(this->current_skill_.target_id_ == this->GetID()) {
      return SkillResultType::SUCCESS;
    }
  }
  if(cell->cast_type_ & AffectType::OTHER_SELF) {
    if(this->current_skill_.target_id_ != this->GetID()) {
      return SkillResultType::SUCCESS;
    }
  }
  if(this->current_skill_.target_type_ == entity::EntityType::TYPE_ACTOR &&
      this->GetType() == entity::EntityType::TYPE_ACTOR) {
    if(cell->cast_type_ & AffectType::TEAM) {
      if(FacadeRequest::GetInstance()->CheckSameTeam(this->GetID(),
            this->current_skill_.target_id_) == true) {
        return SkillResultType::SUCCESS;
      }
    }
    if(cell->cast_type_ & AffectType::OTHER_TEAM) {
      if(FacadeRequest::GetInstance()->CheckSameTeam(this->GetID(),
            this->current_skill_.target_id_) == false) {
        return SkillResultType::SUCCESS;
      }
    }
  } else {
    if(cell->cast_type_ & AffectType::OTHER_TEAM) {
      return SkillResultType::SUCCESS;
    }
  }
  return SkillResultType::ERROR_TARGET;
}

SkillResultType::type SkillRole::CheckStatusCondition(const SkillCell *cell) {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 非实体直接返回成功
  if(cell->target_type_ != TargetType::ENTITY) {
    return SkillResultType::SUCCESS;
  }

  // 目标角色
  SkillRole *dest_role = SkillRoleManager::GetInstance()->Get(
      this->current_skill_.target_type_, this->current_skill_.target_id_);
  if(dest_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }
  GameRole *dest_game_role = dest_role->GetGameRole();
  if(dest_game_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }

  // 目标角色状态
  if(cell->status_type_ & StatusType::LIVE) {
    if(dest_game_role->GetAttribute(entity::RoleAoiFields::STATUS) !=
        entity::RoleBasicStatus::STATUS_DIE) {
      return SkillResultType::SUCCESS;
    } else {
      return SkillResultType::ERROR_TARGET;
    }
  } else if(cell->status_type_ & StatusType::DEAD) {
    if(dest_game_role->GetAttribute(entity::RoleAoiFields::STATUS) ==
        entity::RoleBasicStatus::STATUS_DIE) {
      return SkillResultType::SUCCESS;
    } else {
      return SkillResultType::ERROR_TARGET;
    }
  }

  return SkillResultType::ERROR_UNKNOWN;
}

SkillResultType::type SkillRole::CheckPKCondition(const SkillCell* cell) {
  if(cell == NULL || this->role_ == NULL) {
    return SkillResultType::ERROR_ARGS;
  }
  // 查看技能是否增益技能
  if(FriendType::HELP == cell->friend_type_)
    return SkillResultType::SUCCESS;

  // 非玩家直接返回成功
  if(cell->target_type_ != TargetType::ENTITY ||
      this->role_->GetType() != entity::EntityType::TYPE_ACTOR) {
    return SkillResultType::SUCCESS;
  }
  // 目标角色
  SkillRole *dest_role = SkillRoleManager::GetInstance()->Get(
      this->current_skill_.target_type_, this->current_skill_.target_id_);
  if(dest_role == NULL) {
    return SkillResultType::ERROR_ARGS;
  }
  if(dest_role->GetGameRole() == NULL) {
    return SkillResultType::ERROR_ARGS;
  }
  // 不是玩家不判断
  if(dest_role->GetType() != entity::EntityType::TYPE_ACTOR) {
    return SkillResultType::SUCCESS;
  }

  request::RequestScenePKCheck request;
  request.__set_src_id_(this->role_->GetID());
  request.__set_dest_id_(this->current_skill_.target_id_);

  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_PK_CHECK,
        &request, sizeof(request)) != 0) {
    global::LogError("%s:%d (%s) 请求 REQUEST_SCENE_PK_CHECK 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return SkillResultType::ERROR_ARGS;
  } 

  switch(request.result_) {
    case request::PKCheckResultType::SUCCESS:
      return SkillResultType::SUCCESS;
    case request::PKCheckResultType::ERROR_PK_MODE:
      return SkillResultType::ERROR_PK_MODE;
    case request::PKCheckResultType::ERROR_SAFE_AREA:
      return SkillResultType::ERROR_SAFEAREA;
    case request::PKCheckResultType::ERROR_ROOKIE_PROTECT:
      return SkillResultType::ERROR_ROOKIE_PROTECT;
    case request::PKCheckResultType::ERROR_DIE_PROTECT_TIME:
      return SkillResultType::ERROR_DIE_PROTECT_TIME;
    default:
      return SkillResultType::ERROR_ARGS;
  }
  return SkillResultType::ERROR_UNKNOWN;
}

SafeAreaResultType::type SkillRole::IsInSafeArea() const {
  if(this->role_ == NULL) {
    return SafeAreaResultType::ERROR;
  }
  request::RequestSceneIsSafe request;
  request.__set_scene_(this->role_->GetScene());
  
  core::uint32 cur_x = 0, cur_y = 0;
  this->role_->GetPosition(cur_x, cur_y);
  request.__set_x_(cur_x);
  request.__set_y_(cur_y);

  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_IS_SAFE,
        &request, sizeof(request)) != 0) {
    global::LogError("%s:%d (%s) 请求 REQUEST_SCENE_IS_SAFE 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return SafeAreaResultType::ERROR;
  } 

  if(request.is_safe_) {
    return SafeAreaResultType::SAFE;
  }

  return SafeAreaResultType::NO_SAFE;
}

bool SkillRole::CheckChangePKMode() {
  if(this->role_ == NULL)
    return false;

  bool rookie_protect = false, ban_change_pk_mode = false, die_protect = false;
  if(GetScenePKInfo(rookie_protect, ban_change_pk_mode, die_protect)) {
    if(ban_change_pk_mode) {
      return false;
    }

    if(rookie_protect) {
      if(this->role_->GetAttribute(entity::RoleAoiFields::LEVEL) <
          MISC_CONF()->rookie_protect_level_) {
        return false;
      }
    }
    return true;
  }
  return false;
}

// 得到是否新手保护
bool SkillRole::GetScenePKInfo(bool &rookie_protect, bool &ban_change_pk_mode, bool &die_protect) {
  if(this->role_ == NULL)
    return false;

  request::RequestScenePKInfo request;
  request.__set_scene_(this->role_->GetScene());
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_PK_INFO,
        &request, sizeof(request)) != 0) {
    global::LogError("%s:%d (%s) 请求 REQUEST_SCENE_PK_INFO 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  } 
  rookie_protect = request.rookie_protect_;
  ban_change_pk_mode = request.ban_change_pk_mode_;
  die_protect = request.die_protect_;
  return true;
}


}  // namespace skill

}  // namespace server

}  // namespace game

