include "contacts.thrift"
include "gender.thrift"
include "vocation.thrift"
include "homeland.thrift"
include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

/////////////////////////////////////////
// 联系人信息
struct ContactsData {
  1 : required string actor_id_;                                // 玩家ID
  2 : required contacts.ContactsType contacts_type_;            // 联系人类型
  3 : optional bool online_;                                    // 是否在线
  4 : optional string actor_name_;                              // 玩家名
  5 : optional i32 level_;                                      // 玩家等级
  6 : optional gender.GenderType gender_;                       // 玩家性别
  7 : optional vocation.VocationType vocation_;                 // 玩家职业
  8 : optional i16 map_;                                        // 玩家所在地图
  9 : optional string last_logout_time_;                        // 上次下线时间
  10: optional string signature_;                               // 签名
}



/////////////////////////////////////////
// 联系人数据初始化
// type: MESSAGE_SOCIAL_CONTACTS_INITIALIZE
// dir: gateway->client
struct MessageSocialContactsInitialize {
  1 : required list<ContactsData> contacts_;                    // 联系人数据
  2 : required string signature_;                               // 签名
}



/////////////////////////////////////////
// 联系人操作类型
enum ContactsOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 联系人数据同步
// type: MESSAGE_SOCIAL_CONTACTS_SYNCHRONIZE
// dir: gateway->client
struct MessageSocialContactsSynchronize {
  1 : required ContactsOperateType type_;                       // 操作类型
  2 : required ContactsData contacts_;                          // 联系人数据
}



/////////////////////////////////////////
// 编辑签名请求
// type: MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_REQUEST
// dir: client->gateway
struct MessageSocialContactsEditSignatureRequest {
  1 : required string signature_;                               // 签名
}



/////////////////////////////////////////
// 编辑签名回复
// type: MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_RESPONSE
// dir: gateway->client
struct MessageSocialContactsEditSignatureResponse {
  1 : required bool result_;                                    // 结果
  2 : optional string signature_;                               // 签名
}



/////////////////////////////////////////
// 新增联系人请求
// type: MESSAGE_SOCIAL_CONTACTS_ADD_REQUEST
// dir: client->gateway
struct MessageSocialContactsAddRequest {
  1 : required string actor_name_;                              // 玩家名
  2 : required contacts.ContactsType contacts_type_;            // 联系人类型
}



/////////////////////////////////////////
// 新增联系人回复
// type: MESSAGE_SOCIAL_CONTACTS_ADD_RESPONSE
// dir: gateway->client
struct MessageSocialContactsAddResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 新增联系人通知
// type: MESSAGE_SOCIAL_CONTACTS_ADD_NOTIFY
// dir: gateway->client
struct MessageSocialContactsAddNotify {
  1 : required string actor_name_;                              // 玩家名
}



/////////////////////////////////////////
// 删除联系人请求
// type: MESSAGE_SOCIAL_CONTACTS_REMOVE_REQUEST
// dir: client->gateway
struct MessageSocialContactsRemoveRequest {
  1 : required string actor_id_;                                // 玩家ID
  2 : required contacts.ContactsType contacts_type_;            // 联系人类型
}



/////////////////////////////////////////
// 删除联系人回复
// type: MESSAGE_SOCIAL_CONTACTS_REMOVE_RESPONSE
// dir: gateway->client
struct MessageSocialContactsRemoveResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 移动联系人请求
// type: MESSAGE_SOCIAL_CONTACTS_MOVE_REQUEST
// dir: client->gateway
struct MessageSocialContactsMoveRequest {
  1 : required string actor_id_;                                // 玩家ID
  2 : required contacts.ContactsType src_contacts_type_;        // 源联系人类型
  3 : required contacts.ContactsType dst_contacts_type_;        // 目标联系人类型
}



/////////////////////////////////////////
// 移动联系人回复
// type: MESSAGE_SOCIAL_CONTACTS_MOVE_RESPONSE
// dir: gateway->client
struct MessageSocialContactsMoveResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 推荐联系人请求
// type: MESSAGE_SOCIAL_CONTACTS_RECOMMEND_REQUEST
// dir: client->gateway
struct MessageSocialContactsRecommendRequest {
}



/////////////////////////////////////////
// 推荐联系人信息
struct RecommendContactsData {
  1 : required string actor_name_;                              // 玩家名
  2 : required i32 level_;                                      // 玩家等级
}

