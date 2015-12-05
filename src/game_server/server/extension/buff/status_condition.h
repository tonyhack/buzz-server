//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 19:14:58.
// File name: status_condition.h
//
// Description:
// Define status condition.
//

#ifndef __GAME__SERVER__BUFF__STATUS__CONDITION__H
#define __GAME__SERVER__BUFF__STATUS__CONDITION__H

#include "game_server/server/extension/buff/condition.h"

namespace game {

namespace server {

namespace buff {

class SoulStatusCondition : public Condition {
 public:
  SoulStatusCondition();
  virtual ~SoulStatusCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::SOUL_STATUS;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventCommonStatusChange(const void *data, size_t size);
};

class NotSoulStatusCondition : public Condition {
 public:
  NotSoulStatusCondition();
  virtual ~NotSoulStatusCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::NOT_SOUL_STATUS;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventCommonStatusChange(const void *data, size_t size);
};

class ForbidSkillStatusCondition : public Condition {
 public:
  ForbidSkillStatusCondition();
  virtual ~ForbidSkillStatusCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::FORBID_USE_SKILL;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventCommonStatusChange(const void *data, size_t size);
};

class ForbidMoveStatusCondition : public Condition {
 public:
  ForbidMoveStatusCondition();
  virtual ~ForbidMoveStatusCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::FORBID_MOVE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventCommonStatusChange(const void *data, size_t size);
};

class ForbidItemStatusCondition : public Condition {
 public:
  ForbidItemStatusCondition();
  virtual ~ForbidItemStatusCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::FORBID_USE_ITEM;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventCommonStatusChange(const void *data, size_t size);
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__STATUS__CONDITION__H

