#ifndef __GAME__SERVER__ITEM__LOAD__HUNT__TREASURE__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__HUNT__TREASURE__CONFIGURE__H

#include <cstddef>
#include <string>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class HuntTreasureRandCell {
 public:
  core::int32 item_;
  core::int32 odds_;
  core::int32 position_;
  core::int32 drop_;
  core::int32 integral_type_; // (0 : 传奇积分  1 : 奇迹积分)
};

struct IntegralType {
  enum type {
    MIRACLE = 0,
    LEGEND,
  };

};

class LoadHuntTreasureConfigure : public core::Noncopyable {
  typedef std::vector<HuntTreasureRandCell> RandVec;
  typedef std::map<int, RandVec> RandMap;
 public:
  LoadHuntTreasureConfigure();
  ~LoadHuntTreasureConfigure();

  bool LoadConfigure(const std::string &file);

  HuntTreasureRandCell *GetRand(core::int32 id);

  IntegralType::type GetItegralType(core::int32 id);
  
 private:
  RandMap randon_map_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__HUNT__TREASURE__CONFIGURE__H

