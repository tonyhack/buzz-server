//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 02:03:59.
// File name: buff.h
//
// Description:
// Define class Buff.
//

#ifndef __GAME__SERVER__BUFF__BUFF__H
#define __GAME__SERVER__BUFF__BUFF__H

#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"
#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/buff/configure.h"

namespace game {

namespace server {

class GameRole;

namespace buff {

class BuffRole;
class BuffCell;
class Condition;

class Buff : public core::TimerHandler {
  enum { BUFF_TIMER = 0, INTERVAL_TIMER = 1, };

  typedef std::vector<Condition *> ConditionVector;

 public:
  Buff();
  ~Buff();

  bool Initialize(BuffRole *role, core::int32 buff_id, core::uint32 during_time,
      entity::EntityType::type type, core::uint64 id);
  void Finalize();
  
  bool Start();
  bool End();

  inline void SetIndex(core::uint64 index) {
    this->index_ = index;
  }
  inline core::uint64 GetBuffIndex() const {
    return this->index_;
  }

  inline const BuffCell *GetBuffCell() {
    return this->buff_cell_;
  }
  inline core::int32 GetBuffID() const {
    if (this->buff_cell_)
      return this->buff_cell_->id_;
    return 0;
  }

  inline core::uint32 GetStackCount() const {
    return this->stack_count_;
  }

  inline bool IsNeedSaveDB() {
    return (this->buff_cell_ && this->buff_cell_->save_db_);
  }

  inline void SetSource(entity::EntityType::type type, core::uint64 id) {
    this->source_type_ = type;
    this->source_id_ = id;
  }
  inline entity::EntityType::type GetSourceType() const {
    return this->source_type_;
  }
  inline core::int64 GetSourceID() const {
    return this->source_id_;
  }

  core::int32 GetLeftTime(); 

  // 检测效果条件
  bool CheckEffectCondition() const;
  // 检测BUFF条件
  bool CheckBuffCondition() const;

  // 执行条件效果
  void ExecuteConditionEffect();
  // 执行条件BUFF
  void ExecuteConditionBuff();

  void StartConditionEffect();
  void StopConditionEffect();
  void StartConditionBuff();

  void OnEvent(event::EventType::type type, const void *data, size_t size);

  virtual void OnTimer(core::uint32 id);

 private:
  void OnTimerBuffEnd();
  void OnTimerInterval();

  void UpdateTimer();

  core::uint64 index_;
  BuffRole *buff_role_;
  const BuffCell *buff_cell_;

  // BUFF的结束时间
  time_t end_time_;

  // buff持续时间
  core::uint32 during_time_;

  // 叠加次数
  core::uint32 stack_count_;

  // BUFF来源
  entity::EntityType::type source_type_;
  core::uint64 source_id_;

  bool effect_condition_;
  ConditionVector effect_conditions_;

  ConditionVector buff_conditions_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__BUFF__H

