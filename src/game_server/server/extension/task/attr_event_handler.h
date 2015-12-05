//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-11 23:52:16.
// File name: attr_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__ATTR__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__ATTR__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class AttrEventHandler : public core::Noncopyable {
 public:
  AttrEventHandler();
  ~AttrEventHandler();

  bool Initialize();
  void Finalize();

 private:

  // 精灵培养
  void OnSpiritFeedEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventConstellationUpGrade(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_spirit_feed_;
  coresh::Communicator::Connection conn_constellation_up_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__ATTR__EVENT__HANDLER__H

