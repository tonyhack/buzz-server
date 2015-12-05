//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-18 12:21:18.
// File name: npc_builder.h
//
// Description:
// Define class NpcBuilder.
//

#ifndef __GAME__SERVER__SCENE__NPC__BUILDER__H
#define __GAME__SERVER__SCENE__NPC__BUILDER__H

#include <time.h>

#include <map>
#include <set>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/direction_types.h"
#include "game_server/server/game_npc.h"

namespace game {

namespace server {

namespace scene {

class Scene;

class NpcBuildInfo {
 public:
  NpcBuildInfo()
    : template_id_(0), x_(0), y_(0),
    dir_(entity::DirectionType::MAX) {}

  core::uint32 template_id_;
  core::uint32 x_;
  core::uint32 y_;
  entity::DirectionType::type dir_;
};

class DisappearNpcInfo {
 public:
  DisappearNpcInfo() : id_(0), time_stamp_(0) {}

  bool operator <(const DisappearNpcInfo &other) const {
    if(this->time_stamp_ != other.time_stamp_) {
      return this->time_stamp_ < other.time_stamp_;
    } else {
      return this->id_ < other.id_;
    }
  }

  core::uint64 id_;
  time_t time_stamp_;
};

class NpcBuilder : public core::Noncopyable {
  typedef std::multimap<time_t, NpcBuildInfo> NpcMultimap;
  typedef std::set<DisappearNpcInfo> DisappearNpcSet;

 public:
  NpcBuilder();
  ~NpcBuilder();

  bool Initialize(Scene *scene);
  void Finalize();

  // 召唤一个NPC
  GameNpc* SummonNpc(core::uint32 template_id, core::uint32 x, core::uint32 y,
      entity::DirectionType::type dir, core::uint32 delay);

  // NPC死亡延时删除尸体
  bool DisappearNpc(core::uint64 id, core::uint32 disappear_delay);

  // 批量生成npc
  void BatchBorn();

  // 批量删除npc
  void BathchDisappear();
  
  // 传入坐标 X,Y 范围 width,height 得出结果 result_x,result_y
  bool RandCoordinate(core::int32 x, core::int32 y, core::int32 width,
      core::int32 heigth, core::int32 &result_x, core::int32 &result_y);

  // 清除批量生成
  void ClearBatchBorn();

 private:
  GameNpc *SummonNpcImmediate(core::uint32 template_id, core::uint32 x, core::uint32 y,
      entity::DirectionType::type dir);

  bool DisappearNpc(core::uint64 id);

  void AddNpc(const NpcBuildInfo &info, core::uint32 delay);

  void DestoryNpcEvent(core::uint64 id);

  Scene *scene_;

  NpcMultimap npcs_;
  DisappearNpcSet disappear_npcs_;

  static const size_t kMaxBatchNumber = 100;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__NPC__BUILDER__H

