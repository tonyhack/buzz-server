//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 16:22:51.
// File name: screen.h
//
// Description:
// Define class Screen.
//

#ifndef __GAME__SERVER__SCENE__SCREEN__H
#define __GAME__SERVER__SCENE__SCREEN__H

#include <cstddef>
#include <set>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

class GameEntity;

namespace scene {

class Screen : public core::Noncopyable {
 public:
  typedef std::set<GameEntity *> EntitySet;

  Screen();
  ~Screen();

  bool Initialize();
  void Finalize();

  bool Add(GameEntity *entity);
  bool Remove(GameEntity *entity);

  inline const EntitySet &GetActors() const {
    return this->actors_;
  }
  inline const EntitySet &GetNpcs() const {
    return this->npcs_;
  }
  inline const EntitySet &GetItems() const {
    return this->items_;
  }

  // 得到除 except以外的所有实体
  bool GetEntitysExcept(const GameEntity *except, GameEntity *entitys[], size_t &number);

 private:
  EntitySet actors_;
  EntitySet npcs_;
  EntitySet items_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCREEN__H

