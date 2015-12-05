include "guild.thrift"
include "gateway_protocol.thrift"

namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

/////////////////////////////////////////
// 公会错误码
enum GuildErrorCode {
  SUCCESS = 0,                                                  // 成功
  GUILD_NAME_EXIST,                                             // 公会名已存在
  GUILD_NAME_INVALID,                                           // 公会名非法
  PENDING_MEMBER_FULL,                                          // 待审核成员已满
  MEMBER_FULL,                                                  // 成员已满
  TARGET_OFFLINE,                                               // 目标玩家不在线
  TARGET_GUILD_MODULE_DISABLED,                                 // 目标玩家未开启公会
  TARGET_ALREADY_IN_GUILD,                                      // 目标玩家已有公会
  ERROR_UNKNOWN,                                                // 未知错误
}



/////////////////////////////////////////
// 公会AOI数据
struct GuildAoiData {
  1 : required string actor_id_;                                // 玩家ID
  2 : required string guild_name_;                              // 公会名
}



/////////////////////////////////////////
// 公会待审核成员数据
struct GuildPendingMemberData {
  1 : required string actor_id_;                                // 玩家ID
  2 : optional string actor_name_;                              // 玩家名
  3 : optional bool online_;                                    // 是否在线
  4 : optional string last_logout_time_;                        // 玩家上次在线时间
  5 : optional i32 level_;                                      // 玩家等级
  6 : optional i16 map_;                                        // 玩家所在地图
}



/////////////////////////////////////////
// 公会成员数据
struct GuildMemberData {
  1 : required string actor_id_;                                // 玩家ID
  2 : optional string actor_name_;                              // 玩家名
  3 : optional bool online_;                                    // 是否在线
  4 : optional string last_logout_time_;                        // 玩家上次在线时间
  5 : optional i32 level_;                                      // 玩家等级
  6 : optional i16 map_;                                        // 玩家所在地图
  7 : optional i32 guild_position_;                             // 玩家公会职位
  8 : optional i32 total_contribution_value_;                   // 玩家总贡献值
  9 : optional i32 gold_contribution_;                          // 玩家金币捐赠量(万)
  10: optional i32 freedom_dollars_contribution_;               // 玩家钻石捐赠量
  11: optional i32 light_crystal_contribution_;                 // 玩家光晶石捐赠量
  12: optional i32 dark_crystal_contribution_;                  // 玩家暗晶石捐赠量
  13: optional i32 holy_crystal_contribution_;                  // 玩家圣晶石捐赠量
  14: optional i32 axe_fight_score_;                            // 战士战斗力
  15: optional i32 shoot_fight_score_;                          // 弓手战斗力
  16: optional i32 magic_fight_score_;                          // 法师战斗力
}



/////////////////////////////////////////
// 公会事件数据
struct GuildEventData {
  1 : required guild.GuildEventType event_type_;                // 事件类型
  2 : required string timestamp_;                               // 时间戳
  3 : required list<string> vars_;                              // 变量表
}



/////////////////////////////////////////
// 公会技能数据
struct GuildSkillData {
  1 : required i32 id_;                                         // 公会技能ID
  2 : optional i32 level_;                                      // 公会技能等级
  3 : optional i32 effective_level_;                            // 公会技能有效等级
}



/////////////////////////////////////////
// 公会列表数据
struct GuildListData {
  1 : required string guild_id_;                                // 公会ID
  2 : optional string guild_name_;                              // 公会名
  3 : optional i32 guild_level_;                                // 公会等级
  4 : optional i32 member_num_;                                 // 成员数
  5 : optional string leader_actor_id_;                         // 会长玩家ID
  6 : optional string leader_actor_name_;                       // 会长玩家名
  7 : optional bool leader_online_;                             // 会长是否在线
  8 : optional string total_fight_score_;                       // 总战斗力
  9 : optional bool suppress_join_;                             // 是否关闭申请
  10: optional bool joined_;                                    // 是否正在申请这个公会
}



/////////////////////////////////////////
// 公会AOI数据同步
// type: MESSAGE_GUILD_AOI_SYNCHRONIZE
// dir: gateway->client
struct MessageGuildAoiSynchronize {
  1 : required list<GuildAoiData> aoi_data_;                    // AOI数据
}

