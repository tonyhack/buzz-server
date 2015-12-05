//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-18 12:23:05.
// File name: entity_builder.h
//
// Description:
// Define class EntityBuilder.
//

#ifndef __GAME__SERVER__SCENE__ENTITY__BUILDER__H
#define __GAME__SERVER__SCENE__ENTITY__BUILDER__H

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "game_server/server/extension/scene/npc_builder.h"

namespace game {

namespace server {

namespace scene {

class Scene;

class EntityBuilder : public core::TimerHandler {
  enum { TIMER_ENTITY_SUMMON = 0, TIMER_ENTITY_DISAPPEAR, };
  enum { SUMMON_INTERVAL_TIME = 2000, DISAPPEAR_INTERVAL_TIME = 1000, };

 public:
  EntityBuilder(Scene *scene);
  virtual ~EntityBuilder();

  bool Initialize();
  void Finalize();

  virtual void OnTimer(core::uint32 id);

  // 召唤一个NPC(delay :0 指针返回NULL才失败)
  GameNpc *SummonNpc(core::uint32 template_id, core::uint32 x, core::uint32 y,
      core::int32 width, core::int32 heigth, entity::DirectionType::type dir, core::int32 delay = 0);

  // NPC死亡延时删除尸体
  bool DisappearNpc(core::uint64 id, core::uint32 disappear_delay);

  inline void ClearBatchBornNpc() {
    this->npc_builder_.ClearBatchBorn();
  }

 private:
  void OnBorn();
  void OnDisappear();

  Scene *scene_;

  NpcBuilder npc_builder_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__ENTITY__BUILDER__H

