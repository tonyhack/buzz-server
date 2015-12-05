#include "game_server/server/fight_score_calculator.h"

#include "game_server/server/configure.h"

namespace game {

namespace server {

FightScoreCalculator::FightScoreCalculator() :
  physique_(0),
  strength_(0),
  intelligence_(0),
  agile_(0),
  spirit_(0),
  max_hp_(0),
  max_mp_(0),
  physics_attack_(0),
  physics_defence_(0),
  magic_attack_(0),
  magic_defence_(0),
  dodge_(0),
  crit_(0),
  ignore_defence_(0),
  move_speed_(0),
  dodge_percent_add_value_(0),
  crit_percent_add_value_(0),
  crit_effect_percent_add_value_(0),
  ignore_defence_percent_add_value_(0),
  physics_hurt_reduce_add_percent_(0),
  magic_hurt_reduce_add_percent_(0),
  hurt_reduce_add_percent_(0),
  hurt_random_percent_add_(0),
  cure_percent_add_(0),
  skill_level_(0) {}

FightScoreCalculator::~FightScoreCalculator() {}

core::int32 FightScoreCalculator::GetFightScore() {
  const AttrFightScoreCell *cell = Configure::GetInstance()->GetAttrFightScoreCell();
  core::int64 fight_score = 0;

#define ADD_FIGHT_SCORE(_attr)                \
  if (this->_attr != 0) {                     \
    fight_score += this->_attr * cell->_attr; \
  }                                           \

#define ADD_FIGHT_SCORE_PERCENT(_attr)              \
  if (this->_attr != 0) {                           \
    fight_score += this->_attr * cell->_attr / 100; \
  }                                                 \

  ADD_FIGHT_SCORE(physique_)
  ADD_FIGHT_SCORE(strength_)
  ADD_FIGHT_SCORE(intelligence_)
  ADD_FIGHT_SCORE(agile_)
  ADD_FIGHT_SCORE(spirit_)
  ADD_FIGHT_SCORE(max_hp_)
  ADD_FIGHT_SCORE(max_mp_)
  ADD_FIGHT_SCORE(physics_attack_)
  ADD_FIGHT_SCORE(physics_defence_)
  ADD_FIGHT_SCORE(magic_attack_)
  ADD_FIGHT_SCORE(magic_defence_)
  ADD_FIGHT_SCORE(dodge_)
  ADD_FIGHT_SCORE(crit_)
  ADD_FIGHT_SCORE(ignore_defence_)
  ADD_FIGHT_SCORE(move_speed_)
  ADD_FIGHT_SCORE(skill_level_);
  ADD_FIGHT_SCORE_PERCENT(dodge_percent_add_value_)
  ADD_FIGHT_SCORE_PERCENT(crit_percent_add_value_)
  ADD_FIGHT_SCORE_PERCENT(crit_effect_percent_add_value_)
  ADD_FIGHT_SCORE_PERCENT(ignore_defence_percent_add_value_)
  ADD_FIGHT_SCORE_PERCENT(physics_hurt_reduce_add_percent_)
  ADD_FIGHT_SCORE_PERCENT(magic_hurt_reduce_add_percent_)
  ADD_FIGHT_SCORE_PERCENT(hurt_reduce_add_percent_)
  ADD_FIGHT_SCORE_PERCENT(hurt_random_percent_add_)
  ADD_FIGHT_SCORE_PERCENT(cure_percent_add_)

#undef ADD_FIGHT_SCORE
#undef ADD_FIGHT_SCORE_PERCENT

  return fight_score / 100;
}

}  // namespace server

}  // namespace game

