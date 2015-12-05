//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-28 14:45:15.
// File name: load_random_attribute.cc
//
// Description:
// Define class LoadRandomAttribute.
//

#include "game_server/server/extension/item/load_random_attribute.h"

#include "core/tinyxml/tinyxml.h"
#include "entity/equipment_types.h"
#include "game_server/server/extension_manager.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace item {

LoadRandomAttribute::LoadRandomAttribute() {}
LoadRandomAttribute::~LoadRandomAttribute() {}

bool LoadRandomAttribute::LoadConfigure(const std::string &file) {
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

  RandomAttributeCell cell;

  int id = 0;
  const char *str = NULL;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("id", &id) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("odds", (int *)(&cell.odds_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-odds]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("attr_type", (int *)(&cell.type_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-attr_type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("attr_value", (int *)(&cell.value_)) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-attr_value]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell.odds_ == 0) {
      cell_node = cell_node->NextSiblingElement("cell");
      continue;
    }

    // 随机范围判断
    if(cell.odds_ < 0 || cell.odds_ > global::kPercentDenom) {
      global::LogError("%s:%d (%s) 属性 [cell-attr_type] 值(%u)非法，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, cell.odds_, file.c_str());
      return false;
    }

    // 类型合法性
    if(this->CheckValid(cell.type_) == false) {
      global::LogError("%s:%d (%s) 属性类型(%u, %u)不合法，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, cell.type_, id, file.c_str());
      return false;
    }

    // 插入到表中
    AttributeHashmap::iterator iterator = this->attrs_.find(id);
    if(iterator != this->attrs_.end()) {
      // 同一ID内类型不能重复
      if(this->CheckExist(cell.type_, iterator->second) == false) {
        global::LogError("%s:%d (%s) 属性类型(%u, %u)重复，文件(%s)",
            __FILE__, __LINE__, __FUNCTION__, cell.type_, id, file.c_str());
        return false;
      }

      iterator->second.push_back(cell);
    } else {
      this->attrs_.insert(std::make_pair(id, AttributeVector(1, cell)));
    }

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const RandomAttributeCell *LoadRandomAttribute::RandomAttribute(core::uint32 id) const {
  AttributeHashmap::const_iterator iterator = this->attrs_.find(id);
  if(iterator == this->attrs_.end()){
    return NULL;
  }

  core::uint32 odds_comp = 0;
  core::uint32 odds =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);

  AttributeVector::const_iterator iterator2 = iterator->second.begin();
  for(; iterator2 != iterator->second.end(); ++iterator2) {
    odds_comp += iterator2->odds_;
    if(odds <= odds_comp) {
      return &(*iterator2);
    }
  }

  return NULL;
}

const LoadRandomAttribute::AttributeVector *LoadRandomAttribute::GetRandomVector(
    core::uint32 id) const {
  AttributeHashmap::const_iterator iterator = this->attrs_.find(id);
  if(iterator != this->attrs_.end()){
    return &iterator->second;
  }
  return NULL;
}

bool LoadRandomAttribute::CheckValid(core::uint8 type) const {
  if(type <= entity::EquipAttributeType::MIN ||
      type >= entity::EquipAttributeType::MAX) {
    return false;
  }

  return true;
}

bool LoadRandomAttribute::CheckExist(core::uint8 type, const AttributeVector &cells) const {
  AttributeVector::const_iterator iterator = cells.begin();
  for(; iterator != cells.end(); ++iterator) {
    if(type == iterator->type_) {
      return false;
    }
  }

  return true;
}

}  // namespace item

}  // namespace server

}  // namespace game

