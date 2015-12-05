//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-13 10:48:43.
// File name: game_object.h
//
// Description:
// Define class GameObject.
//

#ifndef __GAME__SERVER__GAME__OBJECT__H
#define __GAME__SERVER__GAME__OBJECT__H

#include <stdio.h>

#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

class GameObject : public core::Noncopyable {
 public:
  GameObject();
  virtual ~GameObject();

  void Allocate();
  void Deallocate();

  void Initialize();
  void Finalize();

  inline const std::string GetStringID() const {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lu", this->id_);
    return std::string(buffer);
  }

  inline core::uint64 GetID() const {
    return this->id_;
  }
  inline void SetID(core::uint64 id) {
    this->id_ = id;
  }

 protected:
  // 非负修正
  static void NonNegativeRevise(core::int32 &value);
  // 非负百分比修正(万分之)
  static void NonNegativePercentRevise(core::int32 &value);
  // 百分比修正(万分之)
  static void PercentRevise(core::int32 &value);

  //唯一标识
  core::uint64 id_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__OBJECT__H

