#ifndef __SESSION__SERVER__GUILD__GUILD__EXTENSION__H
#define __SESSION__SERVER__GUILD__GUILD__EXTENSION__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension/guild/actor_login_event_handler.h"
#include "session_server/server/extension/guild/actor_logout_event_handler.h"
#include "session_server/server/extension/guild/actor_save_event_handler.h"
#include "session_server/server/extension/guild/actor_jump_scene_event_handler.h"
#include "session_server/server/extension/guild/actor_jump_login_event_handler.h"
#include "session_server/server/extension/guild/actor_functionality_enable_event_handler.h"
#include "session_server/server/extension/guild/actor_level_up_event_handler.h"
#include "session_server/server/extension/guild/actor_change_fight_score_event_handler.h"
#include "session_server/server/extension/guild/actor_weekly_clean_event_handler.h"
#include "session_server/server/extension/guild/guild_daily_clean_event_handler.h"
#include "session_server/server/extension/guild/guild_weekly_clean_event_handler.h"
#include "session_server/server/extension/guild/game_guild_message_handler.h"
#include "session_server/server/extension/guild/guild_message_handler.h"
#include "session_server/server/extension/guild/guild_request_handler.h"
#include "session_server/server/extension/guild/storage_guild_actor_login_response.h"

namespace session {

namespace server {

namespace guild {

class GuildExtension : public core::Noncopyable {
 public:
  GuildExtension();
  ~GuildExtension();

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
  ActorJumpLoginEventHandler actor_jump_login_event_;
  ActorFunctionalityEnableEventHandler actor_functionality_enable_event_;
  ActorLevelUpEventHandler actor_levelup_event_;
  ActorChangeFightScoreEventHandler actor_change_fight_score_event_;
  ActorWeeklyCleanEventHandler actor_weekly_clean_event_;
  GuildDailyCleanEventHandler guild_daily_clean_event_;
  GuildWeeklyCleanEventHandler guild_weekly_clean_event_;

  // 消息回调
  GameGuildMessageHandler game_guild_message_handler_;
  GuildMessageHandler guild_message_handler_;

  // 请求回调
  GuildRequestHandler guild_request_handler_;

  // 数据库回调
  StorageGuildActorLoginResponse storage_guild_actor_login_;
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__GUILD__EXTENSION__H

