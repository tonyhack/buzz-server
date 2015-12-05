//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-22 14:46:21.
// File name: condition_pool.cc
//
// Description:
// Define class ConditionPool.
//

#include "game_server/server/extension/buff/condition_pool.h"

#include "game_server/server/extension/buff/attack_condition.h"
#include "game_server/server/extension/buff/buff_condition.h"
#include "game_server/server/extension/buff/hp_condition.h"
#include "game_server/server/extension/buff/mp_condition.h"
#include "game_server/server/extension/buff/skill_condition.h"
#include "game_server/server/extension/buff/status_condition.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

bool ConditionPool::Initialize(size_t initial_number, size_t extend_number) {
  if(this->hp_percent_below_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 HpPercentBelowCondition 对象池失败");
    return false;
  }
  if(this->hp_percent_above_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 HpPercentAboveCondition 对象池失败");
    return false;
  }
  if(this->hp_percent_equal_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 HpPercentEqualCondition 对象池失败");
    return false;
  }
  if(this->hp_value_below_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 HpValueBelowCondition 对象池失败");
    return false;
  }
  if(this->hp_value_above_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 HpValueAboveCondition 对象池失败");
    return false;
  }
  if(this->hp_value_equal_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 HpValueAboveCondition 对象池失败");
    return false;
  }
  if(this->mp_percent_below_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 MpPercentBelowCondition 对象池失败");
    return false;
  }
  if(this->mp_percent_above_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 MpPercentAboveCondition 对象池失败");
    return false;
  }
  if(this->mp_percent_equal_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 MpPercentEqualCondition 对象池失败");
    return false;
  }
  if(this->mp_value_below_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 MpValueBelowCondition 对象池失败");
    return false;
  }
  if(this->mp_value_above_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 MpValueAboveCondition 对象池失败");
    return false;
  }
  if(this->mp_value_equal_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 MpValueEqualCondition 对象池失败");
    return false;
  }
  if(this->attack_hit_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 AttackHitCondition 对象池失败");
    return false;
  }
  if(this->attack_crit_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 AttackCritCondition 对象池失败");
    return false;
  }
  if(this->attack_dodge_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 AttackDodgeCondition 对象池失败");
    return false;
  }
  if(this->attack_ignore_defence_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 AttackIgnoreDefenceCondition 对象池失败");
    return false;
  }
  if(this->attacked_hit_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 AttackedHitCondition 对象池失败");
    return false;
  }
  if(this->attacked_crit_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 AttackedCritCondition 对象池失败");
    return false;
  }
  if(this->attacked_dodge_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 AttackedDodgeCondition 对象池失败");
    return false;
  }
  if(this->attacked_ignore_defence_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 AttackedIgnoreDefenceCondition 对象池失败");
    return false;
  }
  if(this->soul_status_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 SoulStatusCondition 对象池失败");
    return false;
  }
  if(this->not_soul_status_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 NotSoulStatusCondition 对象池失败");
    return false;
  }
  if(this->forbid_skill_status_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 ForbidSkillStatusCondition 对象池失败");
    return false;
  }
  if(this->forbid_move_status_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 ForbidMoveStatusCondition 对象池失败");
    return false;
  }
  if(this->forbid_item_status_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 ForbidItemStatusCondition 对象池失败");
    return false;
  }
  if(this->buff_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 BuffCondition 对象池失败");
    return false;
  }
  if(this->buff_group_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 BuffGroupCondition 对象池失败");
    return false;
  }
  if(this->skill_use_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 SkillUseCondition 对象池失败");
    return false;
  }
  if(this->skill_group_use_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化 SkillGroupUseCondition 对象池失败");
    return false;
  }

  return true;
}

void ConditionPool::Finalize() {
  this->hp_percent_below_.Finalize();
  this->hp_percent_above_.Finalize();
  this->hp_percent_equal_.Finalize();
  this->hp_value_below_.Finalize();
  this->hp_value_above_.Finalize();
  this->hp_value_equal_.Finalize();
  this->mp_percent_below_.Finalize();
  this->mp_percent_above_.Finalize();
  this->mp_percent_equal_.Finalize();
  this->mp_value_below_.Finalize();
  this->mp_value_above_.Finalize();
  this->mp_value_equal_.Finalize();
  this->attack_hit_.Finalize();
  this->attack_crit_.Finalize();
  this->attack_dodge_.Finalize();
  this->attack_ignore_defence_.Finalize();
  this->attacked_hit_.Finalize();
  this->attacked_crit_.Finalize();
  this->attacked_dodge_.Finalize();
  this->attacked_ignore_defence_.Finalize();
  this->soul_status_.Finalize();
  this->not_soul_status_.Finalize();
  this->forbid_skill_status_.Finalize();
  this->forbid_move_status_.Finalize();
  this->forbid_item_status_.Finalize();
  this->buff_.Finalize();
  this->buff_group_.Finalize();
  this->skill_use_.Finalize();
  this->skill_group_use_.Finalize();
}

