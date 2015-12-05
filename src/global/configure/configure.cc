#include "global/configure/configure.h"

#include <cstddef>

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace global {

namespace configure {

Configure ConfigureSingleton::instance_;

Configure::Configure() {}
Configure::~Configure() {}

bool Configure::LoadConfigure(const std::string &file) {
  const std::string &config_dir =
    global::ServerConfigureSingleton::GetInstance().GetConfigueDir();
  const char *node_attr = NULL;

  TiXmlDocument doc;
  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed", file.c_str());
    return false;
  }

  // 加载 configure 结点
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if (NULL == configure_node) {
    LOG_ERROR("Config file (%s) /configure not found.", file.c_str());
    return false;
  }

  // 加载 global_configure 结点
  TiXmlElement *global_configure_node = configure_node->FirstChildElement("global_configure");
  if (NULL == global_configure_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure not found.", file.c_str());
    return false;
  }

  // 加载 dirty_word 结点
  TiXmlElement *dirty_word_node = global_configure_node->FirstChildElement("dirty_word");
  if (NULL == dirty_word_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure_node/dirty_word not found.", file.c_str());
    return false;
  }
  node_attr = dirty_word_node->Attribute("value");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/global_configure_node/dirty_word[value] not found.", file.c_str());
    return false;
  }
  this->dirty_word_file_ = config_dir + node_attr;

  // 加载 misc 结点
  TiXmlElement *misc_node = global_configure_node->FirstChildElement("misc");
  if (NULL == misc_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure/misc not found.", file.c_str());
    return false;
  }
  node_attr = misc_node->Attribute("value");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/global_configure/misc[value] not found.", file.c_str());
    return false;
  }
  if (this->misc_configure_.LoadConfigure(config_dir + node_attr) == false) {
    LOG_ERROR("Load misc config file failed.");
    return false;
  }

  // 加载 social 结点
  TiXmlElement *social_node = global_configure_node->FirstChildElement("social");
  if (NULL == social_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure/social not found.", file.c_str());
    return false;
  }
  node_attr = social_node->Attribute("value");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/global_configure/social[value] not found.", file.c_str());
    return false;
  }
  if (this->social_configure_.LoadConfigure(config_dir + node_attr) == false) {
    LOG_ERROR("Load social config file failed.");
    return false;
  }

  // 加载 playing 结点
  TiXmlElement *playing_node = global_configure_node->FirstChildElement("playing");
  if (NULL == playing_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure/playing not found.", file.c_str());
    return false;
  }
  node_attr = playing_node->Attribute("value");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/global_configure/playing[value] not found.", file.c_str());
    return false;
  }
  if (this->playing_configure_.LoadConfigure(config_dir + node_attr) == false) {
    LOG_ERROR("Load playing config file failed.");
    return false;
  }

  // 加载 guild 节点
  TiXmlElement *guild_node = global_configure_node->FirstChildElement("guild");
  if (NULL == guild_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure/guild not found.", file.c_str());
    return false;
  }
  node_attr = guild_node->Attribute("value");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/global_configure/guild[value] not found.", file.c_str());
    return false;
  }
  if (this->guild_configure_.LoadConfigure(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild config file failed.");
    return false;
  }

  // 加载 world_boss 节点
  TiXmlElement *boss_node = global_configure_node->FirstChildElement("world_boss");
  if (NULL == boss_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure/world_boss not found.", file.c_str());
    return false;
  }
  node_attr = boss_node->Attribute("value");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/global_configure/world_boss[value] not found.", file.c_str());
    return false;
  }
  if (this->boss_configure_.LoadConfigure(config_dir + node_attr) == false) {
    LOG_ERROR("Load world_boss config file failed.");
    return false;
  }

  // 加载 vip 节点
  TiXmlElement *vip_node = global_configure_node->FirstChildElement("vip");
  if (NULL == vip_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure/vip not found.", file.c_str());
    return false;
  }
  node_attr = vip_node->Attribute("value");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/global_configure/vip[value] not found.", file.c_str());
    return false;
  }
  if (this->vip_configure_.LoadConfigure(config_dir + node_attr) == false) {
    LOG_ERROR("Load vip config file failed.");
    return false;
  }

  // 加载 auction 节点
  TiXmlElement *auction_node = global_configure_node->FirstChildElement("auction");
  if (NULL == auction_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure/auction not found.", file.c_str());
    return false;
  }
  node_attr = auction_node->Attribute("value");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/global_configure/auction[value] not found.", file.c_str());
    return false;
  }
  if (this->auction_configure_.LoadConfigure(config_dir + node_attr) == false) {
    LOG_ERROR("Load auction config file failed.");
    return false;
  }

  // 加载 maze 节点
  TiXmlElement *maze_node = global_configure_node->FirstChildElement("maze");
  if (NULL == maze_node) {
    LOG_ERROR("Config file (%s) /configure/global_configure/maze not found.", file.c_str());
    return false;
  }
  node_attr = maze_node->Attribute("value");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/global_configure/maze[value] not found.", file.c_str());
    return false;
  }
  if (this->maze_configure_.LoadConfigure(config_dir + node_attr) == false) {
    LOG_ERROR("Load maze config file failed.");
    return false;
  }

  return true;
}

}  // namespace configure

}  // namespace global

