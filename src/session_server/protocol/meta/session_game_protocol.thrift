include "session_protocol.thrift"

namespace cpp session.protocol

enum GameMessageType {
  MESSAGE_MIN = 0,

  //////////////////////////////////////
  // 玩家相关
  MESSAGE_ACTOR_LOGIN_REQUEST = 0,              // 玩家登陆请求(session->game)
  MESSAGE_ACTOR_LOGIN_RESPONSE,                 // 玩家登陆回应(game->session)

  MESSAGE_ACTOR_LOGOUT,                         // 玩家退出请求(session->game)

  MESSAGE_ACTOR_BATCH_LOGIN_REQUEST,            // 玩家批量登陆请求(game->session)

  MESSAGE_ACTOR_LEVEL_SYNCHRONIZE,              // 玩家等级同步(game->session)
  MESSAGE_ACTOR_VOCATION_SYNCHRONIZE,           // 玩家职业同步(game->session)
  MESSAGE_ACTOR_FUNCTIONALITY_STATE_SYCHRONIZE, // 玩家系统功能开启状态同步(game->session)
  MESSAGE_ACTOR_FIGHT_SCORE_SYCHRONIZE,         // 玩家战斗力同步(game->session)

  MESSAGE_ACTOR_COMPARE_REQUEST,                // 玩家对比请求(game->session)
  MESSAGE_ACTOR_COMPARE_SYNCHRONIZE,            // 玩家对比同步(session->game)

  MESSAGE_ACTOR_FACADE_SYNCHRONIZE,             // 玩家外观同步(session->game)

  MESSAGE_ACTOR_VIP_SYNCHRONIZE,                // 玩家VIP信息同步(game->session)
  //////////////////////////////////////

  //////////////////////////////////////
  // GM相关
  MESSAGE_GM_COMMAND,                           // 通用GM指令(game->session)
  //////////////////////////////////////

  //////////////////////////////////////
  // 场景相关
  MESSAGE_SCENE_LOGIN_REQUEST,                  // 场景登陆请求(game->session)
  MESSAGE_SCENE_LOGIN_RESPONSE,                 // 场景登陆回应(session->game)

  MESSAGE_SCENE_BATCH_LOGIN_REQUEST,            // 场景批量登陆请求(game->session)
  MESSAGE_SCENE_BATCH_LOGIN_RESPONSE,           // 场景批量登陆回应(session->game)

  MESSAGE_SCENE_LOGOUT_REQUEST,                 // 场景登出请求(game->session)
  MESSAGE_SCENE_LOGOUT_RESPONSE,                // 场景登出回应(session->game)

  MESSAGE_SCENE_JUMP_NOTICE,                    // 场景跳转通知(session->game)

  MESSAGE_SCENE_SUMMON_WORLD_BOSS,              // 通知召唤世界BOSS(session->game)
  MESSAGE_SCENE_WORLD_BOSS_KILLED_NOTICE,       // 场景BOSS被杀死通知session(game->session)

  MESSAGE_SCENE_ACTOR_JUMP_MAP_REQUEST,         // 跳转地图请求(game->session)
  MESSAGE_SCENE_ACTOR_JUMP_MAP_RESPONSE,        // 跳转地图回应(session->game)

  MESSAGE_SCENE_JUMP_SCENE_REQUEST,             // 跳转场景请求(game->session)
  MESSAGE_SCENE_JUMP_SCENE_RESPONSE,            // 跳转场景回应(session->game)

  MESSAGE_SCENE_ACTOR_JUMP_LOGIN_REQUEST,       // 玩家跳转登入请求到会话(game->session)
  MESSAGE_SCENE_ACTOR_JUMP_LOGOUT_REQUEST,      // 玩家跳转登出请求到会话(game->session)

  MESSAGE_SCENE_NPC_JUMP_SYNCHRONIZE,           // npc跳转同步(session->game)

  MESSAGE_SCENE_BRANCH_MAP_SYNCHRONIZE,         // 地图开线同步(session->game)

  MESSAGE_SCENE_ACTOR_SWITCH_BRANCH_NOTICE,     // 切线通知(session->game);
  //////////////////////////////////////

  //////////////////////////////////////
  // 聊天相关
  MESSAGE_CHAT_REQUEST,                         // 聊天请求(game->session)
  //////////////////////////////////////

  //////////////////////////////////////
  // 组队相关
  MESSAGE_TEAM_SYNCHRONIZE,                     // 组队同步(session->game)
  MESSAGE_TEAM_BUFF_SYNCHRONIZE,                // 组队BUFF同步(session->game)
  MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE,         // 组队成员属性同步(game->session)
  //////////////////////////////////////
  
  //////////////////////////////////////
  // 邮件相关
  MESSAGE_MAIL_SEND_REQUEST,                    // 发送邮件请求(game->session)
  MESSAGE_MAIL_SEND_RESPONSE,                   // 发送邮件返回(session->game)
  MESSAGE_MAIL_NOTICE,                          // 通知邮件到达(session->game)
  //////////////////////////////////////

