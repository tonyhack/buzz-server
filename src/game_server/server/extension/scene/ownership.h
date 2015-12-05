//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-03 14:03:10.
// File name: ownership.h
//
// Description:
// Define class Ownership.
//

#ifndef __GAME__SERVER__SCENE__OWNERSHIP__H
#define __GAME__SERVER__SCENE__OWNERSHIP__H

#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/npc_types.h"
#include "game_server/server/event/game_event_types.h"

namespace game {

namespace server {

namespace scene {

class SceneNpc;

class Ownership : public core::Noncopyable {
 public:
  Ownership() {}
  virtual ~Ownership() {}

  virtual bool Initialize(SceneNpc *npc) {
    this->npc_ = npc;
    this->value_ = 0;
    return true;
  }
  virtual void Finalize() {
    this->npc_ = NULL;
    this->value_ = 0;
  }

  virtual core::uint64 GetValue() const {
    return this->value_;
  }

  virtual entity::NpcOwnerType::type GetType() = 0;

  virtual void SetValue(core::uint64 value) {
    this->value_ = value;
  }

  virtual void OnEvent(event::EventType::type type,
      const void *message, size_t size) = 0;

 protected:
  core::uint64 value_;
  SceneNpc *npc_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__OWNERSHIP__H

