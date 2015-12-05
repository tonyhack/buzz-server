//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-17 11:10:28.
// File name: fighting_status.h
//
// Description:
// Define class FightingStatus.
//

#ifndef __GAME__SERVER__SKILL__FIGHTING__STATUS__H
#define __GAME__SERVER__SKILL__FIGHTING__STATUS__H

#include <time.h>

#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace skill {

class SkillRole;

class FightingStatus : public core::Noncopyable,
  public core::TimerHandler {
 public:
  FightingStatus();
  ~FightingStatus();

  bool Initialize(SkillRole *role);
  void Finalize();

  // 设置/重置 战斗状态
  bool SetStatus();
  void ResetStatus(bool force = false);

  // 设置 PVP战斗
  void SetPvpFighting();

  virtual void OnTimer(core::uint32 id);

  inline SkillRole *GetRole() {
    return this->role_;
  }

  inline bool GetStatus() const {
    return this->fighting_status_;
  }

  inline void SetLastPvpTime(time_t time) {
    this->last_pvp_time_ = time;
  }

  inline bool GetPvpStatus() const {
    return this->pvp_status_;
  }

 private:
  void EventEnter();
  void EventLeave();

  // 战斗状态
  bool fighting_status_;

  // 引用计数
  core::uint32 fighting_ref_count_;

  // PVP状态
  bool pvp_status_;

  // 上次进入pvp时间
  time_t last_pvp_time_;

  SkillRole *role_;

  // PVP持续时间(秒数)
  static const core::uint32 kPvpDurationSecs_ = 10;

  static const core::uint32 kTimerFightingCheckID_ = 1;
  static const core::uint32 kFightingCheckInterval_ = 2;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__FIGHTING__STATUS__H

