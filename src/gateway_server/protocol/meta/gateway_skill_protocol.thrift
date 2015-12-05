include "entity.thrift"
include "skill.thrift"
include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

///////////////////////////////////////////////////////////////
// 技能信息
struct SkillContext {
  1 : optional entity.EntityType src_type_;               // 源类型(BOOT/LAUNCH时有效)
  2 : optional string src_id_;                            // 源ID(BOOT/LAUNCH时有效)
  3 : optional entity.EntityType target_type_;            // 目标类型(技能目标类型为实体时有效)
  4 : optional string target_id_;                         // 目标ID(技能目标类型为实体时有效)
  5 : optional i32 x_;                                    // 目标坐标x(技能目标类型为坐标时有效)
  6 : optional i32 y_;                                    // 目标坐标y(技能目标类型为坐标时有效)
  7 : optional i16 direction_;                            // 方向(技能类型为无目标碰撞类时有效)
}
// 技能操作类型
enum SkillOperateType {
  USE = 0,                                                // 使用(client->server)
  BOOT,                                                   // 引导(server->client)
  LAUNCH,                                                 // 发射(server->client)
  MAX,
}
// type: MESSAGE_SKILL_OPERATE
// dir: client->gateway, gateway->client
struct MessageSkillOperate {
  1 : required i32 skill_id_;                             // 技能ID
  2 : required SkillOperateType type_;                    // 技能操作类型
  3 : required SkillContext context_;                     // 技能相关信息
}


//////////////////////////////////////////////////////////////
// 一般技能由服务器广播到客户端
// 碰撞类技能由客户端发送到服务器，服务器检测通过后再发回客户端
// type: MESSAGE_SKILL_HIT
// dir: client->gateway, gateway->client
struct MessageSkillHit {
  1 : required i32 skill_id_;                             // 技能ID
  2 : required entity.EntityType src_type_;               // 目标类型
  3 : required string src_id_;                            // 目标ID
  4 : required entity.EntityType target_type_;            // 目标类型
  5 : required string target_id_;                         // 目标ID
  6 : required skill.SkillHitType hit_type_;              // 命中类型
  7 : optional i32 value_;                                // 数值增加值
}


//////////////////////////////////////////////////////////////
// 玩家选择PK模式
// type: MESSAGE_ACTOR_CHOOSE_PK_MODE
// dir: client->gateway
struct MessageActorChoosePKMode {
  1 : required i32 pk_mode_;                              // 模式选择
}


//////////////////////////////////////////////////////////////
// 进入战斗状态
// type: MESSAGE_ENTER_FIGHTING_STATUS
// dir: gateway->client
struct MessageEnterFightingStatus {
}



//////////////////////////////////////////////////////////////
// 离开战斗状态
// type: MESSAGE_LEAVE_FIGHTING_STATUS
// dir: gateway->client
struct MessageLeaveFightingStatus {
}


//////////////////////////////////////////////////////////////
// 进入PVP状态
// type: MESSAGE_ENTER_PVP_STATUS
// dir: gateway->client
struct MessageEnterPvpStatus {
  1 : required string id_;                                // ID
}


//////////////////////////////////////////////////////////////
// 离开PVP状态
// type: MESSAGE_LEAVE_PVP_STATUS
// dir: gateway->client
struct MessageLeavePvpStatus {
  1 : required string id_;                                // ID
}


/////////////////////////////////////////////////////////////
// 广播PVP状态(server->client)
// type: MESSAGE_PVP_STATUS_BROADCAST
// dir: gateway->client
struct MessagePvpStatusBroadcast {
  1 : required list<string> actors_;                      // PVP状态玩家列表
}


/////////////////////////////////////////////////////////////
// 技能打断
// type: MEESAGE_SKILL_BREAK_SYNCHRONIZE
// dir: gateway->client
struct MessageSkillBreakSynchronize {
  1 : required string role_id_;                           // 角色ID
  2 : required entity.EntityType role_type_;              // 角色类型
  3 : required i32 skill_id_;                             // 技能ID
}


/////////////////////////////////////////////////////////////
// 技能属性同步
// type: MESSAGE_SKILL_ATTRIBUTE_SYNCHRONIZE
// dir: gateway->client
struct MessageSkillAttributeSynchronize {
  1 : required i32 skill_id_;                             // 技能ID
  2 : required list<skill.SkillVarField> attributes_;     // 属性
}

