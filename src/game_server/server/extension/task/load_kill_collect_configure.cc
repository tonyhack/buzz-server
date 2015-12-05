//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 15:35:21.
// File name: load_kill_collect_configure.cc
//
// Description:
// Define class LoadKillCollectConfigure.
//

#include "game_server/server/extension/task/load_kill_collect_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

LoadKillCollectConfigure::LoadKillCollectConfigure() {}
LoadKillCollectConfigure::~LoadKillCollectConfigure() {}

bool LoadKillCollectConfigure::LoadConfigure(const std::string &file) {
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

  KillCollectCell cell;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    if(cell_node->Attribute("id", &cell.task_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("template_id", (int *)&cell.template_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-template_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("monster_id", (int *)&cell.npc_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-monster_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("odds", (int *)&cell.odds_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-odds]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    this->cells_.insert(std::make_pair( cell.template_id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const KillCollectCell *LoadKillCollectConfigure::GetKillCollect(const
    core::uint32 key) const {
  KillCollectmap::const_iterator iterator = this->cells_.find(key);
  if(iterator != this->cells_.end()) {
    return &iterator->second;
  }

  return NULL;
}

}  // namespace task

}  // namespace server

}  // namespace game

