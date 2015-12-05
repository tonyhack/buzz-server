//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-23 19:43:01.
// File name: request_task_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__TASK__REQUEST__TASK__HANDLER__H
#define __GAME__SERVER__TASK__REQUEST__TASK__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace task {

class RequestTaskHandler : public core::Noncopyable {
 public:
  RequestTaskHandler();
  ~RequestTaskHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
  int OnCheckRequest(void *message, size_t size);
  int OnRequestTaskConditionCoordinate(void *message, size_t size);
  int OnRequestTaskAcceptAndFinishCoordinate(void *message, size_t size);
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__REQUEST__TASK__HANDLER__H

