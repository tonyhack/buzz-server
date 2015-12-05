namespace cpp entity
namespace py ccentity.camp
namespace as3 com.dreamfire.sanguo.data.definition.entity

//////////////////////////////////
// 阵营类型
enum CampType {
  TYPE_NONE = 1,                  // 无阵营
  TYPE_NPC_FIELD,                 // 野外NPC
  TYPE_MOSTER_FIELD,              // 野外怪物
  TYPE_ACTOR_A_ARENA,             // 竞技场A方玩家
  TYPE_ACTOR_B_ARENA,             // 竞技场B方玩家
  TYPE_ACTOR_A_PVP,               // PVP副本A方玩家
  TYPE_NPC_A_PVP,                 // PVP副本A方NPC
  TYPE_ACTOR_B_PVP,               // PVP副本B方玩家
  TYPE_NPC_B_PVP,                 // PVP副本B方NPC

  TYPE_EXTENTION = 20,            // 定义类型扩展
}

////////////////////////////////
// 阵营关系类型
enum CampRelationType {
  MIN = 0,
  HOSTLITY,                       // 敌对
  FRIENDLY,                       // 友好
  NEUTRAL,                        // 中立
  NONE,                           // 无
  MAX,
}

