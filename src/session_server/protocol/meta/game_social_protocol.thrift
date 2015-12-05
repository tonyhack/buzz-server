include "homeland.thrift"
include "session_game_protocol.thrift"

namespace cpp session.protocol

////////////////////////////////////////
// 清除升级冷却时间请求
// type: MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST
// dir: session->game
struct MessageHomelandClearUpgradeCoolingRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 upgrade_cooling_;                            // 升级剩余冷却时间
}



////////////////////////////////////////
// 清除升级冷却时间回复
// type: MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE
// dir: game->session
struct MessageHomelandClearUpgradeCoolingResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
}



////////////////////////////////////////
// 神殿升级请求
// type: MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST
// dir: session->game
struct MessageHomelandTempleUpgradeRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 temple_level_;                               // 神殿等级
}



////////////////////////////////////////
// 神殿升级回复
// type: MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE
// dir: game->session
struct MessageHomelandTempleUpgradeResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 temple_level_;                               // 神殿等级
}



////////////////////////////////////////
// 神殿升级通知
// type: MESSAGE_HOMELAND_TEMPLE_UPGRADE_NOTIFY
// dir: session->game
struct MessageHomelandTempleUpgradeNotify {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 temple_level_;                               // 神殿等级
}



////////////////////////////////////////
// 神殿征收请求
// type: MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST
// dir: session->game
struct MessageHomelandTempleHarvestRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 temple_level_;                               // 神殿等级
}



////////////////////////////////////////
// 清除神殿征收冷却时间请求
// type: MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST
// dir: session->game
struct MessageHomelandTempleClearHarvestCoolingRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 temple_harvest_cooling_;                     // 神殿征收剩余冷却时间 
}



////////////////////////////////////////
// 清除神殿征收冷却时间回复
// type: MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE
// dir: game->session
struct MessageHomelandTempleClearHarvestCoolingResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
}



////////////////////////////////////////
// 神殿强征请求
// type: MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST
// dir: session->game
struct MessageHomelandTempleForceHarvestRequest { 
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 temple_level_;                               // 神殿等级
  3 : required i32 temple_force_harvest_times_;                 // 神殿强征次数
}



////////////////////////////////////////
// 神殿强征回复
// type: MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE
// dir: game->session
struct MessageHomelandTempleForceHarvestResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
}



////////////////////////////////////////
// 金矿升级请求
// type: MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST
// dir: session->game
struct MessageHomelandGoldmineUpgradeRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 goldmine_level_;                             // 金矿等级
}



////////////////////////////////////////
// 金矿升级回复
// type: MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE
// dir: game->session
struct MessageHomelandGoldmineUpgradeResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 goldmine_level_;                             // 金矿等级
}



////////////////////////////////////////
// 金矿收获请求
// type: MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST
// dir: session->game
struct MessageHomelandGoldmineHarvestRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 goldmine_level_;                             // 金矿等级
  3 : required i32 goldmine_loss_rate_;                         // 金矿被劫掠损失的金币比例
}



////////////////////////////////////////
// 神秘之源升级请求
// type: MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST
// dir: session->game
struct MessageHomelandPoolUpgradeRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 pool_level_;                                 // 神秘之源等级
}



////////////////////////////////////////
// 神秘之源升级回复
// type: MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE
// dir: game->session
struct MessageHomelandPoolUpgradeResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 pool_level_;                                 // 神秘之源等级
}



////////////////////////////////////////
// 神秘之源领取请求
// type: MESSAGE_HOMELAND_POOL_HARVEST_REQUEST
// dir session->game
struct MessageHomelandPoolHarvestRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 pool_level_;                                 // 神秘之源等级
  3 : required i32 accumulate_times_;                           // 累计次数
}



////////////////////////////////////////
// 神秘之源双倍领取请求
// type: MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST
// dir: session->game
struct MessageHomelandPoolDoubleHarvestRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 pool_level_;                                 // 神秘之源等级
  3 : required i32 accumulate_times_;                           // 累计次数
}



////////////////////////////////////////
// 神秘之源双倍领取回复
// type: MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE
// dir: game->session
struct MessageHomelandPoolDoubleHarvestResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 pool_level_;                                 // 神秘之源等级
  4 : optional i32 accumulate_times_;                           // 累计次数
}



////////////////////////////////////////
// 神秘之源双倍领取请求2
// type: MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST2
// dir: session->game
struct MessageHomelandPoolDoubleHarvestRequest2 {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 pool_level_;                                 // 神秘之源等级
  3 : required i32 accumulate_times_;                           // 累计次数
}



////////////////////////////////////////
// 祈福堂钻石祈福请求
// type: MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST
// dir: session->game
struct MessageHomelandChurchPaidPrayRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 chest_id_;                                   // 宝箱ID
}



