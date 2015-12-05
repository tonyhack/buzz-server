#include "game_server/server/extension/item/load_treasure_spend_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/types.h"
#include "global/logging.h"
#include "game_server/server/extension_manager.h"

namespace game {

namespace server {

namespace item {

LoadTreasureSpendConfigure::LoadTreasureSpendConfigure() {}
LoadTreasureSpendConfigure::~LoadTreasureSpendConfigure() {}

bool LoadTreasureSpendConfigure::LoadConfigure(const std::string &file) {
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
    TreasureSpendCell cell;
    if(cell_node->Attribute("id", (int *)(&cell.id_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-id] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("item", (int *)(&cell.item_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-item] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("item_num", (int *)(&cell.item_num_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-item_num] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("tab_id", (int *)(&cell.tab_id_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-tab_id] 出错", file.c_str());
      return false;
    }
    if(cell_node->Attribute("integral_num", (int *)(&cell.integral_num_)) == false) {
      LOG_ERROR("加载配置文件(%s) [cell-integral_num] 出错", file.c_str());
      return false;
    }
    TreasureKeyCell key(cell.tab_id_, cell.id_);
    this->spends_.insert(SpendMap::value_type(key, cell));


    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const TreasureSpendCell *LoadTreasureSpendConfigure::GetCell(core::int32 id,
    core::int32 num) const {
  TreasureKeyCell key(id, num);
  SpendMap::const_iterator iter = this->spends_.find(key);
  if(iter == this->spends_.end()) {
    return NULL;
  }
  return &(iter->second);
}

}  // namespace item

}  // namespace server

}  // namespace game

