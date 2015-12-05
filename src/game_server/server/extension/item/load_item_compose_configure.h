#ifndef __GAME__SERVER__ITEM__LOAD__ITEM__COMPOSE__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__ITEM__COMPOSE__CONFIGURE__H

#include <cstddef>
#include <string>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class LoadItemComposeConfigure : public core::Noncopyable {
 public:
  typedef __gnu_cxx::hash_map<core::int32, ItemComposeRecipeCell> ItemComposeRecipeHashmap;

  LoadItemComposeConfigure();
  ~LoadItemComposeConfigure();

  bool LoadConfigure(const std::string &file);

  const ItemComposeRecipeCell *GetItemComposeRecipe(core::int32 id) const;

 private:
  ItemComposeRecipeHashmap recipes_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__ITEM__COMPOSE__CONFIGURE__H

