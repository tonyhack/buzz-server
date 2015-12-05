//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 12:10:40.
// File name: team_extension.h
//
// Description:
// Define class TeamExtension.
//

#ifndef __GAME__SERVER__TEAM__TEAM__EXTENSION__H
#define __GAME__SERVER__TEAM__TEAM__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/team/actor_change_attribute_event_handler.h"
#include "game_server/server/extension/team/actor_login_event_handler.h"
#include "game_server/server/extension/team/actor_logout_event_handler.h"
#include "game_server/server/extension/team/scene_jump_map_event_handler.h"
#include "game_server/server/extension/team/scene_role_create_event_handler.h"
#include "game_server/server/extension/team/session_message_handler.h"
#include "game_server/server/extension/team/team_request_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace team {

class TeamExtension : public global::SingletonFactory<TeamExtension> {
 friend class global::SingletonFactory<TeamExtension>;
 public:
  TeamExtension();
  ~TeamExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  ActorChangeAttributeEventHandler actor_change_attr_event_;
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  SceneJumpMapEventHandler scene_jump_map_event_;
  SceneRoleCreateEventHandler scene_role_create_event_;

  SessionMessageHandler session_message_handler_;

  TeamRequestHandler team_request_handler_;
};

}  // namespace team

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEAM__TEAM__EXTENSION__H

