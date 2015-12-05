//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 13:38:13.
// File name: scene_extension.h
//
// Description:
// Define class SceneExtension.
//

#ifndef __GAME__SERVER__SCENE__SCEHE__EXTENSION__H
#define __GAME__SERVER__SCENE__SCEHE__EXTENSION__H

#include "core/base/noncopyable.h"

#include "game_server/server/extension/scene/actor_attacked_event_handler.h"
#include "game_server/server/extension/scene/actor_login_event_handler.h"
#include "game_server/server/extension/scene/actor_logout_event_handler.h"
#include "game_server/server/extension/scene/actor_load_finish_event_handler.h"
#include "game_server/server/extension/scene/actor_message_handler.h"
#include "game_server/server/extension/scene/actor_move_event_handler.h"
#include "game_server/server/extension/scene/bar_finish_event_handler.h"
#include "game_server/server/extension/scene/fighting_status_event_handler.h"
#include "game_server/server/extension/scene/login_session_event_handler.h"
#include "game_server/server/extension/scene/npc_create_event_handler.h"
#include "game_server/server/extension/scene/request_actor_broadcast.h"
#include "game_server/server/extension/scene/request_clear_all_npc.h"
#include "game_server/server/extension/scene/request_clear_npc.h"
#include "game_server/server/extension/scene/request_get_around_actors.h"
#include "game_server/server/extension/scene/request_role_move.h"
#include "game_server/server/extension/scene/request_router.h"
#include "game_server/server/extension/scene/request_select_targets.h"
#include "game_server/server/extension/scene/request_scene_create_map.h"
#include "game_server/server/extension/scene/request_scene_forbid_drop_item.h"
#include "game_server/server/extension/scene/request_scene_get_npcs.h"
#include "game_server/server/extension/scene/request_scene_issafe.h"
#include "game_server/server/extension/scene/request_scene_jump.h"
#include "game_server/server/extension/scene/request_scene_pk_check.h"
#include "game_server/server/extension/scene/request_scene_pkinfo.h"
#include "game_server/server/extension/scene/request_scene_relocation.h"
#include "game_server/server/extension/scene/request_scene_revive.h"
#include "game_server/server/extension/scene/request_summon_npc.h"
#include "game_server/server/extension/scene/role_basic_status_change_handler.h"
#include "game_server/server/extension/scene/role_killed_event_handler.h"
#include "game_server/server/extension/scene/skill_event_handler.h"
#include "game_server/server/extension/scene/session_message_handler.h"
#include "game_server/server/extension/scene/world_jump_message_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class SceneActor;

class SceneExtension : public global::SingletonFactory<SceneExtension> {
 friend class global::SingletonFactory<SceneExtension>;
 public:
  SceneExtension();
  ~SceneExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

  SceneActor *GetSceneActor(core::uint64 id);

 private:
  // 事件处理回调
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;
  ActorLoadFinishEventHandler actor_load_finish_event_;
  LoginSessionEventHandler login_session_event_;
  NpcCreateEventHandler npc_create_event_;
  RoleBasicStatusChangeHandler role_basic_status_change_event_;
  RoleKilledEventHandler role_killed_event_;
  BarFinishEventHandler bar_finish_event_;
  SkillEventHandler skill_event_;
  ActorMoveEventHandler actor_move_event_;
  ActorAttackedEventHandler actor_attacked_event_;
  FightingStatusEventHandler fighting_status_event_;

  // 玩家消息处理回调
  ActorMessageHandler actor_message_handler_;
  WorldJumpMessageHandler world_jump_message_handler_;

  // 会话消息处理回调
  SessionMessageHandler session_message_handler_;

  // 请求处理回调
  RequestActorBroadcast req_actor_broadcast_;
  RequestClearAllNpc req_clear_all_npc_;
  RequestClearNpc req_clear_npc_;
  RequestGetAroundActors req_get_around_actors_;
  RequestRoleMove req_role_move_;
  RequestSummonNpc req_summon_npc_;
  RequestSceneCreateMap req_scene_create_map_;
  RequestSceneJump req_scene_jump_;
  RequestScenePKInfo req_scene_pkinfo_;
  RequestSceneIsSafe req_scene_issafe_;
  RequestSelectTargets req_select_targets_;
  RequestRouter request_router_;
  RequestScenePKCheck req_scene_pk_check_;
  RequestSceneRelocation req_scene_relocation_;
  RequestSceneRevive req_scene_revive_;
  RequestSceneForbidDropItem req_scene_forbid_drop_item_;
  RequestSceneGetNpcs req_scene_get_npcs_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCEHE__EXTENSION__H

