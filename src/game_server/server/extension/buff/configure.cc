//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 23:45:44.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "game_server/server/extension/buff/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/server_configure.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s) 失败", file.c_str());
    return false;
  }

  // 加载 configure 结点
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    LOG_ERROR("找不到结点 [configure]，文件(%s)", file.c_str());
    return false;
  }

  // 加载 buff 结点
  TiXmlElement *buff_node = configure_node->FirstChildElement("buff");
  if(buff_node == NULL) {
    LOG_ERROR("找不到结点 [configure-buff]，文件(%s)", file.c_str());
    return false;
  }
  const char *buff_file = buff_node->Attribute("file");
  if(buff_file == NULL) {
    LOG_ERROR("找不到属性 [buff-file]，文件(%s)", file.c_str());
    return false;
  }

  // 加载 buff 配置
  if(this->LoadBuffConfigure(global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        buff_file) == false) {
    LOG_ERROR("加载 buff 配置失败，文件(%s)", file.c_str());
    return false;
  }

  // 加载 buff_pool 结点
  TiXmlElement *buff_pool_node = configure_node->FirstChildElement("buff_pool");
  if(buff_pool_node == NULL) {
    LOG_ERROR("找不到结点 [configure-buff_pool]，文件(%s)", file.c_str());
    return false;
  }
  if(buff_pool_node->Attribute("initial", (int *)&this->buff_pool_.initial_number_) == NULL) {
    LOG_ERROR("找不到属性 [buff_pool-initial]，文件(%s)", file.c_str());
    return false;
  }
  if(buff_pool_node->Attribute("extend", (int *)&this->buff_pool_.extend_number_) == NULL) {
    LOG_ERROR("找不到属性 [buff_pool-extend]，文件(%s)", file.c_str());
    return false;
  }

  // 加载 buff_condition 结点
  TiXmlElement *buff_condition_node = configure_node->FirstChildElement("buff_condition");
  if(buff_condition_node == NULL) {
    LOG_ERROR("找不到结点 [configure-buff_condition]，文件(%s)", file.c_str());
    return false;
  }
  const char *buff_condition_file = buff_condition_node->Attribute("file");
  if(buff_condition_file == NULL) {
    LOG_ERROR("找不到属性 [buff_condition-file]，文件(%s)", file.c_str());
    return false;
  }

  // 加载 buff condition 配置
  if(this->LoadConditionConfigure(global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
    buff_condition_file) == false) {
    LOG_ERROR("加载 buff condition 配置失败，文件(%s)", file.c_str());
    return false;
  }

  // 加载 buff_condition_pool 结点
  TiXmlElement *buff_condition_pool_node = configure_node->FirstChildElement("buff_condition_pool");
  if(buff_condition_pool_node == NULL) {
    LOG_ERROR("找不到结点 [configure-buff_condition_pool]，文件(%s)", file.c_str());
    return false;
  }
  if(buff_condition_pool_node->Attribute("initial", (int *)&this->condition_pool_.initial_number_) == NULL) {
    LOG_ERROR("找不到属性 [buff_pool-initial]，文件(%s)", file.c_str());
    return false;
  }
  if(buff_condition_pool_node->Attribute("extend", (int *)&this->condition_pool_.extend_number_) == NULL) {
    LOG_ERROR("找不到属性 [buff_pool-extend], 文件(%s)", file.c_str());
    return false;
  }

  return true;
}

