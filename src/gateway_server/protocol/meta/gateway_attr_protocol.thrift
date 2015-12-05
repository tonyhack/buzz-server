include "spirit.thrift"
include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

/////////////////////////////////////////
// 属性AOI数据
struct AttrAoiData {
  1 : required string actor_id_;                                // 玩家ID
  2 : optional i32 noble_level_;                                // 贵族等级
}



/////////////////////////////////////////
// 属性AOI数据同步
// type: MESSAGE_ATTR_AOI_SYNCHRONIZE
// dir: gateway->client
struct MessageAttrAoiSynchronize {
  1 : required list<AttrAoiData> aoi_data_;                     // AOI数据
}



/////////////////////////////////////////
// 魔剑数据初始化
// type: MESSAGE_ATTR_SWORD_INITIALIZE
// dir: gateway->client
struct MessageAttrSwordInitialize {
  1 : required i32 step_;                                       // 阶数
  2 : required i32 lucky_value_;                                // 幸运值
  3 : required string gain_time_;                               // 获取时间(unix时间戳)
}



/////////////////////////////////////////
// 魔剑进阶请求
// type: MESSAGE_ATTR_SWORD_UPGRADE_REQUEST
// dir: client->gateway
struct MessageAttrSwordUpgradeRequest {
}



/////////////////////////////////////////
// 魔剑进阶回复
// type: MESSAGE_ATTR_SWORD_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageAttrSwordUpgradeResponse {
  1 : required bool result_;                                    // 是否成功
}



/////////////////////////////////////////
// 魔剑属性同步
// type: MESSAGE_ATTR_SWORD_SYNCHRONIZE
// dir: gateway->client
struct MessageAttrSwordSynchronize {
  1 : optional i32 step_;                                       // 阶数
  2 : optional i32 lucky_value_;                                // 幸运值
}



/////////////////////////////////////////
// 魔剑装备请求
// type: MESSAGE_ATTR_SWORD_EQUIP
// dir: client->gateway
struct MessageAttrSwordEquip {
  1 : required i32 step_;                                       // 装备魔剑的阶数
}



/////////////////////////////////////////
// 魔剑卸下请求
// type: MESSAGE_ATTR_SWORD_UNEQUIP
// dir: client->gateway
struct MessageAttrSwordUnequip {
}



/////////////////////////////////////////
// 魔剑对比请求
// type: MESSAGE_ATTR_SWORD_COMPARE_REQUEST
// dir: client->gateway
struct MessageAttrSwordCompareRequest {
  1 : required string actor_id_;                                // 玩家ID
}



/////////////////////////////////////////
// 魔剑对比回复
// type: MESSAGE_ATTR_SWORD_COMPARE_RESPONSE
// dir: client->gateway
struct MessageAttrSwordCompareResponse {
  1 : required string actor_id_;                                // 玩家ID
  2 : required i32 step_;                                       // 魔剑阶数
  3 : required i32 level_;                                      // 魔剑等级
}



/////////////////////////////////////////
// 贵族数据初始化
// type: MESSAGE_ATTR_NOBLE_INITIALIZE
// dir: gateway->client
struct MessageAttrNobleInitialize {
  1 : required i32 level_;
  2 : required i32 used_item_count_;
  3 : required i32 max_hp_add_value_;
  4 : required i32 max_mp_add_value_;
  5 : required i32 physics_attack_add_value_;
  6 : required i32 physics_defence_add_value_;
  7 : required i32 magic_attack_add_value_;
  8 : required i32 magic_defence_add_value_;
  9 : required i32 dodge_add_value_;
  10: required i32 crit_add_value_;
  11: required i32 ignore_defence_add_value_;
}



/////////////////////////////////////////
// 贵族提升请求
// type: MESSAGE_ATTR_NOBLE_PROMOTE_REQUEST
// dir: client->gateway
struct MessageAttrNoblePromoteRequest {
}