// 推荐联系人回复
// type: MESSAGE_SOCIAL_CONTACTS_RECOMMEND_RESPONSE
// dir: gateway->client
struct MessageSocialContactsRecommendResponse {
  1 : required list<RecommendContactsData> contacts_;
}



/////////////////////////////////////////
// 仇人信息
struct SocialEnemyData {
  1 : required string actor_id_;                                // 玩家ID
  2 : optional bool online_;                                    // 是否在线
  3 : optional string actor_name_;                              // 玩家名
  4 : optional i32 level_;                                      // 玩家等级
  5 : optional gender.GenderType gender_;                       // 玩家性别
  6 : optional vocation.VocationType vocation_;                 // 玩家职业
  7 : optional i16 map_;                                        // 玩家所在地图
}



/////////////////////////////////////////
// 仇人信息初始化 
// type: MESSAGE_SOCIAL_ENEMY_INITIALIZE
// dir: gateway->client
struct MessageSocialEnemyInitialize {
  1 : required list<SocialEnemyData> enemies_;                  // 仇人数据
}



/////////////////////////////////////////
// 仇人操作类型
enum SocialEnemyOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 仇人信息同步
// type: MESSAGE_SOCIAL_ENEMY_SYNCHRONIZE 
// dir: gateway->client
struct MessageSocialEnemySynchronize {
  1 : required SocialEnemyOperateType type_;                    // 操作类型
  2 : required SocialEnemyData enemy_;                          // 仇人数据
}



/////////////////////////////////////////
// 新增仇人请求
// type: MESSAGE_SOCIAL_ENEMY_ADD_REQUEST
// dir: client->gateway
struct MessageSocialEnemyAddRequest {
  1 : required string actor_name_;                              // 玩家名
}



/////////////////////////////////////////
// 新增仇人回复
// type: MESSAGE_SOCIAL_ENEMY_ADD_RESPONSE
// dir: gateway->client
struct MessageSocialEnemyAddResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 删除仇人请求 
// type: MESSAGE_SOCIAL_ENEMY_REMOVE_REQUEST 
// dir: client->gateway
struct MessageSocialEnemyRemoveRequest {
  1 : required string actor_id_;                                // 玩家ID
}



/////////////////////////////////////////
// 删除仇人回复
// type: MESSAGE_SOCIAL_ENEMY_REMOVE_RESPONSE
// dir: gateway->client
struct MessageSocialEnemyRemoveResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 家园好友数据
struct HomelandFriendData {
  1 : required string actor_id_;                                // 好友玩家ID
  2 : optional bool can_visit_;                                 // 是否可访问
  3 : optional bool tree_can_water_;                            // 神树是否可浇水
  4 : optional bool tree_can_steal_;                            // 神树是否可抢收
}



/////////////////////////////////////////
// 家园侵入者数据
struct HomelandInvaderData {
  1 : required string actor_id_;                                // 玩家ID
  2 : optional string actor_name_;                              // 玩家名 
  3 : optional i32 level_;                                      // 玩家等级
  4 : optional bool can_visit_;                                 // 是否可访问
  5 : optional bool can_strike_back_;                           // 是否可反击
}



/////////////////////////////////////////
// 家园被支援英灵数据
struct HomelandSupportedSoulData {
  1 : required string actor_id_;                                // 玩家ID
  2 : optional string actor_name_;                              // 玩家名
  3 : optional i32 soul_id_;                                    // 支援的英灵ID
  4 : optional i32 soul_level_;                                 // 支援的英灵等级
  5 : optional i32 leave_cooling_;                              // 英灵返回剩余时间
}



/////////////////////////////////////////
// 家园支援英灵数据
struct HomelandSupportSoulData {
  1 : required i32 soul_id_;                                    // 英灵ID
  2 : optional string actor_id_;                                // 支援对象的玩家ID
  3 : optional string actor_name_;                              // 支援对象的玩家名
  4 : optional i32 cooling_;                                    // 支援剩余冷却时间
}



/////////////////////////////////////////
// 家园告示板信息数据
struct HomelandEventData {
  1 : required homeland.HomelandEventType event_type_;          // 事件类型
  2 : required string timestamp_;                               // 时间戳
  3 : required list<string> vars_;                              // 变量表
}