  //////////////////////////////////////
  // 家园相关
  MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_REQUEST,         // 清除升级冷却时间请求(session->game)
  MESSAGE_HOMELAND_CLEAR_UPGRADE_COOLING_RESPONSE,        // 清除升级冷却时间回复(game->session)
  MESSAGE_HOMELAND_TEMPLE_UPGRADE_REQUEST,                // 神殿升级请求(session->game)
  MESSAGE_HOMELAND_TEMPLE_UPGRADE_RESPONSE,               // 神殿升级回复(game->session)
  MESSAGE_HOMELAND_TEMPLE_UPGRADE_NOTIFY,                 // 神殿升级通知(session->game)
  MESSAGE_HOMELAND_TEMPLE_HARVEST_REQUEST,                // 神殿征收请求(session->game)
  MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_REQUEST,  // 清除神殿征收冷却时间请求(session->game)
  MESSAGE_HOMELAND_TEMPLE_CLEAR_HARVEST_COOLING_RESPONSE, // 清除神殿征收冷却时间回复(game->session)
  MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_REQUEST,          // 神殿强征请求(session->game)
  MESSAGE_HOMELAND_TEMPLE_FORCE_HARVEST_RESPONSE,         // 神殿强征回复(game->session)
  MESSAGE_HOMELAND_GOLDMINE_UPGRADE_REQUEST,              // 金矿升级请求(session->game)
  MESSAGE_HOMELAND_GOLDMINE_UPGRADE_RESPONSE,             // 金矿升级回复(game->session)
  MESSAGE_HOMELAND_GOLDMINE_HARVEST_REQUEST,              // 金矿收获请求(session->game)
  MESSAGE_HOMELAND_POOL_UPGRADE_REQUEST,                  // 神秘之源升级请求(session->game)
  MESSAGE_HOMELAND_POOL_UPGRADE_RESPONSE,                 // 神秘之源升级回复(game->session)
  MESSAGE_HOMELAND_POOL_HARVEST_REQUEST,                  // 神秘之源领取请求(session->game)
  MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST,           // 神秘之源双倍领取请求(session->game)
  MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_RESPONSE,          // 神秘之源双倍领取回复(game->session)
  MESSAGE_HOMELAND_POOL_DOUBLE_HARVEST_REQUEST2,          // 神秘之源双倍领取请求2(session->game)
  MESSAGE_HOMELAND_CHURCH_PAID_PRAY_REQUEST,              // 祈福堂钻石祈福请求(session->game)
  MESSAGE_HOMELAND_CHURCH_PAID_PRAY_RESPONSE,             // 祈福堂钻石祈福回复(game->session)
  MESSAGE_HOMELAND_CHURCH_REFRESH_REQUEST,                // 祈福堂刷新请求(session->game)
  MESSAGE_HOMELAND_CHURCH_REFRESH_RESPONSE,               // 祈福堂刷新回复(game->session)
  MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_REQUEST,            // 祈福堂高级刷新请求(session->game)
  MESSAGE_HOMELAND_CHURCH_VIP_REFRESH_RESPONSE,           // 祈福堂高级刷新回复(game->session)
  MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST,                // 祈福堂领取奖励请求(session->game)
  MESSAGE_HOMELAND_CHURCH_HARVEST_RESPONSE,               // 祈福堂领取奖励回复(game->session)
  MESSAGE_HOMELAND_CHURCH_HARVEST_REQUEST2,               // 祈福堂领取奖励请求2(session->game)
  MESSAGE_HOMELAND_TREE_ASSIST_WATER_REQUEST,             // 神树帮助好友浇水请求(session->game)
  MESSAGE_HOMELAND_TREE_STEAL_REQUEST,                    // 神树抢收请求(session->game)
  MESSAGE_HOMELAND_TREE_HARVEST_REQUEST,                  // 神树收获请求(session->game)
  MESSAGE_HOMELAND_TOWER_UPGRADE_REQUEST,                 // 炮塔升级请求(session->game)
  MESSAGE_HOMELAND_TOWER_UPGRADE_RESPONSE,                // 炮塔升级回复(game->session)
  MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_REQUEST,            // 军团据点升级请求(session->game)
  MESSAGE_HOMELAND_STRONGHOLD_UPGRADE_RESPONSE,           // 军团据点升级回复(game->session)
  MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_REQUEST,            // 军团据点支援请求(session->game)
  MESSAGE_HOMELAND_STRONGHOLD_SUPPORT_RESPONSE,           // 军团据点支援回复(game->session)
  MESSAGE_HOMELAND_BATTLE_MATCH_REQUEST,                  // 家园攻防战匹配请求(session->game)
  MESSAGE_HOMELAND_BATTLE_STRIKE_BACK_REQUEST,            // 家园攻防战反击请求(session->game)
  MESSAGE_HOMELAND_SOUL_SYNCHRONIZE,                      // 家园英灵数据同步(game->session)
  MESSAGE_HOMELAND_TREE_WATER,                            // 神树浇水(session->game)
  //////////////////////////////////////

