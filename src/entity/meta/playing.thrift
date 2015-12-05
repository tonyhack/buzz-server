include "resource.thrift"

namespace cpp entity
namespace py ccentity.playing
namespace as3 com.dreamfire.sanguo.data.definition.entity

//////////////////////////////////////
// 副本类型
enum PlayingType {
  MIN = 0,

  COMMON,                                             // 通用
  DAILY_PASS,                                         // 每日关卡

  MAX,
}


//////////////////////////////////////
// 副本阵营类型
enum PlayingCampType {
  MIN = 0,

  PLAYER,                                             // 单人
  TEAM,                                               // 组队
  GUILD,                                              // 公会
  ROOM,                                               // 房间
  ACTIVITY,                                           // 活动

  MAX,
}


//////////////////////////////////////
// 日常副本组类型
enum DailyPassGroup {
  MIN = 0,

  PLOT,                                               // 剧情组
  IDOL,                                               // 神殿组

  MAX,
}


//////////////////////////////////////
// 副本分数类型
enum PlayingScoreType {
  MIN = 0,

  KILL_NPC,                                           // 杀死NPC
  KILL_ACTOR,                                         // 杀死玩家
  EXP,                                                // 获得经验
  DEAD_COUNT,                                         // 死亡次数
  POWER,                                              // 体力值
  DAMAGE,                                             // 伤害
  STAGE,                                              // 阶段

  MAX,
}


//////////////////////////////////////
// 副本结果类型
enum PlayingResultType {
  MIN = 0,

  SIMPLE_SOCRE,                                       // 简单计分
  SIMPLE_PASS,                                        // 简单过关

  MAX,
}


//////////////////////////////////////
// 副本自动进行类型
enum PlayingAutoType {
  MIN = 0,

  RECORD,                                             // 历史最快记录
  IMMEDIATE,                                          // 立刻完成

  MAX,

}


//////////////////////////////////////
// 副本周期类型
enum PlayingPeriodType {
  MIN = 0,

  DAILY,                                              // 每日类型
  WEEKLY,                                             // 每周类型

  MAX,
}


//////////////////////////////////////
// 副本道具数据
struct PlayingItemField {
  1 : required i32 template_;                         // 道具模板ID
  2 : required i32 number_;                           // 数量
}


//////////////////////////////////////
// 副本奖励数据
struct PlayingAwardField {
  1 : required resource.ResourceType type_;           // 资源类型
  2 : required i32 id_;                               // 资源ID
  3 : required i32 number_;                           // 资源数量
}


//////////////////////////////////////
// 副本奖励列表
struct PlayingAwardFields {
  1 : required i32 playing_template_;                 // 副本模板ID
  2 : required list<PlayingAwardField> awards_;       // 副本奖励表
}


//////////////////////////////////////
// 副本得分数据
struct PlayingScoreField {
  1 : required PlayingScoreType type_;                // 类型
  2 : required i32 key_;                              // 键
  3 : required i32 value_;                            // 值
}


//////////////////////////////////////
// 副本排行数据
struct PlayingRankingField {
  1 : required string name_;                          // 玩家名
  2 : required i32 value_;                            // 数值
}


//////////////////////////////////////
// 副本对象
struct PlayingRecordField {
  1 : required i32 period_count_;                     // 周期次数
  2 : required i32 spend_secs_;                       // 耗时(为0表示未完成过)
  3 : required bool first_pass_awarded_;              // 是否已领取过首通奖励
  4 : required list<i32> values_;                     // 值
}


//////////////////////////////////////
// 副本结果
struct PlayingResultField {
  1 : required bool awarded_;                         // 是否已领取奖励
  2 : required i32 award_count_;                      // 奖励次数
  3 : required i32 paid_award_count_;                 // 有偿奖励次数
  4 : required list<i32> values_;                     // 值
}


//////////////////////////////////////
// 副本队伍状态
enum PlayingTeamStatus {
  MIN = 0,

  WAITING,                                            // 等待
  READY,                                              // 准备

  MAX,
}


//////////////////////////////////////
// 副本队伍信息
struct PlayingTeamField {
  1 : required string id_;                            // 队伍ID
  2 : required string name_;                          // 队伍名称
  3 : required i32 min_fight_score_;                  // 最小战斗力限制
  4 : required i32 member_number_;                    // 成员个数
  5 : required PlayingTeamStatus status_;             // 队伍状态
  6 : required i32 capacity_;                         // 容量
  7 : required bool auto_open_;                       // 自动开启
}


//////////////////////////////////////
// 副本队伍队员信息
struct PlayingTeamMemberField {
  1 : required string actor_;                         // 玩家ID
  2 : required string name_;                          // 名称
  3 : required i32 fight_score_;                      // 战斗力
  4 : required i32 level_;                            // 当前等级
  5 : required bool ready_;                           // 是否已准备
}

