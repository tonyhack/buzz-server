//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-03 20:05:09.
// File name: bar_extension.h
//
// Description:
//
#ifndef __GAME__SERVER__BAR__BAR__EXTENSION__H
#define __GAME__SERVER__BAR__BAR__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/bar/actor_die_event_handler.h"
#include "game_server/server/extension/bar/actor_attacked_event_handler.h"
#include "game_server/server/extension/bar/actor_login_event_handler.h"
#include "game_server/server/extension/bar/actor_logout_event_handler.h"
#include "game_server/server/extension/bar/actor_move_event_handler.h"
#include "game_server/server/extension/bar/actor_use_item_event_handler.h"
#include "game_server/server/extension/bar/actor_use_skill_event_handler.h"
#include "game_server/server/extension/bar/bar_message_handler.h"
#include "game_server/server/extension/bar/request_bar_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace bar {

class BarExtension : public global::SingletonFactory<BarExtension> {
  friend class global::SingletonFactory<BarExtension>;
 public:
  BarExtension();
  ~BarExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ActorLoginEventHandler login_event_handler_;
  ActorLogoutEventHandler logout_event_handler_;
  ActorMoveEventHandler move_event_handler_;
  ActorAttackedEventHandler hurt_event_handler_;
  ActorUseSkillEventHandler use_skill_event_handler_;
  ActorUseItemEventHandler use_item_event_handler_;
  ActorDieEventHandler die_event_handler_;

  // 消息回调
  BarMessageHandler message_handler_;

  // 请求
  RequestBarHandler request_handler_;
};

}  // namespace bar

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BAR__BAR__EXTENSION__H

