#ifndef __GAME__SERVER__GUILD__GUILD__EXTENSION__H
#define __GAME__SERVER__GUILD__GUILD__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/guild/login_session_event_handler.h"
#include "game_server/server/extension/guild/extension_load_event_handler.h"
#include "game_server/server/extension/guild/actor_logout_event_handler.h"
#include "game_server/server/extension/guild/scene_role_create_event_handler.h"
#include "game_server/server/extension/guild/session_guild_message_handler.h"
#include "game_server/server/extension/guild/guild_message_handler.h"
#include "game_server/server/extension/guild/guild_request_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace guild {

class GuildExtension : public global::SingletonFactory<GuildExtension> {
 friend class global::SingletonFactory<GuildExtension>;
 public:
  GuildExtension();
  ~GuildExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  LoginSessionEventHandler login_session_event_;
  ExtensionLoadEventHandler extension_load_event_;
  ActorLogoutEventHandler actor_logout_event_;
  SceneRoleCreateEventHandler scene_role_create_event_;

  // 消息回调
  SessionGuildMessageHandler session_guild_message_handler_;
  GuildMessageHandler guild_message_handler_;

  // 请求回调
  GuildRequestHandler guild_request_handler_;
};

}  // namespace guild

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GUILD__GUILD__EXTENSION__H

