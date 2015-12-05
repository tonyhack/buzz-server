//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-11 14:10:17.
// File name: actor_change_attribute_event_handler.h
//
// Description:
// Define class ActorChangeAttributeEventHandler.
//

#ifndef __GAME__SERVER__TEAM__ACTOR__CHANGE__ATTRIBUTE__EVENT__HANDLER__H
#define __GAME__SERVER__TEAM__ACTOR__CHANGE__ATTRIBUTE__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace team {

class ActorChangeAttributeEventHandler : public core::Noncopyable {
 public:
  ActorChangeAttributeEventHandler();
  ~ActorChangeAttributeEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnChangeHpEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnChangeMaxHpEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnChangeMpEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnChangeMaxMpEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnChangeLocationEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_change_hp_;
  coresh::Communicator::Connection conn_change_max_hp_;
  coresh::Communicator::Connection conn_change_mp_;
  coresh::Communicator::Connection conn_change_max_mp_;
  coresh::Communicator::Connection conn_change_location_;
};

}  // namespace team

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEAM__ACTOR__CHANGE__ATTRIBUTE__EVENT__HANDLER__H