/////////////////////////////////////////
// 公会数据初始化
// type: MESSAGE_GUILD_INITIALIZE
// dir: gateway->client
struct MessageGuildInitialize {
  1 : required string guild_id_;                                // 公会ID
  2 : optional string guild_name_;                              // 公会名
  3 : optional i32 guild_logo_;                                 // 公会标志
  4 : optional i32 guild_level_;                                // 公会等级
  5 : optional string guild_announcement_;                      // 公会公告
  6 : optional i32 guild_gold_;                                 // 公会金币数量(万)
  7 : optional i32 guild_light_crystal_;                        // 公会光晶石数量
  8 : optional i32 guild_dark_crystal_;                         // 公会暗晶石数量
  9 : optional i32 guild_holy_crystal_;                         // 公会圣晶石数量
  10: optional i32 guild_light_attribute_;                      // 公会光属性值
  11: optional i32 guild_dark_attribute_;                       // 公会暗属性值
  12: optional i32 guild_holy_attribute_;                       // 公会圣属性值
  13: optional i32 guild_shop_level_;                           // 公会商店等级
  14: optional i32 guild_college_level_;                        // 公会学院等级
  15: optional i32 guild_barrack_level_;                        // 公会军营等级
  16: optional bool suppress_join_;                             // 是否关闭申请
  17: optional list<GuildPendingMemberData> pending_members_;   // 待审核成员数据
  18: optional list<GuildMemberData> members_;                  // 成员数据
  19: optional list<GuildEventData> events_;                    // 公会事件
  20: optional i32 current_contribution_value_;                 // 玩家当前贡献值
  21: optional list<GuildSkillData> guild_skills_;              // 公会技能
  22: optional list<i32> guild_buffs_;                          // 公会BUFF
  23: optional string current_playing_id_;                      // 当前正在进行中的副本ID
  24: optional i32 current_playing_template_id_;                // 当前正在进行中的副本模板ID
  25: optional list<i32> opened_playing_groups_;                // 解封的公会副本组
  26: optional list<i32> finished_playings_;                    // 完成的公会副本(公会进度)
  27: optional list<i32> awarded_playings_;                     // 已领取过奖励的公会副本
}



/////////////////////////////////////////
// 公会数据同步 
// type: MESSAGE_GUILD_SYNCHRONIZE
// dir: gateway->client
struct MessageGuildSynchronize {
  1 : optional i32 guild_level_;                                // 公会等级
  2 : optional string guild_announcement_;                      // 公会公告
  3 : optional i32 guild_gold_;                                 // 公会金币数量(万)
  4 : optional i32 guild_light_crystal_;                        // 公会光晶石数量
  5 : optional i32 guild_dark_crystal_;                         // 公会暗晶石数量
  6 : optional i32 guild_holy_crystal_;                         // 公会圣晶石数量
  7 : optional i32 guild_light_attribute_;                      // 公会光属性值
  8 : optional i32 guild_dark_attribute_;                       // 公会暗属性值
  9 : optional i32 guild_holy_attribute_;                       // 公会圣属性值
  10: optional i32 guild_shop_level_;                           // 公会商店等级
  11: optional i32 guild_college_level_;                        // 公会学院等级
  12: optional i32 guild_barrack_level_;                        // 公会军营等级
  13: optional bool suppress_join_;                             // 是否关闭申请
  14: optional i32 current_contribution_value_;                 // 玩家当前贡献值
  15: optional string current_playing_id_;                      // 当前正在进行中的副本ID
  16: optional i32 current_playing_template_id_;                // 当前正在进行中的副本模板ID
}



/////////////////////////////////////////
// 公会待审核成员操作类型
enum GuildPendingMemberOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 公会待审核成员数据同步
// type: MESSAGE_GUILD_PENDING_MEMBER_SYNCHRONIZE
// dir: gateway->client
struct MessageGuildPendingMemberSynchronize {
  1 : required GuildPendingMemberOperateType type_;             // 操作类型
  2 : required GuildPendingMemberData data_;                    // 数据
}



/////////////////////////////////////////
// 公会成员操作类型
enum GuildMemberOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 公会成员数据同步
// type: MESSAGE_GUILD_MEMBER_SYNCHRONIZE
// dir: gateway->client
struct MessageGuildMemberSynchronize {
  1 : required GuildMemberOperateType type_;                    // 操作类型
  2 : required GuildMemberData data_;                           // 数据
}



/////////////////////////////////////////
// 公会事件操作类型
enum GuildEventOperateType {
  ADD = 0,                                                      // 增加
}

// 公会事件数据同步
// type: MESSAGE_GUILD_EVENT_SYNCHRONIZE
// dir: gateway->client
struct MessageGuildEventSynchronize {
  1 : required GuildEventOperateType type_;                     // 操作类型
  2 : required GuildEventData data_;                            // 数据
} 



