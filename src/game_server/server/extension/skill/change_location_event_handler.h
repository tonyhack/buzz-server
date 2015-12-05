//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-08 15:26:50.
// File name: change_location_event_handler.h
//
// Description:
// Define class ChangeLocationEventHandler.
//

#ifndef __GAME__SERVER__SKILL__CHANGE__LOCATION__EVENT__HANDLER__H
#define __GAME__SERVER__SKILL__CHANGE__LOCATION__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace skill {

class ChangeLocationEventHandler : public core::Noncopyable {
 public:
  ChangeLocationEventHandler();
  ~ChangeLocationEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection1_;
  coresh::Communicator::Connection connection2_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__CHANGE__LOCATION__EVENT__HANDLER__H

