//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-22 18:37:43.
// File name: attribute_change_event_handler.h
//
// Description:
// Define class AttributeChangeEventHandler.
//

#ifndef __GAME__SERVER__BUFF__ATTRIBUTE__CHANGE__EVENT__HANDLER__H
#define __GAME__SERVER__BUFF__ATTRIBUTE__CHANGE__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace buff {

class AttributeChangeEventHandler : public core::Noncopyable {
 public:
  AttributeChangeEventHandler();
  ~AttributeChangeEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventRoleChangeHp(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleChangeMp(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleCommonStatusChange(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleChangeCurrentEnergy(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_npc_change_hp_;
  coresh::Communicator::Connection conn_actor_change_hp_;
  coresh::Communicator::Connection conn_npc_change_mp_;
  coresh::Communicator::Connection conn_actor_change_mp_;
  coresh::Communicator::Connection conn_npc_change_common_status_;
  coresh::Communicator::Connection conn_actor_change_common_status_;
  coresh::Communicator::Connection conn_npc_change_current_energy_;
  coresh::Communicator::Connection conn_actor_change_current_energy_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__ATTRIBUTE__CHANGE__EVENT__HANDLER__H

