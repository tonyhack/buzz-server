namespace cpp entity
namespace py ccentity.constants
namespace as3 com.dreamfire.sanguo.data.definition.entity


// 八方向坐偏移(下标见direction.thrift)
const list<i32> kDirectionOffsetX = [
  1, -1,  0,  0,  1, -1,  1, -1,
]

const list<i32> kDirectionOffsetY = [
  0,  0,  1, -1, -1, -1,  1,  1,
]


// 装备随机属性最大个数
const i32 kMaxRandomAttriNum = 6;

// 邮件标题最大长度
const i32 kMaxMailTitleLen = 120;

// 循环任务开启等级
const i32 kMaxCircleLimitLevel = 20;

// 循环任务等级段限制
const i32 kMaxCirclelevelStage = 5;

// 循环任务开始等级段
const i32 kMaxCircleLevelStageStart = 3;

// 循环任务每轮环任务数量
const i32 kMaxCircleNum = 20;

// 奇遇任务等级段
const i32 kMaxTripLevelStageStart = 3;

// 奇遇任务等级段限制
const i32 kMaxTripLevelStage = 5;

// 道具快捷栏最大个数
const i32 kMaxItemShortcutSize = 3;

