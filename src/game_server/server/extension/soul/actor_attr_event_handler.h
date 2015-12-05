//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-08 14:36:22.
// File name: actor_attr_event_handler.h
//
// Description:
// Define class ActorAttrEventHandler.
//

#ifndef __GAME__SERVER__SOUL__ACTOR__ATTR__EVENT__HANDLER__H
#define __GAME__SERVER__SOUL__ACTOR__ATTR__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace soul {

class ActorAttrEventHandler : public core::Noncopyable {
 public:
  ActorAttrEventHandler();
  ~ActorAttrEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventRoleChangeCurrentEnergy(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleChangeMaxEnergy(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleCommonStatusChange(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_change_current_energy_;
  coresh::Communicator::Connection conn_change_max_energy_;
  coresh::Communicator::Connection conn_common_status_change_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__ACTOR__ATTR__EVENT__HANDLER__H

