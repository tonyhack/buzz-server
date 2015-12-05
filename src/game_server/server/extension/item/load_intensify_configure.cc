//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-12 16:36:55.
// File name: load_intensify_configure.cc
//
// Description:
// Define class LoadIntensifyConfigure.
//

#include "game_server/server/extension/item/load_intensify_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadIntensifyConfigure::LoadIntensifyConfigure() {}
LoadIntensifyConfigure::~LoadIntensifyConfigure() {}

bool LoadIntensifyConfigure::LoadConfigure(const std::string &file) {
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

  IntensifyKey key;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    IntensifyCell cell;
    if(cell_node->Attribute("equip_level", (int *)(&key.equip_level_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-equip_level] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("intensify", (int *)(&key.intensify_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-intensify] 出错", file.c_str());
      return false;
    }
    if(this->intensifys_.find(key) != this->intensifys_.end()) {
      LOG_ERROR("配置文件(%s) 配置(%d,%d) 重复", file.c_str(),
          key.equip_level_, key.intensify_);
      return false;
    }
    if(cell_node->Attribute("min_odds", (int *)(&cell.min_odds_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-min_odds] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("max_odds", (int *)(&cell.max_odds_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-max_odds] 出错", file.c_str());
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
    if(cell_node->Attribute("gold", (int *)(&cell.spend_gold_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-gold] 出错", file.c_str());
      return false;
    }
    this->intensifys_.insert(std::make_pair(key, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const IntensifyCell *LoadIntensifyConfigure::GetIntensify(core::int32 equip_level,
    core::int32 intensify) const {
  IntensifyHashmap::const_iterator iterator = this->intensifys_.find(
      IntensifyKey(equip_level, intensify));
  if(iterator != this->intensifys_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace item

}  // namespace server

}  // namespace game

