namespace cpp entity
namespace py ccentity.task
namespace as3 com.dreamfire.sanguo.data.definition.entity

///////////////////////////////////////////////
// 任务条件类型
enum TaskCondition {
  MIN = 0,

  KILL_MOB,                   // 杀怪
  kill_COLLECT,               // 收集道具
  PICK,                       // 采集
  TALK,                       // 对话
  USE_ITEM,                   // 使用道具
  COLLECT_ITEM,               // 收集道具
  WAITE_TIME,                 // 等待时间
  EQUIP_STRENGHT,             // 装备强化
  EQUIP_REFINE,               // 装备洗练
  DIVINE_GEM,                 // 占星
  SPIRIT_FEED,                // 精灵培养
  EQUIP_INLAY,                // 宝石镶嵌
  SOUL_FEED,                  // 英灵培养
  EQUIP_SOUL,                 // 装备英灵
  ROLE_LV,                    // 角色等级
  PASS_PLAYING,               // 通关副本
  PROTECT_TASK,               // 保护任务
  ANSWER_TASK,                // 答题任务
  SPORTS_TASK_1,              // 竞技任务
  SPORTS_TASK_2,              // 竞技任务
  SOUL_APPEAR,                // 幻化
  EQUIPMENT,                  // 穿装备
  CONSTELLATION_UP,           // 星辰之力
  SUMMON_NPC,                 // 召唤NPC任务
  GET_RESOURCE,               // 获得资源
  SPEND_RESOURCE,             // 消费资源
  ROB,                        // 掠夺
  WATERING,                   // 浇水
  KILL_ACTOR,                 // 杀人
  KILL_MOB_LEVEL,             // 击杀等级范围内的NPC
  KILL_MOB_TYPE,              // 击杀指定类型的NPC
  FINISH_TASK_TYPE,           // 完成指定类型的任务(循环任务)
  GET_VIP_PRIVILEGE,          // 获取VIP特权
  TRANSPORT,                  // 护送

  MAX,
}


//////////////////////////////////////////////
// 循环任务刷新类型
enum TaskCircleRefulshType {
  INVALID = 0,                // 无效
  REFULSH_ROUND,              // 刷新本轮循环任务，直接领取奖励(消耗)
  REFULSH_TASK_CONDITION,     // 刷新当前的本轮循环任务中得当前任务的条件(消耗)
  REFULSH_CIRCLE,             // 刷新当前的本轮循环任务中得当前环完成(消耗)
  REQUEST_FINISH_ROUND,       // 请求完成本轮任务
  REQUEST_FINISH_CIRCLE,      // 请求完成环
  REQUEST_OPEN_ROUND,         // 请求开启一轮循环任务
}

/////////////////////////////////////////////
// 任务类型
enum TaskType {
  INVALID = 0,
  TASK_MAIN,                  // 主线任务
  TASK_CIRCLE,                // 循环任务
  TASK_RISK,                  // 历险任务
  TASK_TRIP,                  // 奇遇任务
  TASK_DAILY,                 // 每日任务
  TASK_TRANSPORT,             // 护送任务
}

////////////////////////////////////////////
// 轮状态
enum TaskCircleRoundState {
  INVALID = 0,                // 无效
  ACTIVE = 1,                 // 活动
  FINISH = 2,                 // 完成
  PICKED_AWARD = 3,           // 已经领取奖励
} 

///////////////////////////////////////////
// 奇遇任务操作类型
enum TaskTripOptType {
  INVALID = 0,                // 无效的类型
  REQUEST_FINISH,             // 请求完成
  REQUEST_GIVE_UP,            // 请求放弃
  SPENT_FINISH,               // 花费完成
}


///////////////////////////////////////////
// 掠夺任务定义
enum RobTaskType {
  SUCCESS = 0,
  SUCCESS_AND_FAILED = 1,
}

// 浇水任务定义
enum WateringTaskType {
  SELF = 0,
  OTHER = 1,
  SELF_AND_OTHER = 2,
}


///////////////////////////////////////////
// 护送任务状态
enum TaskTransportStatus {
  MIN = 0,

  IDOL,                       // 空闲中
  TRANSPORT,                  // 护送中
  FAILURE,                    // 护送失败

  MAX,
}

