#ifndef __SESSION__SERVER__BRIEF__ACTOR__H
#define __SESSION__SERVER__BRIEF__ACTOR__H

#include <cstdio>
#include <string>

#include "core/base/types.h"
#include "entity/gender_types.h"
#include "entity/fight_score_types.h"
#include "entity/functionality_types.h"
#include "entity/vip_types.h"
#include "entity/vocation_types.h"
#include "global/configure/load_vip_configure.h"

namespace session {

namespace server {

class BriefActor {
  typedef std::vector<core::uint8> FunctionalityStates;
  typedef std::vector<core::int32> FightScores;

 public:
  BriefActor(core::uint64 id, const std::string &name, core::int32 level,
      entity::VocationType::type vocation, entity::GenderType::type gender,
      core::uint16 map, const std::string &func_state,
      core::int32 axe_fight_score,
      core::int32 shoot_fight_score,
      core::int32 magic_fight_score,
      time_t last_logout_time,
      entity::VipType::type vip_type,
      time_t vip_expire_time);

  inline core::uint64 GetID() const {
    return this->id_;
  }
  inline const std::string GetStringID() const {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lu", this->id_);
    return std::string(buffer);
  }

  inline const std::string &GetName() const {
    return this->name_;
  }
  inline void SetName(const std::string &name) {
    this->name_ = name;
  }

  inline core::int32 GetLevel() const {
    return this->level_;
  }
  inline void SetLevel(core::int32 level) {
    this->level_ = level;
  }

  inline entity::VocationType::type GetVocation() const {
    return this->vocation_;
  }
  inline void SetVocation(entity::VocationType::type vocation) {
    this->vocation_ = vocation;
  }

  inline entity::GenderType::type GetGender() const {
    return this->gender_;
  }
  inline void SetGender(entity::GenderType::type gender) {
    this->gender_ = gender;
  }

  inline core::uint16 GetMap() const {
    return this->map_;
  }
  inline void SetMap(core::uint16 map) {
    this->map_ = map;
  }

  bool CheckFunctionalityState(entity::FunctionalityType::type type) const;
  void SetFunctionalityState(entity::FunctionalityType::type type, bool state);

  core::int32 GetFightScore(entity::FightScoreType::type type) const;
  void SetFightScore(entity::FightScoreType::type type, core::int32 score);
  core::int32 GetCurrentFightScore() const;

  inline const core::uint64 GetLastLogoutTime() {
    return this->last_logout_time_;
  }
  inline const std::string GetStringLastLogoutTime() const {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lu", this->last_logout_time_);
    return std::string(buffer);
  }
  inline void SetLastLogoutTime(core::uint64 last_logout_time) {
    this->last_logout_time_ = last_logout_time;
  }
  entity::VipType::type GetVipType() const {
    return this->vip_type_;
  }
  void SetVipType(entity::VipType::type type) {
    this->vip_type_ = type;
  }
  void SetVipExpireTime(time_t expire_time) {
    this->vip_expire_time_ = expire_time;
  }
  const global::configure::VipCell *GetVipPrivilege(
      entity::VipPrivilegeType::type type) const;
  const global::configure::VipCell *GetVipPrivilege(
      entity::VipPrivilegeType::type type, core::int32 key) const;

  static time_t VipTimeRemainderToExpired(time_t last_logout_time, 
      core::int32 remainder_time);

 private:
  core::uint64 id_;
  std::string name_;
  core::int32 level_;
  entity::VocationType::type vocation_;
  entity::GenderType::type gender_;
  core::uint16 map_;
  FunctionalityStates func_states_;
  FightScores fight_scores_;
  time_t last_logout_time_;
  entity::VipType::type vip_type_;
  time_t vip_expire_time_;
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__BRIEF__ACTOR__H

