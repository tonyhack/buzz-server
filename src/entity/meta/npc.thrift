namespace cpp entity
namespace py ccentity.npc
namespace as3 com.dreamfire.sanguo.data.definition.entity


// npc 类型
enum NpcKind {
  MIN = 0,

  FUNCTION,                         // 功能类
  MONSTER,                          // 怪物类
  TELEPORTER,                       // 传送门类
  TASK,                             // 任务类
  TASK_GATHER,                      // 任务采集
  GATHER,                           // 普通采集
  TRANSPORT,                        // 护送类NPC

  MAX,
}


// npc 归属类型
enum NpcOwnerType {
  MIN = 0,

  ALL = 0,
  KILL,                             // 击杀归属
  DAMAGE,                           // 伤害归属
  CAMP_COMPETE,                     // 同阵营竞争归属
  CAMP_EACH,                        // 每个人都有

  MAX,
}


enum NpcAoiFields {
  AOI_BEGIN = 0,

  AOI_END,
}

enum NpcClientFields {
  CLIENT_BEGIN = 0,

  CLIENT_END,
}

enum NpcMobType {
  GENERAL_NPC = 0,                  // 普通怪
  CREAM_NPC,                        // 精英怪
  BOSS_NPC,                         // 世界BOSS
}

struct NpcAoiField {
  1 : required NpcAoiFields field_;
  2 : required i32 value_;
}

// npc的快照数据
struct NpcSnapshotField {
  1 : required i32 template_id_;    // npc模板ID
  2 : required i32 current_hp_;     // 当前生命值
  3 : required i32 current_mp_;     // 当前魔法值
  4 : required binary buff_;        // BUFF数据
  5 : required binary transport_;   // 护送数据
}

