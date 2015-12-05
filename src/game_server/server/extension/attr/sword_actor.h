//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-07 13:12:11.
// File name: sword_actor.h
//
// Description:
// Define class SwordActor.
//

#ifndef __GAME__SERVER__ATTR__SWORD__ACTOR__H
#define __GAME__SERVER__ATTR__SWORD__ACTOR__H

#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_attr_actor_login_types.h"

namespace game {

namespace server {

namespace attr {

class AttrActor;

class SwordActor : public core::TimerHandler, public core::Noncopyable {
  enum { DISABLE_TIMER = 0, };

 public:
  SwordActor();
  virtual ~SwordActor();

  bool Initialize(AttrActor *attr_actor, const database::protocol::StorageAttrActorField &db_field);
  void Finalize();

  void Save(database::protocol::StorageAttrActorField &db_field);

  // 获取/设置当前最大阶数
  inline core::int32 GetAvailStep() const {
    return this->avail_step_;
  }
  void SetAvailStep(core::int32 step);

  // 获取/设置当前阶数
  core::int32 GetCurrentStep() const;
  void SetCurrentStep(core::int32 step);

  // 获取/设置当前等级
  inline core::int32 GetLevel() const {
    return this->level_;
  }
  void SetLevel(core::int32 level);

  // 获取/设置幸运值
  inline core::int32 GetLuckyValue() const {
    return this->lucky_value_;
  }
  inline void SetLuckyValue(core::int32 value) {
    this->lucky_value_ = value;
  }

  // 获取/设置获取时间
  inline time_t GetGainTime() const {
    return this->gain_time_;
  }
  inline void SetGainTime(time_t time) {
    this->gain_time_ = time;
  }

  // 每日清理
  void DailyClean();

  // 开启失效定时器
  void StartExpireTimer();

  // 开启魔剑功能
  bool EnableSwordFunc();

  // 更新魔剑战斗力
  void UpdateSwordFightScore(bool synchronize = true);

  virtual void OnTimer(core::uint32 id);

 private:
  AttrActor *attr_actor_;

  // 当前可用最大阶数
  core::int32 avail_step_;

  // 当前等级
  core::int32 level_;

  // 幸运值
  core::int32 lucky_value_;

  // 魔剑的获取时间
  time_t gain_time_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__SWORD__ACTOR__H

