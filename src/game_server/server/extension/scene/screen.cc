//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 16:29:46.
// File name: screen.cc
//
// Description:
// Define class Screen.
//

#include "game_server/server/extension/scene/screen.h"

#include "game_server/server/game_entity.h"

namespace game {

namespace server {

namespace scene {

Screen::Screen() {}
Screen::~Screen() {}

bool Screen::Initialize() {
  this->actors_.clear();
  this->npcs_.clear();
  return true;
}

void Screen::Finalize() {
  this->actors_.clear();
  this->npcs_.clear();
}

bool Screen::Add(GameEntity *entity) {
  if(entity == NULL) {
    return false;
  }

  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    EntitySet::iterator iterator = this->actors_.find(entity);
    if(iterator == this->actors_.end()) {
      this->actors_.insert(entity);
      return true;
    }
  } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
    EntitySet::iterator iterator = this->npcs_.find(entity);
    if(iterator == this->npcs_.end()) {
      this->npcs_.insert(entity);
      return true;
    }
  } else if(entity->GetType() == entity::EntityType::TYPE_SCENE_ITEM) {
    EntitySet::iterator iterator = this->items_.find(entity);
    if(iterator == this->items_.end()) {
      this->items_.insert(entity);
      return true;
    }
  }

  return false;
}

bool Screen::Remove(GameEntity *entity) {
  if(entity == NULL) {
    return false;
  }

  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    EntitySet::iterator iterator = this->actors_.find(entity);
    if(iterator != this->actors_.end()) {
      this->actors_.erase(entity);
      return true;
    }
  } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
    EntitySet::iterator iterator = this->npcs_.find(entity);
    if(iterator != this->npcs_.end()) {
      this->npcs_.erase(entity);
      return true;
    }
  } else if(entity->GetType() == entity::EntityType::TYPE_SCENE_ITEM) {
    EntitySet::iterator iterator = this->items_.find(entity);
    if(iterator != this->items_.end()) {
      this->items_.erase(entity);
      return true;
    }
  }

  return false;
}

bool Screen::GetEntitysExcept(const GameEntity *except, GameEntity *entitys[], size_t &number) {
  if(except == NULL) {
    return false;
  }

  size_t total_number = number;
  number = 0;

  // 玩家
  EntitySet::iterator iterator1 = this->actors_.begin();
  for(; iterator1 != this->actors_.end(); ++iterator1) {
    if(*iterator1 && (except->GetType() != entity::EntityType::TYPE_ACTOR ||
          (*iterator1)->GetID() != except->GetID())) {
      if(number >= total_number) {
        return false;
      }
      entitys[number++] = *iterator1;
    }
  }

  // NPC
  EntitySet::iterator iterator2 = this->npcs_.begin();
  for(; iterator2 != this->npcs_.end(); ++iterator2) {
    if(*iterator2 && (except->GetType() != entity::EntityType::TYPE_NPC ||
          (*iterator2)->GetID() != except->GetID())) {
      if(number >= total_number) {
        return false;
      }
      entitys[number++] = *iterator2;
    }
  }

  // 场景道具
  EntitySet::iterator iterator3 = this->items_.begin();
  for(; iterator3 != this->items_.end(); ++iterator3) {
    if(*iterator3 && (except->GetType() != entity::EntityType::TYPE_SCENE_ITEM ||
          (*iterator3)->GetID() != except->GetID())) {
      if(number >= total_number) {
        return false;
      }
      entitys[number++] = *iterator3;
    }
  }

  return true;
}

}  // namespace scene

}  // namespace server

}  // namespace game

