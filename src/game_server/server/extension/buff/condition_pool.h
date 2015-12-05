//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-22 14:11:21.
// File name: condition_pool.h
//
// Description:
// Define class ConditionPool.
//

#ifndef __GAME__SERVER__BUFF__CONDITION__POOL__H
#define __GAME__SERVER__BUFF__CONDITION__POOL__H

#include "entity/buff_types.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace buff {

class Condition;
class HpPercentBelowCondition;
class HpPercentAboveCondition;
class HpPercentEqualCondition;
class HpValueBelowCondition;
class HpValueAboveCondition;
class HpValueEqualCondition;
class MpPercentBelowCondition;
class MpPercentAboveCondition;
class MpPercentEqualCondition;
class MpValueBelowCondition;
class MpValueAboveCondition;
class MpValueEqualCondition;
class AttackHitCondition;
class AttackCritCondition;
class AttackDodgeCondition;
class AttackIgnoreDefenceCondition;
class AttackedHitCondition;
class AttackedCritCondition;
class AttackedDodgeCondition;
class AttackedIgnoreDefenceCondition;
class SoulStatusCondition;
class NotSoulStatusCondition;
class ForbidSkillStatusCondition;
class ForbidMoveStatusCondition;
class ForbidItemStatusCondition;
class BuffCondition;
class BuffGroupCondition;
class SkillUseCondition;
class SkillGroupUseCondition;

class ConditionPool : public global::SingletonFactory<ConditionPool> {
  friend class global::SingletonFactory<ConditionPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Condition *Allocate(entity::BuffConditionType::type type);
  void Deallocate(Condition *condition);

 private:
  ConditionPool() {}
  ~ConditionPool() {}

  global::PoolTemplate<HpPercentBelowCondition> hp_percent_below_;
  global::PoolTemplate<HpPercentAboveCondition> hp_percent_above_;
  global::PoolTemplate<HpPercentEqualCondition> hp_percent_equal_;
  global::PoolTemplate<HpValueBelowCondition> hp_value_below_;
  global::PoolTemplate<HpValueAboveCondition> hp_value_above_;
  global::PoolTemplate<HpValueEqualCondition> hp_value_equal_;
  global::PoolTemplate<MpPercentBelowCondition> mp_percent_below_;
  global::PoolTemplate<MpPercentAboveCondition> mp_percent_above_;
  global::PoolTemplate<MpPercentEqualCondition> mp_percent_equal_;
  global::PoolTemplate<MpValueBelowCondition> mp_value_below_;
  global::PoolTemplate<MpValueAboveCondition> mp_value_above_;
  global::PoolTemplate<MpValueEqualCondition> mp_value_equal_;
  global::PoolTemplate<AttackHitCondition> attack_hit_;
  global::PoolTemplate<AttackCritCondition> attack_crit_;
  global::PoolTemplate<AttackDodgeCondition> attack_dodge_;
  global::PoolTemplate<AttackIgnoreDefenceCondition> attack_ignore_defence_;
  global::PoolTemplate<AttackedHitCondition> attacked_hit_;
  global::PoolTemplate<AttackedCritCondition> attacked_crit_;
  global::PoolTemplate<AttackedDodgeCondition> attacked_dodge_;
  global::PoolTemplate<AttackedIgnoreDefenceCondition> attacked_ignore_defence_;
  global::PoolTemplate<SoulStatusCondition> soul_status_;
  global::PoolTemplate<NotSoulStatusCondition> not_soul_status_;
  global::PoolTemplate<ForbidSkillStatusCondition> forbid_skill_status_;
  global::PoolTemplate<ForbidMoveStatusCondition> forbid_move_status_;
  global::PoolTemplate<ForbidItemStatusCondition> forbid_item_status_;
  global::PoolTemplate<BuffCondition> buff_;
  global::PoolTemplate<BuffGroupCondition> buff_group_;
  global::PoolTemplate<SkillUseCondition> skill_use_;
  global::PoolTemplate<SkillGroupUseCondition> skill_group_use_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__CONDITION__POOL__H

