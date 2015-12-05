//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Ebar: lxf@gbar.com.
// Last modify: 2013-11-04 03:12:16.
// File name: request_bar_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__BAR__REQUEST__BAR__HANDLER__H
#define __GAME__SERVER__BAR__REQUEST__BAR__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace bar {

class RequestBarHandler : public core::Noncopyable {
 public:
  RequestBarHandler();
  ~RequestBarHandler();

  bool Initialize();
  void Finalize();

 private:
  // 请求安装进度条
  int OnRequestBarStart(void *message, size_t size);
  // 请求检查进度条
  int OnRequestBarCheck(void *message, size_t size);

};

}  // namespace bar

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BAR__REQUEST__BAR__HANDLER__H