/////////////////////////////////////////
// 贵族提升回复
// type: MESSAGE_ATTR_NOBLE_PROMOTE_RESPONSE
// dir: gateway->client
struct MessageAttrNoblePromoteResponse {
  1 : required bool result_;
  2 : optional i32 max_hp_add_value_;
  3 : optional i32 max_mp_add_value_;
  4 : optional i32 physics_attack_add_value_;
  5 : optional i32 physics_defence_add_value_;
  6 : optional i32 magic_attack_add_value_;
  7 : optional i32 magic_defence_add_value_;
  8 : optional i32 dodge_add_value_;
  9 : optional i32 crit_add_value_;
  10: optional i32 ignore_defence_add_value_;
}



/////////////////////////////////////////
// 贵族突破请求
// type: MESSAGE_ATTR_NOBLE_BREAKTHROUGH_REQUEST
// dir: client->gateway
struct MessageAttrNobleBreakthroughRequest {
  1 : required i32 use_item_count_;
}



/////////////////////////////////////////
// 贵族突破回复
// type: MESSAGE_ATTR_NOBLE_BREAKTHROUGH_RESPONSE
// dir: gateway->client
struct MessageAttrNobleBreakthroughResponse {
  1 : required bool result_;
  2 : optional i32 level_;
  3 : optional i32 used_item_count_;
}



/////////////////////////////////////////
// 贵族对比请求
// type: MESSAGE_ATTR_NOBLE_COMPARE_REQUEST
// dir: client->gateway
struct MessageAttrNobleCompareRequest {
  1 : required string actor_id_;
}



/////////////////////////////////////////
// 贵族对比回复
// type: MESSAGE_ATTR_NOBLE_COMPARE_RESPONSE
// dir: gateway->client
struct MessageAttrNobleCompareResponse {
  1 : required string actor_id_;
  2 : required bool result_;
  3 : optional i32 level_;
  4 : optional i32 max_hp_add_value_;
  5 : optional i32 max_mp_add_value_;
  6 : optional i32 physics_attack_add_value_;
  7 : optional i32 physics_defence_add_value_;
  8 : optional i32 magic_attack_add_value_;
  9 : optional i32 magic_defence_add_value_;
  10: optional i32 dodge_add_value_;
  11: optional i32 crit_add_value_;
  12: optional i32 ignore_defence_add_value_;
}



/////////////////////////////////////////
// 元素数据初始化
// type: MESSAGE_ATTR_ELEMENT_INITIALIZE
// dir: gateway->client
struct MessageAttrElementInitialize {
  1 : required i32 level_;
  2 : required i32 max_hp_add_value_;
  3 : required i32 max_mp_add_value_;
  4 : required i32 physics_attack_add_value_;
  5 : required i32 physics_defence_add_value_;
  6 : required i32 magic_attack_add_value_;
  7 : required i32 magic_defence_add_value_;
  8 : required i32 dodge_add_value_;
  9 : required i32 crit_add_value_;
  10: required i32 ignore_defence_add_value_;
}



/////////////////////////////////////////
// 元素升级请求
// type: MESSAGE_ATTR_ELEMENT_UPGRADE_REQUEST
// dir: client->gateway
struct MessageAttrElementUpgradeRequest {
  1 : required i32 level_;
}



/////////////////////////////////////////
// 元素升级回复
// type: MESSAGE_ATTR_ELEMENT_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageAttrElementUpgradeResponse {
  1 : required bool result_;
  2 : optional i32 level_;
  3 : optional i32 max_hp_add_value_;
  4 : optional i32 max_mp_add_value_;
  5 : optional i32 physics_attack_add_value_;
  6 : optional i32 physics_defence_add_value_;
  7 : optional i32 magic_attack_add_value_;
  8 : optional i32 magic_defence_add_value_;
  9 : optional i32 dodge_add_value_;
  10: optional i32 crit_add_value_;
  11: optional i32 ignore_defence_add_value_;
}



/////////////////////////////////////////
// 元素对比请求
// type: MESSAGE_ATTR_ELEMENT_COMPARE_REQUEST
// dir: client->gateway
struct MessageAttrElementCompareRequest {
  1 : required string actor_id_;
}



