
#include "game_server/server/extension/welfare/load_task_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "entity/achieve_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

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

  const char *str = NULL;
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    AchieveTaskCell cell;
    std::vector<core::int32> conditions;
    std::vector<core::int32> awards;
    if(cell_node->Attribute("id", &cell.id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("achieve_point", (int *)&cell.achieve_point_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-achieve_point]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("title", (int *)&cell.title_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-title]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    str = cell_node->Attribute("awards");
    if(str == NULL) {
      LOG_ERROR("奖励配置没有找到");
      return false;
    }
    awards.clear();
    global::StringSplit(str, ":", awards);
    if(awards.size() == 4) {
      cell.award_.type_ = awards[0];
      cell.award_.id_   = awards[1];
      cell.award_.number_ = awards[2];
      cell.award_.bind_ = awards[3];
    }
    str = cell_node->Attribute("condtion1");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-condtion1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    conditions.clear();
    global::StringSplit(str, ":", conditions);
    if(conditions.size() >= 1) {
      ConditionCell condition;
      condition.type_ = conditions[0]; 
      if(conditions.size() >= 2) {
        condition.param1_ = conditions[1];
      }
      if(conditions.size() >= 3) {
        condition.param2_ = conditions[2];
      }
      if(conditions.size() >= 4) {
        condition.param3_ = conditions[3];
      }
      if(condition.param3_ == 0) {
        condition.param3_ = 1;
      }
      if(condition.type_  == (core::int32)entity::AchieveConditionType::FUNCTIONALITY_OPEN ) {
        condition.param1_ = game::server::Configure::GetInstance()->GetServerDefineByFunctionality(condition.param1_);
      }
      cell.conditions_.push_back(condition);
    }

    str = cell_node->Attribute("condtion2");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-condtion2]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    conditions.clear();
    global::StringSplit(str, ":", conditions);
    if(conditions.size() >= 1) {
      ConditionCell condition;
      condition.type_ = conditions[0]; 
      if(conditions.size() >= 2) {
        condition.param1_ = conditions[1];
      } 
      if(conditions.size() >= 3) {
        condition.param2_ = conditions[2];
      }
      if(conditions.size() >= 4) {
        condition.param3_ = conditions[3];
      }
      if(condition.param3_ == 0) {
        condition.param3_ = 1;
      }
      if(condition.type_  == (core::int32)entity::AchieveConditionType::FUNCTIONALITY_OPEN ) {
        condition.param1_ = game::server::Configure::GetInstance()->GetServerDefineByFunctionality(condition.param1_);
      }
      cell.conditions_.push_back(condition);
    }

    if(cell_node->Attribute("physique", (int *)&cell.physique_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-physique]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("strength", (int *)&cell.strength_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-strength]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("intelligence", (int *)&cell.intelligence_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-intelligence]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("agile", (int *)&cell.agile_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-agile]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("spirit", (int *)&cell.spirit_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-spirit]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    this->achieve_map_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

}  // namespace welfare

}  // namespace server

}  // namespace game

