//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-14 02:21:02.
// File name: load_world_jump_configure.cc
//
// Description:
//

#include "game_server/server/extension/scene/load_world_jump_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

LoadWorldJumpConfigure::LoadWorldJumpConfigure() {}
LoadWorldJumpConfigure::~LoadWorldJumpConfigure() {}

bool LoadWorldJumpConfigure::LoadConfigure(const std::string &file) {
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
    WorldJumpCell cell;
    if(cell_node->Attribute("map_id", (core::int32 *)&cell.map_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-map_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    JumpHashmap::iterator iterator = this->jumps_.find(cell.map_id_);
    if(iterator != this->jumps_.end()) {
      global::LogError("%s:%d (%s) map_id(%d) 配置重复，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, cell.map_id_, file.c_str());
      return false;
    }
    if(cell_node->Attribute("x", &cell.x_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-x]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("y", &cell.y_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-y]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    this->jumps_.insert(std::make_pair(cell.map_id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const WorldJumpCell *LoadWorldJumpConfigure::GetJumpCell(
    core::uint32 map_id) const{
  JumpHashmap::const_iterator iterator = this->jumps_.find(map_id);
  if(iterator != this->jumps_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace scene

}  // namespace server

}  // namespace game

