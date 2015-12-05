//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-12 16:29:33.
// File name: load_intensify_configure.h
//
// Description:
// Define class LoadIntensifyConfigure.
//

#ifndef __GAME__SERVER__ITEM__LOAD__INTENSIFY__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__INTENSIFY__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class IntensifyKey {
 public:
  IntensifyKey() : equip_level_(0), intensify_(0) {}
  IntensifyKey(core::int32 equip_level, core::int32 intensify)
    : equip_level_(equip_level), intensify_(intensify) {}

  bool operator==(const IntensifyKey &key) const {
    return this->equip_level_ == key.equip_level_ &&
      this->intensify_ == key.intensify_;
  }
  bool operator<(const IntensifyKey &key) const {
    if(this->equip_level_ != key.equip_level_) {
      return this->equip_level_ < key.equip_level_;
    } else {
      return this->intensify_ < key.intensify_;
    }
  }

  core::int32 equip_level_;
  core::int32 intensify_;
};

class IntensifyFunction {
 public:
  size_t operator()(const IntensifyKey &key) const {
    size_t hash = key.equip_level_;
    return (hash << 32) + key.intensify_;
  }
};

class LoadIntensifyConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<IntensifyKey, IntensifyCell, IntensifyFunction>
    IntensifyHashmap;

 public:
  LoadIntensifyConfigure();
  ~LoadIntensifyConfigure();

  bool LoadConfigure(const std::string &file);

  const IntensifyCell *GetIntensify(core::int32 equip_level,
      core::int32 intensify) const;

 private:
  IntensifyHashmap intensifys_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__INTENSIFY__CONFIGURE__H

