//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-29 19:43:01.
// File name: request_task_max_layer.h
//
// Description:
//

#ifndef __GAME__SERVER__TASK__REQUEST__TASK__MAX__LAYER__H
#define __GAME__SERVER__TASK__REQUEST__TASK__MAX__LAYER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace task {

class RequestTaskMaxLayer : public core::Noncopyable {
 public:
  RequestTaskMaxLayer();
  ~RequestTaskMaxLayer();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__REQUEST__TASK__MAX__LAYER__H
