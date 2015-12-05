include "gateway_protocol.thrift"
include "soul.thrift"
include "vocation.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway


////////////////////////////////////////////////////////////
// 英灵初始化
// type: MESSAGE_SOUL_INITIALIZE
// dir: gateway->client
struct MessageSoulInitialize {
  1 : required list<soul.SoulField> souls_;             // 英灵数据
  2 : required list<soul.SoulSettingField> settings_;   // 设置数据
  3 : required i32 axe_group_;                          // 当前近战设置组
  4 : required i32 shoot_group_;                        // 当前远程设置组
  5 : required i32 magic_group_;                        // 当前法术设置组
}



////////////////////////////////////////////////////////////
// 英灵数据同步
// type: MESSAGE_SOUL_SYNCHRONIZE
// dir: gateway->client
struct MessageSoulSynchronize {
  1 : required soul.SoulField soul_;                    // 英灵数据
}



////////////////////////////////////////////////////////////
// 英灵解锁请求
// type: MESSAGE_SOUL_UNLOCK_REQUEST
// dir: client->gateway
struct MessageSoulUnlockRequest {
  1 : required i32 soul_id_;                            // 英灵ID
}



////////////////////////////////////////////////////////////
// 英灵解锁回复
// type: MESSAGE_SOUL_UNLOCK_RESPONSE
// dir: gateway->client
struct MessageSoulUnlockResponse {
  1 : required bool result_;                            // 结果
  2 : optional i32 soul_id_;                            // 英灵ID
}



////////////////////////////////////////////////////////////
// 英灵升阶请求
// type: MESSAGE_SOUL_UPGRADE_STEP_REQUEST
// dir: client->gateway
struct MessageSoulUpgradeStepRequest {
  1 : required i32 soul_id_;                            // 英灵ID
}



////////////////////////////////////////////////////////////
// 英灵升阶回复
// type: MESSAGE_SOUL_UPGRADE_STEP_RESPONSE
// dir: gateway->client
struct MessageSoulUpgradeStepResponse {
  1 : required bool result_;                            // 结果
  2 : optional i32 soul_id_;                            // 英灵ID
}



////////////////////////////////////////////////////////////
// 英灵升级请求
// type: MESSAGE_SOUL_UPGRADE_LEVEL_REQUEST
// dir: client->gateway
struct MessageSoulUpgradeLevelRequest {
  1 : required i32 soul_id_;                            // 英灵ID
}



////////////////////////////////////////////////////////////
// 英灵升级回复
// type: MESSAGE_SOUL_UPGRADE_LEVEL_RESPONSE
// dir: gateway->client
struct MessageSoulUpgradeLevelResponse {
  1 : required bool result_;                            // 结果
  2 : required i32 soul_id_;                            // 英灵ID
}



////////////////////////////////////////////////////////////
// 英灵启用请求
// type: MESSAGE_SOUL_SET_REQUEST
// dir: client->gateway
struct MessageSoulSetRequest {
  1 : required vocation.VocationType vocation_;         // 职业
  2 : required i32 group_;                              // 组
  3 : required i32 soul_id_;                            // 英灵ID
  4 : required i32 position_;                           // 位置
}



////////////////////////////////////////////////////////////
// 英灵启用回复
// type: MESSAGE_SOUL_SET_RESPONSE
// dir: gateway->client
struct MessageSoulSetResponse {
  1 : required bool result_;                            // 结果
  2 : optional vocation.VocationType vocation_;         // 职业
  3 : optional i32 group_;                              // 组
  4 : optional i32 soul_id_;                            // 英灵ID
  5 : required i32 position_;                           // 位置
}



////////////////////////////////////////////////////////////
// 英灵取消请求
// type: MESSAGE_SOUL_RESET_REQUEST
// dir: client->gateway
struct MessageSoulResetRequest {
  1 : required vocation.VocationType vocation_;         // 职业
  2 : required i32 group_;                              // 组
  3 : required i32 position_;                           // 位置
}



////////////////////////////////////////////////////////////
// 英灵取消回复
// type: MESSAGE_SOUL_RESET_RESPONSE
// dir: gateway->client
struct MessageSoulResetResponse {
  1 : required bool result_;                            // 结果
  2 : optional vocation.VocationType vocation_;         // 职业
  3 : optional i32 group_;                              // 组
  4 : optional i32 position_;                           // 位置
}



////////////////////////////////////////////////////////////
// 英灵设置启用请求
// type: MESSAGE_SOUL_SETTING_STAND_REQUEST
// dir: client->gateway
struct MessageSoulSettingStandRequest {
  1 : required vocation.VocationType vocation_;         // 职业
  2 : required i32 group_;                              // 组
}



