//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-11 00:43:37.
// File name: bar_message_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__BAR__BAR__MESSAGE__HANDLER__H
#define __GAME__SERVER__BAR__BAR__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace bar {

class BarMessageHandler : public core::Noncopyable {
 public:
  BarMessageHandler();
  ~BarMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 请求完成进度条
  void OnMessageFinishBar(core::uint64 id, const char *data, size_t size);

};

}  // namespace bar

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BAR__BAR__MESSAGE__HANDLER__H

