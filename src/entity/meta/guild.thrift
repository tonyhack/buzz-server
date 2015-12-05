namespace cpp entity
namespace py ccentity.guild
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////////////////
// 公会职位类型定义(只定义了会长和成员两个特殊值)
enum GuildPositionType {
  MIN = 0, 

  MEMBER = 0,                         // 成员
  LEADER,                             // 会长

  MAX,
}



////////////////////////////////////////////////
// 公会捐赠类型
enum GuildDonateType {
  MIN = 0,

  GOLD = 0,
  FREEDOM_DOLLARS,
  LIGHT_CRYSTAL,
  DARK_CRYSTAL,
  HOLY_CRYSTAL,

  MAX,
}



////////////////////////////////////////////////
// 公会事件类型定义
enum GuildEventType {
  MIN = 0,

  CREATE_GUILD = 0,                   // 创建公会
  JOIN_GUILD,                         // 加入公会
  LEAVE_GUILD,                        // 离开公会
  SET_MEMBER_POSITION,                // 任命
  RESET_MEMBER_POSITION,              // 解除任命
  GIVE_UP_POSITION,                   // 放弃职位
  GIVE_UP_LEADER_POSITION,            // 放弃会长职位
  GUILD_UPGRADE,                      // 公会升级
  GUILD_SHOP_UPGRADE,                 // 公会商店升级
  GUILD_COLLEGE_UPGRADE,              // 公会学院升级
  GUILD_BARRACK_UPGRADE,              // 公会军营升级

  MAX,
}

