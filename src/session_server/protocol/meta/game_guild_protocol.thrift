include "guild.thrift"
include "session_game_protocol.thrift"

namespace cpp session.protocol

////////////////////////////////////////
// 公会数据
struct GuildData {
  1 : required i64 id_;                                         // 公会ID
  2 : optional string name_;                                    // 公会名
  3 : optional i32 level_;                                      // 公会等级
  4 : optional i32 light_attribute_;                            // 光属性值
  5 : optional i32 dark_attribute_;                             // 暗属性值
  6 : optional i32 holy_attribute_;                             // 圣属性值
  7 : optional i32 shop_level_;                                 // 商店等级
  8 : optional i32 college_level_;                              // 学院等级
  9 : optional i32 barrack_level_;                              // 军营等级
}



/////////////////////////////////////////
// 公会技能数据
struct GuildSkillData {
  1 : required i32 id_;                                         // 公会技能ID
  2 : optional i32 effective_level_;                            // 公会技能有效等级
}



/////////////////////////////////////////
// 公会玩家数据
struct GuildActorData {
  1 : required i64 guild_id_;                                   // 公会ID
  2 : required i32 guild_position_;                             // 公会职位
  3 : required i32 current_contribution_value_;                 // 当前贡献值
  4 : required list<GuildSkillData> guild_skills_;              // 公会技能
  5 : required list<i32> guild_buffs_;                          // 公会BUFF
}



////////////////////////////////////////
// 公会数据请求
// type: MESSAGE_GUILD_INFO_REQUEST
// dir: game->session
struct MessageGuildInfoRequest {
}



////////////////////////////////////////
// 公会数据回复
// type: MESSAGE_GUILD_INFO_RESPONSE
// dir: session->game
struct MessageGuildInfoResponse {
  1 : required list<GuildData> guilds_;                         // 公会数据
}



////////////////////////////////////////
// 公会数据操作类型
enum GuildOperateType {
  ADD = 0,                                                      // 增加
  REMOVE,                                                       // 删除
  UPDATE,                                                       // 更新
}

// 公会数据同步
// type: MESSAGE_GUILD_INFO_SYNCHRONIZE
// dir: session->game
struct MessageGuildInfoSynchronize {
  1 : required GuildOperateType type_;                          // 操作类型
  2 : required GuildData data_;                                 // 数据
}



////////////////////////////////////////
// 玩家公会数据初始化
// type: MESSAGE_GUILD_ACTOR_INITIALIZE
// dir: session->game
struct MessageGuildActorInitialize {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required GuildActorData data_;                            // 公会玩家数据
}



////////////////////////////////////////
// 玩家公会数据同步
// type: MESSAGE_GUILD_ACTOR_SYNCHRONIZE
// dir: session->game
struct MessageGuildActorSynchronize {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : optional i64 guild_id_;                                   // 公会ID
  3 : optional string guild_name_;                              // 公会名
  4 : optional i32 guild_position_;                             // 公会职位
  5 : optional i32 current_contribution_value_;                 // 当前贡献值
}



/////////////////////////////////////////
// 公会技能操作类型
enum GuildSkillOperateType {
  ADD = 0,                                                      // 增加
  UPDATE,                                                       // 更新
  CLEAR,                                                        // 清空
}

// 公会技能数据同步
// type: MESSAGE_GUILD_SKILL_SYNCHRONIZE
// dir: session->game
struct MessageGuildSkillSynchronize {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required GuildSkillOperateType type_;                     // 操作类型
  3 : required GuildSkillData data_;                            // 数据
}



/////////////////////////////////////////
// 公会BUFF操作类型
enum GuildBuffOperateType {
  ADD = 0,                                                      // 增加
  CLEAR,                                                        // 清空
}

// 公会BUFF数据同步
// type: MESSAGE_GUILD_BUFF_SYNCHRONIZE
// dir: session->game
struct MessageGuildBuffSynchronize {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required GuildBuffOperateType type_;                      // 操作类型
  3 : required i32 guild_buff_id_;                              // 数据
}



/////////////////////////////////////////
// 创建公会请求
// type: MESSAGE_GUILD_CREATE_REQUEST
// dir: game->session 
struct MessageGuildCreateRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required string guild_name_;                              // 公会名
  3 : required i32 guild_logo_;                                 // 公会标志
}



/////////////////////////////////////////
// 公会捐赠请求
// type: MESSAGE_GUILD_DONATE_REQUEST
// dir: game->session
struct MessageGuildDonateRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required guild.GuildDonateType type_;                     // 捐赠类型
  3 : required i32 num_;                                        // 捐赠数量
}



/////////////////////////////////////////
// 购买公会商店道具请求
// type: MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST
// dir: game->session
struct MessageGuildShopBuyItemRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 item_id_;                                    // 道具ID
  3 : required i32 item_count_;                                 // 道具数量
}



/////////////////////////////////////////
// 购买公会商店道具回复
// type: MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE
// dir: session->game
struct MessageGuildShopBuyItemResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 item_id_;                                    // 道具ID
  3 : required i32 item_count_;                                 // 道具数量
}



/////////////////////////////////////////
// 公会副本领取奖励请求
// type: MESSAGE_GUILD_PLAYING_AWARD_REQUEST
// dir: game->session
struct MessageGuildPlayingAwardRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 playing_template_id_;                        // 副本模板ID
}



/////////////////////////////////////////
// 公会副本领取奖励回复
// type: MESSAGE_GUILD_PLAYING_AWARD_RESPONSE
// dir: session->game
struct MessageGuildPlayingAwardResponse {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required bool result_;                                    // 结果
  3 : optional i32 playing_template_id_;                        // 副本模板ID
}



/////////////////////////////////////////
// 增加公会贡献值请求
// type: MESSAGE_GUILD_ADD_CONTRIBUTION_REQUEST
// dir: game->session
struct MessageGuildAddContributionRequest {
  1 : required i64 actor_id_;                                   // 玩家ID
  2 : required i32 add_value_;                                  // 增加值
}

