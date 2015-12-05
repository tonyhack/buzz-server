//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 16:39:44.
// File name: scene.h
//
// Description:
// Define class Scene.
//

#ifndef __GAME__SERVER__SCENE__SCENE__H
#define __GAME__SERVER__SCENE__SCENE__H

#include <ext/hash_map>
#include <string>

#include "core/base/types.h"
#include "entity/direction_types.h"
#include "game_server/protocol/game_protocol_types.h"
#include "game_server/server/extension/scene/entity_builder.h"
#include "game_server/server/extension/scene/grid_map.h"
#include "game_server/server/extension/scene/scene_daynight.h"
#include "game_server/server/extension/scene/scene_router.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "game_server/server/game_server.h"
#include "global/global_packet.h"
#include "global/game_misc.h"

namespace game {

namespace server {

class GameEntity;
class GameActor;

namespace scene {

class SceneActor;
class Screen;

class Scene {
  enum SceneAttrType{ 
    ROOKIE_PROTECT = 0x1,       // 新手保护
    BAN_CHANGE_PK_MODE = 0x2,   // 禁止切换PK
    DIE_PROTECT = 0x4,          // 死亡保护时间
    PUNISH_FOR_PK = 0x8,        // PK惩罚
    REVIVE_STAY = 0x10,         // 原地复活
  };

 public:
  typedef __gnu_cxx::hash_map<core::uint64, SceneActor *> ActorHashmap;

  Scene();
  ~Scene();

  bool Allocate(core::uint16 map_id);
  void Deallocate();

  bool Initialize(core::uint64 scene_id, bool dynamic);
  void Finalize();

  inline core::uint16 GetMapID() const {
    return this->map_id_;
  }
  inline core::uint64 GetSceneID() const {
    return this->scene_id_;
  }

  inline const ActorHashmap &GetActors() const {
    return this->actors_;
  }
  inline size_t GetActorSize() const {
    return this->actors_.size();
  }

  // 是否新手保护
  bool IsRookieProtect() const { return (this->map_attr_mask_ & ROOKIE_PROTECT); }
  // 是否禁止切换PK模式
  bool IsBanChangePKMode() const { return (this->map_attr_mask_ & BAN_CHANGE_PK_MODE); }
  // 是否死亡保护
  bool IsDieProtect() const { return (this->map_attr_mask_ & DIE_PROTECT); }
  // 是否PK惩罚
  bool IsPunishForPK() const { return (this->map_attr_mask_ & PUNISH_FOR_PK); }
  // 是否可以原地复活
  bool IsReviveStay() const{ return (this->map_attr_mask_ & REVIVE_STAY); }

  // 该场景是否成功登陆到会话服务器
  inline bool CheckLogined() const {
    return this->logined_;
  }

  // 设置场景成功登陆到会话服务器
  inline void SetLogined() {
    this->logined_ = true;
  }

  // 动态场景
  inline bool CheckDynamic() const {
    return this->dynamic_;
  }

  // 清除场景中的NPC
  void ClearNpcs(core::int32 delay_secs_ = 0, bool force_clear_reborn = false);

  // 找一个附近的非阻挡点
  bool GetNearWalkPosition(core::uint32 &x, core::uint32 &y,
      core::uint32 radius = Scene::kMaxRandomPositionCount_);
  // 找一个附近的NPC非阻挡点
  bool GetNearNpcWalkPosition(core::uint32 &x, core::uint32 &y,
      core::uint32 radius = Scene::kMaxRandomPositionCount_);

  // 增加/删除一个实体角色
  bool AddEntity(GameEntity *entity, bool login = true);
  bool RemoveEntity(GameEntity *entity);

  // 坐标是否可以加入场景道具
  bool CheckAddSceneItem(core::uint32 x, core::uint32 y) const;

  // 坐标是否有障碍
  bool CheckWalkBlock(core::uint32 x, core::uint32 y) const;
  // 坐标是否有NPC障碍
  bool CheckNpcWalkBlock(core::uint32 x, core::uint32 y) const;
  
  // 坐标是否安全区
  bool CheckSafeArea(core::uint32 x, core::uint32 y) const;

  // 静态地图跳转
  bool JumpMap(GameEntity *entity, core::uint16 dest_map, core::uint32 dest_x,
      core::uint32 dest_y);

  // 场景跳转
  bool JumpScene(GameEntity *entity, core::uint64 dest_scene, core::uint32 dest_x,
      core::uint32 dest_y);

  // 直接跳转(非移动指令驱动)
  bool Jump(GameEntity *entity, core::uint32 dest_x, core::uint32 dest_y,
      core::uint8 grid_offs_x = core::kuint8max, core::uint8 grid_offs_y = core::kuint8max);

  // 普通移动(移动指令驱动)
  bool Walk(GameEntity *entity, entity::DirectionType::type dir,
      core::uint8 offset_x = core::kuint8max, core::uint8 offset_y = core::kuint8max);

  // 重新定位
  bool Relocation(GameEntity *entity, core::uint32 x, core::uint32 y, bool broadcast = false,
      core::uint8 offset_x = core::kuint8max, core::uint8 offset_y = core::kuint8max);

  // 获取坐标点所在玩家的集合
  bool GetNineScreenActors(core::uint32 x, core::uint32 y, core::uint64 actors[], size_t &number);

  // 得到场景玩家对象
  SceneActor *GetActor(core::uint64 actor);

