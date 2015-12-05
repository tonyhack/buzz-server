//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-08-15 22:42:31.
// File name: load_equip_refine_configure.h
//
// Description:
//

#ifndef __GAME__SERVER__ITEM__LOAD__EQUIP__REFINE__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__EQUIP__REFINE__CONFIGURE__H

#include <ext/hash_map>
#include <vector>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class LoadEquipRefineConfigure : public core::Noncopyable {
  class EquipRefineKey {
   public:
    bool operator==(const EquipRefineKey &key) const {
      return key.equip_level_ == this->equip_level_ &&
        key.equip_quality_ == this->equip_quality_;
    }

    core::int32 equip_level_;      // 装备等级
    core::int32 equip_quality_;    // 装备品质
  };

  class EquipRefineKeyHash {
   public:
     size_t operator()(const EquipRefineKey& key) const {
      size_t hash = key.equip_level_;
      return (hash << 32 ) + key.equip_quality_;
     }
  };

  typedef  __gnu_cxx::hash_map<EquipRefineKey, EquipRefineSpendCell, EquipRefineKeyHash> EquipRefineHashmap;
  typedef  std::vector<EquipRefineSpendLockCell> EquipRefineVec;
 public:
  LoadEquipRefineConfigure();
  ~LoadEquipRefineConfigure();

  bool LoadRefineConfigure(const std::string &file);

  bool LoadEquipLockConfigure(const std::string &file);

  const EquipRefineSpendCell *GetEquipRefineSpend(core::uint32 lv, core::int32 quality) const;
  
  const EquipRefineSpendLockCell *GetEquiplockSpend(core::int32 lock_num) const;

 private:
   EquipRefineVec equipRefine_locks_; 
   EquipRefineHashmap equipRefines_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__EQUIP__REFINE__CONFIGURE__H

