//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-19 19:25:29.
// File name: load_soul_level_configure.cc
//
// Description:
// Define class LoadSoulGroupConfigure.
//

#include "game_server/server/extension/soul/load_soul_level_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace soul {

LoadSoulLevelConfigure::LoadSoulLevelConfigure() {}
LoadSoulLevelConfigure::~LoadSoulLevelConfigure() {}

bool LoadSoulLevelConfigure::LoadConfigure(const std::string &file) {
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
    SoulLevelKey key;
    SoulLevelCell cell;
    if (cell_node->Attribute("group", (int *)&key.group_) == false) {
      LOG_ERROR("找不到属性 [data-group], 文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("level", (int *)&key.level_) == false) {
      LOG_ERROR("找不到属性 [data-level], 文件(%s)", file.c_str());
      return false;
    }
    if (this->levels_.find(key) != this->levels_.end()) {
      LOG_ERROR("配置重复的 key(%d,%d), 文件(%s)",
          key.group_, key.level_, file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_item", (int *)&cell.spend_item_) == false) {
      LOG_ERROR("找不到属性 [data-spend_item]", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_number", (int *)&cell.spend_number_) == false) {
      LOG_ERROR("找不到属性 [data-spend_number]", file.c_str());
      return false;
    }
    this->levels_.insert(std::make_pair(key, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const SoulLevelCell *LoadSoulLevelConfigure::GetLevel(core::int32 group,
    core::int32 level) const {
  LevelHashmap::const_iterator iterator = this->levels_.find(SoulLevelKey(group, level));
  if(iterator != this->levels_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace soul

}  // namespace server

}  // namespace game

