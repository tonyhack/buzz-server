#include "game_server/server/extension/scene/load_consecutive_kill_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

LoadConsecutiveKillConfigure::LoadConsecutiveKillConfigure() {}
LoadConsecutiveKillConfigure::~LoadConsecutiveKillConfigure() {}

bool LoadConsecutiveKillConfigure::LoadConfigure(const std::string &file) {
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

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    ConsecutiveKillCell cell;
    if(cell_node->Attribute("num", (core::int32 *)&cell.num_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-num]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    if(cell_node->Attribute("span_time", &cell.span_time_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-span_time]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(cell_node->Attribute("buff_id", &cell.buff_id_) == false) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-buff_id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }

    this->consecutive_kills_.push_back(cell);
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const ConsecutiveKillCell *LoadConsecutiveKillConfigure::GetNearCell(core::int32 kill_num) const {
  for(size_t i = 0; i < this->consecutive_kills_.size(); ++i) {
    if(this->consecutive_kills_[i].num_ > kill_num) {
      return &(this->consecutive_kills_[i]);
    }
  }
  return NULL;
}

const ConsecutiveKillCell *LoadConsecutiveKillConfigure::GetCell(core::int32 kill_num) const {
  for(size_t i = 0; i < this->consecutive_kills_.size(); ++i) {
    if(this->consecutive_kills_[i].num_ == kill_num)
      return &(this->consecutive_kills_[i]);
    if(this->consecutive_kills_[i].num_ > kill_num) {
      return NULL;
    }
  }
  return NULL;
}


}  // namespace scene

}  // namespace server

}  // namespace game

