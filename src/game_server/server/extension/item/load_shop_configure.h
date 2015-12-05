//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-16 19:04:16.
// File name: load_shop_configure.h
//
// Description:
//

#ifndef __GAME__SERVER__ITEM__LOAD__SHOP__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__SHOP__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class LoadShopConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::int32, core::int32> ShopHashmap;

 public:
  LoadShopConfigure();
  ~LoadShopConfigure();

  bool LoadConfigure(const std::string &file);

  bool GetTemplateID(core::int32 id) const;

 private:
  ShopHashmap templateIDs_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__SHOP__CONFIGURE__H

