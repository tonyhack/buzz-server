//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-28 15:35:18.
// File name: load_vocation_growth_configure.cc
//
// Description:
// Define class LoadVocationGrowthConfigure.
//

#include "game_server/server/load_vocation_growth_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

LoadVocationGrowthConfigure::LoadVocationGrowthConfigure() {}
LoadVocationGrowthConfigure::~LoadVocationGrowthConfigure() {}

bool LoadVocationGrowthConfigure::LoadConfigure(const std::string &file) {
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

  int vocation = 0;
  VocationGrowthCell cell;

  // 加载 cell 结点
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("vocation", &vocation) == NULL) {
      LOG_ERROR("找不到[cell-vocation]，文件(%s)", file.c_str());
      return false;
    }
    if(vocation < entity::VocationType::MIN || vocation >= entity::VocationType::MAX) {
      LOG_ERROR("属性[cell-vocation]错误，文件(%s)", file.c_str());
      return false;
    }
    if(this->cells_.find((entity::VocationType::type)vocation) != this->cells_.end()) {
      LOG_ERROR("配置重复[cell-vocation]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("physique", (int *)(&cell.physique_)) == NULL) {
      LOG_ERROR("找不到属性[cell-physique]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("strength", (int *)(&cell.strength_)) == NULL) {
      LOG_ERROR("找不到属性[cell-strength]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("intelligence", (int *)(&cell.intelligence_)) == NULL) {
      LOG_ERROR("找不到属性[cell-intelligence]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("agile", (int *)(&cell.agile_)) == NULL) {
      LOG_ERROR("找不到属性[cell-agile]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("spirit", (int *)(&cell.spirit_)) == NULL) {
      LOG_ERROR("找不到属性[cell-spirit]，文件(%s)", file.c_str());
      return false;
    }
    this->cells_.insert(std::make_pair((entity::VocationType::type)vocation, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const VocationGrowthCell *LoadVocationGrowthConfigure::GetCell(
    entity::VocationType::type type) const {
  CellMap::const_iterator iterator = this->cells_.find(type);
  if(iterator != this->cells_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace server

}  // namespace game

