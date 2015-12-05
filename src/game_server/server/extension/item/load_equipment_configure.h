//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-28 11:35:25.
// File name: load_equipment_configure.h
//
// Description:
// Define class LoadEquipmentConfigure.
//

#ifndef __GAME__SERVER__ITEM__LOAD__EQUIPMENT__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__EQUIPMENT__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class LoadEquipmentConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, EquipmentCell> EquipmentHashmap;

 public:
  LoadEquipmentConfigure();
  ~LoadEquipmentConfigure();

  bool LoadConfigure(const std::string &file);

  const EquipmentCell *GetEquipment(core::uint32 id) const;

 private:
  EquipmentHashmap equipments_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__EQUIPMENT__CONFIGURE__H

