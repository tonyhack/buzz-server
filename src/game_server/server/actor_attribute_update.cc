//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-03 19:20:09.
// File name: actor_attribute_update.cc
//
// Description:
// Define class ActorAttributeUpdate.
//

#include "game_server/server/actor_attribute_update.h"

namespace game {

namespace server {

ActorAttributeUpdate::ActorAttributeUpdate(GameActor *actor) : actor_(actor) {}

ActorAttributeUpdate::~ActorAttributeUpdate() {}

bool ActorAttributeUpdate::Initialize() {
  return true;
}

void ActorAttributeUpdate::Finalize() {}

void ActorAttributeUpdate::UpdateAll(bool synchronize) {
}

void ActorAttributeUpdate::Update(entity::ActorAoiFields::type type, bool synchronize) {
  switch(type) {
    default:
      break;
  }
}

void ActorAttributeUpdate::Update(entity::ActorClientFields::type type,
    bool synchronize) {
  switch(type) {
    default:
      break;
  }
}

void ActorAttributeUpdate::Update(entity::ActorServerFields::type type,
    bool synchronize) {
  switch(type) {
    default:
      break;
  }
}

}  // namespace server

}  // namespace game