////////////////////////////////////////
// 祈福堂钻石祈福回复
// type: MESSAGE_HOMELAND_CHURCH_PAID_PRAY_RESPONSE
// dir: game->session
struct MessageHomelandChurchPaidPrayResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 chest_id_;                                   // 宝箱ID
}



////////////////////////////////////////
// 祈福堂刷新请求
// type: MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST
// dir: session->game
struct MessageHomelandChurchRefreshRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 chest_id_;                                   // 宝箱ID
}



////////////////////////////////////////
// 祈福堂刷新回复
// type: MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE 
// dir: game->session
struct MessageHomelandChurchRefreshResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 chest_id_;                                   // 宝箱ID
}



////////////////////////////////////////
// 祈福堂高级刷新请求
// type: MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST
// dir: session->game
struct MessageHomelandChurchVipRefreshRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 chest_id_;                                   // 宝箱ID
}



////////////////////////////////////////
// 祈福堂高级刷新回复
// type: MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE
// dir: game->session
struct MessageHomelandChurchVipRefreshResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 chest_id_;                                   // 宝箱ID
}



////////////////////////////////////////
// 祈福堂领取奖励请求
// type: MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST
// dir: session->game
struct MessageHomelandChurchHarvestRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 chest_id_;                                   // 宝箱ID
  3 : required i32 bonus_id_;                                   // 奖励ID
}



////////////////////////////////////////
// 祈福堂领取奖励回复
// type: MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE
// dir: game->session
struct MessageHomelandChurchHarvestResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 chest_id_;                                   // 宝箱ID
  4 : optional i32 bonus_id_;                                   // 奖励ID
}



////////////////////////////////////////
// 祈福堂领取奖励请求2
// type: MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST2
// dir: session->game
struct MessageHomelandChurchHarvestRequest2 {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 chest_id_;                                   // 宝箱ID
  3 : required i32 bonus_id_;                                   // 奖励ID
}



////////////////////////////////////////
// 神树帮助好友浇水请求
// type: MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST
// dir: session->game
struct MessageHomelandTreeAssistWaterRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required string friend_actor_id_;                         // 好友玩家ID
  3 : required homeland.HomelandTreeStageType tree_stage_;      // 好友神树所处的阶段
  4 : required i32 tree_watered_times_;                         // 好友神树完成度
}



////////////////////////////////////////
// 神树抢收请求
// type: MESSAGE_HOMELAND_TREE_STEAL_REQUEST
// dir: session->game
struct MessageHomelandTreeStealRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 tree_level_;                                 // 好友神树等级
}



////////////////////////////////////////
// 神树收获请求
// type: MESSAGE_HOMELAND_TREE_HARVEST_REQUEST
// dir: session->game
struct MessageHomelandTreeHarvestRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 tree_level_;                                 // 神树等级
  3 : required i32 stolen_times_;                               // 被抢收次数
}



////////////////////////////////////////
// 炮塔升级请求
// type: MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST
// dir: session->game
struct MessageHomelandTowerUpgradeRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 tower_id_;                                   // 炮塔ID
  3 : required i32 tower_level_;                                // 炮塔等级
}



////////////////////////////////////////
// 炮塔升级回复
// type: MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE
// dir: game->session
struct MessageHomelandTowerUpgradeResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 tower_id_;                                   // 炮塔ID
  4 : optional i32 tower_level_;                                // 炮塔等级
}



////////////////////////////////////////
// 军团据点升级请求
// type: MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST
// dir: session->game
struct MessageHomelandStrongholdUpgradeRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 stronghold_level_;                           // 军团据点等级
}



////////////////////////////////////////
// 军团据点升级回复
// type: MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE
// dir: game->session
struct MessageHomelandStrongholdUpgradeResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 stronghold_level_;                           // 军团据点等级
}



////////////////////////////////////////
// 家园攻防战匹配请求
// type: MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST
// dir: session->game
struct MessageHomelandBattleMatchReqeust {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required string target_actor_name_;                       // 匹配的对方玩家名
  3 : required i32 gold_;                                       // 获得金币
}



////////////////////////////////////////
// 家园攻防战反击请求
// type: MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST
// dir: session->game
struct MessageHomelandBattleStrikeBackRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 gold_;                                       // 获得金币
}



////////////////////////////////////////
// 家园英灵数据
struct HomelandSoulData {
  1 : required i32 soul_id_;                                    // 英灵ID
  2 : required i32 soul_level_;                                 // 英灵等级
}

// 家园英灵数据同步
// type: MESSAGE_HOMELAND_SOUL_SYNCHRONIZE
// dir: game->session
struct MessageHomelandSoulSynchronize {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required list<HomelandSoulData> souls_                    // 英灵数据
}


////////////////////////////////////////
// 神树浇水
// type: MESSAGE_HOMELAND_TREE_WATER
// dir: session->game
struct MessageHomelandTreeWater {
  1 : required i64 id_;                                       // 浇水的人
  2 : required i64 watered_id_;                               // 被浇水的人
}
