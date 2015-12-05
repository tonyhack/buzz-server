include "entity.thrift"
include "game_event.thrift"

namespace cpp game.server.event
namespace py ccevent.scene

///////////////////////////////////////////
// 广播角色创建到玩家集
// type: EVENT_SCENE_BROADCAST_CREATE_ROLE
struct EventSceneBroadcastCreateRole {
  1 : required entity.EntityType type_;           // 正在创建的角色类型
  2 : required i64 id_;                           // 正在创建的角色ID
  3 : required list<i64> actors_;                 // 需要被广播的玩家ID
}

struct SceneRolePair {
  1 : required entity.EntityType type_;
  2 : required i64 id_;
}


///////////////////////////////////////////
// 创建视野内的角色集
// type: EVENT_SCENE_CREATE_AOI_ROLES
struct EventSceneCreateAoiRoles {
  1 : required list<SceneRolePair> roles_;        // 视野内的角色集
  2 : required i64 actor_;                        // 玩家
}


///////////////////////////////////////////
// 角色靠近
// type: EVENT_SCENE_ROLE_COMING
struct EventSceneRoleComing {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required i64 id_;                           // 角色ID
  3 : required list<SceneRolePair> roles_;        // 被靠近的角色集合
}


///////////////////////////////////////////
// 角色远离
// type: EVENT_SCENE_ROLE_LEAVING
struct EventSceneRoleLeaving {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required i64 id_;                           // 角色ID
  3 : required list<SceneRolePair> roles_;        // 被离开的角色集合
}


///////////////////////////////////////////
// 角色改变位置
// type: EVENT_SCENE_ROLE_CHANGE_LOCATION
struct EventSceneRoleChangeLocation {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required i64 id_;                           // 角色ID
  3 : required i64 src_scene_;                    // 源场景ID
  4 : required i32 src_x_;                        // 源坐标X
  5 : required i32 src_y_;                        // 源坐标Y
  6 : required i64 dest_scene_;                   // 目的场景ID
  7 : required i32 dest_x_;                       // 目的坐标X
  8 : required i32 dest_y_;                       // 目的坐标Y
}


///////////////////////////////////////////
// 角色跳转地图
// type: EVENT_SCENE_JUMP_MAP
struct EventSceneJumpMap {
  1 : required entity.EntityType type_;           // 角色类型
  2 : required i64 id_;                           // 角色ID
  3 : required i64 src_scene_;                    // 源场景ID
  4 : required i64 dest_scene_;                   // 目标场景ID
  5 : required i32 dest_x_;                       // 目标x
  6 : required i32 dest_y_;                       // 目标y
}

/////////////////////////////////////////
// 采集事件
// type: EVENT_SCENE_GATHER
struct EventSceneGather {
  1 : required i64 npc_template_id_;               // NPC 模板ID
  2 : required i64 id_;                            // 角色ID
}

