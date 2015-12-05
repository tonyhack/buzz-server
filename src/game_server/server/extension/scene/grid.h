//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 14:39:47.
// File name: grid.h
//
// Description:
// Define class Grid.
//

#ifndef __GAME__SERVER__SCENE__GRID__H
#define __GAME__SERVER__SCENE__GRID__H

#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/extension/scene/block_type.h"

namespace game {

namespace server {

class GameEntity;

namespace scene {

// 描述地图上的一个格子，包含了格子的坐标信息、阻挡信息以及
// 在本格子上的生物列表
class Grid : public core::Noncopyable {
 public:
  typedef std::vector<GameEntity *> EntityVector;

  Grid();
  ~Grid();

  bool Initialize();
  void Finalize();

  // 加入实体
  void AddEntity(GameEntity *entity);

  // 删除实体
  void RemoveEntity(GameEntity *entity);

  inline void SetBlockMask(BlockType::type type) {
    this->block_mask_ = type;
  }

  // 是否可以增加道具
  inline bool CanAddItem() const { return this->CanWalk() && this->items_.size() == 0; }
  // 玩家是否可以行走
  inline bool CanWalk() const { return (this->block_mask_ & BlockType::BLOCK_WALK) == 0; }
  // 是否可以攻击
  inline bool IsSafe() const { return (this->block_mask_ & BlockType::BLOCK_SAFE) != 0; }
  // NPC是否可以行走
  inline bool CanNpcWalk() const {
    if(this->CanWalk() && this->npcs_.size() == 0) {
      return true;
    }
    return false;
  }

  // 得到当前格子上玩家的数量
  inline bool GetActorNumber() const {
    return this->actors_.size();
  }
  // 得到当前格子上的所有玩家
  inline const EntityVector &GetActors() const {
    return this->actors_;
  }

  // 得到当前格子上 npc 的数量
  inline bool GetNpcNumber() const {
    return this->npcs_.size();
  }
  // 得到当前格子上的所有 npc
  inline const EntityVector &GetNpcs() const {
    return this->npcs_;
  }

  // 得到当前格子上道具的数量
  inline bool GetItemNumber() const {
    return this->items_.size();
  }
  // 得到当前格子上的所有的道具
  inline const EntityVector &GetItems() const {
    return this->items_;
  }

 private:
  // 玩家加入格子中
  void AddActor(GameEntity *entity);
  // 从格子中把玩家删除
  void RemoveActor(GameEntity *entity);

  // npc 加入格子
  void AddNpc(GameEntity *entity);
  // 从格子中把 npc 删除
  void RemoveNpc(GameEntity *entity);

  // 道具加入格子
  void AddItem(GameEntity *entity);
  // 从格子中把道具删除
  void RemoveItem(GameEntity *entity);

  // 玩家列表
  // TODO: 这是个可以做内存优化的地方，因为大量生物在聚集的时候
  // 可能是很多个生物短暂地站在一个格子中，这种格子特别多的时候
  // 会占用大量的内存，优化方向就是按一定的算法缩减内存，应该封
  // 装一个类来处理。
  EntityVector actors_;
  // NPC 列表
  EntityVector npcs_;
  // 场景道具列表
  EntityVector items_;

  // 阻挡码
  core::uint16 block_mask_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__GRID__H

