//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-12 03:00:00.
// File name: task_transport.h
//
// Description:
// Define class TaskTransport.
//

#ifndef __GAME__SERVER__TASK__TASK__TRANSPORT__H
#define __GAME__SERVER__TASK__TASK__TRANSPORT__H

#include "core/base/noncopyable.h"

#include "database_server/protocol/storage_task_actor_login_types.h"
#include "entity/task_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/task_configure.h"

namespace game {

namespace server {

namespace task {

class TaskActor;

class TaskTransport : public core::Noncopyable {
 public:
  TaskTransport();
  ~TaskTransport();

  bool Initialize(TaskActor *actor, const database::protocol::StorageTaskActorField &data);
  void Finalize();

  void Synchronize();
  void Save(database::protocol::StorageTaskActorField &field);

  void DailyClean();

  const TaskTransportCell::AwardVector *GetFinishAwards(bool force_success) const;

  void OnEvent(event::EventType::type type, const void *data, size_t size);

  inline core::int32 GetFinishedCount() const {
    return this->finished_count_;
  }
  inline void SetFinishedCount(core::int32 count) {
    this->finished_count_ = count;
  }

  inline core::int32 GetFreeRefreshedCount() const {
    return this->free_refreshed_count_;
  }
  inline void SetFreeRefreshedCount(core::int32 count) {
    this->free_refreshed_count_ = count;
  }

  inline core::int32 GetRobedCount() const {
    return this->robed_count_;
  }
  inline void SetRobedCount(core::int32 count) {
    this->robed_count_ = count;
  }

  inline core::int32 GetCurrentTransport() const {
    return this->current_transport_;
  }
  inline void SetCurrentTransport(core::int32 transport) {
    this->current_transport_ = transport;
  }

  inline entity::TaskTransportStatus::type GetStatus() const {
    return this->status_;
  }
  inline void SetStatus(entity::TaskTransportStatus::type status) {
    this->status_ = status;
  }

 private:
  void OnEventTransportFailure(const void *data, size_t size);

  TaskActor *actor_;

  core::int32 finished_count_;
  core::int32 free_refreshed_count_;
  core::int32 robed_count_;
  core::int32 current_transport_;
  entity::TaskTransportStatus::type status_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__TRANSPORT__H

