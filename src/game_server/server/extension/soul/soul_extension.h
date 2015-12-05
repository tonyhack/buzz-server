//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 12:01:04.
// File name: soul_extension.h
//
// Description:
// Define class SoulExtension.
//

#ifndef __GAME__SERVER__SOUL__SOUL__EXTENSION__H
#define __GAME__SERVER__SOUL__SOUL__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/soul/actor_attr_event_handler.h"
#include "game_server/server/extension/soul/actor_daily_clean_event_handler.h"
#include "game_server/server/extension/soul/actor_functionality_enable_event_handler.h"
#include "game_server/server/extension/soul/actor_logout_event_handler.h"
#include "game_server/server/extension/soul/actor_save_event_handler.h"
#include "game_server/server/extension/soul/attr_event_handler.h"
#include "game_server/server/extension/soul/extension_load_event_handler.h"
#include "game_server/server/extension/soul/fighting_status_event_handler.h"
#include "game_server/server/extension/soul/request_soul_operate.h"
#include "game_server/server/extension/soul/role_killed_event_handler.h"
#include "game_server/server/extension/soul/soul_message_handler.h"
#include "game_server/server/extension/soul/soul_altar_message_handler.h"
#include "game_server/server/extension/soul/storage_soul_login_response.h"
#include "game_server/server/extension/soul/storage_soul_actor_login_response.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace soul {

class SoulExtension : public global::SingletonFactory<SoulExtension> {
 friend class global::SingletonFactory<SoulExtension>;
 public:
  SoulExtension();
  ~SoulExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ActorAttrEventHandler actor_attr_event_;
  ActorDailyCleanEventHandler actor_daily_clean_event_;
  ActorFunctionalityEnableEventHandler actor_functionality_enable_event_;
  ActorLogoutEventHandler actor_logout_event_;
  ActorSaveEventHandler actor_save_event_;
  AttrEventHandler attr_event_;
  ExtensionLoadEventHandler extension_load_event_;
  FightingStatusEventHandler fighting_status_event_;
  RoleKilledEventHandler role_killed_event_;

  // 请求回调
  RequestSoulOperate req_soul_operate_;

  // 存储过程回调
  StorageSoulLoginResponse storage_soul_login_;
  StorageSoulActorLoginResponse storage_soul_actor_login_;

  // 消息回调
  SoulMessageHandler soul_message_handler_;
  SoulAltarMessageHandler soul_altar_message_handler_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__EXTENSION__H

