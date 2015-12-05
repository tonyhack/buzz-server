//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 19:01:59.
// File name: playing_message_handler.h
//
// Description:
// Define class PlayingMessageHandler.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__MESSAGE__HANDLER__H
#define __SESSION__SERVER__PLAYING__PLAYING__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace playing {

class PlayingMessageHandler : public core::Noncopyable {
 public:
  PlayingMessageHandler();
  ~PlayingMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessagePlayingEnterRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingTeamListRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingTeamCreateRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingTeamJoinRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingTeamQuitRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingTeamReadyRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingTeamKickoutRequest(core::uint64 id, const char *data, size_t size);
  void OnMessagePlayingTeamCapacitySetRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__MESSAGE__HANDLER__H

