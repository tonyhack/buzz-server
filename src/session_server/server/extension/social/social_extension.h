#ifndef __SESSION__SERVER__SOCIAL__SOCIAL__EXTENSION__H
#define __SESSION__SERVER__SOCIAL__SOCIAL__EXTENSION__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension/social/actor_login_event_handler.h"
#include "session_server/server/extension/social/actor_logout_event_handler.h"
#include "session_server/server/extension/social/actor_save_event_handler.h"
#include "session_server/server/extension/social/actor_jump_scene_event_handler.h"
#include "session_server/server/extension/social/actor_level_up_event_handler.h"
#include "session_server/server/extension/social/actor_change_vocation_event_handler.h"
#include "session_server/server/extension/social/actor_functionality_enable_event_handler.h"
#include "session_server/server/extension/social/actor_daily_clean_event_handler.h"
#include "session_server/server/extension/social/contacts_message_handler.h"
#include "session_server/server/extension/social/homeland_message_handler.h"
#include "session_server/server/extension/social/homeland_battle_message_handler.h"
#include "session_server/server/extension/social/game_homeland_message_handler.h"
#include "session_server/server/extension/social/enemy_message_handler.h"
#include "session_server/server/extension/social/contacts_request_handler.h"
#include "session_server/server/extension/social/storage_social_actor_login_response.h"

namespace session {

namespace server {

namespace social {

class SocialExtension : public core::Noncopyable {
 public:
  SocialExtension();
  ~SocialExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  ActorSaveEventHandler actor_save_event_;
  ActorJumpSceneEventHandler actor_jump_scene_event_;
  ActorLevelUpEventHandler actor_level_up_event_;
  ActorChangeVocationEventHandler actor_change_vocation_event_;
  ActorFunctionalityEnableEventHandler actor_functionality_enable_event_;
  ActorDailyCleanEventHandler actor_daily_clean_event_;

  // 消息回调
  ContactsMessageHandler contacts_message_handler_;
  EnemyMessageHandler enemy_message_handler_;
  HomelandMessageHandler homeland_message_handler_;
  HomelandBattleMessageHandler homeland_battle_message_handler_;
  GameHomelandMessageHandler game_homeland_message_handler_;

  // 请求回调
  ContactsRequestHandler contacts_request_handler_;

  // 数据库回调
  StorageSocialActorLoginResponse storage_social_actor_login_;
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__SOCIAL__EXTENSION__H

