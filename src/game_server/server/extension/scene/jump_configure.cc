//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-13 19:31:37.
// File name: jump_configure.cc
//
// Description:
// Define class JumpConfigure.
//

#include "game_server/server/extension/scene/jump_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

JumpConfigure::JumpConfigure() {}
JumpConfigure::~JumpConfigure() {}

bool JumpConfigure::LoadConfigure(const std::string &file) {
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

  int id = 0;
  const char *str = NULL;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    JumpCell cell;
    if(cell_node->Attribute("npc", &id) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-npc]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    JumpHashmap::iterator iterator = this->jumps_.find(id);
    if(iterator != this->jumps_.end()) {
      global::LogError("%s:%d (%s) id(%d) 配置重复，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
      return false;
    }

    str = cell_node->Attribute("map_dest");
    std::vector<core::int32> dest;
    global::StringSplit(str, "+", dest);
    if(dest.size() != 3) {
      global::LogError("%s:%d (%s) 配置错误 [cell-map_dest]，npc(%d) 文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
      return false;
    }
    cell.dest_map_ = dest[0];
    cell.dest_x_ = dest[1];
    cell.dest_y_ = dest[2];

    this->jumps_.insert(std::make_pair(id, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const JumpCell *JumpConfigure::GetJump(core::uint32 id) const {
  JumpHashmap::const_iterator iterator = this->jumps_.find(id);
  if(iterator != this->jumps_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace scene

}  // namespace server

}  // namespace game

