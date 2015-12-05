
#ifndef __GAME__SERVER__SCENE__FIGHTING__STATUS__EVENT__HANDLER__H
#define __GAME__SERVER__SCENE__FIGHTING__STATUS__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace scene {

class FightingStatusEventHandler : public core::Noncopyable {
 public:
  FightingStatusEventHandler();
  ~FightingStatusEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventEnterPVPFightingStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventLeavePVPFightingStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_enter_;
  coresh::Communicator::Connection conn_leave_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__FIGHTING__STATUS__EVENT__HANDLER__H

