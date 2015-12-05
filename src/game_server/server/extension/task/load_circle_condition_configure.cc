//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-15 02:34:31.
// File name: load_circle_condition_configure.cc
//
// Description:
//

#include "game_server/server/extension/task/load_circle_condition_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

LoadCircleConditionConfigure::LoadCircleConditionConfigure() {}
LoadCircleConditionConfigure::~LoadCircleConditionConfigure() {}

bool LoadCircleConditionConfigure::LoadConfigure(const std::string &file) {
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
    ConditionCell cell;
    std::vector<core::int32> conditions;
    std::vector<core::int32> coordintes;
    if(cell_node->Attribute("id", &cell.id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("star", (int *)&cell.star_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-star]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("order_id", (int *)&cell.order_id_) == false) {
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
      TaskCoordinate coordx;
      cell.coordinate_.map_ = coordintes[0];
      cell.coordinate_.x_ = coordintes[1];
      cell.coordinate_.y_ = coordintes[2];
    } else {
      LOG_ERROR("配置文件错误[%s]", file.c_str());
      return false;
    }

    ConditionRandMap::iterator itr = this->task_rand_conditions_.find(cell.id_);
    if(itr == this->task_rand_conditions_.end()) {
      ConditionVec conditions;
      conditions.push_back(cell);
      this->task_rand_conditions_.insert(std::make_pair(cell.id_, conditions));
    } else {
      itr->second.push_back(cell);
    }
    this->task_conditions_.insert(std::make_pair(cell.order_id_, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }
  return true;
}

const ConditionCell *LoadCircleConditionConfigure::RandCondition(core::int32 level) const {
  LoadCircleConditionConfigure::ConditionRandMap::const_iterator iterator
       = this->task_rand_conditions_.find(level);
  if(iterator == this->task_rand_conditions_.end()) {
     LOG_ERROR("配置文件出错,lv[%d]", level);
     return NULL;
  }
  
  // 随机一个条件
  core::uint32 rand =
    ExtensionManager::GetInstance()->GetRandom().Random(iterator->second.size());
  if(rand >= iterator->second.size()) {
    LOG_ERROR("随机出错");
    return NULL;
  }
  return &(iterator->second[rand]);
}

const ConditionCell *LoadCircleConditionConfigure::GetCondition(core::int32 id) const {
  LoadCircleConditionConfigure::ConditionMap::const_iterator iterator
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

