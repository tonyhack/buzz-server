//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 12:27:18.
// File name: skill_status.h
//
// Description:
// Define class SkillStatus.
//

#ifndef __GAME__SERVER__SKILL__SKILL__STATUS__H
#define __GAME__SERVER__SKILL__SKILL__STATUS__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/request/game_request_types.h"

namespace game {

namespace server {

namespace skill {

class SkillRole;

// 技能状态
struct SkillStatusType {
  enum type {
    MIN = 0,
    IDLE = 0,               // 待机状态
    BOOT,                   // 引导状态
    LAUNCH,                 // 释放状态
    FLIGHT,                 // 飞行状态
    FINISH,                 // 结束状态
    MAX,
  };
};

// 技能释放信息
class SkillContext {
 public:
  SkillContext() {}
  ~SkillContext() {}

  const SkillContext &operator=(const SkillContext &data) {
    this->id_ = data.id_;
    this->target_type_ = data.target_type_;
    this->target_id_ = data.target_id_;
    this->x_ = data.x_;
    this->y_ = data.y_;
    this->direction_ = data.direction_;
    return *this;
  }

  void Clear() {
    this->id_ = 0;
    this->target_type_ = entity::EntityType::TYPE_MAX;
    this->target_id_ = 0;
    this->x_ = 0;
    this->y_ = 0;
    this->direction_ = 0;
  }

  // 技能ID
  core::uint32 id_;

  // 技能当前目标
  entity::EntityType::type target_type_;
  core::uint64 target_id_;

  // 技能当前位置目标
  core::uint32 x_;
  core::uint32 y_;

  // 技能当前方向
  core::uint16 direction_;
};

class SkillStatus : public core::Noncopyable {
 public:
  SkillStatus();
  virtual ~SkillStatus();

  bool Initialize(SkillRole *role);
  void Finalize();

  inline bool CheckLeaving() const {
    return this->leaving_;
  }
  inline void SetLeaving(bool leaving) {
    this->leaving_ = leaving;
  }

  // 当前状态
  virtual SkillStatusType::type GetStatus() const = 0;

  // 状态进入/退出
  virtual void OnEnter() = 0;
  virtual void OnLeave() = 0;

  // 消息处理
  virtual int OnExecute(request::RequestType::type type,
      const void *data, size_t size) = 0;

  // 事件处理
  virtual void OnEvent(event::EventType::type type, const void *message,
      size_t size) {}

 protected:
  SkillRole *role_;

  bool leaving_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__STATUS__H
