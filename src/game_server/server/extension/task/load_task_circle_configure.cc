//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-15 00:23:54.
// File name: load_task_circle_configure.cc
//
// Description:
//
#include "game_server/server/extension/task/load_task_circle_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace task {

LoadTaskCircleConfigure::LoadTaskCircleConfigure() {}
LoadTaskCircleConfigure::~LoadTaskCircleConfigure() {}

bool LoadTaskCircleConfigure::LoadConfigure(const std::string &file) {
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
    TaskCircleCell cell;
    std::vector<core::int32> rewards;
    TaskCircleKey key;

    if(cell_node->Attribute("id", &key.id_) == false) {
      LOG_ERROR("cell->[id]没有找到");
      return false;
    }
    if(cell_node->Attribute("circle", &key.circle_num_) == false) {
      LOG_ERROR("cell->[circle]没有找到");
      return false;
    }
    if(cell_node->Attribute("refresh_condition", &cell.refresh_condition_) == false) {
      LOG_ERROR("cell->[refresh_condition]没有找到");
      return false;
    }
    if(cell_node->Attribute("jump_spend_gold", &cell.jump_spend_gold_) == false) {
      LOG_ERROR("cell->[jump_spend_gold]没有找到");
      return false;
    }
    if(cell_node->Attribute("star_coe", &cell.star_coe_) == false) {
      LOG_ERROR("cell->[jump_spend_gold]没有找到");
      return false;
    }

    str = cell_node->Attribute("reward1");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-reward1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    rewards.clear();
    global::StringSplit(str, ":", rewards);
    if(rewards.size() == 5) {
      TaskAward award;
      award.type_ = rewards[0];
      award.id_ = rewards[1];
      award.number_ = rewards[2];
      award.bind_ = rewards[3];
      award.intensify_level_ = rewards[4];
      cell.awards_.push_back(award);
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
      TaskAward award;
      award.type_ = rewards[0];
      award.id_ = rewards[1];
      award.number_ = rewards[2];
      award.bind_ = rewards[3];
      award.intensify_level_ = rewards[4];
      cell.awards_.push_back(award);
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
      TaskAward award;
      award.type_ = rewards[0];
      award.id_ = rewards[1];
      award.number_ = rewards[2];
      award.bind_ = rewards[3];
      award.intensify_level_ = rewards[4];
      cell.awards_.push_back(award);
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
      TaskAward award;
      award.type_ = rewards[0];
      award.id_ = rewards[1];
      award.number_ = rewards[2];
      award.bind_ = rewards[3];
      award.intensify_level_ = rewards[4];
      cell.awards_.push_back(award);
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
      TaskAward award;
      award.type_ = rewards[0];
      award.id_ = rewards[1];
      award.number_ = rewards[2];
      award.bind_ = rewards[3];
      award.intensify_level_ = rewards[4];
      cell.awards_.push_back(award);
    }
    this->task_circles_.insert(LoadTaskCircleConfigure::CircleMap::value_type(key, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }
  return true;
}

const TaskCircleCell* LoadTaskCircleConfigure::GetTaskCircleCell(const TaskCircleKey& key) {
  LoadTaskCircleConfigure::CircleMap::const_iterator const_itr = 
        this->task_circles_.find(key);
  if(const_itr == this->task_circles_.end()) {
     return NULL;
  }
  return &const_itr->second;
}

}  // task
 
}  // server

}  // game