/////////////////////////////////////////
// 公会技能操作类型
enum GuildSkillOperateType {
  ADD = 0,                                                      // 增加
  UPDATE,                                                       // 更新
}

// 公会技能数据同步
// type: MESSAGE_GUILD_SKILL_SYNCHRONIZE
// dir: gateway->client
struct MessageGuildSkillSynchronize {
  1 : required GuildSkillOperateType type_;                     // 操作类型
  2 : required GuildSkillData data_;                            // 数据
}



/////////////////////////////////////////
// 公会BUFF操作类型
enum GuildBuffOperateType {
  ADD = 0,                                                      // 增加
  CLEAR,                                                        // 清空
}

// 公会BUFF数据同步
// type: MESSAGE_GUILD_BUFF_SYNCHRONIZE
// dir: gateway->client
struct MessageGuildBuffSynchronize {
  1 : required GuildBuffOperateType type_;                      // 操作类型
  2 : required i32 guild_buff_id_;                              // 数据
}



/////////////////////////////////////////
// 公会列表数据请求
// type: MESSAGE_GUILD_LIST_REQUEST
// dir: client->gateway
struct MessageGuildListRequest {
}



/////////////////////////////////////////
// 公会列表数据回复
// type: MESSAGE_GUILD_LIST_RESPONSE
// dir: gateway->client
struct MessageGuildListResponse {
  1 : required bool result_;                                    // 结果
  2 : optional list<GuildListData> guilds_;                     // 公会信息
}



/////////////////////////////////////////
// 公会列表操作类型
enum GuildListOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 公会列表数据同步
// type: MESSAGE_GUILD_LIST_SYNCHRONIZE
// dir: gateway->client
struct MessageGuildListSynchronize {
  1 : required GuildListOperateType type_;                      // 操作类型
  2 : required GuildListData data_;                             // 数据
}



/////////////////////////////////////////
// 创建公会请求
// type: MESSAGE_GUILD_CREATE_REQUEST
// dir: client->gateway
struct MessageGuildCreateRequest {
  1 : required string guild_name_;                              // 公会名
  2 : required i32 guild_logo_;                                 // 公会标志
}



/////////////////////////////////////////
// 创建公会回复
// type: MESSAGE_GUILD_CREATE_RESPONSE
// dir: gateway->client
struct MessageGuildCreateResponse {
  1 : required GuildErrorCode result_;                          // 结果
}



/////////////////////////////////////////
// 公会解散通知
// type: MESSAGE_GUILD_DISMISS_NOTIFY
// dir: gateway->client
struct MessageGuildDismissNotify {
}



/////////////////////////////////////////
// 公会关闭申请请求
// type: MESSAGE_GUILD_SUPPRESS_JOIN_REQUEST
// dir: client->gateway
struct MessageGuildSuppressJoinRequest {
  1 : required bool suppress_join_;                             // 是否关闭申请
}



/////////////////////////////////////////
// 公会关闭申请回复
// type: MESSAGE_GUILD_SUPPRESS_JOIN_RESPONSE
// dir: gateway->client
struct MessageGuildSuppressJoinResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会申请请求
// type: MESSAGE_GUILD_JOIN_REQUEST
// dir: client->gateway
struct MessageGuildJoinRequest {
  1 : required string guild_id_;                                // 公会ID
}



/////////////////////////////////////////
// 公会申请回复
// type: MESSAGE_GUILD_JOIN_RESPONSE
// dir: gateway->client
struct MessageGuildJoinResponse {
  1 : required GuildErrorCode result_;                          // 结果
}



/////////////////////////////////////////
// 公会取消申请请求
// type: MESSAGE_GUILD_CANCEL_JOIN_REQUEST
// dir: client->gateway
struct MessageGuildCancelJoinRequest {
  1 : required string guild_id_;                                // 公会ID
}



/////////////////////////////////////////
// 公会取消申请回复
// type: MESSAGE_GUILD_CANCEL_JOIN_RESPONSE
// dir: gateway->client
struct MessageGuildCancelJoinResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会审核成员加入请求
// type: MESSAGE_GUILD_AUDIT_PENDING_MEMBER_REQUEST
// dir: client->gateway
struct MessageGuildAuditPendingMemberRequest {
  1 : required string actor_id_;                                // 玩家ID
  2 : required bool audit_result_;                              // 审核结果
}



/////////////////////////////////////////
// 公会审核成员加入回复
// type: MESSAGE_GUILD_AUDIT_PENDING_MEMBER_RESPONSE
// dir: gateway->client
struct MessageGuildAuditPendingMemberResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会邀请请求
// type: MESSAGE_GUILD_INVITE_REQUEST
// dir: client->gateway
struct MessageGuildInviteRequest {
  1 : required string actor_name_;                              // 玩家ID
}



