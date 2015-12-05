//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-16 18:36:47.
// File name: shop_configure.cc
//
// Description:
//

#include "game_server/server/extension/item/load_shop_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadShopConfigure::LoadShopConfigure() {}
LoadShopConfigure::~LoadShopConfigure() {}

bool LoadShopConfigure::LoadConfigure(const std::string &file) {
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

  core::int32 id = 0;
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("template_id", &id) == false) {
      global::LogError("%s:%d (%s) 找不到属形[cell-template_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    this->templateIDs_.insert(std::make_pair(id, id));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool LoadShopConfigure::GetTemplateID(core::int32 id) const {
  ShopHashmap::const_iterator itr= this->templateIDs_.find(id);
  if(itr != this->templateIDs_.end()) {
    return true;
  }
  return false;
}

} // namespace item

} // namespace server

} // namespace game

