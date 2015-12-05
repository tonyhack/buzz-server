//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 17:28:16.
// File name: cooling_extension.h
//
// Description:
// Define class CoolingExtension.
//

#ifndef __GAME__SERVER__COOLING__COOLING__EXTENSION__H
#define __GAME__SERVER__COOLING__COOLING__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/cooling/actor_login_event_handler.h"
#include "game_server/server/extension/cooling/actor_logout_event_handler.h"
#include "game_server/server/extension/cooling/actor_save_event_handler.h"
#include "game_server/server/extension/cooling/npc_create_event_handler.h"
#include "game_server/server/extension/cooling/request_check_in_cooling.h"
#include "game_server/server/extension/cooling/request_start_cooling.h"
#include "game_server/server/extension/cooling/storage_cooling_login_response.h"
#include "game_server/server/extension/cooling/storage_cooling_save_response.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace cooling {

class CoolingExtension : public global::SingletonFactory<CoolingExtension> {
 friend class global::SingletonFactory<CoolingExtension>;
 public:
  CoolingExtension();
  ~CoolingExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 冷却扩展读取数据库回调
  StorageCoolingLoginResponse storage_cooling_login_;
  StorageCoolingSaveResponse storage_cooling_save_;

  // 事件回调
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  ActorSaveEventHandler actor_save_event_;
  NpcCreateEventHandler npc_create_event_;

  // 请求回调
  RequestCheckInCooling req_check_in_cooling_;
  RequestStartCooling req_start_cooling_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__COOLING__EXTENSION__H

