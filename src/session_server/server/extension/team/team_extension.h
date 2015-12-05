//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-22 15:28:29.
// File name: team_extension.h
//
// Description:
// Define class TeamExtension.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__EXTENSION__H
#define __SESSION__SERVER__TEAM__TEAM__EXTENSION__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension/team/actor_jump_scene_event_handler.h"
#include "session_server/server/extension/team/actor_levelup_event_handler.h"
#include "session_server/server/extension/team/actor_login_event_handler.h"
#include "session_server/server/extension/team/actor_logout_event_handler.h"
#include "session_server/server/extension/team/game_message_handler.h"
#include "session_server/server/extension/team/team_message_handler.h"
#include "session_server/server/extension/team/team_request_handler.h"

namespace session {

namespace server {

namespace team {

class TeamExtension : public core::Noncopyable {
 public:
  TeamExtension();
  ~TeamExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 消息回调
  GameMessageHandler game_message_handler_;
  TeamMessageHandler team_message_handler_;

  // 事件回调
  ActorJumpSceneEventHandler actor_jump_scene_handler_;
  ActorLevelupEventHandler actor_levelup_handler_;
  ActorLoginEventHandler actor_login_handler_;
  ActorLogoutEventHandler actor_logout_handler_;

  // 请求回调
  TeamRequestHandler team_request_handler_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__EXTENSION__H

