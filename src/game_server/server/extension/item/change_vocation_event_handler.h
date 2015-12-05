//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-01 17:36:35.
// File name: change_vocation_event_handler.h
//
// Description:
// Define class ChangeVocationEventHandler.
//

#ifndef __GAME__SERVER__ITEM__CHANGE__VOCATION__EVENT__HANDLER__H
#define __GAME__SERVER__ITEM__CHANGE__VOCATION__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace item {

class ChangeVocationEventHandler : public core::Noncopyable {
 public:
  ChangeVocationEventHandler();
  ~ChangeVocationEventHandler();

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

#endif  // __GAME__SERVER__ITEM__CHANGE__VOCATION__EVENT__HANDLER__H

