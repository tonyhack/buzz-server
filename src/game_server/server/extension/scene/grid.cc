//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 15:15:27.
// File name: grid.cc
//
// Description:
// Define class Grid.
//

#include "game_server/server/extension/scene/grid.h"

#include "game_server/server/game_entity.h"

namespace game {

namespace server {

namespace scene {

Grid::Grid() : block_mask_(0) {}
Grid::~Grid() {}

bool Grid::Initialize() {
  this->actors_.clear();
  this->npcs_.clear();
  this->items_.clear();
  return true;
}

void Grid::Finalize() {
  this->actors_.clear();
  this->npcs_.clear();
  this->items_.clear();
}

void Grid::AddEntity(GameEntity *entity) {
  if(entity == NULL) {
    return ;
  }

  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    this->AddActor(entity);
  } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
    this->AddNpc(entity);
  } else if(entity->GetType() == entity::EntityType::TYPE_SCENE_ITEM) {
    this->AddItem(entity);
  }
}

void Grid::RemoveEntity(GameEntity *entity) {
  if(entity == NULL) {
    return ;
  }

  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    this->RemoveActor(entity);
  } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
    this->RemoveNpc(entity);
  } else if(entity->GetType() == entity::EntityType::TYPE_SCENE_ITEM) {
    this->RemoveItem(entity);
  }
}

void Grid::AddActor(GameEntity *entity) {
  if(entity == NULL) {
    return ;
  }

  this->actors_.push_back(entity);
}

void Grid::RemoveActor(GameEntity *entity) {
  if(entity == NULL) {
    return ;
  }

  EntityVector::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    if(*iterator == entity) {
      this->actors_.erase(iterator);
      break;
    }
  }
}

void Grid::AddNpc(GameEntity *entity) {
  this->npcs_.push_back(entity);
}

void Grid::RemoveNpc(GameEntity *entity) {
  EntityVector::iterator iterator = this->npcs_.begin();
  for(; iterator != this->npcs_.end(); ++iterator) {
    if(*iterator == entity) {
      this->npcs_.erase(iterator);
      break;
    }
  }
}

void Grid::AddItem(GameEntity *entity) {
  this->items_.push_back(entity);
}

void Grid::RemoveItem(GameEntity *entity) {
  EntityVector::iterator iterator = this->items_.begin();
  for(; iterator != this->items_.end(); ++iterator) {
    if(*iterator == entity) {
      this->items_.erase(iterator);
      break;
    }
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game

