//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 14:38:15.
// File name: actor_message_handler.h
//
// Description:
// Define class PlayingMessageHandler.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__MESSAGE__HANDLER__H
#define __GAME__SERVER__PLAYING__PLAYING__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace playing {

class PlayingMessageHandler : public core::Noncopyable {
 public:
  PlayingMessageHandler();
  ~PlayingMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessagePlayingCreateRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingCompleteRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingLeaveRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingAwardRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingAutoRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingAutoFinishRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingPaidAwardRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__MESSAGE__HANDLER__H

