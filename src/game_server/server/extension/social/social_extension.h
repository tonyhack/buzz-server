#ifndef __GAME__SERVER__SOCIAL__SOCIAL__EXTENSION__H
#define __GAME__SERVER__SOCIAL__SOCIAL__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/social/extension_load_event_handler.h"
#include "game_server/server/extension/social/actor_logout_event_handler.h"
#include "game_server/server/extension/social/soul_add_event_handler.h"
#include "game_server/server/extension/social/soul_upgrade_event_handler.h"
#include "game_server/server/extension/social/session_homeland_message_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace social {

class SocialExtension : public global::SingletonFactory<SocialExtension> {
  friend class global::SingletonFactory<SocialExtension>;

 public:
  SocialExtension();
  ~SocialExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ExtensionLoadEventHandler extension_load_event_;
  ActorLogoutEventHandler actor_logout_event_;
  SoulAddEventHandler soul_add_event_;
  SoulUpgradeEventHandler soul_upgrade_event_;

  // 消息回调
  SessionHomelandMessageHandler session_homeland_message_handler_;
};

}  // namespace social

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOCIAL__SOCIAL__EXTENSION__H

