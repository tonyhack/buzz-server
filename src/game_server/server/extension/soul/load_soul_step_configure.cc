//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-07 16:03:41.
// File name: load_soul_step_configure.cc
//
// Description:
// Define class LoadSoulStepConfigure.
//

#include "game_server/server/extension/soul/load_soul_step_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace soul {

LoadSoulStepConfigure::LoadSoulStepConfigure() {}
LoadSoulStepConfigure::~LoadSoulStepConfigure() {}

bool LoadSoulStepConfigure::LoadConfigure(const std::string &file) {
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
    SoulStepKey key;
    SoulStepCell cell;
    if (cell_node->Attribute("group", (int *)&key.group_) == false) {
      LOG_ERROR("找不到属性 [data-group], 文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("level", (int *)&key.level_) == false) {
      LOG_ERROR("找不到属性 [data-level], 文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("step", (int *)&key.step_) == false) {
      LOG_ERROR("找不到属性 [data-step], 文件(%s)", file.c_str());
      return false;
    }
    if (this->steps_.find(key) != this->steps_.end()) {
      LOG_ERROR("配置重复的 step(%lu) key(%d,%d,%d), 文件(%s)",
          this->steps_.size(), key.group_, key.level_, key.step_, file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_nimbus", (int *)&cell.spend_nimbus_) == false) {
      LOG_ERROR("找不到属性 [data-spend_nimbus]", file.c_str());
      return false;
    }
    if (cell_node->Attribute("odds", (int *)&cell.odds_) == false) {
      LOG_ERROR("找不到属性 [data-odds]", file.c_str());
      return false;
    }
    const char *attrs = cell_node->Attribute("attrs", (int *)&attrs);
    if (attrs == NULL) {
      LOG_ERROR("找不到属性 [data-attrs]", file.c_str());
      return false;
    }
    std::vector<std::string> pairs;
    global::StringSplit(attrs, "+", pairs);
    for (size_t pos = 0; pos < pairs.size(); ++pos) {
      std::vector<core::int32> attr;
      global::StringSplit(pairs[pos].c_str(), ":", attr);
      if (attr.size() != 2) {
        LOG_ERROR("配置错误 [cell-attrs], 文件(%s)", file.c_str());
        return false;
      }
      cell.attrs_.insert(std::make_pair((entity::SoulAttributeType::type)attr[0], attr[1]));
    }
    this->steps_.insert(std::make_pair(key, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  for (StepMap::reverse_iterator iterator = this->steps_.rbegin();
      iterator != this->steps_.rend(); ++iterator) {
    StepMap::reverse_iterator iterator2 = iterator;
    ++iterator2;
    while (iterator2 != this->steps_.rend() &&
        iterator2->first.group_ == iterator->first.group_) {
      for (SoulStepCell::AttrMap::iterator attr_iterator2 = iterator2->second.attrs_.begin();
          attr_iterator2 != iterator2->second.attrs_.end(); ++attr_iterator2) {
        SoulStepCell::AttrMap::iterator attr_iterator =
          iterator->second.attrs_.find(attr_iterator2->first);
        if (attr_iterator != iterator->second.attrs_.end()) {
          attr_iterator->second += attr_iterator2->second;
        } else {
          iterator->second.attrs_.insert(*attr_iterator2);
        }
      }
      ++iterator2;
    }
  }

  return true;
}

const SoulStepCell *LoadSoulStepConfigure::GetStep(core::int32 group, core::int32 level,
    core::int32 step) const {
  SoulStepKey key(group, level, step);
  StepMap::const_iterator iterator = this->steps_.find(key);
  if (iterator == this->steps_.end()) {
    return NULL;
  }
  return &iterator->second;
}

}  // namespace soul

}  // namespace server

}  // namespace game

