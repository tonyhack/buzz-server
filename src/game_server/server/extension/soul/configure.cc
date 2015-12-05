//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 19:21:36.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "game_server/server/extension/soul/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace soul {

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

  // 加载 pool 结点
  TiXmlElement *pool_node = configure_node->FirstChildElement("pool");
  if(pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(pool_node->Attribute("soul_initial", (int *)(&this->soul_pool_initial_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [pool-soul_initial]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(pool_node->Attribute("soul_extend", (int *)(&this->soul_pool_extend_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [pool-soul_extend]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 step 结点
  TiXmlElement *step_node = configure_node->FirstChildElement("step");
  if (step_node == NULL) {
    LOG_ERROR("找不到结点 [configure-level], 文件(%s)", file.c_str());
    return false;
  }
  const char *step_file = step_node->Attribute("file");
  if (step_file == NULL) {
    LOG_ERROR("找不到结点 [configure-level], 文件(%s)", file.c_str());
    return false;
  }
  if (this->load_soul_step_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() + step_file) == false) {
    LOG_ERROR("加载英灵 step 表失败");
    return false;
  }

  // 加载 level 结点
  TiXmlElement *level_node = configure_node->FirstChildElement("level");
  if(level_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-level]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *str = level_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [level-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_soul_level_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() + str) == false) {
    global::LogError("%s:%d (%s) 加载 英灵等级配置失败", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 skill_level 结点
  TiXmlElement *skill_level_node = configure_node->FirstChildElement("skill_level");
  if (skill_level_node == NULL) {
    LOG_ERROR("找不到结点 [configure-skill_level]，文件(%s)", file.c_str());
    return false;
  }
  const char *skill_level_file = skill_level_node->Attribute("file");
  if (skill_level_node == NULL) {
    LOG_ERROR("找不到属性 [level-file]，文件(%s)", file.c_str());
    return false;
  }
  if (this->load_skill_soul_level_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        skill_level_file) == false) {
    LOG_ERROR("加载 英灵技能等级配置 失败");
    return false;
  }

  // 加载 soul 结点
  TiXmlElement *soul_node = configure_node->FirstChildElement("soul");
  if(soul_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-soul]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = soul_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [soul-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_soul_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() + str) == false) {
    global::LogError("%s:%d (%s) 加载 英灵配置失败", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

}  // namespace soul

}  // namespace server

}  // namespace game

