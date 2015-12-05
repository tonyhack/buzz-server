//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-19 16:26:08.
// File name: load_material_configure.cc
//
// Description:
// Define class LoadMaterialConfigure.
//

#include "game_server/server/extension/item/load_material_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension/item/material.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadMaterialConfigure::LoadMaterialConfigure() {}
LoadMaterialConfigure::~LoadMaterialConfigure() {}

bool LoadMaterialConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s) 出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  MaterialCell cell;
  int id = 0;
  const char *str = NULL;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("id", &id) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    str = cell_node->Attribute("name");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-name]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    cell.name_ = str;
    if(cell_node->Attribute("level", (int *)(&cell.level_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-level]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_coin", (int *)(&cell.buy_coin_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_coin]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_gold", (int *)(&cell.buy_gold_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_gold]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_lockedgold", (int *)(&cell.buy_lockedgold_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_lockedgold]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buy_honour", (int *)(&cell.buy_honour_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buy_honour]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sell_coin", (int *)(&cell.sell_coin_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sell_coin]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sell_drop", (int *)(&cell.sell_drop_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sell_drop]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("quality", (int *)(&cell.quality_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-quality]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("operate_right", (int *)(&cell.operate_right_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-operate_right]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("stack_count", (int *)(&cell.stack_count_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-stack_count]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("sub_type", (int *)(&cell.sub_type_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-sub_type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    this->materials_.insert(std::make_pair(id, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  global::LogDebug("Material 配置个数(%u)", this->materials_.size());

  return true;
}

const MaterialCell *LoadMaterialConfigure::GetMaterial(core::uint32 id) const {
  MaterialHashmap::const_iterator iterator = this->materials_.find(id);
  if(iterator != this->materials_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace item

}  // namespace server

}  // namespace game

