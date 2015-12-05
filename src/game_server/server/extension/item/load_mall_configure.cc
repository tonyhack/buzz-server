//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-25 02:06:51.
// File name: load_mall_configure.cc
//
// Description:
//
#include "game_server/server/extension/item/load_mall_configure.h"

#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadMallConfigure::LoadMallConfigure() {}
LoadMallConfigure::~LoadMallConfigure() {}

const entity::MallTreasureCell *LoadMallConfigure::GetMallCell(const MallConfigureKey& key) const {
  MallCellMap::const_iterator itr = this->mall_cells_.find(key);
  if(itr != this->mall_cells_.end()) {
    return &itr->second;
  }
  return NULL;
}

bool LoadMallConfigure::AddMallLine(const entity::MallTreasureCell &cell) {
  MallConfigureKey key(cell.tab_, cell.shop_type_, cell.location_);
  const entity::MallTreasureCell *mall = this->GetMallCell(key);
  if(mall != NULL) {
    LOG_ERROR("加入的位置重复tab(%d)shop_type(%d)pos(%d)",
        cell.tab_, cell.shop_type_, cell.location_);
    return false;
  }
  this->mall_cells_.insert(MallCellMap::value_type(key, cell));
  return true;
}

bool LoadMallConfigure::Initialize() {
  this->mall_cells_.clear();
  return true;
}

void LoadMallConfigure::Finalize() {
  this->mall_cells_.clear();
}

} // namespace item

} // namespace  server

} // namespace game