  // 寻路接口
  bool Router(const RouterCoord &src, const RouterCoord &dest,
      size_t max_path_size, core::int32 dest_radius);
  // 得到上次寻路路径
  const entity::DirectionType::type *GetRouterPaths(size_t &size);
  // 寻路结果是否需要反转
  inline bool CheckReverse() const {
    return this->router_.CheckReverse();
  }
  // 方向反转
  inline entity::DirectionType::type ReverseDirection(
      entity::DirectionType::type dir) const {
    return this->router_.ReverseDirection(dir);
  }

  // 寻路接口
  bool Router(const RouterCoord &src, const RouterCoord &dest,
      entity::DirectionType::type paths[], size_t &number, size_t max_path_size, 
      core::int32 dest_radius);

  // 根据坐标得到一个格子
  const Grid *GetGrid(core::uint32 x, core::uint32 y) const {
    return this->grids_.Get(x, y);
  }

  // 得到实体创建器
  EntityBuilder &GetEntityBuilder() {
    return this->entity_builder_;
  }

  // 得到昼夜系统
  SceneDayNight &GetSceneDayNight() {
    return this->daynight_;
  }

  // 检测PK条件
  request::PKCheckResultType::type PKCheck(GameActor *src_actor, GameActor *dest_actor); 

  // 广播昼夜转换
  void BroadcastDayNightChange();

  // 获取最大 x\y
  core::uint32 GetMaxX() const {
    return this->max_x_;
  }
  core::uint32 GetMaxY() const {
    return this->max_y_;
  }
  void SetWorldBossID(core::uint64 id) {
    this->world_boss_id_ = id;
  }
  core::uint64 GetWorldBossID() const {
    return this->world_boss_id_;
  }

  void GetNpcs(std::vector<core::int64> &npcs) const;

 private:
  // 玩家进入场景后广播给周围9屏
  void EnterSceneToNine(GameEntity *entity);

  // 玩家离开场景后广播给周围9屏
  void ExitSceneToNine(GameEntity *entity);

  // 同步周围9屏给当前玩家(玩家进入后同步创建周围实体给场景)
  void SynchronizeCreateNineScreen(GameActor *actor);

  // 删除周围9屏到当前玩家(玩家离开时同步销毁周围实体给场景)
  void SynchronizeDestoryNineScreen(GameActor *actor);

  // 加入/删除实体到屏
  bool AddScreen(GameEntity *entity);
  bool RemoveScreen(GameEntity *entity);

  // 加入/删除实体到格子
  bool AddGrid(GameEntity *entity, core::uint32 dest_x, core::uint32 dest_y);
  bool RemoveGrid(GameEntity *entity, core::uint32 src_x, core::uint32 src_y);

  // 是否可以加入一个实体角色到目标格子
  bool CheckAddGrid(GameEntity *entity, core::uint32 dest_x, core::uint32 dest_y);

  // 根据坐标得到屏
  Screen *GetPositionScreen(core::uint32 x, core::uint32 y);
  // 根据屏坐标得到屏
  Screen *GetScreen(core::uint32 screen_x, core::uint32 screen_y);

  // 检测x/y是否合法
  bool CheckPositionValid(core::uint32 x, core::uint32 y) const;

  // 根据坐标得到九屏
  void GetNineScreen(core::uint32 screen_x, core::uint32 screen_y,
      Screen *screens[], size_t &screen_number);

  // 广播实体创建
  void BroadcastCreate(GameEntity *entity, core::uint64 actors[], size_t number);

  // 广播实体退出
  void BroadcastDestory(GameEntity *entity, core::uint64 actors[], size_t number);

  // 广播角色靠近事件
  void BroadcastComingEvent(GameEntity *entity, GameEntity *entitys[], size_t number);

  // 广播角色远离事件
  void BroadcastLeavingEvent(GameEntity *entity, GameEntity *entitys[], size_t number);

  // 进入场景发送靠近事件
  void EnterSceneComingEvent(GameEntity *entity);

  // 离开场景发送远离事件
  void ExitSceneLeavingEvent(GameEntity *entity);

  // 广播实体移动
  void BroadcastMove(GameEntity *entity, entity::DirectionType::type dir,
      core::uint64 actors[], size_t number);

  // 发送需要创建的实体集
  void SynchronizeCreate(GameEntity *entitys[], size_t number, GameActor *actor);

  // 发送需要销毁的实体集
  void SynchronizeDestory(GameEntity *entitys[], size_t number, GameActor *actor);

  // 增加/删除场景玩家
  bool AddSceneActor(SceneActor *actor);
  SceneActor *RemoveSceneActor(core::uint64 id);
  // 取得场景玩家
  SceneActor *GetSceneActor(core::uint64 id);

  static const core::uint32 kMaxRandomPositionCount_ = 20;

  // 场景中所有坐标格子信息
  GridMap grids_;
  // 屏索引数组
  Screen *screens_;

  // 地图ID
  core::uint32 map_id_;
  // 场景ID
  core::uint64 scene_id_;

  // 最大x/y坐标
  core::uint32 max_x_;
  core::uint32 max_y_;

  // 屏的最大x/y坐标
  core::uint32 screen_max_x_;
  core::uint32 screen_max_y_;

  // 屏数组的大小
  core::uint32 screen_length_;

  // 玩家角色列表
  ActorHashmap actors_;

  // 实体创建器
  EntityBuilder entity_builder_;

  // 昼夜系统
  SceneDayNight daynight_;

  // 寻路
  SceneRouter<Scene> router_;

  // 地图属性码
  core::uint16 map_attr_mask_;

  // 是否登陆成功
  bool logined_;

  // 是否是动态场景
  bool dynamic_;

  // 场景世界boss
  core::uint64 world_boss_id_;

  // 屏宽
  size_t screen_width_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__H


