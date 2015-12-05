namespace cpp entity
namespace py ccentity.resource
namespace as3 com.dreamfire.sanguo.data.definition.entity

/////////////////////////////////
// 资源类型

enum ResourceType {
  MIN = 0,

  ITEM,                           // 道具
  RESOURCE,                       // 资源

  MAX,
}

enum ResourceID {
  MIN = 0,

  GOLD,                           // 金币
  EXP,                            // 经验
  RESTRICT_DOLLARS,               // 绑定币(礼券)
  NIMBUS,                         // 灵气
  WAR_SOUL,                       // 战魂(神威)
  FREEDOM_DOLLARS,                // 非绑定币(钻石)
  GEM_SHARD,                      // 宝石碎片
  HONOUR,                         // 荣誉(军功)
  JUSTICE,                        // 正义点
  MIRACLE_INTEGRAL,               // 奇迹积分
  LEGEND_INTEGRAL,                // 传奇积分
  ARENA_INTEGRAL,                 // 竞技场积分
  GUILD_CONTRIBUTION              // 公会贡献值

  MAX,
}

