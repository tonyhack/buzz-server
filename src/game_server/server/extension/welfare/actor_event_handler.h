#ifndef __GAME__SERVER__WELFARE__ACTOR__EVENT__HANDLER__H
#define __GAME__SERVER__WELFARE__ACTOR__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace welfare {

class ActorEventHandler {
 public:
  ActorEventHandler();
  ~ActorEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventRelive(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventSceneNum(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventResourceChange(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventVipStatusChange(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventFuncationalityOpen(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_relive_;
  coresh::Communicator::Connection conn_scene_num_;
  coresh::Communicator::Connection conn_resource_change_;
  coresh::Communicator::Connection conn_vip_status_change_;
  coresh::Communicator::Connection conn_funcationality_open_;

};

} // namespace welfare

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__WELFARE__ACTOR__EVENT__HANDLER__H
