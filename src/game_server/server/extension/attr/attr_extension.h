#ifndef __GAME__SERVER__ATTR__ATTR__EXTENSION__H
#define __GAME__SERVER__ATTR__ATTR__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/attr/extension_load_event_handler.h"
#include "game_server/server/extension/attr/actor_compare_event_handler.h"
#include "game_server/server/extension/attr/actor_daily_clean_event_handler.h"
#include "game_server/server/extension/attr/actor_functionality_enable_event_handler.h"
#include "game_server/server/extension/attr/actor_logout_event_handler.h"
#include "game_server/server/extension/attr/actor_levelup_event_handler.h"
#include "game_server/server/extension/attr/actor_save_event_handler.h"
#include "game_server/server/extension/attr/actor_vocation_event_handler.h"
#include "game_server/server/extension/attr/scene_role_create_event_handler.h"
#include "game_server/server/extension/attr/bar_finish_event_handler.h"
#include "game_server/server/extension/attr/noble_message_handler.h"
#include "game_server/server/extension/attr/noble_request_handler.h"
#include "game_server/server/extension/attr/sword_message_handler.h"
#include "game_server/server/extension/attr/sword_request_handler.h"
#include "game_server/server/extension/attr/element_message_handler.h"
#include "game_server/server/extension/attr/spirit_message_handler.h"
#include "game_server/server/extension/attr/spirit_request_handler.h"
#include "game_server/server/extension/attr/rune_message_handler.h"
#include "game_server/server/extension/attr/legendary_weapon_message_handler.h"
#include "game_server/server/extension/attr/storage_attr_actor_login_response.h"
#include "game_server/server/extension/attr/storage_spirit_facade_login_response.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace attr {

class AttrExtension : public global::SingletonFactory<AttrExtension> {
 friend class global::SingletonFactory<AttrExtension>;
 public:
  AttrExtension();
  ~AttrExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ExtensionLoadEventHandler extension_load_event_;
  ActorCompareEventHandler actor_compare_event_;
  ActorLogoutEventHandler actor_logout_event_;
  ActorSaveEventHandler actor_save_event_;
  ActorDailyCleanEventHandler actor_daily_clean_event_;
  ActorLeveupEventHandler actor_levelup_event_;
  ActorFunctionalityEnableEventHandler actor_functionality_enable_event_;
  ActorVocationEventHandler actor_vocation_;
  SceneRoleCreateEventHandler scene_role_create_event_;
  BarFinishEventHandler bar_event_;
  
  // 消息回调
  NobleMessageHandler noble_message_handler_;
  SwordMessageHandler sword_message_handler_;
  ElementMessageHandler element_message_handler_;
  SpiritMessageHandler spirit_message_handler_;
  RuneMessageHandler rune_message_handler_;
  LegendaryWeaponMessageHandler legendary_weapon_message_handler_;

  // 请求回调
  NobleRequestHandler noble_request_handler_;
  SpiritRequestHandler spirit_request_handler_;
  SwordRequestHandler sword_request_handler_;

  // 数据库回调
  StorageAttrActorLoginResponse storage_attr_actor_login_;
  StorageSpiritFacadeLoginResponse storage_spirit_facade_login_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ATTR__EXTENSION__H