/////////////////////////////////////////
// 家园数据初始化
// type: MESSAGE_HOMELAND_INITIALIZE
// dir: gateway->client
struct MessageHomelandInitialize {
  1 : required i32 upgrade_cooling_;                            // 升级剩余冷却时间
  2 : required bool upgrade_cooling_red_;                       // 升级剩余冷却时间是否变红
  3 : required i32 temple_level_;                               // 神殿等级
  4 : required i32 temple_harvest_times_;                       // 神殿征收次数
  5 : required i32 temple_force_harvest_times_;                 // 神殿强征次数
  6 : required i32 temple_harvest_cooling_;                     // 神殿征收剩余冷却时间
  7 : required i32 goldmine_level_;                             // 金矿等级
  8 : required i32 goldmine_harvest_cooling_;                   // 金矿收获剩余冷却时间
  9 : required i32 goldmine_robbed_times_;                      // 金矿被劫掠次数
  10: required i32 goldmine_loss_rate_;                         // 金矿被劫掠损失的金币比例
  11: required i32 goldmine_rob_times_;                         // 金矿劫掠次数
  12: required i32 goldmine_rob_cooling_;                       // 金矿劫掠剩余冷却时间
  13: required i32 pool_level_;                                 // 神秘之源等级
  14: required i32 pool_accumulate_time_;                       // 神秘之源蓄水时间
  15: required i32 church_free_pray_times_;                     // 祈福堂免费祈福次数
  16: required i32 church_paid_pray_times_;                     // 祈福堂钻石祈福次数
  17: required i32 church_free_refresh_times_;                  // 祈福堂免费刷新次数
  18: required i32 church_chest_type_;                          // 祈福堂宝箱类型(0表示未祈福状态)
  19: required i32 tree_level_;                                 // 神树等级
  20: required i32 tree_harvest_times_;                         // 神树收获次数
  21: required homeland.HomelandTreeStageType tree_stage_;      // 神树所处的阶段
  22: required i32 tree_watered_times_;                         // 神树完成度
  23: required i32 tree_stolen_times_;                          // 神树被抢收次数
  24: required i32 tree_water_cooling_;                         // 神树浇水剩余冷却时间
  25: required i32 tree_growing_cooling_;                       // 神树生长剩余冷却时间
  26: required i32 tree_assist_water_times_;                    // 神树帮助好友浇水次数
  27: required i32 tower1_level_;                               // 炮塔1等级
  28: required i32 tower2_level_;                               // 炮塔2等级
  29: required i32 stronghold_level_;                           // 军团据点等级
  30: required i32 stronghold_call_support_cooling_;            // 军团据点申请支援冷却时间
  31: required i32 stronghold_support_times_;                   // 军团据点支援次数
  32: required list<HomelandSupportedSoulData>
               stronghold_supported_souls_;                     // 军团据点被支援英灵
  33: required list<HomelandSupportSoulData>
               stronghold_support_souls_;                       // 军团据点支援英灵
  34: required list<HomelandFriendData> friends_;               // 家园好友
  35: required list<HomelandInvaderData> invaders_;             // 家园侵入者
  36: required list<HomelandEventData> events_;                 // 告示板信息
}



