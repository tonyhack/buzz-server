//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-15 09:34:46.
// File name: according_level_configure.cc
//
// Description:
// Define class AccordingLevelConfigure.
//

#include "game_server/server/according_level_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

AccordingLevelConfigure::AccordingLevelConfigure() {}
AccordingLevelConfigure::~AccordingLevelConfigure() {}

bool AccordingLevelConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 data 结点标签
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点[data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  AccordingLevelCell cell;
  int level = 0;

  // 加载 cell 结点标签
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("level", &level) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-level]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(this->cells_.find(level) != this->cells_.end()) {
      global::LogError("%s:%d (%s) 等级(%d)配置重复, 文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, level, file.c_str());
      return false;
    }
    if(cell_node->Attribute("exp", (int *)(&cell.exp_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-exp]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("max_nimbus", (int *)(&cell.max_nimbus_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-max_nimbus]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("speed", (int *)(&cell.speed_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-speed]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("crit_effect_coe", (int *)(&cell.crit_effect_coe_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-crit_effect_coe]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("hurt_random", (int *)(&cell.hurt_random_per_)) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-hurt_random]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_energy", (int *)(&cell.max_energy_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-max_energy], 文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("energy_increment", (int *)(&cell.energy_increment_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-energy_increment], 文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("zazen_add_exp", (int *)(&cell.zazen_add_exp_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-zazen_add_exp], 文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("zazen_add_nimbus", (int *)(&cell.zazen_add_nimbus_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-zazen_add_nimbus], 文件(%s)", file.c_str());
      return false;
    }

    this->cells_.insert(std::make_pair(level, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const AccordingLevelCell *AccordingLevelConfigure::GetCell(core::uint32 level) const {
  CellHashmap::const_iterator iterator = this->cells_.find(level);
  if(iterator != this->cells_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace server

}  // namespace game

