//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-24 01:08:15.
// File name: load_summon_npc_configure.cc
//
// Description:
//

#include "game_server/server/extension/task/load_summon_npc_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

LoadSummonNpcConfigure::LoadSummonNpcConfigure() {}
LoadSummonNpcConfigure::~LoadSummonNpcConfigure() {}

bool LoadSummonNpcConfigure::LoadConfigure(const std::string &file) {
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
    SummonNpcCell cell;
    if(cell_node->Attribute("npc_id", &cell.npc_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-npc_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    str = cell_node->Attribute("summon_id");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-summon_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    std::vector<core::int32> npc_infos;
    global::StringSplit(str, "+", npc_infos);
    if(npc_infos.size() == 3) {
      cell.id_ = npc_infos[0];
      cell.x_ = npc_infos[1];
      cell.y_ = npc_infos[2];
    } else {
      LOG_ERROR("配置文件错误[%s]", file.c_str());
      return false;
    }
    if(this->npcs_.find(cell.id_) != this->npcs_.end()) {
      LOG_ERROR("配置文件有错误");
      return false;
    }
    this->npcs_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const SummonNpcCell *LoadSummonNpcConfigure::GetCell(core::int32 id) const {
  LoadSummonNpcConfigure::SummonNpcMap::const_iterator iterator
    = this->npcs_.find(id);
  if(iterator == this->npcs_.end()) {
    LOG_ERROR("配置文件出错,[%d]", id);
    return NULL;
  }
  return &(iterator->second);
}

}  // namespace task

}  // namespace server

}  // namespace game

