//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-13 23:15:05.
// File name: load_soul_skill_level_configure.cc
//
// Description:
// Define class LoadSoulSkillLevelConfigure.
//

#include "game_server/server/extension/soul/load_soul_skill_level_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace soul {

LoadSoulSkillLevelConfigure::LoadSoulSkillLevelConfigure() {}
LoadSoulSkillLevelConfigure::~LoadSoulSkillLevelConfigure() {}

bool LoadSoulSkillLevelConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s) 出错", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if (data_node == NULL) {
    LOG_ERROR("找不到结点 [data], 文件(%s)", file.c_str());
    return false;
  }

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    SoulSkillLevelCell cell;
    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("找不到属性 [data-level], 文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_nimbus", (int *)&cell.spend_nimbus_) == false) {
      LOG_ERROR("找不到属性 [data-spend_nimbus, 文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_gold", (int *)&cell.spend_gold_) == false) {
      LOG_ERROR("找不到属性 [data-spend_gold, 文件(%s)", file.c_str());
      return false;
    }
    if (this->levels_.find(cell.level_) != this->levels_.end()) {
      LOG_ERROR("重复的配置 level=%d, 文件(%s)", cell.level_, file.c_str());
      return false;
    }
    this->levels_.insert(std::make_pair(cell.level_, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const SoulSkillLevelCell *LoadSoulSkillLevelConfigure::GetSkillLevel(
    core::int32 skill_level) const {
  SkillLevelHashmap::const_iterator iterator = this->levels_.find(skill_level);
  if (iterator != this->levels_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace soul

}  // namespace server

}  // namespace game

