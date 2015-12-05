//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-24 16:36:43.
// File name: actor_compare_event_handler.h
//
// Description:
// Define class ActorCompareEventHandler.
//

#ifndef __GAME__SERVER__ATTR__ACTOR__COMPARE__EVENT__HANDLER__H
#define __GAME__SERVER__ATTR__ACTOR__COMPARE__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace attr {

class AttrActor;

class ActorCompareEventHandler : public core::Noncopyable {
 public:
  ActorCompareEventHandler();
  ~ActorCompareEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnSwordCompare(AttrActor *compare_attr_actor,
      AttrActor *attr_actor, core::uint64 actor);
  void OnNobleCompare(AttrActor *compare_attr_actor,
      AttrActor *attr_actor, core::uint64 actor);
  void OnElementCompare(AttrActor *compare_attr_actor,
      AttrActor *attr_actor, core::uint64 actor);
  void OnSpiritCompare(AttrActor *compare_attr_actor,
      AttrActor *attr_actor, core::uint64 actor);

  coresh::Communicator::Connection connection_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ACTOR__COMPARE__EVENT__HANDLER__H

