include "actor.thrift"
include "entity.thrift"
include "equipment.thrift"
include "npc.thrift"
include "gateway_protocol.thrift"
include "role.thrift"
include "direction.thrift"
include "scene_item.thrift"
include "vocation.thrift"
include "gender.thrift"
include "functionality.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


///////////////////////////////////////////
// type: MESSAGE_CREATE_ACTOR
// dir: gateway->other
struct MessageCreateActor {
  1 : required string name_;
  2 : required list<i32> entity_client_fields_;
  3 : required list<i32> role_aoi_fields_;
  4 : required list<i32> role_client_fields_;
  5 : required list<i32> actor_aoi_fields_;
  6 : required list<i32> actor_client_fields_;
  7 : required string system_now_;                                          // 系统当前时间
  8 : required string functionality_state_;                                 // 系统功能开启状态
  9 : required list<i32> sub_fight_scores_;                                 // 子战斗力
  10: required i32 vip_remainder_time_;                                     // vip 剩余时间
  11: required i32 world_jump_vip_num_;                                     // 世界跳转VIP次数
  12: required i32 fcm_online_time_;                                        // 防沉迷累计在线时间
  13: required string create_time_;                                         // 账号创建时间
  14: required i32 current_hp_tasliman_value_;                              // 当前的血护符
  15: required i32 current_mp_tasliman_value_;                              // 当前的蓝护符
}



///////////////////////////////////////////
// type: MESSAGE_SYNCHRONIZE_ACTOR
// dir: gateway->other
struct MessageSynchronizeActor {
  1 : required string actor_id_;
  2 : optional list<entity.EntityClientField> entity_client_fields_;
  3 : optional list<role.RoleAoiField> role_aoi_fields_;
  4 : optional list<role.RoleClientField> role_client_fields_;
  5 : optional list<actor.ActorAoiField> actor_aoi_fields_;
  6 : optional list<actor.ActorClientField> actor_client_fields_;
}



///////////////////////////////////////////
// type: MESSAGE_SYNCHRONIZE_ACTOR_FUNCTIONALITY_STATE 
// dir: gateway->other
struct MessageSynchronizeActorFunctionalityState {
  1 : required i32 id_;
  2 : required bool state_;
}



///////////////////////////////////////////
// type: MESSAGE_SYNCHRONIZE_ACTOR_SUB_FIGHT_SCORE
// dir: gateway->other
struct MessageSynchronizeActorSubFightScore {
  1 : required i32 id_;
  2 : required i32 sub_fight_score_;
}



///////////////////////////////////////////
// type: MESSAGE_SYNCHRONIZE_NPC
// dir: gateway->other
struct MessageSynchronizeNpc {
  1 : required string npc_id_;
  2 : optional list<entity.EntityClientField> entity_client_fields_;
  3 : optional list<role.RoleAoiField> role_aoi_fields_;
  4 : optional list<role.RoleClientField> role_client_fields_;
  5 : optional list<npc.NpcAoiField> npc_aoi_fields_;
}



///////////////////////////////////////////
// type: MESSAGE_SYNCHRONIZE_SCENE_ITEM
// dir: gateway->other
struct MessageSynchronizeSceneItem {
  1 : required string id_;                                                  // ID
  2 : optional list<entity.EntityClientField> entity_client_fields_;        // Entity客户端属性
  3 : optional list<scene_item.SceneItemAoiField> scene_item_aoi_fields_;   // 场景道具AOI属性
  4 : optional list<string> owner_actors_;                                  // 从属玩家集
  5 : required npc.NpcOwnerType owner_type_;                                // 掉落物从属类型
}



///////////////////////////////////////////
// type: MESSAGE_ACTOR_ENTER_MAP
// dir: gateway->other
struct MessageActorEnterMap {
  1 : required i16 map_;
  2 : required i32 x_;
  3 : required i32 y_;
}