/////////////////////////////////////////
// 家园数据同步
// type: MESSAGE_HOMELAND_SYNCHRONIZE
// dir: gateway->client
struct MessageHomelandSynchronize {
  1 : optional i32 upgrade_cooling_;                            // 升级剩余冷却时间
  2 : optional bool upgrade_cooling_red_;                       // 升级剩余冷却时间是否变红
  3 : optional i32 temple_level_;                               // 神殿等级
  4 : optional i32 temple_harvest_times_;                       // 神殿征收次数
  5 : optional i32 temple_force_harvest_times_;                 // 神殿强征次数
  6 : optional i32 temple_harvest_cooling_;                     // 神殿征收剩余冷却时间
  7 : optional i32 goldmine_level_;                             // 金矿等级
  8 : optional i32 goldmine_harvest_cooling_;                   // 金矿收获剩余冷却时间
  9 : optional i32 goldmine_robbed_times_;                      // 金矿被劫掠次数
  10: optional i32 goldmine_loss_rate_;                         // 金矿被劫掠损失的金币比例
  11: optional i32 goldmine_rob_times_;                         // 金矿劫掠次数
  12: optional i32 goldmine_rob_cooling_;                       // 金矿劫掠剩余冷却时间
  13: optional i32 pool_level_;                                 // 神秘之源等级
  14: optional i32 pool_accumulate_time_;                       // 神秘之源蓄水时间
  15: optional i32 church_free_pray_times_;                     // 祈福堂免费祈福次数
  16: optional i32 church_paid_pray_times_;                     // 祈福堂钻石祈福次数
  17: optional i32 church_free_refresh_times_;                  // 祈福堂免费刷新次数
  18: optional i32 church_chest_type_;                          // 祈福堂宝箱类型(0表示未祈福状态)
  19: optional i32 tree_level_;                                 // 神树等级
  20: optional i32 tree_harvest_times_;                         // 神树收获次数
  21: optional homeland.HomelandTreeStageType tree_stage_;      // 神树所处的阶段
  22: optional i32 tree_watered_times_;                         // 神树完成度
  23: optional i32 tree_stolen_times_;                          // 神树被抢收次数
  24: optional i32 tree_water_cooling_;                         // 神树浇水剩余冷却时间
  25: optional i32 tree_growing_cooling_;                       // 神树生长剩余冷却时间
  26: optional i32 tree_assist_water_times_;                    // 神树帮助好友浇水次数
  27: optional i32 tower1_level_;                               // 炮塔1等级
  28: optional i32 tower2_level_;                               // 炮塔2等级
  29: optional i32 stronghold_level_;                           // 军团据点等级
  30: optional i32 stronghold_call_support_cooling_;            // 军团据点申请支援冷却时间
  31: optional i32 stronghold_support_times_;                   // 军团据点支援次数
}



/////////////////////////////////////////
// 家园好友操作类型
enum HomelandFriendOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 家园好友数据同步
// type: MESSAGE_HOMELAND_FRIEND_SYNCHRONIZE
// dir: gateway->client
struct MessageHomelandFriendSynchronize {
  1 : required HomelandFriendOperateType type_;                 // 操作类型
  2 : required HomelandFriendData data_;                        // 数据
}



/////////////////////////////////////////
// 家园侵入者操作类型
enum HomelandInvaderOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 家园侵入者数据同步
// type: MESSAGE_HOMELAND_INVADER_SYNCHRONIZE
// dir: gateway->client
struct MessageHomelandInvaderSynchronize {
  1 : required HomelandInvaderOperateType type_;                // 操作类型
  2 : required HomelandInvaderData data_;                       // 数据
}



/////////////////////////////////////////
// 家园被支援英灵操作类型
enum HomelandSupportedSoulOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
}

// 家园被支援英灵数据同步
// type: MESSAGE_HOMELAND_SUPPORTED_SOUL_SYNCHRONIZE
// dir: gateway->client
struct MessageHomelandSupportedSoulSynchronize {
  1 : required HomelandSupportedSoulOperateType type_;          // 操作类型
  2 : required HomelandSupportedSoulData data_;                 // 数据
}



/////////////////////////////////////////
// 家园支援英灵操作类型
enum HomelandSupportSoulOperateType {
  ADD = 0,                                                      // 增加
}

// 家园支援英灵数据同步
// type: MESSAGE_HOMELAND_SUPPORT_SOUL_SYNCHRONIZE
// dir: gateway->client
struct MessageHomelandSupportSoulSynchronize {
  1 : required HomelandSupportSoulOperateType type_;            // 操作类型
  2 : required HomelandSupportSoulData data_;                   // 数据
}



/////////////////////////////////////////
// 家园告示板信息操作类型
enum HomelandEventOperateType {
  ADD = 0,
}

// 家园告示板信息数据同步
// type: MESSAGE_HOMELAND_EVENT_SYNCHRONIZE
// dir: gateway->client
struct MessageHomelandEventSynchronize {
  1 : required HomelandEventOperateType type_;                  // 操作类型
  2 : required HomelandEventData data_;                         // 数据
}



/////////////////////////////////////////
// 访问好友家园请求
// type: MESSAGE_HOMELAND_VISIT_REQUEST
// dir: client->gateway
struct MessageHomelandVisitRequest {
  1 : required string actor_id_;                                // 好友玩家ID
}



