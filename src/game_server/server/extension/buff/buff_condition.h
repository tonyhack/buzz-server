//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-26 10:58:51.
// File name: buff_condition.h
//
// Description:
// Define buff condition.
//

#ifndef __GAME__SERVER__BUFF__BUFF__CONDITION__H
#define __GAME__SERVER__BUFF__BUFF__CONDITION__H

#include "game_server/server/extension/buff/condition.h"

namespace game {

namespace server {

namespace buff {

class BuffCondition : public Condition {
 public:
  BuffCondition();
  virtual ~BuffCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::BUFF;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventBuffStart(const void *data, size_t size);
  bool OnEventBuffEnd(const void *data, size_t size);
};

class BuffGroupCondition : public Condition {
 public:
  BuffGroupCondition();
  virtual ~BuffGroupCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::BUFF_GROUP;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

 private:
  bool OnEventBuffStart(const void *data, size_t size);
  bool OnEventBuffEnd(const void *data, size_t size);
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__BUFF__CONDITION__H

