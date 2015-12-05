//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-17 19:06:17.
// File name: game_message_handler.h
//
// Description:
// Define class GameMessageHandler.
//

#ifndef __SESSION__SERVER__PLAYING__GAME__MESSAGE__HANDLER__H
#define __SESSION__SERVER__PLAYING__GAME__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/configure/playing_configure.h"
#include "session_server/server/session_terminal.h"

namespace session {

namespace server {

namespace playing {

class Playing;

class GameMessageHandler : public core::Noncopyable {
 public:
  GameMessageHandler();
  ~GameMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessagePlayingCreateRequest(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessagePlayingCreateFinishRequest(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessagePlayingDestoryRequest(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessagePlayingEndRequest(SessionTerminal *terminal,
      const char *data, size_t size);
  void OnMessagePlayingActorPageSynchronize(SessionTerminal *terminal,
    const char *data, size_t size);

  Playing *CreateActorPlaying(core::uint64 actor, const global::configure::PlayingCell *cell);
  Playing *CreateTeamPlaying(core::uint64 actor, const global::configure::PlayingCell *cell);
  Playing *CreateGuildPlaying(core::uint64 actor, const global::configure::PlayingCell *cell);
  Playing *CreateRoomPlaying(core::uint64 actor, const global::configure::PlayingCell *cell);
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__GAME__MESSAGE__HANDLER__H