///////////////////////////////////////////
// type: MESSAGE_ACTOR_MOVE
// dir: other->gateway, gateway->other
struct MessageActorMove {
  1 : required direction.DirectionType dir_;        // 移动方向
  2 : required byte offset_x_;                      // 格子内偏移
  3 : required byte offset_y_;                      // 格子内偏移
}



///////////////////////////////////////////
// type: MESSAGE_ACTOR_RELOCATION
// dir: gateway->other
struct MessageActorRelocation {
  1 : required string id_;
  2 : required i32 x_;
  3 : required i32 y_;
  4 : optional byte offset_x_;                      // 格子内偏移
  5 : optional byte offset_y_;                      // 格子内偏移
}



///////////////////////////////////////////
// type: MESSAGE_ROLE_MOVE_SYNCHRONIZE
// dir: gateway->other
struct MessageRoleMoveSynchronize {
  1 : required string id_;
  2 : required entity.EntityType type_;
  3 : required direction.DirectionType dir_;        // 移动方向(如果对没有设置格子偏移，放置于格子中间)
  4 : optional byte offset_x_;                      // 格子内偏移
  5 : optional byte offset_y_;                      // 格子内偏移
}



///////////////////////////////////////////
// type: MESSAGE_CREATE_ACTOR_SYNCHRONIZE
// dir: gateway->other
struct MessageCreateActorSynchronize {
  1 : required string name_;
  2 : required string id_;
  3 : required list<i32> role_aoi_fields_;
  4 : required list<i32> actor_aoi_fields_;
  5 : required i32 pos_x_;
  6 : required i32 pos_y_;
  7 : optional byte offset_x_;                      // 格子内偏移
  8 : optional byte offset_y_;                      // 格子内偏移
}



///////////////////////////////////////////
// type: MESSAGE_CREATE_NPC_SYNCHRONIZE
// dir: gateway->other
struct MessageCreateNpcSynchronize {
  1 : required string id_;
  2 : required i32 template_id_;
  3 : required list<i32> role_aoi_fields_;
  4 : required list<i32> npc_aoi_fields_;
  5 : required i32 pos_x_;
  6 : required i32 pos_y_;
}



///////////////////////////////////////////
// type: MESSAGE_CREATE_SCENE_ITEM_SYNCHRONIZE
// dir: gateway->other
struct MessageCreateSceneItemSynchronize {
  1 : required string id_;                          // ID
  2 : required list<i32> scene_item_aoi_fields_;    // AOI属性
  3 : required i32 pos_x_;                          // 坐标x
  4 : required i32 pos_y_;                          // 坐标y
  5 : required list<string> owner_actors_;          // 从属玩家集
  6 : required npc.NpcOwnerType owner_type_;        // 掉落物从属类型
}



///////////////////////////////////////////
// type: MESSAGE_DESTORY_ROLE_SYNCHRONIZE
// dir: gateway->other
struct MessageDestoryRoleSynchronize {
  1 : required string id_;
  2 : required entity.EntityType type_;
}



///////////////////////////////////////////
// 玩家AOI数据
struct AoiActorData {
  1 : required string id_;
  2 : required string name_;
  3 : required list<i32> role_aoi_fields_;      // 实体aoi属性
  4 : required list<i32> actor_aoi_fields_;     // 玩家aoi属性
  5 : required i32 pos_x_;
  6 : required i32 pos_y_;
  7 : optional byte offset_x_;                  // 格子内偏移
  8 : optional byte offset_y_;                  // 格子内偏移
}
// Npc AOI 数据
struct AoiNpcData {
  1 : required string id_;
  2 : required i32 template_id_;                // npc的模板ID
  3 : required list<i32> role_aoi_fields_;      // 实体aoi属性
  4 : required list<i32> npc_aoi_fields_;       // npc aoi属性
  5 : required i32 pos_x_;
  6 : required i32 pos_y_;
}
// 场景道具 AOI 数据
struct AoiSceneItemData {
  1 : required string id_;                      // ID
  2 : required list<i32> item_aoi_fields_;      // 道具aoi属性
  3 : required i32 pos_x_;                      // 坐标x
  4 : required i32 pos_y_;                      // 坐标y
  5 : required list<string> owner_actors_;      // 从属玩家集
  6 : required npc.NpcOwnerType owner_type_;    // 场景道具类型
}
// type: MESSAGE_CREATE_AOI_ROLES
// dir: gateway->other
struct MessageCreateAoiRoles {
  1 : required list<AoiActorData> actors_;      // 玩家列表
  2 : required list<AoiNpcData> npcs_;          // npc列表
  3 : required list<AoiSceneItemData> items_;   // 场景道具列表
}



