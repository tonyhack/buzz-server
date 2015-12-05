include "direction.thrift"
include "entity.thrift"
include "game_request.thrift"

namespace cpp game.server.request
namespace py ccrequest.scene

////////////////////////////////////////////////
// 请求获取周围玩家列表
// type: REQUEST_SCENE_GET_AROUND_ACTORS
struct RequestSceneGetAroundActors {
  1 : required entity.EntityType type_;
  2 : required i64 id_;
  3 : required list<i64> actors_;
}


////////////////////////////////////////////////
// 请求召唤NPC
// type: REQUEST_SCENE_SUMMON_NPC
struct RequestSceneSummonNpc {
  1 : required i64 scene_;                    // 场景ID       (IN)
  2 : required i32 template_id_;              // npc模板ID    (IN)
  3 : required i32 x_;                        // 坐标x        (IN)
  4 : required i32 y_;                        // 坐标y        (IN)
  5 : optional i32 width_;                    // 区域宽       (IN)
  6 : optional i32 heigth_;                   // 区域高       (IN)
  7 : required direction.DirectionType dir_;  // 面向         (IN)
  8 : required i32 delay_secs_ = 0;           // 延迟秒数     (IN)
  9 : required i64 npc_id_;                   // NPC ID       (OUT)
}


////////////////////////////////////////////////
// 请求清除NPC
// type: REQUEST_SCENE_CLEAR_NPC
struct RequestSceneClearNpc {
  1 : required i64 id_;                       // ID
  2 : optional i32 delay_secs_;               // 延迟删除(秒)
}


////////////////////////////////////////////////
enum AreaType {
  AREA_CIRCLE = 0,                            // 圆形
  AREA_SECTOR_QUARTER,                        // 90度扇形
  AREA_RAY,                                   // 射线

  AREA_MAX,
}

enum SectorRadianType {
  MIN = 0,

  QUARTER = 0,                                // 90度
  HALF,                                       // 180度

  MAX,
}

struct CircleAreaCondition {
  1 : required byte radius_;                  // 半径
}
struct SectorAreaCondition {
  1 : required byte radius_;                  // 半径
  2 : required direction.DirectionType dir_;  // 方向
  3 : required SectorRadianType radian_;      // 弧度
}
struct RayAreaCondition {
  1 : required byte width_;                   // 宽度
  2 : required byte length_;                  // 长度
  3 : required i16 direction_;                // 方向(0~360)
}

struct AreaRolePair {
  1 : required entity.EntityType type_;       // 类型
  2 : required i64 id_;                       // ID
}

// 获取类型
enum SceneEntityType {
  ACTOR = 0x1;
  NPC = 0x2;
}


////////////////////////////////////////////////
// 选择场景上的目标集
// type: REQUEST_SCENE_SELECT_TARGETS
enum SelectMaskType {
  // 是否可以是自己
  SELF = 0x00000001,                              // 自己
  NOT_SELF = 0x00000002,                          // 非自己

  // 对象类型
  NPC = 0x00000010,                               // NPC
  ACTOR = 0x00000020,                             // 玩家

  // 基本状态
  ALIVE = 0x00000100,                             // 活人
  DEAD = 0x00000200,                              // 死人

  // 阵营
  CAMP_ENEMY = 0x00001000,                        // 敌方
  CAMP_FRIEND = 0x00002000,                       // 友方
  CAMP_NEUTRAL = 0x00004000,                      // 中立
  CAMP_NONE = 0x00008000,                         // 无

  // 技能作用类型
  TEAM = 0x00010000,                              // 同队
  OTHER_TEAM = 0x00020000,                        // 非本队

  // PK检测
  PK_CHECK = 0x00100000,                          // pk检测
}

struct RequestSceneSelectTargets {
  1 : required entity.EntityType type_;           // 实体类型
  2 : required i64 id_;                           // 实体ID
  3 : required AreaType area_type_;               // 区域类型
  4 : optional CircleAreaCondition cond_circle_;  // 圆形条件
  5 : optional SectorAreaCondition cond_sector_;  // 扇形条件
  6 : required i32 x_;                            // 坐标x
  7 : required i32 y_;                            // 坐标y
  8 : required i32 select_mask_;                  // 筛选掩码
  9 : required i32 max_count_;                    // 最大个数
  10: required list<AreaRolePair> targets_;       // 目标列表
}


////////////////////////////////////////////////
enum MoveResultType {
  SUCCESS = 0,              // 成功
  FAILURE_BLOCK,            // 有阻挡
  FAILURE_ERROR,            // 错误
}

// 角色移动
// type: REQUEST_SCENE_ROLE_MOVE
struct RequestSceneRoleMove {
  1 : required entity.EntityType type_;           // 实体类型
  2 : required i64 id_;                           // ID
  3 : required direction.DirectionType dir_;      // 方向
  4 : required MoveResultType result_;            // 移动结果
}




////////////////////////////////////////////////
// 场景跳转
// type: REQUEST_SCENE_JUMP
struct RequestSceneJump {
  1 : required entity.EntityType type_;           // 实体类型
  2 : required i64 id_;                           // 实体ID
  3 : optional i32 map_;                          // 目标地图ID
  4 : optional i64 scene_;                        // 场景ID
  5 : required i32 x_;                            // 目标坐标x
  6 : required i32 y_;                            // 目标坐标y
  7 : required bool result_;                      // 结果
}



