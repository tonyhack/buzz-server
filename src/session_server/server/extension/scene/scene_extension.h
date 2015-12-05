//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-26 16:18:22.
// File name: scene_extension.h
//
// Description:
// Define class SceneExtension.
//

#ifndef __SESSION__SERVER__SCENE__SCENE__EXTENSION__H
#define __SESSION__SERVER__SCENE__SCENE__EXTENSION__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension/scene/actor_login_event_handler.h"
#include "session_server/server/extension/scene/actor_logout_event_handler.h"
#include "session_server/server/extension/scene/game_message_handler.h"
#include "session_server/server/extension/scene/request_get_map_scene_id.h"
#include "session_server/server/extension/scene/request_scene_get_actors.h"
#include "session_server/server/extension/scene/request_scene_get_game_server.h"
#include "session_server/server/extension/scene/request_scene_jump.h"
#include "session_server/server/extension/scene/server_logout_event_handler.h"
#include "session_server/server/extension/scene/scene_message_handler.h"

namespace session {

namespace server {

namespace scene {

class SceneExtension : public core::Noncopyable {
 public:
  SceneExtension();
  ~SceneExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  GameMessageHandler game_message_event_;
  SceneMessageHandler scene_message_handler_;

  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  ServerLogoutEventHandler server_logout_event_;

  RequestGetMapSceneID request_get_map_scene_id_;
  RequestSceneGetActors request_scene_get_actors_;
  RequestSceneGetGameServer request_scene_get_game_server_;
  RequestSceneJump request_scene_jump_;
};

}  // namespace scene

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__SCENE__EXTENSION__H