///////////////////////////////////////////
// type: MESSAGE_DESTORY_AOI_ROLES
// dir: gateway->other
struct MessageDestoryAoiRoles {
  1 : required list<string> actors_;
  2 : required list<string> npcs_;
  3 : required list<string> items_;
}



///////////////////////////////////////////
// type: MESSAGE_ADD_EXP_SYNCHRONIZE
// dir: gateway->client
struct MessageAddExpSynchronize {
  1 : required i32 exp_;                        // 经验增加同步              
}



///////////////////////////////////////////
// type: MESSAGE_ACTOR_FACADE_REQUEST
// dir: client->gateway
struct MessageActorFacadeRequest {
  1 : required string actor_;                   // 玩家ID
}



///////////////////////////////////////////
// type: MESSAGE_ACTOR_FACADE_RESPONSE
// dir: gateway->client
struct MessageActorFacadeResponse {
  1 : required string actor_;                                 // 玩家ID
  2 : required vocation.VocationType vocation_;               // 职业
  3 : required gender.GenderType gender_;                     // 性别
  4 : required i32 appearance_;                               // 外貌
  5 : required list<equipment.EquipFacadeField> equipments_;  // 装备
  6 : required i32 sword_;                                    // 魔剑
  7 : required i32 spirit_facade_;                            // 精灵外观
}



//////////////////////////////////////////
// type: MESSAGE_ACTOR_VIP_SYNCHRONIZE
// dir: gateway->client
struct MessageActorVipSynchronize {
  1 : required i32 remainder_time_;                           // VIP剩余时间
}


//////////////////////////////////////////
// type: MESSAGE_WORLD_JUMP_VIP_NUM_SYNCHRONIZE
// dir: gateway->client
struct MessageWorldJumpVipNumSynchronize {
  1 : required i32 remainder_num_;                            // 剩余次数
}

//////////////////////////////////////////
// type: MESSAGE_ACTOR_FUNCTIONALITY_OPEN_REQUEST
// dir: client->gateway
struct MessageActorFunctionalityOpenRequest {
  1 : required functionality.FunctionalityType  functionality_id_;  // 功能id
}

//////////////////////////////////////////
// type: MESSAGE_ACTOR_FUNCTIONALITY_OPEN_RESPONSE
// dir: client->gateway
struct MessageActorFunctionalityOpenResponse {
  1 : required functionality.FunctionalityType functionality_id_;  // 功能id
}

/////////////////////////////////////////
// 使用护符类型
enum UseTaslimanType {
  HP,
  MP,
}

/////////////////////////////////////////
// type: MESSAGE_ACTOR_USE_TASLIMAN_REQUEST
// dir: client->gateway
struct MessageActorUseTaslimanRequest {
  1 : required UseTaslimanType type_;                             // 使用护符类型
}

////////////////////////////////////////
// type: MESSAGE_ACTOR_USE_TASLIMAN_RESPONSE
// dir: gateway->client
struct MessageActorUseTaslimanResponse {
  1 : required UseTaslimanType type_;                             // 使用护符类型
}

///////////////////////////////////////
// type: MESSAGE_ACTOR_TASLIMAN_SYNCHRONIZE
// dir: gateway->client
struct MessageActorTaslimanSynchronize {
  1 : required UseTaslimanType type_;                             // 护符类型
  2 : required i32 value_;                                        // 护符当前值
}