/////////////////////////////////////////
// 元素对比回复
// type: MESSAGE_ATTR_ELEMENT_COMPARE_RESPONSE
// dir: gateway->client
struct MessageAttrElementCompareResponse {
  1 : required string actor_id_;
  2 : required bool result_;
  3 : optional i32 level_;
  4 : optional i32 max_hp_add_value_;
  5 : optional i32 max_mp_add_value_;
  6 : optional i32 physics_attack_add_value_;
  7 : optional i32 physics_defence_add_value_;
  8 : optional i32 magic_attack_add_value_;
  9 : optional i32 magic_defence_add_value_;
  10: optional i32 dodge_add_value_;
  11: optional i32 crit_add_value_;
  12: optional i32 ignore_defence_add_value_;
}



/////////////////////////////////////////
// 精灵外观数据
struct SpiritFacadeData {
  1 : required i32 id_;                                         // 外观ID
  2 : required i32 expire_time_;                                // 失效时间
}



////////////////////////////////////////
// 精灵的符文结构
struct SpiritRuneData {
  1 : required byte location_;                                  // 符文槽位置
  2 : required i32 template_id_;                                // 符文槽的道具id
}



/////////////////////////////////////////
// 精灵数据初始化
// type: MESSAGE_ATTR_SPIRIT_INITIALIZE
// dir: gateway->client
struct MessageAttrSpiritInitialize {
  1 : required i32 level_;                                      // 精灵等级
  2 : required i32 lucky_value_;                                // 幸运值
  3 : required spirit.SpiritFacadeType current_facade_type_;    // 精灵当前的外观形态
  4 : required i32 horse_facade_id_;                            // 精灵野兽形态外观ID
  5 : required i32 wing_facade_id_;                             // 精灵翅膀形态外观ID
  6 : required list<SpiritFacadeData> facades_;                 // 精灵外观数据
  7 : required list<SpiritRuneData> runes_;                     // 符文数据
}



/////////////////////////////////////////
// 精灵数据同步
// type: MESSAGE_ATTR_SPIRIT_SYNCHRONIZE 
// dir: gateway->client
struct MessageAttrSpiritSynchronize {
  1 : optional i32 level_;                                      // 精灵等级
  2 : optional i32 lucky_value_;                                // 幸运值
  3 : optional spirit.SpiritFacadeType current_facade_type_;    // 精灵当前的外观形态
  4 : optional i32 horse_facade_id_;                            // 精灵野兽形态外观ID
  5 : optional i32 wing_facade_id_;                             // 精灵翅膀形态外观ID
}



/////////////////////////////////////////
// 精灵外观操作类型
enum SpiritFacadeOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 精灵外观数据同步
// type: MESSAGE_ATTR_SPIRIT_FACADE_SYNCHRONIZE
// dir: gateway->client
struct MessageAttrSpiritFacadeSynchronize {
  1 : required SpiritFacadeOperateType type_;
  2 : required SpiritFacadeData facade_;
}



/////////////////////////////////////////
// 精灵进阶请求
// type: MESSAGE_ATTR_SPIRIT_UPGRADE_REQUEST
// dir: client->gateway
struct MessageAttrSpiritUpgradeRequest {
}



/////////////////////////////////////////
// 精灵进阶回复
// type: MESSAGE_ATTR_SPIRIT_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageAttrSpiritUpgradeResponse {
  1 : required bool result_;                                    // 是否成功 
}



/////////////////////////////////////////
// 精灵转换外观请求
// type: MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_REQUEST
// dir: client->gateway
struct MessageAttrSpiritTransformFacadeRequest {
  1 : required i32 facade_id_;                                  // 外观ID
}



/////////////////////////////////////////
// 精灵转换外观回复
// type: MESSAGE_ATTR_SPIRIT_TRANSFORM_FACADE_RESPONSE 
// dir: gateway->client
struct MessageAttrSpiritTransformFacadeResponse {
  1 : required bool result_;                                    // 是否成功 
}



/////////////////////////////////////////
// 精灵对比请求
// type: MESSAGE_ATTR_SPIRIT_COMPARE_REQUEST
// dir: client->gateway
struct MessageAttrSpiritCompareRequest {
  1 : required string actor_id_;                                // 玩家ID
}