Condition *ConditionPool::Allocate(entity::BuffConditionType::type type) {
  Condition *condition = NULL;
  switch(type) {
    case entity::BuffConditionType::HP_PERCENT_BELOW:
      condition = this->hp_percent_below_.Allocate();
      break;
    case entity::BuffConditionType::HP_PERCENT_ABOVE:
      condition = this->hp_percent_above_.Allocate();
      break;
    case entity::BuffConditionType::HP_PERCENT_EQUAL:
      condition = this->hp_percent_equal_.Allocate();
      break;
    case entity::BuffConditionType::HP_VALUE_BELOW:
      condition = this->hp_value_below_.Allocate();
      break;
    case entity::BuffConditionType::HP_VALUE_ABOVE:
      condition = this->hp_value_above_.Allocate();
      break;
    case entity::BuffConditionType::HP_VALUE_EQUAL:
      condition = this->hp_value_equal_.Allocate();
      break;
    case entity::BuffConditionType::MP_PERCENT_BELOW:
      condition = this->mp_percent_below_.Allocate();
      break;
    case entity::BuffConditionType::MP_PERCENT_ABOVE:
      condition = this->mp_percent_above_.Allocate();
      break;
    case entity::BuffConditionType::MP_PERCENT_EQUAL:
      condition = this->mp_percent_equal_.Allocate();
      break;
    case entity::BuffConditionType::MP_VALUE_BELOW:
      condition = this->mp_value_below_.Allocate();
      break;
    case entity::BuffConditionType::MP_VALUE_ABOVE:
      condition = this->mp_value_above_.Allocate();
      break;
    case entity::BuffConditionType::MP_VALUE_EQUAL:
      condition = this->mp_value_equal_.Allocate();
      break;
    case entity::BuffConditionType::ATTACK_HIT:
      condition = this->attack_hit_.Allocate();
      break;
    case entity::BuffConditionType::ATTACK_DODGE:
      condition = this->attack_dodge_.Allocate();
      break;
    case entity::BuffConditionType::ATTACK_CRIT:
      condition = this->attack_crit_.Allocate();
      break;
    case entity::BuffConditionType::ATTACK_IGNORE_DEFENCE:
      condition = this->attack_ignore_defence_.Allocate();
      break;
    case entity::BuffConditionType::ATTACKED_HIT:
      condition = this->attacked_hit_.Allocate();
      break;
    case entity::BuffConditionType::ATTACKED_DODGE:
      condition = this->attacked_dodge_.Allocate();
      break;
    case entity::BuffConditionType::ATTACKED_CRIT:
      condition = this->attacked_crit_.Allocate();
      break;
    case entity::BuffConditionType::ATTACKED_IGNORE_DEFENCE:
      condition = this->attacked_ignore_defence_.Allocate();
      break;
    case entity::BuffConditionType::SOUL_STATUS:
      condition = this->soul_status_.Allocate();
      break;
    case entity::BuffConditionType::NOT_SOUL_STATUS:
      condition = this->not_soul_status_.Allocate();
      break;
    case entity::BuffConditionType::FORBID_USE_SKILL:
      condition = this->forbid_skill_status_.Allocate();
      break;
    case entity::BuffConditionType::FORBID_MOVE:
      condition = this->forbid_move_status_.Allocate();
      break;
    case entity::BuffConditionType::FORBID_USE_ITEM:
      condition = this->forbid_item_status_.Allocate();
      break;
    case entity::BuffConditionType::BUFF:
      condition = this->buff_.Allocate();
      break;
    case entity::BuffConditionType::BUFF_GROUP:
      condition = this->buff_group_.Allocate();
      break;
    case entity::BuffConditionType::SKILL_USE:
      condition = this->skill_use_.Allocate();
      break;
    case entity::BuffConditionType::SKILL_GROUP_USE:
      condition = this->skill_group_use_.Allocate();
      break;
    default:
      break;
  }

  return condition;
}

