#include "game_server/server/extension/item/load_suit_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

LoadSuitConfigure::LoadSuitConfigure() {}
LoadSuitConfigure::~LoadSuitConfigure() {}

bool LoadSuitConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s)出错",
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

  int id = 0;
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    SuitEffectCell cell;
    int suit_id = 0;
    if(cell_node->Attribute("suit_id", &suit_id) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-suit_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("suit_num", (int *)(&cell.suit_num_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-suit_num]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("effect_id", (int *)(&cell.effect_id_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-effect_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    SuitMap::iterator iter = this->suits_.find(suit_id);
    if(iter == this->suits_.end()) {
      EffectVecs vec;
      vec.push_back(cell);
      this->suits_.insert(SuitMap::value_type(suit_id, vec));
    } else {
      iter->second.push_back(cell);
    }

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const LoadSuitConfigure::EffectVecs *LoadSuitConfigure::GetCell(core::int32 suit_id) const {
  SuitMap::const_iterator iterator = this->suits_.find(suit_id);
  if(iterator != this->suits_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace item

}  // namespace server

}  // namespace game

