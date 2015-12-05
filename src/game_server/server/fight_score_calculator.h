#ifndef __GAME__SERVER__FIGHT__SCORE__CALCULATOR__H
#define __GAME__SERVER__FIGHT__SCORE__CALCULATOR__H

#include "core/base/types.h"

namespace game {

namespace server {

class FightScoreCalculator {
 public:
  FightScoreCalculator();
  ~FightScoreCalculator();

  core::int32 GetFightScore();

  // 一级属性
  core::int32 physique_;
  core::int32 strength_;
  core::int32 intelligence_;
  core::int32 agile_;
  core::int32 spirit_;
  // 二级属性
  core::int32 max_hp_;
  core::int32 max_mp_;
  core::int32 physics_attack_;
  core::int32 physics_defence_;
  core::int32 magic_attack_;
  core::int32 magic_defence_;
  core::int32 dodge_;
  core::int32 crit_;
  core::int32 ignore_defence_;
  core::int32 move_speed_;
  core::int32 dodge_percent_add_value_;
  core::int32 crit_percent_add_value_;
  core::int32 crit_effect_percent_add_value_;
  core::int32 ignore_defence_percent_add_value_;
  core::int32 physics_hurt_reduce_add_percent_;
  core::int32 magic_hurt_reduce_add_percent_;
  core::int32 hurt_reduce_add_percent_;
  core::int32 hurt_random_percent_add_;
  core::int32 cure_percent_add_;
  // 技能等级
  core::int32 skill_level_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__FIGHT__SCORE__CALCULATOR__H

