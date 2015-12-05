//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-13 10:58:17.
// File name: game_object.cc
//
// Description:
// Define class GameObject.
//

#include "game_server/server/game_object.h"

namespace game {

namespace server {

GameObject::GameObject() {}
GameObject::~GameObject() {}

void GameObject::Allocate() {
  this->id_ = 0;
}
void GameObject::Deallocate() {}

void GameObject::Initialize() {}

void GameObject::Finalize() {}

void GameObject::NonNegativeRevise(core::int32 &value) {
  if(value < 0) {
    value = 0;
  }
}

void GameObject::NonNegativePercentRevise(core::int32 &value) {
  if(value < 0) {
    value = 0;
  } else if(value > 10000) {
    value = 10000;
  }
}

void GameObject::PercentRevise(core::int32 &value) {
  if(value < -10000) {
    value = -10000;
  } else if(value > 10000) {
    value = 10000;
  }
}

}  // namespace server

}  // namespace game