/////////////////////////////////////////
// 精灵对比回复
// type: MESSAGE_ATTR_SPIRIT_COMPARE_RESPONSE
// dir: gateway->client
struct MessageAttrSpiritCompareResponse {
  1 : required bool result_;                                    // 是否成功 
  2 : required string actor_id_;                                // 玩家ID
  3 : optional i32 level_;                                      // 精灵等级
  4 : optional spirit.SpiritFacadeType current_facade_type_;    // 精灵当前的外观形态
  5 : optional i32 horse_facade_id_;                            // 精灵野兽形态外观ID
  6 : optional i32 wing_facade_id_;                             // 精灵翅膀形态外观ID
  7 : optional list<SpiritFacadeData> facades_;                 // 精灵外观数据
  8 : optional list<SpiritRuneData> runes_;                     // 符文数据
}


/////////////////////////////////////////
// 符文移动时类型
enum RuneMoveType {
  PACKET = 1,                                                    // 背包
  RUNE_SLOT,                                                     // 符文槽
}



////////////////////////////////////////
// 符文合成
// type:MESSAGE_ATTR_RUNE_COMPOSE_REQUEST
// dir:  client->gateway
struct MessageRuneComposeRequest {
  1 : required string item_id_;                                 // 合成符文消耗的道具id
  2 : required i32 num_;                                        // 合成符文消耗的道具数目
}



///////////////////////////////////////
// 符文合成响应
// type:MESSAGE_ATTR_RUNE_COMPOSE_RESPONSE
// dir: gateway->client
struct MessageRuneComposeResponse {
  1 : required i32 template_id_;                                // 新生成的道具模板id
  2 : required i32 success_num_;                                // 成功次数
  3 : required i32 fail_num_;                                   // 失败次数
}



////////////////////////////////////////
// 符文移动
// type:MESSAGE_ATTR_RUNE_MOVE_REQUEST
// dir: client->gateway
struct MessageRuneMoveRequest {
  1 : required i16 src_location_;                               // 原位置
  2 : required RuneMoveType src_type_;                          // 原容器
  3 : required i16 dest_location_;                              // 目的位置
  4 : required RuneMoveType dest_type_;                         // 目的容器
}



///////////////////////////////////////
// 符文移动响应
// type:MESSAGE_ATTR_RUNE_MOVE_RESPONSE
// dir: gateway->client
struct MessageRuneMoveResponse {
  1 : required bool result_;                                    // 返回结果
}



//////////////////////////////////////
// 符文槽位同步
// type: MESSAGE_ATTR_RUNE_SYNCHRONIZE
// dir: gateway->client
struct MessageRuneSynchronize {
  1 : required SpiritRuneData data_;                            // 符文槽数据同步 
}



//////////////////////////////////////
// 名武数据
struct LegendaryWeaponData {
    1 : required i32 id_;                                       // 名武ID
    2 : required list<i32> pieces_;                             // 名武碎片数量
}

// 名武数据初始化
// type: MESSAGE_ATTR_LEGENDARY_WEAPON_INITIALIZE
// dir: gateway->client
struct MessageAttrLegendaryWeaponInitialize {
    1 : required list<LegendaryWeaponData> weapons_;            // 名武碎片信息
}



//////////////////////////////////////
// 名武碎片数据同步
// type: MESSAGE_ATTR_LEGENDARY_WEAPON_PIECE_SYNCHRONIZE
// dir: gateway->client
struct MessageAttrLegendaryWeaponPieceSynchronize {
    1 : required i32 weapon_id_;                                // 名武ID
    2 : required i32 piece_index_;                              // 名武碎片编号(0~2)
    3 : required i32 piece_count_;                              // 名武碎片个数
}



//////////////////////////////////////
// 修复目标数据
struct LegendaryWeaponRepairData {
    1 : required i32 weapon_id_;                                // 名武ID
    2 : required i32 piece_index_;                              // 名武碎片编号(0~2)
    3 : required i32 use_item_count_;                           // 使用道具数量
}

// 名武修复请求
// type: MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_REQUEST
// dir: client->gateway
struct MessageAttrLegendaryWeaponRepairRequest {
    1 : required list<LegendaryWeaponRepairData> targets_;      // 修复目标
}



//////////////////////////////////////
// 名武修复回复
// type: MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_RESPONSE
// dir: gateway->client
struct MessageAttrLegendaryWeaponRepairResponse {
  1 : required bool result_;                                    // 是否成功 
}

