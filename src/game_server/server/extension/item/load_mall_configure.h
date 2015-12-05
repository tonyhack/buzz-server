//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-25 01:44:27.
// File name: load_mall_configure.h
//
// Description:
//
#ifndef __GAME__SERVER__ITEM__LOAD__MALL__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__MALL__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/mall_types.h"

namespace game {

namespace server {

namespace item {

class MallConfigureKey {
 public:
  MallConfigureKey(core::int8 tab, core::int8 shop_type, core::int8 pos) {
    this->tab_ = tab;
    this->shop_type_ = shop_type;
    this->pos_ = pos;
  }

  bool operator < (const MallConfigureKey& key) const{
    if(this->tab_ < key.tab_) {
      return true;
    }
    if(this->tab_ == key.tab_) {
      if(this->shop_type_ < key.shop_type_) {
        return true;
      }
      if(this->shop_type_ == key.shop_type_) {
        if(this->pos_ < key.pos_) {
          return true;
        }
        return false;
      }
      return false;
    }
    return false;
  }

  core::int8 tab_;
  core::int8 shop_type_;
  core::int8 pos_;
};

class LoadMallConfigure : public core::Noncopyable {
  typedef std::map<MallConfigureKey, entity::MallTreasureCell, std::less<MallConfigureKey> > MallCellMap;

 public:
  LoadMallConfigure();
  ~LoadMallConfigure();

  bool Initialize();
  void Finalize();

  const entity::MallTreasureCell *GetMallCell(const MallConfigureKey& key) const;

  bool AddMallLine(const entity::MallTreasureCell &cell);

 private:
  MallCellMap mall_cells_;
};

} // namespace item

} // namespace server 

} // namespace game

#endif //__GAME__SERVER__ITEM__LOAD__MALL__CONFIGURE__H