////////////////////////////////////////////////
// 重置坐标
// type: REQUEST_SCENE_RELOCATION
struct RequestSceneRelocation {
  1 : required entity.EntityType type_;           // 实体类型
  2 : required i64 id_;                           // 实体ID
  3 : required i32 x_;                            // 目标坐标x
  4 : required i32 y_;                            // 目标坐标y
  5 : required bool result_;                      // 结果
}


////////////////////////////////////////////////
enum DayNightStatusType {
  STATUS_MORNING = 0,       // 早上
  STATUS_DAYTIME,           // 白天
  STATUS_EVENING,           // 傍晚
  STATUS_NIGHT,             // 夜晚
}

// 获得场景昼夜信息
// type: REQUEST_SCENE_DAYNIGHT_INFO
struct RequestSceneDayNightInfo {
  1 : required i64 scene_;                        // 场景ID
  2 : required DayNightStatusType status_;        // 状态
  3 : required i32 aoi_coe_;                      // 视野
}




////////////////////////////////////////////////
// 寻路
// type: REQUEST_SCENE_ROUTER
struct RequestSceneRouter {
 1 : required i64 scene_;                             // 场景ID
 2 : required i32 src_x_;                             // 起点坐标x
 3 : required i32 src_y_;                             // 起点坐标y
 4 : required i32 dest_x_;                            // 终点坐标x
 5 : required i32 dest_y_;                            // 终点坐标y
 6 : required i32 dest_radius_;                       // 终点半径
 7 : required i16 max_path_;                          // 最大路径个数
 8 : required list<direction.DirectionType> paths_;   // 寻路结果
 9 : required bool result_;                           // 结果
}



///////////////////////////////////////////////
// 获得场景PK信息
// type: REQUEST_SCENE_PK_INFO
struct RequestScenePKInfo {
  1 : required i64 scene_;                        // 场景ID
  2 : required bool rookie_protect_;              // 是否新手保护
  3 : required bool ban_change_pk_mode_;          // 是否禁止切换pk模式
  4 : required bool die_protect_;                 // 是否死亡保护
}

//////////////////////////////////////////////
// 是否安全区
// type: REQUEST_SCENE_IS_SAFE
struct RequestSceneIsSafe {
  1 : required i64 scene_;                        // 场景ID
  2 : required i32 x_;                            // 坐标x
  3 : required i32 y_;                            // 坐标y
  4 : required bool is_safe_;                     // 是否安全区
}

/////////////////////////////////////////////
enum PKCheckResultType {
  SUCCESS = 0,                                   // 可以使用
  ERROR_SAFE_AREA,                               // 安全区域
  ERROR_PK_MODE,                                 // PK模式错误
  ERROR_ROOKIE_PROTECT,                          // 新手保护
  ERROR_DIE_PROTECT_TIME,                        // 死亡保护时间
  ERROR_ARGS,                                    // 参数错误
}

// 检测PK条件
// type: REQUEST_SCENE_PK_CHECK
struct RequestScenePKCheck { 
  1 : required i64 src_id_;                       // 技能源玩家
  2 : required i64 dest_id_;                      // 目标玩家
  3 : required PKCheckResultType result_;         // 检测返回
}




/////////////////////////////////////////////
// 请求场景玩家广播
// type: REQUEST_SCENE_ACTOR_BROADCAST
struct RequestSceneActorBroadcast {
  1 : required i32 type_;                         // 类型
  2 : required binary message_;                   // 消息二进制
  3 : required i64 scene_;                        // 场景ID
}



/////////////////////////////////////////////
// 请求创建场景
// type: REQUEST_SCENE_CREATE_MAP
struct RequestSceneCreateMap {
  1 : required i32 map_;                          // 地图ID
  2 : required i64 scene_;                        // 场景ID
}


/////////////////////////////////////////////
// 请求销毁场景
// type: REQUEST_SCENE_DESTORY_MAP
struct RequestSceneDestoryMap {
  1 : required i64 scene_;                        // 场景ID
  2 : required bool result_;                      // 结果
}


/////////////////////////////////////////////
// 清除场景中所有的NPC
// type: REQUEST_SCENE_CLEAR_ALL_NPC
struct RequestSceneClearAllNpc {
  1 : required i64 scene_;                        // 场景ID
  2 : required i32 delay_secs_ = 0;               // 延迟秒数
  3 : required bool force_clear_reborn_ = false;  // 强制清楚在重生或延迟刷新的NPC
}


/////////////////////////////////////////////
// 请求复活
// type: REQUEST_SCENE_REVIVE
struct RequestSceneRevive {
  1 : required i64 actor_;                        // 玩家ID
  2 : required bool stay_revive_;                 // 是否原地复活
}


/////////////////////////////////////////////
// 设置禁止掉落
// type: REQUEST_SCENE_FORBID_DROP_ITEM
struct RequestSceneForbidDropItem {
  1 : required list<i64> npcs_;                   // NPC列表
  2 : required bool forbid_;                      // 是否禁止
}


/////////////////////////////////////////////
// 获取场景所有NPC
// type: REQUEST_SCENE_GET_NPCS
struct RequestSceneGetNpcs {
  1 : required i64 scene_;                        // 场景ID
  2 : required list<i64> npcs_;                   // NPC列表
}

