//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-21 02:20:59.
// File name: vip_alter_event_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__VIP__ALTER__EVENT__HANDLER__H
#define __GAME__SERVER__VIP__ALTER__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

class VipAlterEventHandler : public core::Noncopyable {
 public:
  VipAlterEventHandler();
  ~VipAlterEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__VIP__ALTER__EVENT__HANDLER__H