/////////////////////////////////////////
// 公会邀请回复 
// type: MESSAGE_GUILD_INVITE_RESPONSE
// dir: gateway->client
struct MessageGuildInviteResponse {
  1 : required GuildErrorCode result_;                          // 结果
}



/////////////////////////////////////////
// 公会邀请通知
// type: MESSAGE_GUILD_INVITE_NOTIFY
// dir: gateway->client
struct MessageGuildInviteNotify {
  1 : required string guild_id_;                                // 公会ID
}



/////////////////////////////////////////
// 公会邀请处理请求
// type: MESSAGE_GUILD_INVITE_DISPOSE_REQUEST
// dir: client->gateway
struct MessageGuildInviteDisposeRequest {
  1 : required string guild_id_;                                // 公会ID
  2 : required bool result_;                                    // 是否同意加入
}



/////////////////////////////////////////
// 公会邀请处理回复
// type: MESSAGE_GUILD_INVITE_DISPOSE_RESPONSE
// dir: gateway->client
struct MessageGuildInviteDisposeResponse {
  1 : required GuildErrorCode result_;                          // 结果
}



/////////////////////////////////////////
// 离开公会请求
// type: MESSAGE_GUILD_LEAVE_REQUEST
// dir: client->gateway
struct MessageGuildLeaveRequest {
}



/////////////////////////////////////////
// 离开公会回复
// type: MESSAGE_GUILD_LEAVE_RESPONSE
// dir: gateway->client
struct MessageGuildLeaveResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 踢出成员请求
// type: MESSAGE_GUILD_KICK_MEMBER_REQUEST
// dir: client->gateway
struct MessageGuildKickMemberRequest {
  1 : required string actor_id_;                                // 玩家ID
}



/////////////////////////////////////////
// 踢出成员回复
// type: MESSAGE_GUILD_KICK_MEMBER_RESPONSE
// dir: gateway->client
struct MessageGuildKickMemberResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 踢出成员通知
// type: MESSAGE_GUILD_KICK_MEMBER_NOTIFY
// dir: gateway->client
struct MessageGuildKickMemberNotify {
}



/////////////////////////////////////////
// 放弃会长职位请求
// type: MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_REQUEST
// dir: client->gateway
struct MessageGuildGiveUpLeaderPositionRequest {
  1 : required string new_leader_actor_id_;
}



/////////////////////////////////////////
// 放弃会长职位回复
// type: MESSAGE_GUILD_GIVE_UP_LEADER_POSITION_RESPONSE
// dir: gateway->client
struct MessageGuildGiveUpLeaderPositionResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 放弃公会职位请求
// type: MESSAGE_GUILD_GIVE_UP_POSITION_REQUEST
// dir: client->gateway
struct MessageGuildGiveUpPositionRequest {
}



/////////////////////////////////////////
// 放弃公会职位回复
// type: MESSAGE_GUILD_GIVE_UP_POSITION_RESPONSE
// dir: gateway->client
struct MessageGuildGiveUpPositionResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会职位任免请求
// type: MESSAGE_GUILD_SET_MEMBER_POSITION_REQUEST
// dir: client->gateway
struct MessageGuildSetMemberPositionRequest {
  1 : required string actor_id_;                                // 玩家ID
  2 : required i32 guild_position_;                             // 玩家公会职位
}



/////////////////////////////////////////
// 公会职位任免回复
// type: MESSAGE_GUILD_SET_MEMBER_POSITION_RESPONSE
// dir: gateway->client
struct MessageGuildSetMemberPositionResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 编辑公会公告请求
// type: MESSAGE_GUILD_EDIT_ANNOUNCEMENT_REQUEST
// dir: client->gateway
struct MessageGuildEditAnnouncementRequest {
  1 : required string guild_announcement_;                      // 公会公告
}



/////////////////////////////////////////
// 编辑公会公告回复
// type: MESSAGE_GUILD_EDIT_ANNOUNCEMENT_RESPONSE
// dir: gateway->client
struct MessageGuildEditAnnouncementResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会升级请求
// type: MESSAGE_GUILD_UPGRADE_REQUEST
// dir: client->gateway
struct MessageGuildUpgradeRequest {
}



/////////////////////////////////////////
// 公会升级回复
// type: MESSAGE_GUILD_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageGuildUpgradeResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会捐赠请求 
// type: MESSAGE_GUILD_DONATE_REQUEST
// dir: client->gateway
struct MessageGuildDonateRequest {
  1 : required guild.GuildDonateType type_;                     // 捐赠类型
  2 : required i32 num_;                                        // 捐赠数量
}



