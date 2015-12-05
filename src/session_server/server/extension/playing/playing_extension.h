//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-23 16:36:24.
// File name: playing_extension.h
//
// Description:
// Define class PlayingExtension.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__EXTENSION__H
#define __SESSION__SERVER__PLAYING__PLAYING__EXTENSION__H

#include "core/base/noncopyable.h"

#include "session_server/server/extension/playing/actor_change_fight_score_event_handler.h"
#include "session_server/server/extension/playing/actor_jump_scene_event_handler.h"
#include "session_server/server/extension/playing/actor_levelup_event_handler.h"
#include "session_server/server/extension/playing/actor_login_event_handler.h"
#include "session_server/server/extension/playing/actor_logout_event_handler.h"
#include "session_server/server/extension/playing/game_maze_message_handler.h"
#include "session_server/server/extension/playing/game_message_handler.h"
#include "session_server/server/extension/playing/playing_message_handler.h"
#include "session_server/server/extension/playing/playing_request_handler.h"
#include "session_server/server/extension/playing/system_event_handler.h"

namespace session {

namespace server {

namespace playing {

class PlayingExtension : public core::Noncopyable {
 public:
  PlayingExtension();
  ~PlayingExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  ActorChangeFightScoreEventHandler actor_change_fight_score_event_;
  ActorJumpSceneEventHandler actor_jump_event_;
  ActorLevelupEventHandler actor_levelup_event_;
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  SystemEventHandler system_event_;

  GameMessageHandler game_message_handler_;
  GameMazeMessageHandler game_maze_message_handler_;

  PlayingMessageHandler playing_message_handler_;

  PlayingRequestHandler playing_request_handler_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__EXTENSION__H

