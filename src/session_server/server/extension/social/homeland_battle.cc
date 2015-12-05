#include "session_server/server/extension/social/homeland_battle.h"

#include <cstddef>

#include "global/types.h"
#include "global/logging.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/server/extension_manager.h"

namespace session {

namespace server {

namespace social {

static core::int32 GetRandomPercent() {
  return ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
}

static void InitAliveUnitIndex(const HomelandBattle::HomelandBattleUnitMap *units,
    HomelandBattle::HomelandBattleAliveUnitIndex *index, core::int32 *alive_count) {
  index->clear();
  *alive_count = units->size();

  for (HomelandBattle::HomelandBattleUnitMap::const_iterator iter = units->begin();
       iter != units->end(); ++iter) {
    HomelandBattleUnit *unit = iter->second;
    if (unit->CheckDead()) {
      --(*alive_count);
    } else {
      (*index)[unit->row_].insert(unit->column_);
    }
  }
}

HomelandBattle::HomelandBattle(HomelandBattleUnitMap *left_units,
    HomelandBattleUnitMap *right_units, HomelandBattleRoundDataVector *record) :
    left_units_(left_units), right_units_(right_units), record_(record), current_round_(NULL)
{
  // 初始化存活单位索引
  InitAliveUnitIndex(this->left_units_, &this->left_alive_index_, &this->left_alive_);
  InitAliveUnitIndex(this->right_units_, &this->right_alive_index_, &this->right_alive_);
}

HomelandBattle::~HomelandBattle() {}

void HomelandBattle::NewRoundStart() {
  gateway::protocol::HomelandBattleRoundData new_round;
  this->record_->push_back(new_round);
  this->current_round_ = &this->record_->back();

  this->left_iter_ = this->left_units_->begin();
  this->right_iter_ = this->right_units_->begin();
}

void HomelandBattle::DoLeftAoe(core::int32 cast_unit_id,
    entity::HomelandBattleUnitType::type cast_unit_type, core::int32 hurt,
    core::int32 *left_alive, core::int32 *right_alive) {
  this->DoAoe(cast_unit_id, cast_unit_type, hurt,
      this->left_units_, &this->left_alive_index_, &this->left_alive_);
  *left_alive = this->left_alive_;
  *right_alive = this->right_alive_;
}

void HomelandBattle::DoRightAoe(core::int32 cast_unit_id,
    entity::HomelandBattleUnitType::type cast_unit_type, core::int32 hurt,
    core::int32 *left_alive, core::int32 *right_alive) {
  this->DoAoe(cast_unit_id, cast_unit_type, hurt,
      this->right_units_, &this->right_alive_index_, &this->right_alive_);
  *left_alive = this->left_alive_;
  *right_alive = this->right_alive_;
}

void HomelandBattle::DoAoe(core::int32 cast_unit_id,
    entity::HomelandBattleUnitType::type cast_unit_type, core::int32 hurt, 
    HomelandBattleUnitMap *enemies,
    HomelandBattleAliveUnitIndex *enemy_alive_index, core::int32 *enemy_alive) {
  this->NewRoundStart();

  gateway::protocol::HomelandBattleActionData action_data;
  action_data.__set_unit_id_(cast_unit_id);
  if (entity::HomelandBattleUnitType::ACTOR == cast_unit_type) {
    action_data.__set_type_(entity::HomelandBattleActionType::AOE_ACTOR);
  } else if (entity::HomelandBattleUnitType::TOWER == cast_unit_type) {
    action_data.__set_type_(entity::HomelandBattleActionType::AOE_TOWER);
  }

  for (HomelandBattleUnitMap::iterator iter = enemies->begin();
       iter != enemies->end(); ++iter) {
    HomelandBattleUnit *target = iter->second;

    gateway::protocol::HomelandBattleStateChangeData state_data;
    state_data.__set_unit_id_(target->id_);
    state_data.__set_type_(entity::HomelandBattleStateChangeType::HURT);
    state_data.__set_value_(hurt);
    action_data.result_.push_back(state_data);

    if (target->hp_ > hurt) {
      target->hp_ -= hurt;
    } else {
      target->hp_ = 0;
      // 死亡状态变化
      gateway::protocol::HomelandBattleStateChangeData state_data;
      state_data.__set_unit_id_(target->id_);
      state_data.__set_type_(entity::HomelandBattleStateChangeType::DEAD);
      action_data.result_.push_back(state_data);
      // 更新存活单位索引
      (*enemy_alive_index)[target->row_].erase(target->column_);
      // 更新存活单位数量
      --(*enemy_alive);
    }
  }

  this->current_round_->actions_.push_back(action_data);
}

void HomelandBattle::DoBattle(core::int32 max_round,
    core::int32 *left_alive, core::int32 *right_alive) {
  if (0 == this->left_alive_ || 0 == this->right_alive_) {
    *left_alive = this->left_alive_;
    *right_alive = this->right_alive_;
    return;
  }

  // 战斗开始
  for (core::int32 i = 0; i < max_round; ++i) {
    this->NewRoundStart();

    for (;;) {
      // 左边行动
      HomelandBattleUnit *left_unit = this->GetLeftDoActionUnit();
      if (left_unit != NULL) {
        this->DoAction(left_unit, this->right_units_,
            &this->right_alive_index_, &this->right_alive_);
      }
      // 战斗结束
      if (0 == this->right_alive_) {
        *left_alive = this->left_alive_;
        *right_alive = this->right_alive_;
        return;
      }

      // 右边行动
      HomelandBattleUnit *right_unit = this->GetRightDoActionUnit();
      if (right_unit != NULL) {
        this->DoAction(right_unit, this->left_units_,
            &this->left_alive_index_, &this->left_alive_);
      }
      // 战斗结束
      if (0 == this->left_alive_) {
        *left_alive = this->left_alive_;
        *right_alive = this->right_alive_;
        return;
      }

      // 回合结束
      if (NULL == left_unit && NULL == right_unit) {
        break;
      }
    }
  }
}

HomelandBattleUnit *HomelandBattle::GetLeftDoActionUnit() {
  HomelandBattleUnit *unit = NULL;

  for (; this->left_iter_ != this->left_units_->end(); ++this->left_iter_) {
    if (this->left_iter_->second->CheckDead() == false) {
      unit = this->left_iter_->second;
      ++this->left_iter_;
      break;
    }
  }

  return unit;
}

HomelandBattleUnit *HomelandBattle::GetRightDoActionUnit() {
  HomelandBattleUnit *unit = NULL;

  for (; this->right_iter_ != this->right_units_->end(); ++this->right_iter_) {
    if (this->right_iter_->second->CheckDead() == false) {
      unit = this->right_iter_->second;
      ++this->right_iter_;
      break;
    }
  }

  return unit;
}

static HomelandBattleUnit *GetAliveUnit(const HomelandBattle::HomelandBattleUnitMap *units,
    core::int32 row, core::int32 column) {
  core::uint64 index = ((core::uint64)row << 32) + (core::uint64)column;

  HomelandBattle::HomelandBattleUnitMap::const_iterator iter = units->find(index);
  if (iter == units->end()) {
    return NULL;
  }
  if (iter->second->CheckDead()) {
    return NULL;
  }
  return iter->second;
}

static HomelandBattleUnit *GetAttackTarget(const HomelandBattle::HomelandBattleUnitMap *enemies,
    const HomelandBattle::HomelandBattleAliveUnitIndex *index, core::int32 column) {
  for (HomelandBattle::HomelandBattleAliveUnitIndex::const_iterator iter = index->begin();
       iter != index->end(); ++iter) {
    core::int32 target_row = iter->first;
    core::int32 target_column = -1;

    const std::set<core::int32> &row_index = iter->second;
    if (row_index.empty()) {
      continue;
    }

    std::set<core::int32>::const_iterator iter2 = row_index.lower_bound(column);
    if (iter2 == row_index.end()) {
      target_column = *(--iter2);
    } else {
      if (iter2 == row_index.begin()) {
        target_column = *iter2;
      } else {
        core::int32 large_column = *iter2;
        core::int32 small_column = *(--iter2);

        if (large_column - column < column - small_column) {
          target_column = large_column;
        } else {
          target_column = small_column;
        }
      }
    }

    return GetAliveUnit(enemies, target_row, target_column);
  }

  return NULL;
}

void HomelandBattle::DoAction(HomelandBattleUnit *unit, HomelandBattleUnitMap *enemies,
    HomelandBattleAliveUnitIndex *enemy_alive_index, core::int32 *enemy_alive) {
  // 寻找首要攻击目标
  HomelandBattleUnit *main_target = GetAttackTarget(enemies, enemy_alive_index, unit->column_);
  if (NULL == main_target) {
    return;
  }

  // 使用技能判断
  bool use_skill = (GetRandomPercent() <= unit->special_skill_odds_);

  // 取攻击目标集合
  std::vector<HomelandBattleUnit *> targets;
  targets.push_back(main_target);

  if (use_skill) {
    HomelandBattleUnit *target = NULL;

    if (HomelandBattleUnit::SpecialSkillType::AXE == unit->special_skill_type_) {
      // 目标以及上下两格
      target = GetAliveUnit(enemies, main_target->row_, main_target->column_ - 1);
      if (target != NULL) {
        targets.push_back(target);
      }
      target = GetAliveUnit(enemies, main_target->row_, main_target->column_ + 1);
      if (target != NULL) {
        targets.push_back(target);
      }
    } else if (HomelandBattleUnit::SpecialSkillType::SHOOT == unit->special_skill_type_) {
      // 整列
      for (HomelandBattleAliveUnitIndex::const_iterator iter = enemy_alive_index->begin();
           iter != enemy_alive_index->end(); ++iter) {
        target = GetAliveUnit(enemies, iter->first, main_target->column_);
        if (target != NULL && target != main_target) {
          targets.push_back(target);
        }
      }
    } else if (HomelandBattleUnit::SpecialSkillType::MAGIC == unit->special_skill_type_) {
      // 目标以及上下左右四格
      target = GetAliveUnit(enemies, main_target->row_, main_target->column_ - 1);
      if (target != NULL) {
        targets.push_back(target);
      }
      target = GetAliveUnit(enemies, main_target->row_, main_target->column_ + 1);
      if (target != NULL) {
        targets.push_back(target);
      }
      target = GetAliveUnit(enemies, main_target->row_ + 1, main_target->column_);
      if (target != NULL) {
        targets.push_back(target);
      }
    }
  }

  // 基础伤害 
  core::int32 base_hurt = 0;
  if (use_skill) {
    base_hurt = unit->attack_ * unit->special_skill_hurt_coe_ / global::kPercentDenom;
  } else {
    base_hurt = unit->attack_;
  }

  // 行动数据
  gateway::protocol::HomelandBattleActionData action_data;
  action_data.__set_unit_id_(unit->id_);
  if (use_skill) {
    if (HomelandBattleUnit::SpecialSkillType::AXE == unit->special_skill_type_) {
      action_data.__set_type_(entity::HomelandBattleActionType::SPECIAL_SKILL_AXE);
    } else if (HomelandBattleUnit::SpecialSkillType::SHOOT == unit->special_skill_type_) {
      action_data.__set_type_(entity::HomelandBattleActionType::SPECIAL_SKILL_SHOOT);
    } else if (HomelandBattleUnit::SpecialSkillType::MAGIC == unit->special_skill_type_) {
      action_data.__set_type_(entity::HomelandBattleActionType::SPECIAL_SKILL_MAGIC);
    }
  } else {
    action_data.__set_type_(entity::HomelandBattleActionType::ATTACK);
  }

  // 进行攻击
  for (size_t i = 0; i < targets.size(); ++i) {
    HomelandBattleUnit *target = targets[i];

    // 闪避判断
    if (GetRandomPercent() <= target->dodge_percent_) {
      gateway::protocol::HomelandBattleStateChangeData state_data;
      state_data.__set_unit_id_(target->id_);
      state_data.__set_type_(entity::HomelandBattleStateChangeType::DODGE);
      action_data.result_.push_back(state_data);
      continue;
    }

    core::int32 hurt = 0;
    // 暴击判断
    if (GetRandomPercent() <= unit->crit_percent_) {
      hurt = base_hurt * unit->crit_effect_ / global::kPercentDenom;

      gateway::protocol::HomelandBattleStateChangeData state_data;
      state_data.__set_unit_id_(target->id_);
      state_data.__set_type_(entity::HomelandBattleStateChangeType::CRIT);
      state_data.__set_value_(hurt);
      action_data.result_.push_back(state_data);
    } else {
    // 普通伤害
      hurt = base_hurt;

      gateway::protocol::HomelandBattleStateChangeData state_data;
      state_data.__set_unit_id_(target->id_);
      state_data.__set_type_(entity::HomelandBattleStateChangeType::HURT);
      state_data.__set_value_(hurt);
      action_data.result_.push_back(state_data);
    }

    if (target->hp_ > hurt) {
      target->hp_ -= hurt;
    } else {
      target->hp_ = 0;
      // 死亡状态变化
      gateway::protocol::HomelandBattleStateChangeData state_data;
      state_data.__set_unit_id_(target->id_);
      state_data.__set_type_(entity::HomelandBattleStateChangeType::DEAD);
      action_data.result_.push_back(state_data);
      // 更新存活单位索引
      (*enemy_alive_index)[target->row_].erase(target->column_);
      // 更新存活单位数量
      --(*enemy_alive);
    }
  }

  this->current_round_->actions_.push_back(action_data);
}

}  // namespace socail

}  // namespace server

}  // namespace session
