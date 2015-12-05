//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-06 12:18:03.
// File name: skill_extension.h
//
// Description:
// Define class SkillExtension.
//

#ifndef __GAME__SERVER__SKILL__SKILL__EXTENSION__H
#define __GAME__SERVER__SKILL__SKILL__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/skill/actor_login_event_handler.h"
#include "game_server/server/extension/skill/actor_logout_event_handler.h"
#include "game_server/server/extension/skill/basic_status_change_event_handler.h"
#include "game_server/server/extension/skill/change_location_event_handler.h"
#include "game_server/server/extension/skill/npc_create_event_handler.h"
#include "game_server/server/extension/skill/role_common_status_event_handler.h"
#include "game_server/server/extension/skill/threat_event_handler.h"
#include "game_server/server/extension/skill/request_global_buff.h"
#include "game_server/server/extension/skill/request_set_fighting_status.h"
#include "game_server/server/extension/skill/request_skill_alter.h"
#include "game_server/server/extension/skill/request_skill_hurt.h"
#include "game_server/server/extension/skill/request_stand_skill.h"
#include "game_server/server/extension/skill/request_temp_skill.h"
#include "game_server/server/extension/skill/request_use_skill.h"
#include "game_server/server/extension/skill/scene_role_create_event_handler.h"
#include "game_server/server/extension/skill/skill_message_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace skill {

class SkillExtension : public global::SingletonFactory<SkillExtension> {
 friend class global::SingletonFactory<SkillExtension>;
 public:
  SkillExtension();
  ~SkillExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  NpcCreateEventHandler npc_create_event_;
  BasicStatusChangeEventHandler status_change_event_;
  ChangeLocationEventHandler change_location_event_;
  ThreatEventHandler threat_event_;
  RoleCommonStatusEventHandler common_status_event_;
  SceneRoleCreateEventHandler scene_role_create_event_;

  // 请求回调
  RequestGlobalBuff request_global_buff_;
  RequestSkillAlter request_skill_alter_;
  RequestSkillHurt request_skill_hurt_;
  RequestStandSkill request_stand_skill_;
  RequestTempSkill request_temp_skill_;
  RequestUseSkill request_use_skill_;
  RequestSetFightingStatus request_set_fighting_status_;

  // 消息回调
  SkillMessageHandler skill_message_handler_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__EXTENSION__H

