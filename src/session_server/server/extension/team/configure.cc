//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-09 17:21:57.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "session_server/server/extension/team/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace session {

namespace server {

namespace team {

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;

  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s) 出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 configure 结点
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 team_pool 结点
  TiXmlElement *team_pool_node = configure_node->FirstChildElement("team_pool");
  if(team_pool_node == NULL) {
    LOG_ERROR("找不到结点 [configure-team_pool]，文件(%s)", file.c_str());
    return false;
  }

  if(team_pool_node->Attribute("initial_number",
        (int *)(&this->team_initial_number_)) == false) {
    LOG_ERROR("找不到属性 [team_pool-initial_number]，文件(%s)", file.c_str());
    return false;
  }

  if(team_pool_node->Attribute("extend_number",
        (int *)(&this->team_extend_number_)) == false) {
    LOG_ERROR("找不到属性 [team_pool-extend_number]，文件(%s)", file.c_str());
    return false;
  }

  const char *str = NULL;

  TiXmlElement *team_node = configure_node->FirstChildElement("team");
  if(team_node == NULL) {
    LOG_ERROR("找不到结点 [configure-team]，文件(%s)", file.c_str());
    return false;
  }
  str = team_node->Attribute("file");
  if(str == NULL) {
    LOG_ERROR("找不到属性 [team-file]，文件(%s)", file.c_str());
    return false;
  }

  if(this->load_team_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    LOG_ERROR("加载 team 配置失败");
    return false;
  }

  return true;
}

}  // namespace team

}  // namespace server

}  // namespace session

