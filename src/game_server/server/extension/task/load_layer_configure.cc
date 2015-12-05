//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-25 21:36:26.
// File name: load_layer_configure.cc
//
// Description:
//
#include "game_server/server/extension/task/load_layer_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

LoadLayerConfigure::LoadLayerConfigure() {}
LoadLayerConfigure::~LoadLayerConfigure() {}

bool LoadLayerConfigure::LoadConfigure(const std::string &file) {
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
    TaskLayerCell cell;
    if(cell_node->Attribute("task_id", (int* )&cell.task_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-task_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(this->layers_.find(cell.task_id_) != this->layers_.end()) {
      LOG_ERROR("配置文件错误[%d]",cell.task_id_);
      return false;
    }

    if(cell_node->Attribute("id", (int *)&cell.layer_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-layer]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("physique_add_value", 
          (int *)&cell.physique_add_value_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-physique_add_value]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("strength_add_value", (int *)&cell.strength_add_value_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-strength_add_value]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("intelligence_add_value", 
          (int *)&cell.intelligence_add_value_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-intelligence_add_value_]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("agile_add_value", (int *)&cell.agile_add_value_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-agile_add_value]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("spirit_add_value", 
          (int *)&cell.spirit_add_value_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-spirit_add_value]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    this->layers_.insert(std::make_pair(cell.task_id_, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const TaskLayerCell *LoadLayerConfigure::GetCell(core::uint32 task_id) const {
  LoadLayerConfigure::TaskLayerMap::const_iterator iterator
    = this->layers_.find(task_id);
  if(iterator == this->layers_.end()) {
    // LOG_ERROR("配置文件出错,[%d]", task_id);
    return NULL;
  }
  return &(iterator->second);
}

}  // namespace task

}  // namespace server

}  // namespace game

