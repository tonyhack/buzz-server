#ifndef __SESSION__SERVER__SOCIAL__HOMELAND__BATTLE__H
#define __SESSION__SERVER__SOCIAL__HOMELAND__BATTLE__H

#include <map>
#include <set>
#include <vector>

#include "core/base/types.h"
#include "entity/homeland_types.h"

namespace gateway { namespace protocol {
class HomelandBattleRoundData;
}}

namespace session {

namespace server {

namespace social {

class HomelandBattleUnit {
 public:
  struct SpecialSkillType {
    enum type {
      AXE,       // 目标以及上下两格
      SHOOT,     // 整列
      MAGIC,     // 目标以及上下左右四格
    };
  };

  HomelandBattleUnit() {}
  ~HomelandBattleUnit() {}

  bool CheckDead() const { return this->hp_ <= 0; }

 public:
  core::int32 row_;
  core::int32 column_;
  core::int32 id_;
  core::int32 hp_;
  core::int32 attack_;
  core::int32 dodge_percent_;
  core::int32 crit_percent_;
  core::int32 crit_effect_; 
  SpecialSkillType::type special_skill_type_;
  core::int32 special_skill_odds_;
  core::int32 special_skill_hurt_coe_;
};

class HomelandBattle {
 public:
  typedef std::map<core::uint64, HomelandBattleUnit *> HomelandBattleUnitMap;
  typedef std::vector<gateway::protocol::HomelandBattleRoundData> HomelandBattleRoundDataVector;
  typedef std::map<core::int32, std::set<core::int32> > HomelandBattleAliveUnitIndex;

  HomelandBattle();

  HomelandBattle(HomelandBattleUnitMap *left_units, HomelandBattleUnitMap *right_units,
      HomelandBattleRoundDataVector *record);
  ~HomelandBattle();

  void DoLeftAoe(core::int32 cast_unit_id,
      entity::HomelandBattleUnitType::type cast_unit_type, core::int32 hurt,
      core::int32 *left_alive, core::int32 *right_alive);
  void DoRightAoe(core::int32 cast_unit_id,
      entity::HomelandBattleUnitType::type cast_unit_type, core::int32 hurt,
      core::int32 *left_alive, core::int32 *right_alive);
  void DoBattle(core::int32 max_round, core::int32 *left_alive, core::int32 *right_alive);

 private:
  void NewRoundStart();
  void DoAoe(core::int32 cast_unit_id,
      entity::HomelandBattleUnitType::type cast_unit_type, core::int32 hurt, 
      HomelandBattleUnitMap *enemies,
      HomelandBattleAliveUnitIndex *enemy_alive_index, core::int32 *enemy_alive);
  HomelandBattleUnit *GetLeftDoActionUnit();
  HomelandBattleUnit *GetRightDoActionUnit();
  void DoAction(HomelandBattleUnit *unit, HomelandBattleUnitMap *enemies,
      HomelandBattleAliveUnitIndex *enemy_alive_index, core::int32 *enemy_alive);

  HomelandBattleUnitMap *left_units_;
  HomelandBattleUnitMap *right_units_;
  HomelandBattleRoundDataVector *record_;
  gateway::protocol::HomelandBattleRoundData *current_round_;
  HomelandBattleUnitMap::iterator left_iter_;
  HomelandBattleUnitMap::iterator right_iter_;
  HomelandBattleAliveUnitIndex left_alive_index_;
  HomelandBattleAliveUnitIndex right_alive_index_;
  core::int32 left_alive_;
  core::int32 right_alive_;
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__HOMELAND__BATTLE__H

