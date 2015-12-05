namespace cpp entity
namespace py ccentity.vip
namespace as3 com.dreamfire.sanguo.data.definition.entity


////////////////////////////////////////////////
// VIP卡类型
enum VipType {
  INVALID = 0,                              // 无
  VIP_1,                                    // 勇者
  VIP_2,                                    // 英雄
  VIP_3,                                    // 帝王
  MAX,                                      // 上限
}


///////////////////////////////////////////////
// VIP特权
enum VipPrivilegeType {
  MIN = 0,

  SOCIAL_HOMELAND_ODER = 0,                 // 额外家园建筑序列
  SOCIAL_HOMELAND_JUMP_FIGHT,               // 家园跳过战斗
  SOCIAL_HOMELAND_HELP_WATERING,            // 家园帮好友浇水
  SOCIAL_HOMELAND_FIGHT_NUM,                // 家园额外攻击次数
  SOCIAL_HOMELAND_IMPRESS,                  // 家园强征次数
  CHECKIN_AWARDS,                           // 签到额外奖励
  ATTR_ATTRIBUTE_UP,                        // 一级属性提升
  SCENE_KILL_NPC_UP_EXP,                    // 打怪经验提升
  SCENE_MEDITATE_AWARD_UP,                  // 冥想收益提升
  SCENE_WORLD_JUMP_NUM,                     // 每日VIP传送次数
  CHECKIN_UP_NUM,                           // 补签到(月)
  TRANSPORT_NUM,                            // 押镖次数提高
  DAILY_GIFT,                               // 每日礼包
  TASK_FREE_FLY,                            // 免费的任务传送
  DAILY_HONOUR,                             // 每日获得额外的军功
  MAZE_ACTION_POINT,                        // 地宫每日行动次数
  PLAYING_PERIOD_COUNT,                     // 副本周期次数加成
  PLAYING_GROUP_AUTO,                       // 副本组自动出征(一键自动出征)
  PLAYING_PERIOD_PAID_AWARD_COUNT,          // 副本周期有偿领奖次数
  TRANSPORT_TASK_COUNT,                     // 每日护送次数
  TRANSPORT_TASK_ROB_COUNT,                 // 每日护送抢劫次数
  TRANSPORT_TASK_REFRESH_COUNT,             // 每日护送免费刷新次数

  MAX,
}


///////////////////////////////////////////////
// VIP容许类型
enum VipAdmitType {
  NO_ADMIT = 0,                             // 不容许
  ADMIT,                                    // 容许
}