/////////////////////////////////////////
// 访问好友家园回复
// tpye: MESSAGE_HOMELAND_VISIT_RESPONSE
// dir: gateway->client
struct MessageHomelandVisitResponse {
  1 : required bool result_;                                    // 结果
  2 : optional string actor_id_;                                // 好友玩家ID
  3 : optional string actor_name_;                              // 好友玩家名
  4 : optional i32 temple_level_;                               // 神殿等级
  5 : optional i32 goldmine_level_;                             // 金矿等级
  6 : optional i32 pool_level_;                                 // 神秘之源等级
  7 : optional i32 tree_level_;                                 // 神树等级
  8 : optional bool church_enable_;                             // 祈福堂是否开启
  9 : optional homeland.HomelandTreeStageType tree_stage_;      // 神树所处的阶段
  10: optional i32 tree_watered_times_;                         // 神树完成度
  11: optional bool tree_can_water_;                            // 神树是否可浇水
  12: optional bool tree_can_steal_;                            // 神树是否可抢收
  13: optional i32 tower1_level_;                               // 炮塔1等级
  14: optional i32 tower2_level_;                               // 炮塔2等级
  15: optional i32 stronghold_level_;                           // 军团据点等级
  16: optional bool stronghold_can_support_;                    // 军团据点是否可支援
}



/////////////////////////////////////////
// 清除升级冷却时间请求
// type: MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST
// dir: client->gateway
struct MessageHomelandClearUpgradeCoolingRequest {
}



/////////////////////////////////////////
// 清除升级冷却时间回复
// type: MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE
// dir: gateway->client
struct MessageHomelandClearUpgradeCoolingResponse {
  1 : required bool result_;                                    // 结果
}


/////////////////////////////////////////
// 神殿升级请求
// type: MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST
// dir: client->gateway
struct MessageHomelandTempleUpgradeRequest {
}



/////////////////////////////////////////
// 神殿升级回复
// type: MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageHomelandTempleUpgradeResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 神殿征收请求
// type: MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST
// dir: client->gateway
struct MessageHomelandTempleHarvestRequest {
}



/////////////////////////////////////////
// 神殿征收回复
// type: MESSAGE_HOMELAND_TEMPLE_HARVEST_RESPONSE
// dir: gateway->client
struct MessageHomelandTempleHarvestResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 清除神殿征收冷却时间请求
// type: MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST
// dir: client->gateway
struct MessageHomelandTempleClearHarvestCoolingRequest {
}



/////////////////////////////////////////
// 清除神殿征收冷却时间回复
// type: MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE
// dir: gateway->client
struct MessageHomelandTempleClearHarvestCoolingResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 神殿强征请求
// type: MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST
// dir: client->gateway
struct MessageHomelandTempleForceHarvestRequest {
}



/////////////////////////////////////////
// 神殿强征回复
// type: MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE
// dir: gateway->client
struct MessageHomelandTempleForceHarvestResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 金矿升级请求
// type: MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST
// dir: client->gateway
struct MessageHomelandGoldmineUpgradeRequest {
}



/////////////////////////////////////////
// 金矿升级回复
// type: MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageHomelandGoldmineUpgradeResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 金矿收获请求
// type: MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST
// dir: client->gateway
struct MessageHomelandGoldmineHarvestRequest {
}



/////////////////////////////////////////
// 金矿收获回复
// type: MESSAGE_HOMELAND_GOLDMINE_HARVEST_RESPONSE
// dir: gateway->client
struct MessageHomelandGoldmineHarvestResponse {
  1 : required bool result_;                                    // 结果
  2 : optional i32 gold_;                                       // 获得金币
}



/////////////////////////////////////////
// 神秘之源升级请求
// type: MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST
// dir: client->gateway
struct MessageHomelandPoolUpgradeRequest {
}



/////////////////////////////////////////
// 神秘之源升级回复
// type: MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageHomelandPoolUpgradeResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 神秘之源领取请求
// type: MESSAGE_HOMELAND_POOL_HARVEST_REQUEST
// dir: client->gateway
struct MessageHomelandPoolHarvestRequest {
}



/////////////////////////////////////////
// 神秘之源领取回复
// type: MESSAGE_HOMELAND_POOL_HARVEST_RESPONSE
// dir: gateway->client
struct MessageHomelandPoolHarvestResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 神秘之源双倍领取请求
// type: MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST
// dir: client->gateway
struct MessageHomelandPoolDoubleHarvestRequest {
}



