//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-21 20:38:57.
// File name: change_vip_event_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__ITEM__CHANGE__VIP__EVENT__HANDLER__H
#define __GAME__SERVER__ITEM__CHANGE__VIP__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace item {

class ChangeVipEventHandler : public core::Noncopyable {
 public:
  ChangeVipEventHandler();
  ~ChangeVipEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__CHANGE__VIP__EVENT__HANDLER__H

