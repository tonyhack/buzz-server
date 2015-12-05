#ifndef __GAME__SERVER__SCENE__ZAZEN__ACTOR__H
#define __GAME__SERVER__SCENE__ZAZEN__ACTOR__H

#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"
#include "core/base/types.h"
#include "entity/actor_types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace scene {

class SceneActor;

class ZazenActor : public core::Noncopyable,
  public core::TimerHandler {
  enum { TIMER_IMCOME = 0, };

 public:
  ZazenActor();
  virtual ~ZazenActor();

  bool Initialize(SceneActor *actor);
  void Finalize();

  bool StartTime();

  void StopTime();

  virtual void OnTimer(core::uint32 id);

  GameActor *GetGameActor();

 private:

  // 玩家对象
  SceneActor *actor_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__ZAZEN__ACTOR__H

