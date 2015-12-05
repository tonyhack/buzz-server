//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-11 21:11:04.
// File name: transport_condition.h
//
// Description:
// Define class TransportCondition.
//

#ifndef __GAME__SERVER__TASK__TRANSPORT__CONDITION__H
#define __GAME__SERVER__TASK__TRANSPORT__CONDITION__H

#include "game_server/server/extension/task/condition.h"

namespace game {

namespace server {

namespace task {

class Task;

class TransportCondition : public Condition {
 public:
  TransportCondition();
  virtual ~TransportCondition();

  virtual bool Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value);
  virtual void Finalize();

  virtual bool ExportConditionVar(core::uint32 &var, core::uint32 &value);

  virtual entity::TaskCondition::type GetType() const {
    return entity::TaskCondition::TRANSPORT;
  }

  virtual bool CheckCondition();
  virtual void Synchronize();

  virtual void OnEvent(event::EventType::type type,
      const void *data, size_t size);
  
 private:
  void OnEventTransportFailure(const void *data, size_t size);

  core::int32 map_;
  core::int32 x_;
  core::int32 y_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TRANSPORT__CONDITION__H