/////////////////////////////////////////
// 神秘之源双倍领取回复
// type: MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE
// dir: gateway->client
struct MessageHomelandPoolDoubleHarvestResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 祈福堂祈福请求
// type: MESSAGE_HOMELAND_CHURCH_PRAY_REQUEST
// dir: client->gateway
struct MessageHomelandChurchPrayRequest {
}



/////////////////////////////////////////
// 祈福堂祈福回复
// type: MESSAGE_HOMELAND_CHURCH_PRAY_RESPONSE
// dir: gateway->client
struct MessageHomelandChurchPrayResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 祈福堂刷新请求
// type: MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST
// dir: client->gateway
struct MessageHomelandChurchRefreshRequest {
}



/////////////////////////////////////////
// 祈福堂刷新回复
// type: MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE
// dir: gateway->client
struct MessageHomelandChurchRefreshResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 祈福堂高级刷新请求
// type: MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST
// dir: client->gateway
struct MessageHomelandChurchVipRefreshRequest {
}



/////////////////////////////////////////
// 祈福堂高级刷新回复
// type: MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE
// dir: gateway->client
struct MessageHomelandChurchVipRefreshResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 祈福堂领取奖励请求
// type: MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST
// dir: client->gateway
struct MessageHomelandChurchHarvestRequest {
}



/////////////////////////////////////////
// 祈福堂领取奖励回复
// type: MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE
// dir: gateway->client
struct MessageHomelandChurchHarvestResponse {
  1 : required bool result_;                                    // 结果
  2 : optional i32 chest_type_;                                 // 宝箱类型
  3 : optional i32 bonus_id_;                                   // 奖励ID
}



/////////////////////////////////////////
// 神树浇水请求
// type: MESSAGE_HOMELAND_TREE_WATER_REQUEST
// dir: client->gateway
struct MessageHomelandTreeWaterRequest {
}



/////////////////////////////////////////
// 神树浇水回复
// type: MESSAGE_HOMELAND_TREE_WATER_RESPONSE
// dir: gateway->client
struct MessageHomelandTreeWaterResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 神树帮助好友浇水请求
// type: MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST
// dir: client->gateway
struct MessageHomelandTreeAssistWaterRequest {
  1 : required string actor_id_;                                // 好友玩家ID
}



/////////////////////////////////////////
// 神树帮助好友浇水回复
// type: MESSAGE_HOMELAND_TREE_ASSIST_WATER_RESPONSE
// dir: gateway->client
struct MessageHomelandTreeAssistWaterResponse {
  1 : required bool result_;                                    // 结果
  2 : optional string actor_id_;                                // 好友玩家ID
  3 : optional homeland.HomelandTreeStageType tree_stage_;      // 好友神树所处的阶段
  4 : optional i32 tree_watered_times_;                         // 好友神树完成度
}



/////////////////////////////////////////
// 神树抢收请求
// type: MESSAGE_HOMELAND_TREE_STEAL_REQUEST
// dir: client->gateway
struct MessageHomelandTreeStealRequest {
  1 : required string actor_id_;                                // 好友玩家ID
}



/////////////////////////////////////////
// 神树抢收回复
// type: MESSAGE_HOMELAND_TREE_STEAL_RESPONSE
// dir: gateway->client
struct MessageHomelandTreeStealResponse {
  1 : required bool result_;                                    // 结果
  2 : optional i32 restrict_dollars_;                           // 获得礼金
}



/////////////////////////////////////////
// 神树收获请求
// type: MESSAGE_HOMELAND_TREE_HARVEST_REQUEST
// dir: client->gateway
struct MessageHomelandTreeHarvestRequest {
}



/////////////////////////////////////////
// 神树收获回复
// type: MESSAGE_HOMELAND_TREE_HARVEST_RESPONSE
// dir: gateway->client
struct MessageHomelandTreeHarvestResponse {
  1 : required bool result_;                                    // 结果
  2 : optional i32 restrict_dollars_;                           // 获得礼金
}



/////////////////////////////////////////
// 炮塔升级请求
// type: MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST
// dir: client->gateway
struct MessageHomelandTowerUpgradeRequest {
  1 : required i32 tower_id_;                                   // 炮塔ID
}



/////////////////////////////////////////
// 炮塔升级回复
// type: MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageHomelandTowerUpgradeResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 军团据点升级请求
// type: MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST
// dir: client->gateway
struct MessageHomelandStrongholdUpgradeRequest {
}



