//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-19 16:17:21.
// File name: load_material_configure.h
//
// Description:
// Define class LoadMaterialConfigure.
//

#ifndef __GAME__SERVER__ITEM__LOAD__MATERIAL__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__MATERIAL__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class MaterialCell;

class LoadMaterialConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, MaterialCell> MaterialHashmap;

 public:
  LoadMaterialConfigure();
  ~LoadMaterialConfigure();

  bool LoadConfigure(const std::string &file);

  const MaterialCell *GetMaterial(core::uint32 id) const;

 private:
  MaterialHashmap materials_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__MATERIAL__CONFIGURE__H

