//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-03 23:57:53.
// File name: buff_extension.h
//
// Description:
// Define class BuffExtension.
//

#ifndef __GAME__SERVER__BUFF__BUFF__EXTENSION__H
#define __GAME__SERVER__BUFF__BUFF__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/buff/actor_login_event_handler.h"
#include "game_server/server/extension/buff/actor_logout_event_handler.h"
#include "game_server/server/extension/buff/actor_save_event_handler.h"
#include "game_server/server/extension/buff/attribute_change_event_handler.h"
#include "game_server/server/extension/buff/extension_load_event_handler.h"
#include "game_server/server/extension/buff/buff_event_handler.h"
#include "game_server/server/extension/buff/npc_create_event_handler.h"
#include "game_server/server/extension/buff/request_buff_handler.h"
#include "game_server/server/extension/buff/role_basic_status_change_handler.h"
#include "game_server/server/extension/buff/scene_role_create_event_handler.h"
#include "game_server/server/extension/buff/skill_event_handler.h"
#include "game_server/server/extension/buff/storage_buff_login_response.h"
#include "game_server/server/extension/buff/storage_buff_save_response.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace buff {

class BuffExtension : public global::SingletonFactory<BuffExtension> {
 friend class global::SingletonFactory<BuffExtension>;
 public:
  BuffExtension();
  ~BuffExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  ActorSaveEventHandler actor_save_event_;
  AttributeChangeEventHandler attr_change_event_;
  NpcCreateEventHandler npc_create_event_;
  ExtensionLoadEventHandler extension_load_event_;
  RoleBasicStatusChangeHandler role_basic_status_change_event_;
  SceneRoleCreateEventHandler scene_role_create_event_;
  SkillEventHandler skill_event_;
  BuffEventHandler buff_event_;

  // 请求回调
  RequestBuffHandler req_buff_handler_;

  StorageBuffLoginResponse storage_buff_login_;
  StorageBuffSaveResponse storage_buff_save_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__BUFF__EXTENSION__H

