include "session_game_protocol.thrift"

namespace cpp session.protocol


/////////////////////////////////////////
// 组队同步
// type: MESSAGE_TEAM_SYNCHRONIZE
// dir: session->game
struct MessageTeamSynchronize {
  1 : required i64 actor_;                                    // 玩家ID
  2 : required i64 team_;                                     // 队伍ID
}

/////////////////////////////////////////
// 组队BUFF同步
// type: MESSAGE_TEAM_BUFF_SYNCHRONIZE
// dir: session->game
struct MessageTeamBuffSynchronize {
  1 : required i64 actor_;                                    // 玩家ID
  2 : required i32 buff_;                                     // BUFF
}

/////////////////////////////////////////
// 组队玩家属性同步
// type: MESSAGE_TEAM_MEMBER_ATTR_SYNCHRONIZE
// dir: game->session
enum TeamMemberAttrType {
  HP = 0,                                                     // 生命值
  MAX_HP,                                                     // 最大生命值
  MP,                                                         // 魔法值
  MAX_MP,                                                     // 最大魔法值
  LOCATION_X,                                                 // 坐标x
  LOCATION_Y,                                                 // 坐标y
}
struct TeamMemberAttrFields {
  1 : required TeamMemberAttrType type_;                      // 属性类型
  2 : required i32 value_;                                    // 属性值
}
struct MessageTeamMemberAttrSynchronize {
  1 : required i64 actor_;                                    // 玩家ID
  2 : required list<TeamMemberAttrFields> attrs_;             // 属性集
}

