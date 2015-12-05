//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 14:30:57.
// File name: load_task_configure.cc
//
// Description:
// Define class LoadTaskConfigure.
//

#include "game_server/server/extension/task/load_task_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

LoadTaskConfigure::LoadTaskConfigure() {}
LoadTaskConfigure::~LoadTaskConfigure() {}

bool LoadTaskConfigure::LoadConfigure(const std::string &file) {
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

  int id = 0;
  const char *str = NULL;
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    TaskCell cell;
    std::vector<core::int32> conditions;
    std::vector<core::int32> rewards;
    std::vector<core::int32> npcs;
    std::vector<core::int32> coordintes;

    if(cell_node->Attribute("id", &id) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(this->tasks_.find(id) != this->tasks_.end()) {
      global::LogError("%s:%d (%s) 任务ID(%d) 配置重复",
          __FILE__, __LINE__, __FUNCTION__, id);
      return false;
    }

    if(cell_node->Attribute("level", (int *)&cell.min_level_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-level]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("pre_task", (int *)&cell.pre_task_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-pre_task]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("can_drop", (int *)&cell.can_giveup_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-can_drop]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    
    str = cell_node->Attribute("accept_npc");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-accept_npc]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    npcs.clear();
    global::StringSplit(str, "+", npcs);
    if(npcs.size() == 4) {
      cell.accept_npc_ = npcs[0];
      cell.accept_coordinate_.map_ = npcs[1];
      cell.accept_coordinate_.x_ = npcs[2];
      cell.accept_coordinate_.y_ = npcs[3];
    }

    str = cell_node->Attribute("finish_npc");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-finish_npc]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    npcs.clear();
    global::StringSplit(str, "+", npcs);
    if(npcs.size() == 4) {
      cell.finish_npc_ = npcs[0];
      cell.finish_coordinate_.map_ = npcs[1];
      cell.finish_coordinate_.x_ = npcs[2];
      cell.finish_coordinate_.y_ = npcs[3];
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
      cell.condition_[0].type_ = conditions[1];
      cell.condition_[0].param1_ = conditions[2];
      cell.condition_[0].param2_ = conditions[3];

      str = cell_node->Attribute("position1");
      if(str == NULL) {
        LOG_ERROR("找不到属性 [cell-position1], 文件(%s)", file.c_str());
        return false;
      }
      coordintes.clear();
      global::StringSplit(str, "+", coordintes);
      if(coordintes.size() == 3) {
        TaskCoordinate coordx;
        coordx.map_ = coordintes[0];
        coordx.x_ = coordintes[1];
        coordx.y_ = coordintes[2];
        cell.condition_coordinates_.push_back(coordx);
      } else {
        TaskCoordinate coordx;
        cell.condition_coordinates_.push_back(coordx);
      }
    }

    str = cell_node->Attribute("condition2");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-condition2]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    conditions.clear();
    global::StringSplit(str, "+", conditions);
    if(conditions.size() == 4) {
      cell.condition_[1].type_ = conditions[1];
      cell.condition_[1].param1_ = conditions[2];
      cell.condition_[1].param2_ = conditions[3];

      str = cell_node->Attribute("position2");
      if(str == NULL) {
        LOG_ERROR("找不到属性 [cell-position2], 文件(%s)", file.c_str());
        return false;
      }
      coordintes.clear();
      global::StringSplit(str, "+", coordintes);
      if(coordintes.size() == 3) {
        TaskCoordinate coordx;
        coordx.map_ = coordintes[0];
        coordx.x_ = coordintes[1];
        coordx.y_ = coordintes[2];
        cell.condition_coordinates_.push_back(coordx);
      } else {
        TaskCoordinate coordx;
        cell.condition_coordinates_.push_back(coordx);
      }
    }

    str = cell_node->Attribute("condition3");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-condition3]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    conditions.clear();
    global::StringSplit(str, "+", conditions);
    if(conditions.size() == 4) {
      cell.condition_[2].type_ = conditions[1];
      cell.condition_[2].param1_ = conditions[2];
      cell.condition_[2].param2_ = conditions[3];

      str = cell_node->Attribute("position3");
      if(str == NULL) {
        LOG_ERROR("找不到属性 [cell-position3], 文件(%s)", file.c_str());
        return false;
      }
      coordintes.clear();
      global::StringSplit(str, "+", coordintes);
      if(coordintes.size() == 3) {
        TaskCoordinate coordx;
        coordx.map_ = coordintes[0];
        coordx.x_ = coordintes[1];
        coordx.y_ = coordintes[2];
        cell.condition_coordinates_.push_back(coordx);
      } else {
        TaskCoordinate coordx;
        cell.condition_coordinates_.push_back(coordx);
      }
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
      cell.award_[0].type_ = rewards[0];
      cell.award_[0].id_ = rewards[1];
      cell.award_[0].number_ = rewards[2];
      cell.award_[0].bind_ = rewards[3];
      cell.award_[0].intensify_level_ = rewards[4];
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
      cell.award_[1].type_ = rewards[0];
      cell.award_[1].id_ = rewards[1];
      cell.award_[1].number_ = rewards[2];
      cell.award_[1].bind_ = rewards[3];
      cell.award_[1].intensify_level_ = rewards[4];
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
      cell.award_[2].type_ = rewards[0];
      cell.award_[2].id_ = rewards[1];
      cell.award_[2].number_ = rewards[2];
      cell.award_[2].bind_ = rewards[3];
      cell.award_[2].intensify_level_ = rewards[4];
    }
    std::vector<core::int32> pair;
    std::vector<std::string> choose_rewards;

    str = cell_node->Attribute("choose_reward");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-choose_reward]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    choose_rewards.clear();
    global::StringSplit(str, "+", choose_rewards);
    for(size_t pos = 0; pos < choose_rewards.size() && pos < 5; ++pos) {
      pair.clear();
      global::StringSplit(choose_rewards[pos].c_str(), ":", pair);
      if(pair.size() != 5) {
        LOG_ERROR("任务(%u) choose_reward 配置错误", id);
        return false;
      }
      cell.award_item_[pos].type_ = pair[0];
      cell.award_item_[pos].id_   = pair[1];
      cell.award_item_[pos].number_ = pair[2];
      cell.award_item_[pos].bind_   = pair[3];
      cell.award_item_[pos].intensify_level_ = pair[4];
    }

    this->tasks_.insert(std::make_pair(id, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const TaskCell *LoadTaskConfigure::GetTask(core::uint32 id) const {
  TaskHashmap::const_iterator iterator = this->tasks_.find(id);
  if(iterator != this->tasks_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace task

}  // namespace server

}  // namespace game

