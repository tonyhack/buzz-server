#include "game_server/server/extension/welfare/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace welfare {

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

  // 加载 condition_pool 结点
  TiXmlElement *condition_pool_node = configure_node->FirstChildElement("condition_pool");
  if(condition_pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-condition_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(condition_pool_node->Attribute("initial_number",
        (int *)(&this->condition_initial_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [condition_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(condition_pool_node->Attribute("extend_number",
        (int *)(&this->condition_extend_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [condition_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 task_pool 结点
  TiXmlElement *task_pool_node = configure_node->FirstChildElement("task_pool");
  if(task_pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-task_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(task_pool_node->Attribute("initial_number",
        (int *)(&this->task_initial_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [task_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(task_pool_node->Attribute("extend_number",
        (int *)(&this->task_extend_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [task_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  const char *str = NULL;
  // 加载任务配置
  TiXmlElement *achieve_node = configure_node->FirstChildElement("achieve");
  if(achieve_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-achieve]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = achieve_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[achieve-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_achieve_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 welfare 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载每日签到配置
  TiXmlElement *daily_node = configure_node->FirstChildElement("daily_checkins");
  if(daily_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-daily_checkins]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = daily_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[daily_checkins-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_daily_checkins_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 daily_checkins 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载签到配置
  TiXmlElement *checkins_node = configure_node->FirstChildElement("checkins");
  if(checkins_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-checkins]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = checkins_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[checkins-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_checkins_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 checkins 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载每日在线配置
  TiXmlElement *online_node = configure_node->FirstChildElement("online");
  if(online_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-online]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  str = online_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[online-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(this->load_online_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 online 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

}  // namespace welfare

}  // namespace server

}  // namespace game