/////////////////////////////////////////
// 军团据点升级回复
// type: MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageHomelandStrongholdUpgradeResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 军团据点申请支援请求
// type: MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_REQUEST
// dir: client->gateway
struct MessageHomelandStrongholdCallSupportRequest {
}



/////////////////////////////////////////
// 军团据点申请支援回复
// type: MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_RESPONSE
// dir: gateway->client
struct MessageHomelandStrongholdCallSupportResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 军团据点申请支援通知
// type: MESSAGE_HOMELAND_STRONGHOLD_CALL_SUPPORT_NOTIFY
// dir: gateway->client
struct MessageHomelandStrongholdCallSupportNotify {
  1 : required string actor_id_;                                // 玩家ID
  2 : required string actor_name_;                              // 玩家名
}



/////////////////////////////////////////
// 军团据点支援请求
// type: MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_REQUEST
// dir: client->gateway
struct MessageHomelandStrongholdSupportRequest {
  1 : required string actor_id_;                                // 玩家ID
  2 : required i32 soul_id_;                                    // 英灵ID
}



/////////////////////////////////////////
// 军团据点支援回复 
// type: MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_RESPONSE
// dir: gateway->client
struct MessageHomelandStrongholdSupportResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 家园攻防战单位数据
struct HomelandBattleUnitData {
  1 : required i32 unit_id_;                                    // 单位ID
  2 : required homeland.HomelandBattleSideType side_;           // 攻守方
  3 : required homeland.HomelandBattleUnitType unit_type_;      // 单位类型
  4 : optional i32 row_;                                        // 所在行
  5 : optional i32 column_;                                     // 所在列
  6 : optional i32 soul_id_;                                    // 英灵ID
  7 : optional i32 hp_;                                         // 血量
}

// 家园攻防战单位状态变化数据
struct HomelandBattleStateChangeData {
  1 : required i32 unit_id_;                                    // 单位ID
  2 : required homeland.HomelandBattleStateChangeType type_;    // 单位状态变化类型
  3 : optional i32 value_;                                      // 单位状态变化值
}

// 家园攻防战单位行动数据
struct HomelandBattleActionData {
  1 : required i32 unit_id_;                                    // 单位ID
  2 : required homeland.HomelandBattleActionType type_;         // 单位行动类型
  3 : required list<HomelandBattleStateChangeData> result_;     // 单位行动结果
}

// 家园攻方战回合数据
struct HomelandBattleRoundData {
  1 : required list<HomelandBattleActionData> actions_;         // 单位行动
}



/////////////////////////////////////////
// 家园攻防战匹配请求
// type: MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST
// dir: client->gateway
struct MessageHomelandBattleMatchRequest {
}



/////////////////////////////////////////
// 家园攻防战匹配回复
// type: MESSAGE_HOMELAND_BATTLE_MATCH_RESPONSE
// dir: gateway->client
struct MessageHomelandBattleMatchResponse {
  1 : required bool result_;                                    // 结果
  2 : optional string actor_name_;                              // 匹配的对方玩家名
  3 : optional i32 gold_;                                       // 获得金币
}



/////////////////////////////////////////
// 家园攻防战反击请求
// type: MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST
// dir: client->gateway
struct MessageHomelandBattleStrikeBackRequest {
  1 : required string actor_id_;                                // 玩家ID
}



/////////////////////////////////////////
// 家园攻防战反击回复
// type: MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_RESPONSE
// dir: gateway->client
struct MessageHomelandBattleStrikeBackResponse {
  1 : required bool result_;                                    // 结果
  2 : optional i32 gold_;                                       // 获得金币
}



/////////////////////////////////////////
// 家园攻防战战斗数据通知
// type: MESSAGE_HOMELAND_BATTLE_FIGHT_DATA_NOTIFY
// dir: gateway->client
struct MessageHomelandBattleFightDataNotify {
  1 : required string target_actor_name_;                       // 对方玩家名
  2 : required gender.GenderType target_gender_;                // 对方玩家性别
  3 : required list<HomelandBattleUnitData> units_;             // 参战单位
  4 : required list<HomelandBattleRoundData> commander_battle_; // 主将战
  5 : required list<HomelandBattleRoundData> soul_battle_;      // 英灵战
  6 : required homeland.HomelandBattleResultType
               battle_result_;                                  // 战斗结果评价
}

