//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-09 15:50:25.
// File name: load_team_configure.cc
//
// Description:
// Define class LoadTeamConfigure.
//

#include "session_server/server/extension/team/load_team_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace session {

namespace server {

namespace team {

LoadTeamConfigure::LoadTeamConfigure() {}
LoadTeamConfigure::~LoadTeamConfigure() {}

bool LoadTeamConfigure::LoadConfigure(const std::string &file) {
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

  core::int32 number = 1;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    TeamCell cell;
    if(cell_node->Attribute("number", (int *)&cell.number_) == false) {
      LOG_ERROR("找不到属性 [data-number]，文件(%s)", file.c_str());
      return false;
    }
    if(cell.number_ <= 0 || cell.number_ != number++) {
      LOG_ERROR("属性 [data-number] 错误，文件(%s)", file.c_str());
      return false;
    }
    if(cell_node->Attribute("buff", (int *)&cell.buff_) == false) {
      LOG_ERROR("找不到属性 [data-buff]，文件(%s)", file.c_str());
      return false;
    }
    this->teams_.push_back(cell);
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const TeamCell *LoadTeamConfigure::GetTeam(core::uint32 number) {
  if(number <= 0 || number > this->teams_.size()) {
    return NULL;
  }

  return &this->teams_[number - 1];
}

}  // namespace team

}  // namespace server

}  // namespace session

