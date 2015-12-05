#ifndef __GAME__SERVER__WELFARE__WELFARE__EXTENSION__H
#define __GAME__SERVER__WELFARE__WELFARE__EXTENSION__H

#include "core/base/noncopyable.h"
#include "global/singleton_factory.h"
#include "game_server/server/extension/welfare/actor_daily_clean_event_handler.h"
#include "game_server/server/extension/welfare/actor_event_handler.h"
#include "game_server/server/extension/welfare/actor_extension_event_handler.h"
#include "game_server/server/extension/welfare/actor_logout_event_handler.h"
#include "game_server/server/extension/welfare/actor_message_handler.h"
#include "game_server/server/extension/welfare/attr_event_handler.h"
#include "game_server/server/extension/welfare/item_event_handler.h"
#include "game_server/server/extension/welfare/request_handler.h"
#include "game_server/server/extension/welfare/role_die_event_handler.h"
#include "game_server/server/extension/welfare/scene_event_handler.h"
#include "game_server/server/extension/welfare/skill_event_handler.h"
#include "game_server/server/extension/welfare/storage_login_response.h"

namespace game {

namespace server {

namespace welfare {

class WelfareExtension : public global::SingletonFactory<WelfareExtension> {
 friend class global::SingletonFactory<WelfareExtension>;

 public:
  WelfareExtension();
  ~WelfareExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();
  
 private:
  // 事件
  AttrEventHandler attr_event_;
  ItemEventHandler item_event_;
  RoleDieEventHandler die_event_;
  SkillEventHandler skill_event_;
  ActorEventHandler actor_event_;
  ActorExtensionEventHandler extension_event_;
  ActorLogoutEventHandler logout_event_;
  ActorDailyCleanEventHandler daily_clean_;
  SceneEventHandler scene_event_;

  // 消息
  ActorMessageHandler achieve_message_;
  
  // 数据库
  StorageLoginResponse response_;

  // 请求
  RequestHandler request_;

};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__WELFARE__EXTENSION__H
