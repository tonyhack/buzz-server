//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-25 10:08:54.
// File name: actor_message_handler.h
//
// Description:
// Define class ActorMessageHandler.
//

#ifndef __SESSION__SERVER__ACTOR__MESSAGE__HANDLER__H
#define __SESSION__SERVER__ACTOR__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

class ActorMessageHandler : public core::Noncopyable {
 public:
  ActorMessageHandler();
  ~ActorMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageActorFacadeRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__ACTOR__MESSAGE__HANDLER__H

