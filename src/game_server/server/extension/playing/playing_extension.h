//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-23 12:01:53.
// File name: playing_extension.h
//
// Description:
// Define class PlayingExtension.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__EXTENSION__H
#define __GAME__SERVER__PLAYING__PLAYING__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/playing/actor_daily_clean_event_handler.h"
#include "game_server/server/extension/playing/actor_functionality_enable_event_handler.h"
#include "game_server/server/extension/playing/actor_load_finish_event_handler.h"
#include "game_server/server/extension/playing/actor_login_event_handler.h"
#include "game_server/server/extension/playing/actor_logout_event_handler.h"
#include "game_server/server/extension/playing/actor_save_event_handler.h"
#include "game_server/server/extension/playing/actor_vip_change_event_handler.h"
#include "game_server/server/extension/playing/actor_weekly_clean_event_handler.h"
#include "game_server/server/extension/playing/jump_scene_event_handler.h"
#include "game_server/server/extension/playing/login_session_event_handler.h"
#include "game_server/server/extension/playing/maze_message_handler.h"
#include "game_server/server/extension/playing/playing_message_handler.h"
#include "game_server/server/extension/playing/request_playing_handler.h"
#include "game_server/server/extension/playing/session_maze_message_handler.h"
#include "game_server/server/extension/playing/session_message_handler.h"
#include "game_server/server/extension/playing/storage_playing_actor_login_response.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace playing {

class PlayingExtension : public global::SingletonFactory<PlayingExtension> {
 friend class global::SingletonFactory<PlayingExtension>;
 public:
  PlayingExtension();
  ~PlayingExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  ActorLoadFinishEventHandler actor_load_finish_event_;
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  ActorSaveEventHandler actor_save_event_;
  ActorFunctionalityEnableEventHandler  actor_functionality_enable_event_;
  ActorDailyCleanEventHandler actor_daily_clean_event_;
  ActorWeeklyCleanEventHandler actor_weekly_clean_event_;
  ActorVipChangeEventHandler actor_vip_change_event_;
  JumpSceneEventHandler jump_scene_event_;
  LoginSessionEventHandler login_session_event_;

  SessionMessageHandler session_message_handler_;
  SessionMazeMessageHandler session_maze_message_handler_;

  PlayingMessageHandler playing_message_handler_;
  MazeMessageHandler maze_message_handler_;

  StoragePlayingActorLoginResponse storage_playing_actor_login_;

  RequestPlayingHandler request_playing_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__EXTENSION__H

