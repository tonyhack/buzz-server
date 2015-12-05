
#include "game_server/server/extension/task/load_task_playing.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension_manager.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

LoadTaskPlayingConfigure::LoadTaskPlayingConfigure() {}
LoadTaskPlayingConfigure::~LoadTaskPlayingConfigure() {}

bool LoadTaskPlayingConfigure::LoadConfigure(const std::string &file) {
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
    TaskPlayingCell cell;
    std::vector<core::int32> tempVecs;
    core::int32 id = 0;
    if(cell_node->Attribute("id", &id) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    str = cell_node->Attribute("map_ids");
    if(str == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-map_ids]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    tempVecs.clear();
    global::StringSplit(str, "+", tempVecs);
    for(core::uint32 loop = 0; loop < tempVecs.size(); ++loop) {
      cell.maps_.push_back(tempVecs[loop]);
    }

    this->maps_.insert(std::make_pair(id, cell));
    cell_node = cell_node->NextSiblingElement("cell");
  }
  return true;
}

bool LoadTaskPlayingConfigure::Check(core::int32 id, core::int32 map_id) const {
  Maps::const_iterator itr = this->maps_.find(id);
  if(itr == this->maps_.end()) {
    return false;
  }
  for(core::uint32 i = 0; i < itr->second.maps_.size(); ++i) {
    if(itr->second.maps_[i] == map_id) {
      return true;
    }
  }
  return false;
}

}  // namespace task

}  // namespace server

}  // namespace game