////////////////////////////////////////////////////////////
// 英灵设置启用回复
// type: MESSAGE_SOUL_SETTING_STAND_RESPONSE
// dir: gateway->client
struct MessageSoulSettingStandResponse {
  1 : required bool result_;                            // 结果
  2 : optional vocation.VocationType vocation_;         // 职业
  3 : optional i32 group_;                              // 组
}



////////////////////////////////////////////////////////////
// 英灵祭坛卡片数据
struct SoulAltarCardData {
  1 : required i32 card_pos_;                       // 卡片位置(0~2)
  2 : optional i32 card_id_;                        // 卡片ID
  3 : optional bool exist_;                         // 是否已领取
}



////////////////////////////////////////////////////////////
// 英灵祭坛数据初始化
// type: MESSAGE_SOUL_ALTAR_INITIALIZE
// dir: gateway->client
struct MessageSoulAltarInitialize {
  1 : required bool summon_paid_;                   // 是否已召唤
  2 : required i32 free_summon_times_;              // 免费召唤次数
  3 : required list<SoulAltarCardData> cards_;      // 召唤得到的英灵卡
}



////////////////////////////////////////////////////////////
// 英灵祭坛数据同步
// type: MESSAGE_SOUL_ALTAR_SYNCHRONIZE
// dir: gateway->client
struct MessageSoulAltarSynchronize {
  1 : optional bool summon_paid_;                   // 是否已召唤
  2 : optional i32 free_summon_times_;              // 免费召唤次数
}



////////////////////////////////////////////////////////////
// 英灵祭坛卡片操作类型
enum SoulAltarCardOperateType {
  ADD = 0,                                          // 增加
  UPDATE,                                           // 更新
  CLEAR,                                            // 清空
}

// 英灵祭坛卡片数据同步
// type: MESSAGE_SOUL_ALTAR_CARD_SYNCHRONIZE
// dir: gateway->client
struct MessageSoulAltarCardSynchronize {
  1 : required SoulAltarCardOperateType type_;      // 操作类型
  2 : required SoulAltarCardData data_;             // 数据
}



////////////////////////////////////////////////////////////
// 英灵祭坛召唤请求
// type: MESSAGE_SOUL_ALTAR_SUMMON_REQUEST
// dir: client->gateway
struct MessageSoulAltarSummonRequest {
  1 : optional bool bind_;                          // 使用的道具是否绑定
}



////////////////////////////////////////////////////////////
// 英灵祭坛召唤回复
// type: MESSAGE_SOUL_ALTAR_SUMMON_RESPONSE
// dir: gateway->client
struct MessageSoulAltarSummonResponse {
  1 : required bool result_;                        // 结果
}



////////////////////////////////////////////////////////////
// 英灵祭坛收取请求
// type: MESSAGE_SOUL_ALTAR_TAKE_REQUEST
// dir: client->gateway
struct MessageSoulAltarTakeRequest {
  1 : required i32 card_pos_;                       // 收取卡片的位置
}



////////////////////////////////////////////////////////////
// 英灵祭坛收取回复
// type: MESSAGE_SOUL_ALTAR_TAKE_RESPONSE
// dir: gateway->client
struct MessageSoulAltarTakeResponse {
  1 : required bool result_;                        // 结果
  2 : optional i32 card_id_;                        // 卡片ID
}



////////////////////////////////////////////////////////////
// 英灵祭坛放弃请求
// type: MESSAGE_SOUL_ALTAR_RESET_REQUEST
// dir: client->gateway
struct MessageSoulAltarResetRequest {
}



////////////////////////////////////////////////////////////
// 英灵祭坛放弃回复
// type: MESSAGE_SOUL_ALTAR_RESET_RESPONSE
// dir: gateway->client
struct MessageSoulAltarResetResponse {
  1 : required bool result_;                        // 结果
}



////////////////////////////////////////////////////////////
// 英灵技能升级请求
// type: MESSAGE_SOUL_SKILL_UPGRADE_REQUEST
// dir: client->gateway
struct MessageSoulSkillUpgradeRequest {
  1 : required i32 soul_id_;                        // 英灵ID
  2 : required i32 skill_id_;                       // 技能ID
}



////////////////////////////////////////////////////////////
// 英灵技能升级回应
// type: MESSAGE_SOUL_SKILL_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageSoulSkillUpgradeResponse {
  1 : required bool result_;                        // 结果
  2 : required i32 soul_id_;                        // 英灵ID
  3 : required i32 skill_id_;                       // 技能ID
}