  //////////////////////////////////////
  // 公会相关
  MESSAGE_GUILD_INFO_REQUEST,                   // 公会数据请求(game->session)
  MESSAGE_GUILD_INFO_RESPONSE,                  // 公会数据回复(session->game)
  MESSAGE_GUILD_INFO_SYNCHRONIZE,               // 公会数据同步(session->game)
  MESSAGE_GUILD_ACTOR_INITIALIZE,               // 玩家公会数据初始化(session->game)
  MESSAGE_GUILD_ACTOR_SYNCHRONIZE,              // 玩家公会数据同步(session->game)
  MESSAGE_GUILD_SKILL_SYNCHRONIZE,              // 公会技能数据同步(session->game)
  MESSAGE_GUILD_BUFF_SYNCHRONIZE,               // 公会BUFF数据同步(session->game)
  MESSAGE_GUILD_CREATE_REQUEST,                 // 创建公会请求(game->session)
  MESSAGE_GUILD_DONATE_REQUEST,                 // 公会捐赠请求(game->session)
  MESSAGE_GUILD_SHOP_BUY_ITEM_REQUEST,          // 购买公会商店道具请求(game->session)
  MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE,         // 购买公会商店道具回复(session->game)
  MESSAGE_GUILD_PLAYING_AWARD_REQUEST,          // 公会副本领取奖励请求(game->session)
  MESSAGE_GUILD_PLAYING_AWARD_RESPONSE,         // 公会副本领取奖励回复(session->game)
  MESSAGE_GUILD_ADD_CONTRIBUTION_REQUEST,       // 增加公会贡献值请求(game->session)
  //////////////////////////////////////

  //////////////////////////////////////
  // 副本相关
  MESSAGE_PLAYING_CREATE_REQUEST,               // 创建副本请求(game->session)
  MESSAGE_PLAYING_CREATE_RESPONSE,              // 创建副本回应(session->game)
  MESSAGE_PLAYING_CREATE_SYNCHRONIZE,           // 创建副本同步(session->game)
  MESSAGE_PLAYING_CREATE_FINISH_REQUEST,        // 创建副本完成请求(game->session)
  MESSAGE_PLAYING_CREATE_FINISH_RESPONSE,       // 创建副本完成回应(session->game)
  MESSAGE_PLAYING_ENTER_SYNCHRONIZE,            // 玩家进入副本同步(session->game)
  MESSAGE_PLAYING_DESTORY_REQUEST,              // 销毁副本请求(game->session)
  MESSAGE_PLAYING_DESTORY_RESPONSE,             // 销毁副本回应(session->game)
  MESSAGE_PLAYING_END_REQUEST,                  // 副本结束请求(game->session)
  MESSAGE_PLAYING_END_RESPONSE,                 // 副本结束回应(session->game)
  MESSAGE_PLAYING_ACTOR_PAGE_SYNCHRONIZE,       // 玩家副本数据同步(game->session)
  MESSAGE_PLAYING_TEAM_SYNCHRONIZE,             // 副本队伍同步(session->game)
  MESSAGE_MAZE_MAP_INFO_REQUEST,                // 地宫地图数据请求(game->session)
  MESSAGE_MAZE_MAP_INFO_SYNCHRONIZE,            // 地宫地图数据同步(session->game)
  //////////////////////////////////////

  //////////////////////////////////////
  // 拍卖相关
  MESSAGE_AUCTION_SELL_REQUEST,                 // 上架道具请求(session->game)
  MESSAGE_AUCTION_SELL_RESPONSE,                // 上架道具回复(game->session)
  MESSAGE_AUCTION_BID_REQUEST,                  // 竞拍道具请求(session->game)
  MESSAGE_AUCTION_BID_RESPONSE,                 // 竞拍道具回复(game->session)
  MESSAGE_AUCTION_BUYOUT_REQUEST,               // 一口价购买道具请求(session->game)
  MESSAGE_AUCTION_BUYOUT_RESPONSE,              // 一口价购买道具回复(game->session)
  MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST,        // 提取道具请求(session->game)
  MESSAGE_AUCTION_WITHDRAW_ITEM_RESPONE,        // 提取道具回复(game->session)
  MESSAGE_AUCTION_WITHDRAW_ITEM_REQUEST2,       // 提取道具请求2(session->game)
  MESSAGE_AUCTION_WITHDRAW_MONEY_REQUEST,       // 提取交易金额请求(session->game)
  MESSAGE_AUCTION_GIVE_MONEY_BACK_REQUEST,      // 退还拍卖金请求(session->game)
  //////////////////////////////////////

  //////////////////////////////////////
  // 道具相关
  MESSAGE_ITEM_TREASURE_HUNT_SYNCHRONIZE,       // 寻宝公告内容(game->session)
  MESSAGE_ITEM_MALL_CONFIG_SYNCHRONIZE,         // 商城、藏宝阁配置(session->game)
  //////////////////////////////////////

  MESSAGE_MAX,
}

