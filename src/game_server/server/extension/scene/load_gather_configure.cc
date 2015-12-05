//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-08 01:12:27.
// File name: load_gather_configure.cc
//
// Description:
//

#include "game_server/server/extension/scene/load_gather_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

LoadGatherConfigure::LoadGatherConfigure() {}
LoadGatherConfigure::~LoadGatherConfigure() {}

bool LoadGatherConfigure::LoadConfigure(const std::string &file) {
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
    GatherCell cell;
    if(cell_node->Attribute("id", (core::int32 *)&cell.id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-npc]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    GatherHashmap::iterator iterator = this->gathers_.find(cell.id_);
    if(iterator != this->gathers_.end()) {
      global::LogError("%s:%d (%s) id(%d) 配置重复，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, cell.id_, file.c_str());
      return false;
    }
    if(cell_node->Attribute("template_id", &cell.template_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-template_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("num", &cell.num_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-num]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buff", &cell.buff_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buff]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    this->gathers_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const GatherCell *LoadGatherConfigure::GetCell(core::uint32 id) const {
  GatherHashmap::const_iterator iterator = this->gathers_.find(id);
  if(iterator != this->gathers_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace scene

}  // namespace server

}  // namespace game

