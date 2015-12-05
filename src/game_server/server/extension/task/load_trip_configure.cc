//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-24 02:18:19.
// File name: load_trip_configure.cc
//
// Description:
//

#include "game_server/server/extension/task/load_trip_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

LoadTripConfigure::LoadTripConfigure() {}
LoadTripConfigure::~LoadTripConfigure() {}

bool LoadTripConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载配置文件(%s)出错",
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

  const char *str = NULL;
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    TripTaskCell cell;
    std::vector<core::int32> conditions;
    std::vector<core::int32> coordintes;
    std::vector<core::int32> rewards;
    if(cell_node->Attribute("id", &cell.id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("type", (int *)&cell.type_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("order_id", (int *)&cell.order_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-order_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("finish_spend_gold", (int *)&cell.spent_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-order_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    str = cell_node->Attribute("condition1");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-condition1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    conditions.clear();
    global::StringSplit(str, "+", conditions);
    if(conditions.size() == 4) {
      cell.condition_.type_ = conditions[1];
      cell.condition_.param1_ = conditions[2];
      cell.condition_.param2_ = conditions[3];

      str = cell_node->Attribute("position1");
      if(str == NULL) {
        LOG_ERROR("找不到属性 [cell-position1], 文件(%s)", file.c_str());
        return false;
      }
      coordintes.clear();
      global::StringSplit(str, "+", coordintes);
      if(coordintes.size() != 3) {
        LOG_ERROR("属性 [cell-position1] 不正确");
        return false;
      }
      cell.coordinate_.map_ = coordintes[0];
      cell.coordinate_.x_ = coordintes[1];
      cell.coordinate_.y_ = coordintes[2];
    } else {
      LOG_ERROR("配置文件错误[%s]", file.c_str());
      return false;
    }
    str = cell_node->Attribute("reward1");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    TaskAward temp_award;
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 5) {
      temp_award.type_ = rewards[0];
      temp_award.id_ = rewards[1];
      temp_award.number_ = rewards[2];
      temp_award.bind_ = rewards[3];
      temp_award.intensify_level_ = rewards[4];
      cell.awards_.push_back(temp_award);
    }

    str = cell_node->Attribute("reward2");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward2]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 5) {
      temp_award.type_ = rewards[0];
      temp_award.id_ = rewards[1];
      temp_award.number_ = rewards[2];
      temp_award.bind_ = rewards[3];
      temp_award.intensify_level_ = rewards[4];
      cell.awards_.push_back(temp_award);
    }

    str = cell_node->Attribute("reward3");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward3]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 5) {
      temp_award.type_ = rewards[0];
      temp_award.id_ = rewards[1];
      temp_award.number_ = rewards[2];
      temp_award.bind_ = rewards[3];
      temp_award.intensify_level_ = rewards[4];
      cell.awards_.push_back(temp_award);
    }

    str = cell_node->Attribute("reward4");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward4]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 5) {
      temp_award.type_ = rewards[0];
      temp_award.id_ = rewards[1];
      temp_award.number_ = rewards[2];
      temp_award.bind_ = rewards[3];
      temp_award.intensify_level_ = rewards[4];
      cell.awards_.push_back(temp_award);
    }

    str = cell_node->Attribute("reward5");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward5]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 5) {
      temp_award.type_ = rewards[0];
      temp_award.id_ = rewards[1];
      temp_award.number_ = rewards[2];
      temp_award.bind_ = rewards[3];
      temp_award.intensify_level_ = rewards[4];
      cell.awards_.push_back(temp_award);
    }
    
    TripTaskKey key;
    key.id_ = cell.id_;
    key.type_ = cell.type_;

    ConditionRandMap::iterator itr = this->task_rand_conditions_.find(key);
    if(itr == this->task_rand_conditions_.end()) {
      RandConditionVec rands;
      rands.push_back(cell);
      this->task_rand_conditions_.insert(std::make_pair(key, rands));
    } else {
      itr->second.push_back(cell);
    }
    this->task_conditions_.insert(std::make_pair(cell.order_id_, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }
  return true;
}

const LoadTripConfigure::RandConditionVec *LoadTripConfigure::GetRandCondition(
        core::int32 stage, core::int32 type) const {
  TripTaskKey key;
  key.id_ = stage;
  key.type_ = type;

  LoadTripConfigure::ConditionRandMap::const_iterator iterator
       = this->task_rand_conditions_.find(key);
  if(iterator == this->task_rand_conditions_.end()) {
     LOG_ERROR("配置文件出错,等级段[%d], 类型[%d]", stage, type);
     return NULL;
  }
  return &iterator->second;
}

const TripTaskCell *LoadTripConfigure::GetCondition(core::int32 id) const {
  LoadTripConfigure::ConditionMap::const_iterator iterator
       = this->task_conditions_.find(id);
  if(iterator == this->task_conditions_.end()) {
     LOG_ERROR("配置文件出错,[%d]", id);
     return NULL;
  }
  return &(iterator->second);
}

}  // namespace task

}  // namespace server

}  // namespace game

