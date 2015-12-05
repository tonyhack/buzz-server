//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 19:35:42.
// File name: creator_key.h
//
// Description:
// Define class CreatorKey.
//

#ifndef __GAME__SERVER__EFFECT__CREATOR__KEY__H
#define __GAME__SERVER__EFFECT__CREATOR__KEY__H

#include "core/base/types.h"

namespace game {

namespace server {

namespace effect {

class CreatorKey {
 public:
  CreatorKey() : id_(0), type_(0) {}
  CreatorKey(core::uint64 id, core::uint16 type) : id_(id), type_(type) {}

  bool operator<(const CreatorKey &key) const {
    if(this->id_ != key.id_) {
      return this->id_ < key.id_;
    } else {
      return this->type_ < key.type_;
    }
  }

 private:
  core::uint64 id_;
  core::uint16 type_;
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__CREATOR__KEY__H

