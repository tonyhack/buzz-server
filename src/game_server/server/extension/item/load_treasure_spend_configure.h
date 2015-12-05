#ifndef __GAME__SERVER__ITEM__LOAD__TREASURE__SPEND__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__TREASURE__SPEND__CONFIGURE__H

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

class TreasureSpendCell {
 public:
  core::int32 id_;
  core::int32 item_;
  core::int32 item_num_;
  core::int32 tab_id_;
  core::int32 integral_num_;
};

class TreasureKeyCell {
 public:
  TreasureKeyCell(core::int32 tab_id, core::int32 num) : tab_id_(tab_id), num_(num) {}

  core::int32 tab_id_;
  core::int32 num_;

  bool operator <(const TreasureKeyCell &cell) const {
    if(tab_id_ < cell.tab_id_) {
      return true;
    }
    if(tab_id_ == cell.tab_id_) {
      return num_ < cell.num_;
    }
    return false;
  }
};

class LoadTreasureSpendConfigure : public core::Noncopyable {
  typedef std::map<TreasureKeyCell, TreasureSpendCell> SpendMap;
 public:
  LoadTreasureSpendConfigure();
  ~LoadTreasureSpendConfigure();

  bool LoadConfigure(const std::string &file);

  const TreasureSpendCell *GetCell(core::int32 id, core::int32 num) const;
  
 private:
  SpendMap spends_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__TREASURE__SPEND__CONFIGURE__H