/////////////////////////////////////////
// 公会捐赠回复
// type: MESSAGE_GUILD_DONATE_RESPONSE
// dir: gateway->client
struct MessageGuildDonateResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会商店升级请求
// type: MESSAGE_GUILD_SHOP_UPGRADE_REQUEST
// dir: client->gateway
struct MessageGuildShopUpgradeRequest {
}



/////////////////////////////////////////
// 公会商店升级回复
// type: MESSAGE_GUILD_SHOP_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageGuildShopUpgradeResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 购买公会商店道具请求
// type: MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST
// dir: client->gateway
struct MessageGuildShopBuyItemRequest {
  1 : required i32 item_id_;                                    // 道具ID
  2 : required i32 item_count_;                                 // 道具数量
}



/////////////////////////////////////////
// 购买公会商店道具回复
// type: MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE
// dir: gateway->client
struct MessageGuildShopBuyItemResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会学院升级请求
// type: MESSAGE_GUILD_COLLEGE_UPGRADE_REQUEST
// dir: client->gateway
struct MessageGuildCollegeUpgradeRequest {
}



/////////////////////////////////////////
// 公会学院升级回复
// type: MESSAGE_GUILD_COLLEGE_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageGuildCollegeUpgradeResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 学习公会技能请求
// type: MESSAGE_GUILD_COLLEGE_LEARN_SKILL_REQUEST
// dir: client->gateway
struct MessageGuildCollegeLearnSkillRequest {
  1 : required i32 guild_skill_id_;                             // 公会技能ID
}



/////////////////////////////////////////
// 学习公会技能回复
// type: MESSAGE_GUILD_COLLEGE_LEARN_SKILL_RESPONSE
// dir: gateway->client
struct MessageGuildCollegeLearnSkillResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 神像祈祷请求
// type: MESSAGE_GUILD_IDOL_PRAY_REQUEST
// dir: client->gateway
struct MessageGuildIdolPrayRequest {
}



/////////////////////////////////////////
// 神像祈祷回复
// type: MESSAGE_GUILD_IDOL_PRAY_RESPONSE
// dir: gateway->client
struct MessageGuildIdolPrayResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会军营升级请求
// type: MESSAGE_GUILD_BARRACK_UPGRADE_REQUEST
// dir: client->gateway
struct MessageGuildBarrackUpgradeRequest {
}



/////////////////////////////////////////
// 公会军营升级回复
// type: MESSAGE_GUILD_BARRACK_UPGRADE_RESPONSE
// dir: gateway->client
struct MessageGuildBarrackUpgradeResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会副本组解封请求
// type: MESSAGE_GUILD_PLAYING_GROUP_OPEN_REQUEST
// dir: client->gateway
struct MessageGuildPlayingGroupOpenRequest {
  1 : required i32 playing_group_id_;                           // 副本组ID
}



/////////////////////////////////////////
// 公会副本组解封回复
// type: MESSAGE_GUILD_PLAYING_GROUP_OPEN_RESPONSE
// dir: gateway->client
struct MessageGuildPlayingGroupOpenResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会副本组解封通知
// type: MESSAGE_GUILD_PLAYING_GROUP_OPEN_NOTIFY
// dir: gateway->client
struct MessageGuildPlayingGroupOpenNotify {
  1 : required i32 playing_group_id_;                           // 副本组ID
}



/////////////////////////////////////////
// 公会副本完成通知
// type: MESSAGE_GUILD_PLAYING_FINISHED_NOTIFY
// dir: gateway->client
struct MessageGuildPlayingFinishedNotify {
  1 : required i32 playing_template_id_;                        // 副本模板ID
}



/////////////////////////////////////////
// 公会副本领取奖励请求
// type: MESSAGE_GUILD_PLAYING_AWARD_REQUEST
// dir: client->gateway
struct MessageGuildPlayingAwardRequest {
  1 : required i32 playing_template_id_;                        // 副本模板ID
}



/////////////////////////////////////////
// 公会副本领取奖励回复
// tpye: MESSAGE_GUILD_PLAYING_AWARD_RESPONSE
// dir: gateway->client
struct MessageGuildPlayingAwardResponse {
  1 : required bool result_;                                    // 结果
}



/////////////////////////////////////////
// 公会副本重置通知
// type: MESSAGE_GUILD_PLAYING_RESET_NOTIFY
// dir: gateway->client
struct MessageGuildPlayingResetNotify {
}

