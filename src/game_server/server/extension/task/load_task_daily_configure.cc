
#include "game_server/server/extension/task/load_task_daily_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

LoadTaskDailyConfigure::LoadTaskDailyConfigure() {}
LoadTaskDailyConfigure::~LoadTaskDailyConfigure() {}

bool LoadTaskDailyConfigure::LoadConfigure(const std::string &file) {
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
    TaskDailyCell cell;
    std::vector<core::int32> conditions;
    std::vector<core::int32> rewards;
    if(cell_node->Attribute("id", &cell.id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("award", (int *)&cell.award_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-award]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("front_id", (int *)&cell.front_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-front_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("spend", (int *)&cell.spend_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-spend]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    str = cell_node->Attribute("condition");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-condition]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    conditions.clear();
    global::StringSplit(str, ":", conditions);
    if(conditions.size() == 3) {
      cell.condition_.type_ = conditions[0];
      cell.condition_.param1_ = conditions[1];
      cell.condition_.param2_ = conditions[2];
    } else {
      LOG_ERROR("配置文件错误[%s]", file.c_str());
      return false;
    }

    this->tasks_.insert(std::make_pair(cell.id_, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }
  return true;
}

const TaskDailyCell *LoadTaskDailyConfigure::GetCell(const core::int32 task_id) const {
  TaskDailyMap::const_iterator itr = this->tasks_.find(task_id);
  if(itr != this->tasks_.end()) {
    return &(itr->second);
  }
  return NULL;
}

const LoadTaskDailyConfigure::TaskDailyMap &LoadTaskDailyConfigure::GetDailyList() const {
  return this->tasks_;
}

}  // namespace task

}  // namespace server

}  // namespace game

