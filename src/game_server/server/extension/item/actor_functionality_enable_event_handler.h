#ifndef __GAME__SERVER__ITEM__ACTOR__FUNCTIONALITY__ENABLE__EVENT__HANDLER__H
#define __GAME__SERVER__ITEM__ACTOR__FUNCTIONALITY__ENABLE__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace item {

class ActorFunctionalityEnableEventHandler : public core::Noncopyable {
 public:
  ActorFunctionalityEnableEventHandler();
  ~ActorFunctionalityEnableEventHandler();

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

#endif  // __GAME__SERVER__ITEM__ACTOR__FUNCTIONALITY__ENABLE__EVENT__HANDLER__H