void ConditionPool::Deallocate(Condition *condition) {
  if(condition == NULL) {
    return ;
  }
  switch(condition->GetType()) {
    case entity::BuffConditionType::HP_PERCENT_BELOW:
      this->hp_percent_below_.Deallocate((HpPercentBelowCondition *)condition);
      break;
    case entity::BuffConditionType::HP_PERCENT_ABOVE:
      this->hp_percent_above_.Deallocate((HpPercentAboveCondition *)condition);
      break;
    case entity::BuffConditionType::HP_PERCENT_EQUAL:
      this->hp_percent_equal_.Deallocate((HpPercentEqualCondition *)condition);
      break;
    case entity::BuffConditionType::HP_VALUE_BELOW:
      this->hp_value_below_.Deallocate((HpValueBelowCondition *)condition);
      break;
    case entity::BuffConditionType::HP_VALUE_ABOVE:
      this->hp_value_above_.Deallocate((HpValueAboveCondition *)condition);
      break;
    case entity::BuffConditionType::HP_VALUE_EQUAL:
      this->hp_value_equal_.Deallocate((HpValueEqualCondition *)condition);
      break;
    case entity::BuffConditionType::MP_PERCENT_BELOW:
      this->mp_percent_below_.Deallocate((MpPercentBelowCondition *)condition);
      break;
    case entity::BuffConditionType::MP_PERCENT_ABOVE:
      this->mp_percent_above_.Deallocate((MpPercentAboveCondition *)condition);
      break;
    case entity::BuffConditionType::MP_PERCENT_EQUAL:
      this->mp_percent_equal_.Deallocate((MpPercentEqualCondition *)condition);
      break;
    case entity::BuffConditionType::MP_VALUE_BELOW:
      this->mp_value_below_.Deallocate((MpValueBelowCondition *)condition);
      break;
    case entity::BuffConditionType::MP_VALUE_ABOVE:
      this->mp_value_above_.Deallocate((MpValueAboveCondition *)condition);
      break;
    case entity::BuffConditionType::MP_VALUE_EQUAL:
      this->mp_value_equal_.Deallocate((MpValueEqualCondition *)condition);
      break;
    case entity::BuffConditionType::ATTACK_HIT:
      this->attack_hit_.Deallocate((AttackHitCondition *)condition);
      break;
    case entity::BuffConditionType::ATTACK_DODGE:
      this->attack_dodge_.Deallocate((AttackDodgeCondition *)condition);
      break;
    case entity::BuffConditionType::ATTACK_CRIT:
      this->attack_crit_.Deallocate((AttackCritCondition *)condition);
      break;
    case entity::BuffConditionType::ATTACK_IGNORE_DEFENCE:
      this->attack_ignore_defence_.Deallocate((AttackIgnoreDefenceCondition *)condition);
      break;
    case entity::BuffConditionType::ATTACKED_HIT:
      this->attacked_hit_.Deallocate((AttackedHitCondition *)condition);
      break;
    case entity::BuffConditionType::ATTACKED_DODGE:
      this->attacked_dodge_.Deallocate((AttackedDodgeCondition *)condition);
      break;
    case entity::BuffConditionType::ATTACKED_CRIT:
      this->attacked_crit_.Deallocate((AttackedCritCondition *)condition);
      break;
    case entity::BuffConditionType::ATTACKED_IGNORE_DEFENCE:
      this->attacked_ignore_defence_.Deallocate((AttackedIgnoreDefenceCondition *)condition);
      break;
    case entity::BuffConditionType::SOUL_STATUS:
      this->soul_status_.Deallocate((SoulStatusCondition *)condition);
      break;
    case entity::BuffConditionType::NOT_SOUL_STATUS:
      this->not_soul_status_.Deallocate((NotSoulStatusCondition *)condition);
      break;
    case entity::BuffConditionType::FORBID_USE_SKILL:
      this->forbid_skill_status_.Deallocate((ForbidSkillStatusCondition *)condition);
      break;
    case entity::BuffConditionType::FORBID_MOVE:
      this->forbid_move_status_.Deallocate((ForbidMoveStatusCondition *)condition);
      break;
    case entity::BuffConditionType::FORBID_USE_ITEM:
      this->forbid_item_status_.Deallocate((ForbidItemStatusCondition *)condition);
      break;
    case entity::BuffConditionType::BUFF:
      this->buff_.Deallocate((BuffCondition *)condition);
      break;
    case entity::BuffConditionType::BUFF_GROUP:
      this->buff_group_.Deallocate((BuffGroupCondition *)condition);
      break;
    case entity::BuffConditionType::SKILL_USE:
      this->skill_use_.Deallocate((SkillUseCondition *)condition);
      break;
    case entity::BuffConditionType::SKILL_GROUP_USE:
      this->skill_group_use_.Deallocate((SkillGroupUseCondition *)condition);
      break;
    default:
      break;
  }
}

}  // namespace buff

}  // namespace server

}  // namespace game

