#ifndef __GAME__SERVER__ATTR__ACTOR__SAVE__EVENT__HANDLER__H
#define __GAME__SERVER__ATTR__ACTOR__SAVE__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace attr {

class ActorSaveEventHandler : public core::Noncopyable {
 public:
  ActorSaveEventHandler();
  ~ActorSaveEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_save_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ACTOR__SAVE__EVENT__HANDLER__H

