//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-17 16:50:37.
// File name: threat_event_handler.h
//
// Description:
// Define class ThreatEventHandler.
//

#ifndef __GAME__SERVER__SKILL__THREAT__EVENT__HANDLER__H
#define __GAME__SERVER__SKILL__THREAT__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace skill {

class ThreatEventHandler : public core::Noncopyable {
 public:
  ThreatEventHandler();
  ~ThreatEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventAiThreatStart(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventAiThreatEnd(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_threat_start_;
  coresh::Communicator::Connection conn_threat_end_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__THREAT__EVENT__HANDLER__H

