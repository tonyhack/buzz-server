//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 17:09:23.
// File name: load_soul_configure.cc
//
// Description:
// Define class LoadSoulConfigure.
//

#include "game_server/server/extension/soul/load_soul_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "entity/soul_types.h"
#include "game_server/server/extension/soul/configure.h"
#include "game_server/server/extension/soul/soul_configure.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace soul {

LoadSoulConfigure::LoadSoulConfigure() {}
LoadSoulConfigure::~LoadSoulConfigure() {}

bool LoadSoulConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载 配置文件(%s) 出错", file.c_str());
    return false;
  }

  // 加载 data 结点
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if (data_node == NULL) {
    LOG_ERROR("找不到结点 [data]，文件(%s)", file.c_str());
    return false;
  }

  const char *str = NULL;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    SoulCell cell;
    core::int32 id = 0;
    if (cell_node->Attribute("id", (int *)(&id)) == NULL) {
      LOG_ERROR("找不到属性 [cell-id]，文件(%s)", file.c_str());
      return false;
    }
    if (this->souls_.find(id) != this->souls_.end()) {
      LOG_ERROR("重复的 SoulCell(%d,%d), 文件(%s)",
          id, file.c_str());
      return false;
    }
    if (cell_node->Attribute("vocation", (int *)(&cell.vocation_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-vocation]，文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("step_group", (int *)(&cell.step_group_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-step_group]，文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("level_group", (int *)(&cell.level_group_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-level_group]，文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("common_skill", (int *)(&cell.common_skill_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-common_skill]，文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("appear_skill", (int *)(&cell.appear_skill_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-appear_skill]，文件(%s)", file.c_str());
      return false;
    }
    if (cell_node->Attribute("soul_skill", (int *)(&cell.soul_skill_)) == NULL) {
      LOG_ERROR("找不到属性 [cell-soul_skill]，文件(%s)", file.c_str());
      return false;
    }

    str = cell_node->Attribute("unlock_condition");
    if (str == NULL) {
      LOG_ERROR("找不到属性 [cell-unlock_condition]，文件(%s)", file.c_str());
      return false;
    }
    std::vector<core::int32> unlock_fields;
    global::StringSplit(str, ":", unlock_fields);
    if (unlock_fields.size() != 3) {
      LOG_ERROR("配置错误 [cell-unlock_condition] id(%d), 文件(%s)",
          id, file.c_str());
      return false;
    }
    if (unlock_fields[0] <= entity::SoulUnlockType::MIN ||
        unlock_fields[0] >= entity::SoulUnlockType::MAX) {
      LOG_ERROR("配置错误 [cell-unlock_condition] id(%d), 文件(%s)",
          id, file.c_str());
      return false;
    }
    cell.unlock_type_ = (entity::SoulUnlockType::type)unlock_fields[0];
    cell.unlock_param1_ = unlock_fields[1];
    cell.unlock_param2_ = unlock_fields[2];

    str = cell_node->Attribute("spirit_effects");
    if (str == NULL) {
      LOG_ERROR("找不到属性 [cell-spirit_effects]，文件(%s)", file.c_str());
      return false;
    }
    std::vector<std::string> field_strings;
    global::StringSplit(str, "+", field_strings);
    for (size_t pos = 0; pos < field_strings.size(); ++pos) {
      std::vector<core::int32> effects;
      global::StringSplit(field_strings[pos].c_str(), ":", effects);
      if (effects.size() != 4) {
        LOG_ERROR("配置错误 [cell-spirit_effects] id(%d), 文件(%s)",
            id, file.c_str());
        return false;
      }
      SoulEffectValue soul_effect_value;
      soul_effect_value.type_ = (entity::SoulEffectType::type)effects[1];
      if (soul_effect_value.type_ <= entity::SoulEffectType::MIN ||
          soul_effect_value.type_ >= entity::SoulEffectType::MAX) {
        LOG_ERROR("配置错误 [cell-spirit_effects] id(%d), 文件(%s)",
            id, file.c_str());
        return false;
      }
      soul_effect_value.param1_ = effects[2];
      soul_effect_value.param2_ = effects[3];
      SoulCell::EffectMap::iterator iterator = cell.spirit_effects_.find(effects[0]);
      if (iterator != cell.spirit_effects_.end()) {
        iterator->second.push_back(soul_effect_value);
      } else {
        SoulCell::EffectVector effect_vector;
        effect_vector.push_back(soul_effect_value);
        cell.spirit_effects_.insert(std::make_pair(effects[0], effect_vector));
      }
    }

    str = cell_node->Attribute("sword_effects");
    if (str == NULL) {
      LOG_ERROR("找不到属性 [cell-sword_effects]，文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(str, "+", field_strings);
    for (size_t pos = 0; pos < field_strings.size(); ++pos) {
      std::vector<core::int32> effects;
      global::StringSplit(field_strings[pos].c_str(), ":", effects);
      if (effects.size() != 4) {
        LOG_ERROR("配置错误 [cell-sword_effects] id(%d), 文件(%s)",
            id, file.c_str());
        return false;
      }
      SoulEffectValue soul_effect_value;
      soul_effect_value.type_ = (entity::SoulEffectType::type)effects[1];
      if (soul_effect_value.type_ <= entity::SoulEffectType::MIN ||
          soul_effect_value.type_ >= entity::SoulEffectType::MAX) {
        LOG_ERROR("配置错误 [cell-sword_effects] id(%d), 文件(%s)",
            id, file.c_str());
        return false;
      }
      soul_effect_value.param1_ = effects[2];
      soul_effect_value.param2_ = effects[3];
      SoulCell::EffectMap::iterator iterator = cell.sword_effects_.find(effects[0]);
      if (iterator != cell.sword_effects_.end()) {
        iterator->second.push_back(soul_effect_value);
      } else {
        SoulCell::EffectVector effect_vector;
        effect_vector.push_back(soul_effect_value);
        cell.sword_effects_.insert(std::make_pair(effects[0], effect_vector));
      }
    }

    str = cell_node->Attribute("noble_effects");
    if (str == NULL) {
      LOG_ERROR("找不到属性 [cell-noble_effects]，文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(str, "+", field_strings);
    for (size_t pos = 0; pos < field_strings.size(); ++pos) {
      std::vector<core::int32> effects;
      global::StringSplit(field_strings[pos].c_str(), ":", effects);
      if (effects.size() != 4) {
        LOG_ERROR("配置错误 [cell-noble_effects] id(%d), 文件(%s)",
            id, file.c_str());
        return false;
      }
      SoulEffectValue soul_effect_value;
      soul_effect_value.type_ = (entity::SoulEffectType::type)effects[1];
      if (soul_effect_value.type_ <= entity::SoulEffectType::MIN ||
          soul_effect_value.type_ >= entity::SoulEffectType::MAX) {
        LOG_ERROR("配置错误 [cell-noble_effects] id(%d), 文件(%s)",
            id, file.c_str());
        return false;
      }
      soul_effect_value.param1_ = effects[2];
      soul_effect_value.param2_ = effects[3];
      SoulCell::EffectMap::iterator iterator = cell.noble_effects_.find(effects[0]);
      if (iterator != cell.noble_effects_.end()) {
        iterator->second.push_back(soul_effect_value);
      } else {
        SoulCell::EffectVector effect_vector;
        effect_vector.push_back(soul_effect_value);
        cell.noble_effects_.insert(std::make_pair(effects[0], effect_vector));
      }
    }

    this->souls_.insert(std::make_pair(id, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const SoulCell *LoadSoulConfigure::GetSoul(core::uint32 id) const {
  SoulHashmap::const_iterator iterator = this->souls_.find(id);
  if (iterator != this->souls_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace soul

}  // namespace server

}  // namespace game

