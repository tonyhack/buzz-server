//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-03 23:57:53.
// File name: effect_extension.h
//
// Description:
// Define class EffectExtension.
//

#ifndef __GAME__SERVER__EFFECT__EFFECT__EXTENSION__H
#define __GAME__SERVER__EFFECT__EFFECT__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/effect/actor_logout_event_handler.h"
#include "game_server/server/extension/effect/extension_load_event_handler.h"
#include "game_server/server/extension/effect/npc_create_event_handler.h"
#include "game_server/server/extension/effect/request_effect_end.h"
#include "game_server/server/extension/effect/request_effect_start.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace effect {

class EffectExtension : public global::SingletonFactory<EffectExtension> {
 friend class global::SingletonFactory<EffectExtension>;
 public:
  EffectExtension();
  ~EffectExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ActorLogoutEventHandler actor_logout_event_;
  ExtensionLoadEventHandler extension_load_event_;
  NpcCreateEventHandler npc_create_event_;

  // 请求回调
  RequestEffectStart req_effect_start_;
  RequestEffectEnd req_effect_end_;
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__EFFECT__EXTENSION__H

