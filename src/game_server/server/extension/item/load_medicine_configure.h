//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-28 13:37:40.
// File name: load_medicine_configure.h
//
// Description:
// Define class LoadMedicineConfigure.
//

#ifndef __GAME__SERVER__ITEM__LOAD__MEDICINE__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__MEDICINE__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class MedicineCell;

class LoadMedicineConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, MedicineCell> MedicineHashmap;

 public:
  LoadMedicineConfigure();
  ~LoadMedicineConfigure();

  bool LoadConfigure(const std::string &file);

  const MedicineCell *GetMedicine(core::uint32 id) const;

 private:
  MedicineHashmap medicines_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__MEDICINE__CONFIGURE__H

