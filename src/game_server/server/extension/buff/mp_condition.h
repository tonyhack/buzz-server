//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 15:59:13.
// File name: mp_condition.h
//
// Description:
// Define mp condition.
//

#ifndef __GAME__SERVER__BUFF__MP__CONDITION__H
#define __GAME__SERVER__BUFF__MP__CONDITION__H

#include "game_server/server/extension/buff/condition.h"

namespace game {

namespace server {

namespace buff {

class MpPercentBelowCondition : public Condition {
 public:
  MpPercentBelowCondition();
  virtual ~MpPercentBelowCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::MP_PERCENT_BELOW;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeMp(const void *data, size_t size);
};

class MpPercentAboveCondition : public Condition {
 public:
  MpPercentAboveCondition();
  virtual ~MpPercentAboveCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::MP_PERCENT_ABOVE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeMp(const void *data, size_t size);
};

class MpPercentEqualCondition : public Condition {
 public:
  MpPercentEqualCondition();
  virtual ~MpPercentEqualCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::MP_PERCENT_EQUAL;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeMp(const void *data, size_t size);
};

class MpValueBelowCondition : public Condition {
 public:
  MpValueBelowCondition();
  virtual ~MpValueBelowCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::MP_VALUE_BELOW;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeMp(const void *data, size_t size);
};

class MpValueAboveCondition : public Condition {
 public:
  MpValueAboveCondition();
  virtual ~MpValueAboveCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::MP_VALUE_ABOVE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeMp(const void *data, size_t size);
};

class MpValueEqualCondition : public Condition {
 public:
  MpValueEqualCondition();
  virtual ~MpValueEqualCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::MP_VALUE_EQUAL;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeMp(const void *data, size_t size);
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__MP__CONDITION__H

