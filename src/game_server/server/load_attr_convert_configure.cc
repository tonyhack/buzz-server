//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-28 14:55:44.
// File name: load_attr_convert_configure.cc
//
// Description:
// Define class LoadAttrConvertConfigure.
//

#include "game_server/server/load_attr_convert_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

LoadAttrConvertConfigure::LoadAttrConvertConfigure() {}
LoadAttrConvertConfigure::~LoadAttrConvertConfigure() {}

bool LoadAttrConvertConfigure::LoadConfigure(const std::string &file) {
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

  int id = 0;
  entity::RoleClientFields::type type = entity::RoleClientFields::CLIENT_BEGIN;
  AttrConvertCell cell;

  // 加载 cell 结点
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("id", &id) == NULL) {
      LOG_ERROR("找不到[cell-id]，文件(%s)", file.c_str());
      return false;
    }
    switch(id) {
      case 1:
        type = entity::RoleClientFields::PHYSIQUE;
        break;
      case 2:
        type = entity::RoleClientFields::STRENGTH;
        break;
      case 3:
        type = entity::RoleClientFields::INTELLIGENCE;
        break;
      case 4:
        type = entity::RoleClientFields::AGILE;
        break;
      case 5:
        type = entity::RoleClientFields::SPIRIT;
        break;
      default:
        LOG_ERROR("配置错误[cell-id]，文件(%s)", file.c_str());
        return false;
    }
    if(this->cells_.find(type) != this->cells_.end()) {
      LOG_ERROR("配置重复[cell-id]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("max_hp", (int *)(&cell.max_hp_)) == NULL) {
      LOG_ERROR("找不到属性[cell-max_hp]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("max_mp", (int *)(&cell.max_mp_)) == NULL) {
      LOG_ERROR("找不到属性[cell-max_mp]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("physical_attack", (int *)(&cell.physical_attack_)) == NULL) {
      LOG_ERROR("找不到属性[cell-physical_attack]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("physical_defence", (int *)(&cell.physical_defence_)) == NULL) {
      LOG_ERROR("找不到属性[cell-physical_defence]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("magic_attack", (int *)(&cell.magic_attack_)) == NULL) {
      LOG_ERROR("找不到属性[cell-magic_attack]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("magic_defence", (int *)(&cell.magic_defence_)) == NULL) {
      LOG_ERROR("找不到属性[cell-magic_defence]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("dodge", (int *)(&cell.dodge_)) == NULL) {
      LOG_ERROR("找不到属性[cell-dodge]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("crit", (int *)(&cell.crit_)) == NULL) {
      LOG_ERROR("找不到属性[cell-crit]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("ignore_defence", (int *)(&cell.ignore_defence_)) == NULL) {
      LOG_ERROR("找不到属性[cell-ignore_defence]，文件(%s)", file.c_str());
      return false;
    }
    this->cells_.insert(std::make_pair(type, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const AttrConvertCell *LoadAttrConvertConfigure::GetCell(
    entity::RoleClientFields::type type) const {
  CellMap::const_iterator iterator = this->cells_.find(type);
  if(iterator != this->cells_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace server

}  // namespace game

