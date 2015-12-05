include "entity.thrift"
include "game_event.thrift"
include "role.thrift"
include "vocation.thrift"
include "npc.thrift"

namespace cpp game.server.event
namespace py ccevent.role

////////////////////////////////////////
// 角色状态改变
// type: EVENT_BASIC_STATUS_CHANGE
struct EventBasicStatusChange {
  1 : required entity.EntityType type_;
  2 : required i64 id_;
  3 : required role.RoleBasicStatus from_;
  4 : required role.RoleBasicStatus to_;
}

////////////////////////////////////////
// 常见状态改变
// type: Event_ROLE_COMMON_STATUS_CHANGE
struct EventRoleCommonStatusChange {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
  3 : required i32 from_;                                   // 源状态集
  4 : required i32 to_;                                     // 新状态集
}

////////////////////////////////////////
// 角色被杀死事件
// type: EVENT_ROLE_KILLED
struct EventRoleKilled {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色id
  3 : required entity.EntityType attacker_type_;            // 攻击者类型
  4 : required i64 attacker_id_;                            // 攻击者id
  5 : required i32 skill_id_;                               // 技能ID
}

////////////////////////////////////////
// 生命值改变
// type: EVENT_ROLE_CHANGE_HP
struct EventRoleChangeHp {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
  3 : required i32 value_;                                  // 原生命
}

////////////////////////////////////////
// 生命上限改变
// type: EVENT_ROLE_CHANGE_MAX_HP
struct EventRoleChangeMaxHp {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
  3 : required i32 value_;                                  // 原生命
}

////////////////////////////////////////
// 魔法值改变
// type: EVENT_ROLE_CHANGE_MP
struct EventRoleChangeMp {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
  3 : required i32 value_;                                  // 原生命
}

////////////////////////////////////////
// 魔法上限改变
// type: EVENT_ROLE_CHANGE_MAX_MP
struct EventRoleChangeMaxMp {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
  3 : required i32 value_;                                  // 原生命
}

////////////////////////////////////////
// 进入战斗状态
// type: EVENT_ROLE_ENTER_FIGHTING_STATUS
struct EventRoleEnterFightingStatus {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
}

////////////////////////////////////////
// 离开战斗状态
// type: EVENT_ROLE_LEAVE_FIGHTING_STATUS
struct EventRoleLeaveFightingStatus {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
}

////////////////////////////////////////
// 进入PVP状态
// type: EVENT_ROLE_ENTER_PVP_STATUS
struct EventRoleEnterPvpStatus {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
}

////////////////////////////////////////
// 离开PVP状态
// type: EVENT_ROLE_LEAVE_PVP_STATUS
struct EventRoleLeavePvpStatus {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
}

////////////////////////////////////////
// 切换职业
// type: EVENT_ROLE_CHANGE_VOCATION
struct EventRoleChangeVocation {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
  3 : required vocation.VocationType src_vocation_;         // 原职业
}

////////////////////////////////////////
// 能量值改变
// type: EVENT_ROLE_CHANGE_CURRENT_ENERGY
struct EventRoleChangeCurrentEnergy {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
  3 : required i32 value_;                                  // 原能量
}

////////////////////////////////////////
// 能量上限改变
// type: EVENT_ROLE_CHANGE_MAX_ENERGY
struct EventRoleChangeMaxEnergy {
  1 : required entity.EntityType type_;                     // 角色类型
  2 : required i64 id_;                                     // 角色ID
  3 : required i32 value_;                                  // 原能量上限
}

