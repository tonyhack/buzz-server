namespace cpp entity
namespace py ccentity.bar
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////
// 进度条打断事件
enum BarEventType {
  MIX,
  ACTOR_MOVE,               // 移动
  ACTOR_DIE,                // 死亡
  ACTOR_HURT,               // 受到伤害
  ACTOR_SKILL,              // 使用技能
  ACTOR_USE_ITEM,           // 使用道具
  MAX,
}


///////////////////////////////////
// 进度条类型
enum BarModuleType {
  BAR_INVALID,                // 无效的类型
  BAR_TYPE_GATHER,            // 采集
  BAR_TYPE_SPIRIT_TRANSFORMED,// 精灵变身
}

