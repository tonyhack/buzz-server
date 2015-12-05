//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 15:59:13.
// File name: hp_condition.h
//
// Description:
// Define hp condition.
//

#ifndef __GAME__SERVER__BUFF__HP__CONDITION__H
#define __GAME__SERVER__BUFF__HP__CONDITION__H

#include "game_server/server/extension/buff/condition.h"

namespace game {

namespace server {

namespace buff {

class HpPercentBelowCondition : public Condition {
 public:
  HpPercentBelowCondition();
  virtual ~HpPercentBelowCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::HP_PERCENT_BELOW;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeHp(const void *data, size_t size);
};

class HpPercentAboveCondition : public Condition {
 public:
  HpPercentAboveCondition();
  virtual ~HpPercentAboveCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::HP_PERCENT_ABOVE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeHp(const void *data, size_t size);
};

class HpPercentEqualCondition : public Condition {
 public:
  HpPercentEqualCondition();
  virtual ~HpPercentEqualCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::HP_PERCENT_EQUAL;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeHp(const void *data, size_t size);
};

class HpValueBelowCondition : public Condition {
 public:
  HpValueBelowCondition();
  virtual ~HpValueBelowCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::HP_VALUE_BELOW;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeHp(const void *data, size_t size);
};

class HpValueAboveCondition : public Condition {
 public:
  HpValueAboveCondition();
  virtual ~HpValueAboveCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::HP_VALUE_ABOVE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeHp(const void *data, size_t size);
};

class HpValueEqualCondition : public Condition {
 public:
  HpValueEqualCondition();
  virtual ~HpValueEqualCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::HP_VALUE_EQUAL;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventChangeHp(const void *data, size_t size);
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__HP__CONDITION__H