bool Configure::LoadConditionConfigure(const std::string &file) {
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
  if(cell_node == NULL) {
    LOG_ERROR("找不到结点[data-cell]，文件(%s)", file.c_str());
    return false;
  }

  BuffConditionCell cell;
  int id = 0;
  int value = 0;

  while(cell_node) {
    if(cell_node->Attribute("id", &id) == NULL) {
      LOG_ERROR("找不到属性[cell-id]，文件(%s)", file.c_str());
      return false;
    }
    if(this->conditions_.find(id) != this->conditions_.end()) {
      LOG_ERROR("BUFF条件(%d) 配置重复，文件(%s)", id, file.c_str());
      return false;
    }
    if(cell_node->Attribute("type", &value) == NULL) {
      LOG_ERROR("找不到属性[cell-type]，文件(%s)", file.c_str());
      return false;
    }
    if(value <= entity::BuffConditionType::MIN || value >= entity::BuffConditionType::MAX) {
      LOG_ERROR("属性[cell-type] 值(%d) 错误，文件(%s)", value, file.c_str());
      return false;
    }
    cell.type_ = (entity::BuffConditionType::type)value;
    if(cell_node->Attribute("param1", (int *)&cell.param1_) == NULL) {
      LOG_ERROR("找不到属性[cell-param1]，文件(%s)", file.c_str());
      return false;
    }

    this->conditions_.insert(std::make_pair(id, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool Configure::LoadBuffConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("加载配置文件(%s)失败", file.c_str());
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
  if(cell_node == NULL) {
    LOG_ERROR("找不到结点[data-cell]，文件(%s)", file.c_str());
    return false;
  }

  int value = 0;

  while(cell_node) {
    BuffCell cell;
    if(cell_node->Attribute("buff_id", (int *)&cell.id_) == NULL) {
      LOG_ERROR("找不到属性[cell-buff_id]，文件(%s)", file.c_str());
      return false;
    }
    if(this->buffs_.find(cell.id_) != this->buffs_.end()) {
      LOG_ERROR("BUFF(%d) 配置重复，文件(%s)", cell.id_, file.c_str());
      return false;
    }
    if(cell_node->Attribute("type", &value) == NULL) {
      LOG_ERROR("找不到属性[cell-type]，文件(%s)", file.c_str());
      return false;
    }
    if(value <= entity::BuffType::MIN || value >= entity::BuffType::MAX) {
      LOG_ERROR("属性[cell-type] 值(%d) 错误，文件(%s)", value, file.c_str());
      return false;
    }
    cell.type_ = (entity::BuffType::type)value;
    if(cell_node->Attribute("clear_group", (int *)&cell.clear_group_) == NULL) {
      LOG_ERROR("找不到属性[cell-clear_group]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("condition_group", (int *)&cell.condition_group_) == NULL) {
      LOG_ERROR("找不到属性[cell-condition_group]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("buff_group", (int *)&cell.group_) == NULL) {
      LOG_ERROR("找不到属性[cell-buff_group]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("group_level", (int *)&cell.group_level_) == NULL) {
      LOG_ERROR("找不到属性[cell-group_level]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("boardcast_types", &value) == NULL) {
      LOG_ERROR("找不到属性[cell-boardcast_types]，文件(%s)", file.c_str());
      return false;
    }
    if(value <= entity::BuffBroadcastType::MIN || value >= entity::BuffBroadcastType::MAX) {
      LOG_ERROR("属性[cell-boardcast_types] 值(%d) 错误，文件(%s)", value, file.c_str());
      return false;
    }
    cell.broadcast_type_ = (entity::BuffBroadcastType::type)value;
    if(cell_node->Attribute("during_time", (int *)&cell.during_time_) == NULL) {
      LOG_ERROR("找不到属性[cell-during_time]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("stack_count", (int *)&cell.stack_count_) == NULL) {
      LOG_ERROR("找不到属性[cell-stack_count]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("buff_debuff", (int *)&cell.buff_debuff_) == NULL) {
      LOG_ERROR("找不到属性[cell-buff_debuff]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("interval_time", (int *)&cell.interval_time_) == NULL) {
      LOG_ERROR("找不到属性[cell-interval_time]，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("save_db", &value) == NULL) {
      LOG_ERROR("找不到属性[cell-save_db]，文件(%s)", file.c_str());
      return false;
    }
    cell.save_db_ = value != 0;

    cell.end_conditions_ = 0;
    std::vector<core::int32> end_conditions_vector;
    const char *end_conditions_str = cell_node->Attribute("end_conditions");
    if(end_conditions_str == NULL) {
      LOG_ERROR("找不到属性[cell-end_conditions]，文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(end_conditions_str, "+", end_conditions_vector);
    for(size_t pos = 0; pos < end_conditions_vector.size(); ++pos) {
      core::int32 value = end_conditions_vector[pos];
      if(value < 1 || value >= 32) {
        LOG_ERROR("BUFF(%d) 配置 end_conditions 错误", cell.id_);
        return false;
      }
      cell.end_conditions_ |= (value == 1 ? 1 : (1 << (value - 1)));
    }

    const char *effects_str = cell_node->Attribute("effects");
    if(effects_str == NULL) {
      LOG_ERROR("找不到属性[cell-effects]，文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(effects_str, "+", cell.effects_);

    const char *max_stack_effects_str = cell_node->Attribute("max_stack_effects");
    if(max_stack_effects_str == NULL) {
      LOG_ERROR("找不到属性[cell-max_stack_effects]，文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(max_stack_effects_str, "+", cell.max_stack_effects_);

    const char *effect_conditions_str = cell_node->Attribute("effect_conditions");
    if(effect_conditions_str == NULL) {
      LOG_ERROR("找不到属性[cell-effect_conditions]，文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(effect_conditions_str, "+", cell.effect_conditions_);

    const char *condition_effects_str = cell_node->Attribute("condition_effects");
    if(condition_effects_str == NULL) {
      LOG_ERROR("找不到属性[cell-condition_effects]，文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(condition_effects_str, "+", cell.condition_effects_);

    const char *buff_conditions_str = cell_node->Attribute("buff_conditions");
    if(buff_conditions_str == NULL) {
      LOG_ERROR("找不到属性[cell-buff_conditions]，文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(buff_conditions_str, "+", cell.buff_conditions_);

    const char *condition_buffs_str = cell_node->Attribute("condition_buffs");
    if(condition_buffs_str == NULL) {
      LOG_ERROR("找不到属性[cell-condition_buffs]，文件(%s)", file.c_str());
      return false;
    }
    std::vector<std::string> condition_buffs;
    global::StringSplit(condition_buffs_str, "+", condition_buffs);
    for(size_t pos = 0; pos < condition_buffs.size(); ++pos) {
      std::vector<core::int32> pairs;
      global::StringSplit(condition_buffs[pos].c_str(), ":", pairs);
      if(pairs.size() != 2) {
        LOG_ERROR("属性[cell-condition_buffs_] 错误，文件(%s)", file.c_str());
        return false;
      }
      cell.condition_buffs_.push_back(std::make_pair(pairs[0], pairs[1]));
    }

    const char *interval_effects_str = cell_node->Attribute("interval_effects");
    if(interval_effects_str == NULL) {
      LOG_ERROR("找不到属性[cell-interval_effects]，文件(%s)", file.c_str());
      return false;
    }
    global::StringSplit(interval_effects_str, "+", cell.interval_effects_);

    this->buffs_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const BuffCell *Configure::GetBuffCell(core::uint32 buff_id) const {
  BuffHashmap::const_iterator iterator = this->buffs_.find(buff_id);
  if(iterator != this->buffs_.end()) {
    return &iterator->second;
  }
  return NULL;
}

const BuffConditionCell *Configure::GetConditionCell(core::int32 id) const {
  ConditionHashmap::const_iterator iterator = this->conditions_.find(id);
  if(iterator != this->conditions_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace buff

}  // namespace server

}  // namespace game

