//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 15:41:39.
// File name: condition.h
//
// Description:
// Define class Condition.
//

#ifndef __GAME__SERVER__BUFF__CONDITION__H
#define __GAME__SERVER__BUFF__CONDITION__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/buff_types.h"
#include "game_server/server/event/game_event_types.h"

namespace game {

namespace server {

namespace buff {

class BuffRole;

class Condition : public core::Noncopyable {
 public:
  Condition();
  virtual ~Condition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const = 0;

  virtual bool Check() const {
    return this->passed_;
  }

  // 返回值表示此次事件是否有效
  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size) = 0;

  // 条件全部达成时，执行回调
  virtual void OnExecute() {}

 protected:
  core::int32 id_;
  core::int32 param1_;
  bool passed_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__CONDITION__H

