//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-22 03:09:44.
// File name: rune_message_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__ATTR__RUNE__MESSAGE__HANDLER__H
#define __GAME__SERVER__ATTR__RUNE__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace attr {

class RuneMessageHandler : public core::Noncopyable {
 public:
  RuneMessageHandler();
  ~RuneMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 符文移动
  void OnMessageRuneMove(core::uint64 id, const char *data, size_t size);
  // 符文合成
  void OnMessageRuneCompose(core::uint64 id, const char *data, size_t size);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__RUNE__MESSAGE__HANDLER__H
