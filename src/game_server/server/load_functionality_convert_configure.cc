#include "game_server/server/load_functionality_convert_configure.h"

#include "game_server/server/configure.h"
#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

LoadFunctionalityConvertConfigure::LoadFunctionalityConvertConfigure() {}
LoadFunctionalityConvertConfigure::~LoadFunctionalityConvertConfigure() {}

bool LoadFunctionalityConvertConfigure::LoadConfigure(const std::string& file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s) 失败", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("找不到结点[data]，文件(%s)", file.c_str());
    return false;
  }

  // 加载 cell 结点
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    core::int32 functionality_id = 0;
    if(cell_node->Attribute("id", (int *)&functionality_id) == NULL) {
      LOG_ERROR("找不到属性[cell-id]，文件(%s)", file.c_str());
      return false;
    }

    const char* str = cell_node->Attribute("server_define");
    if(str == NULL) {
      LOG_ERROR("找不到属性[cell-function]，文件(%s)", file.c_str());
      return false;
    }
    this->functionalitys_.insert(FunctionalityMap::value_type(functionality_id, std::string(str)));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const char *LoadFunctionalityConvertConfigure::GetCell(core::int32 functionality_id) {
  FunctionalityMap::iterator iter = this->functionalitys_.find(functionality_id);
  if(iter == this->functionalitys_.end()) {
    return NULL;
  }
  return iter->second.c_str();
}

} // namespace server

} // namespace game
