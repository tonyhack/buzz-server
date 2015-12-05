//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-28 18:29:44.
// File name: game_entity.cc
//
// Description:
// Define class GameEntity.
//

#include "game_server/server/game_entity.h"

namespace game {

namespace server {

GameEntity::GameEntity() {}
GameEntity::~GameEntity() {}

void GameEntity::Allocate() {
  GameObject::Allocate();
  this->client_attribute_.resize(entity::EntityClientFields::CLIENT_END, 0);
  this->server_attribute_.resize(entity::EntityServerFields::SERVER_END, 0);
}

void GameEntity::Deallocate() {
  GameObject::Deallocate();
}

void GameEntity::Initialize() {
  GameObject::Initialize();
}

void GameEntity::Finalize() {
  GameObject::Finalize();
  this->SetID(0);
  this->client_attribute_.assign(this->client_attribute_.size(), 0);
  this->server_attribute_.assign(this->server_attribute_.size(), 0);
}

core::int32 GameEntity::GetAttribute(entity::EntityClientFields::type type) const {
  if(type < entity::EntityClientFields::CLIENT_END) {
    return this->client_attribute_[type];
  }
  return 0;
}

bool GameEntity::SetAttribute(entity::EntityClientFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::EntityClientFields::CLIENT_END) {
    // 更改属性
    this->client_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    if(synchronize) {
      this->Synchronize(type);
    }
    return true;
  }
  return false;
}

core::int32 GameEntity::GetAttribute(entity::EntityServerFields::type type) const {
  if(type < entity::EntityServerFields::SERVER_END) {
    return this->server_attribute_[type];
  }
  return 0;
}

bool GameEntity::SetAttribute(entity::EntityServerFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::EntityServerFields::SERVER_END) {
    // 更改属性
    this->server_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    return true;
  }
  return false;
}

void GameEntity::ReviseAttribute(entity::EntityClientFields::type type) {
  static AttributeReviseFunctor functions[entity::EntityClientFields::CLIENT_END] = {
  };
  // 修正
  if(functions[type]) {
    functions[type](this->client_attribute_[type]);
  }
}

void GameEntity::ReviseAttribute(entity::EntityServerFields::type type) {
  static AttributeReviseFunctor functions[entity::EntityServerFields::SERVER_END] = {
  };
  // 修正
  if(functions[type]) {
    functions[type](this->server_attribute_[type]);
  }
}

}  // namespace server

}  // namespace game

