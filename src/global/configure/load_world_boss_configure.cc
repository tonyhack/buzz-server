//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-04 21:18:06.
// File name: load_world_boss_configure.cc
//
// Description:
//

#include "global/configure/load_world_boss_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace global {

namespace configure {

LoadWorldBossConfigure::LoadWorldBossConfigure() {}
LoadWorldBossConfigure::~LoadWorldBossConfigure() {}

bool LoadWorldBossConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s) 出错",
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
    WorldBossCell cell;
    if(cell_node->Attribute("map_id", (core::int32 *)&cell.map_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-map_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    NpcHashMap::iterator iterator = this->npcs_.find(cell.map_id_);
    if(iterator != this->npcs_.end()) {
      global::LogError("%s:%d (%s) id(%d) 配置重复，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, cell.map_id_, file.c_str());
      return false;
    }
    if(cell_node->Attribute("template_id", (core::int32 *)&cell.template_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-template_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("x1", &cell.coordinates_[0].x_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-x1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("y1", &cell.coordinates_[0].y_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-y1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("width1", &cell.coordinates_[0].width_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-width1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("height1", &cell.coordinates_[0].heigth_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-height1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("x2", &cell.coordinates_[1].x_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-x2]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("y2", &cell.coordinates_[1].y_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-y2]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("width2", &cell.coordinates_[1].width_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-width2]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("height2", &cell.coordinates_[1].heigth_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-height2]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("x3", &cell.coordinates_[2].x_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-x3]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("y3", &cell.coordinates_[2].y_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-y3]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("width3", &cell.coordinates_[2].width_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-width3]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("height3", &cell.coordinates_[2].heigth_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-height3]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("x4", &cell.coordinates_[3].x_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-x4]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("y4", &cell.coordinates_[3].y_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-y4]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("width4", &cell.coordinates_[3].width_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-width4]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("height4", &cell.coordinates_[3].heigth_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-height4]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    this->npcs_.insert(std::make_pair(cell.map_id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const WorldBossCell *LoadWorldBossConfigure::GetCell(core::int32 map_id) const {
  NpcHashMap::const_iterator iterator = this->npcs_.find(map_id);
  if(iterator != this->npcs_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace global

}  // namespace configure
