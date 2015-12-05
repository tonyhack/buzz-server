//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-08-15 23:06:57.
// File name: load_equip_refine_configure.cc
//
// Description:

#include "game_server/server/extension/item/load_equip_refine_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadEquipRefineConfigure::LoadEquipRefineConfigure() {}
LoadEquipRefineConfigure::~LoadEquipRefineConfigure() {}

bool LoadEquipRefineConfigure::LoadRefineConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s)出错", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("加载配置文件(%s) [data] 结点出错", file.c_str());
    return false;
  }


  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    EquipRefineSpendCell cell;
    if(cell_node->Attribute("equip_level", (int *)(&cell.equip_lv_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-equip_level] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("quality", (int *)(&cell.equip_quality_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-quality] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("spend_gold", (int *)(&cell.spend_gold_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-spend_gold] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("spend_item", (int *)(&cell.spend_item_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-spend_item] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("spend_item_num", (int *)(&cell.spend_item_num_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-spend_item_num] 出错", file.c_str());
      return false;
    }
    EquipRefineKey key;
    key.equip_level_ = cell.equip_lv_;
    key.equip_quality_ = cell.equip_quality_;
    this->equipRefines_.insert(std::make_pair(key, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool LoadEquipRefineConfigure::LoadEquipLockConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s)出错", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("加载配置文件(%s) [data] 结点出错", file.c_str());
    return false;
  }


  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    EquipRefineSpendLockCell cell;
    if(cell_node->Attribute("lock_attr_num", (int *)(&cell.lock_num_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-rand_attr_num] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("spend_diamond", (int *)(&cell.spend_diamond_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-spend_diamond] 出错", file.c_str());
      return false;
    }
    
    equipRefine_locks_.push_back(cell);

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const EquipRefineSpendCell *LoadEquipRefineConfigure::GetEquipRefineSpend(core::uint32 lv,
    core::int32 quality) const {
  EquipRefineKey key;
  key.equip_level_ = (core::int32)lv;
  key.equip_quality_ = quality;
  EquipRefineHashmap::const_iterator iterator = this->equipRefines_.find(key);
  if(iterator != this->equipRefines_.end()) {
    return &iterator->second;
  }

  return NULL;
}

const EquipRefineSpendLockCell  *LoadEquipRefineConfigure::GetEquiplockSpend(core::int32 lock_num) const {
  if((size_t)lock_num <= equipRefine_locks_.size() && lock_num > 0) {
    return &(equipRefine_locks_[lock_num-1]);
  }
  else {
    return 0;
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

