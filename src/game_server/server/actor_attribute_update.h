//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-03 19:14:32.
// File name: actor_attribute_update.h
//
// Description:
// Define class ActorAttributeUpdate.
//

#ifndef __GAME__SERVER__ACTOR__ATTRIBUTE__UPDATE__H
#define __GAME__SERVER__ACTOR__ATTRIBUTE__UPDATE__H

#include "core/base/noncopyable.h"
#include "entity/actor_types.h"

namespace game {

namespace server {

class GameActor;

class ActorAttributeUpdate : public core::Noncopyable {
 public:
  ActorAttributeUpdate(GameActor *actor);
  ~ActorAttributeUpdate();

  bool Initialize();
  void Finalize();

  void UpdateAll(bool synchronize = false);

  void Update(entity::ActorAoiFields::type type, bool synchronize = false);
  void Update(entity::ActorClientFields::type type, bool synchronize = false);
  void Update(entity::ActorServerFields::type type, bool synchronize = false);

 private:
  GameActor *actor_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ACTOR__ATTRIBUTE__UPDATE__H

