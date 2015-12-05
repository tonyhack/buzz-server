//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-18 16:50:45.
// File name: actor_message_handler.h
//
// Description:
// Define class ActorMessageHandler.
//

#ifndef __GAME__SERVER__SCENE__ACTOR__MESSAGE__HANDLER__H
#define __GAME__SERVER__SCENE__ACTOR__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class ActorMessageHandler : public core::Noncopyable {
 public:
  ActorMessageHandler();
  ~ActorMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageActorMove(core::uint64 id, const char *data, size_t size);
  void OnMessageRequestJump(core::uint64 id, const char *data, size_t size);
  void OnMessageActorChooseReviveMode(core::uint64 id, const char *data, size_t size);
  void OnMessageActorGather(core::uint64 id, const char *data, size_t size);
  void OnMessagePickUpRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageZazenRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageCancelZazenRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__ACTOR__MESSAGE__HANDLER__H

