//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-19 17:05:31.
// File name: damage_calculation.h
//
// Description:
// Define class DamageCalculation.
//

#ifndef __GAME__SERVER__SKILL__DAMAGE__CALCULATION__H
#define __GAME__SERVER__SKILL__DAMAGE__CALCULATION__H

#include "core/base/types.h"
#include "entity/skill_types.h"
#include "game_server/server/extension/skill/skill_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace skill {

class Skill;
class SkillRole;

class DamageCalculation : public global::SingletonFactory<DamageCalculation> {
  friend class global::SingletonFactory<DamageCalculation>;

 public:
  bool Initialize();
  void Finalize();

  // 伤害计算
  SkillResultType::type HurtCalculate(SkillRole *src, SkillRole *dest,
      core::uint32 hurt_id, core::int32 hit_type, const Skill *skill);

 private:
  DamageCalculation() {}
  ~DamageCalculation() {}

  ////////////////////////////////////
  // 伤害分类的公式计算
  // 伤害
  SkillResultType::type HurtCalculateAttack(SkillRole *src, SkillRole *dest,
      core::int32 coe, core::int32 skill_hurt_value, core::int32 threat_coe,
      core::int32 threat_value, bool physics, core::int32 hit_type, const Skill *skill);
  // 直接减血
  SkillResultType::type HurtCalculateReduceHp(SkillRole *src, SkillRole *dest,
      core::int32 hp_percent, core::int32 hp_value, core::int32 threat_coe,
      core::int32 threat_value, const Skill *skill);
  // 直接减魔
  SkillResultType::type HurtCalculateReduceMp(SkillRole *src, SkillRole *dest,
      core::int32 mp_percent, core::int32 mp_value, core::int32 threat_coe,
      core::int32 threat_value, const Skill *skill);
  // 治疗
  SkillResultType::type HurtCalculateCure(SkillRole *src, SkillRole *dest,
      core::int32 coe, core::int32 skill_cure_value, core::int32 threat_coe,
      core::int32 threat_value, bool physics, const Skill *skill);
  // 直接加血
  SkillResultType::type HurtCalculateAddHp(SkillRole *src, SkillRole *dest,
      core::int32 hp_percent, core::int32 hp_value, core::int32 threat_coe,
      core::int32 threat_value, const Skill *skill);
  // 直接回魔
  SkillResultType::type HurtCalculateAddMp(SkillRole *src, SkillRole *dest,
      core::int32 mp_percent, core::int32 mp_value, core::int32 threat_coe,
      core::int32 threat_value, const Skill *skill);
  // 净化buff
  SkillResultType::type HurtCalculateClearBuff(SkillRole *src, SkillRole *dest,
      core::int32 buff, core::int32 threat_coe, core::int32 threat_value,
      const Skill *skill);
  // 净化buff组
  SkillResultType::type HurtCalculateClearBuffGroup(SkillRole *src, SkillRole *dest,
      core::int32 buff_group, core::int32 threat_coe, core::int32 threat_value,
      const Skill *skill);
  ////////////////////////////////////

  // roll点，返回0为普通，1为忽视防御，2为闪避，3为暴击
  entity::SkillHitType::type HurtRoll(core::int32 ignore_def_per, core::int32 dodge_per,
      core::int32 crit_per, core::int32 hit_type);

  void BroadcastThreatEvent(SkillRole *src, SkillRole *dest, core::int32 threat_value);
  void BroadcastAttackHitEvent(SkillRole *src, SkillRole *dest,
      entity::SkillHitType::type hit_type, entity::SkillHurtType::type hurt_type, core::int32 value);
  void OnKillRole(SkillRole *killer_role, SkillRole *dead_role);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__DAMAGE__CALCULATION__H

