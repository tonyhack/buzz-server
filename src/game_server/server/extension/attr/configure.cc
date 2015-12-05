#include "game_server/server/extension/attr/configure.h"

#include <cstddef>

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace attr {

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  const char *node_attr = NULL;
  const char *node_attr2 = NULL;

  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
    return false;
  }

  // 加载节点 configure
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if (NULL == configure_node) {
    LOG_ERROR("Config file (%s) /configure not found.", file.c_str());
    return false;
  }

  // 加载节点 attr_bonus
  TiXmlElement *attr_bonus_node = configure_node->FirstChildElement("attr_bonus");
  if (NULL == attr_bonus_node) {
    LOG_ERROR("Config file (%s) /configure/attr_bonus not found.", file.c_str());
    return false;
  }

  node_attr = attr_bonus_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/attr_bonus[file] not found.", file.c_str());
    return false;
  }

  if (this->load_attr_bonus_.LoadConfigure(
          global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
          node_attr) == false) {
    LOG_ERROR("Load attr_bonus config file failed.");
    return false;
  }

  // 加载节点 noble
  TiXmlElement *noble_node = configure_node->FirstChildElement("noble");
  if (NULL == noble_node) {
    LOG_ERROR("Config file (%s) /configure/noble not found.", file.c_str());
    return false;
  }

  node_attr = noble_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/noble[file] not found.", file.c_str());
    return false;
  }

  if (this->load_noble_.LoadConfigure(
          global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
          node_attr) == false) {
    LOG_ERROR("Load noble config file failed.");
    return false;
  }

  // 加载节点 sword
  TiXmlElement *sword_node = configure_node->FirstChildElement("sword");
  if(sword_node == NULL) {
    LOG_ERROR("Config file (%s) /configure/sword not found.", file.c_str());
    return false;
  }

  node_attr = sword_node->Attribute("sword");
  if(node_attr == NULL) {
    LOG_ERROR("Config file (%s) /configure/sword[sword] not found.", file.c_str());
    return false;
  }

  if(this->load_sword_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        node_attr) == false) {
    LOG_ERROR("Load sword config file failed.");
    return false;
  }

  node_attr = sword_node->Attribute("level");
  if(node_attr == NULL) {
    LOG_ERROR("Config file (%s) /configure/sword[level] not found.", file.c_str());
    return false;
  }

  if(this->load_sword_.LoadLevelConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        node_attr) == false) {
    LOG_ERROR("Load sword level config file failed.");
    return false;
  }

  // 加载节点 element
  TiXmlElement *element_node = configure_node->FirstChildElement("element");
  if (NULL == element_node) {
    LOG_ERROR("Config file (%s) /configure/element not found.", file.c_str());
    return false;
  }

  node_attr = element_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/element[file] not found.", file.c_str());
    return false;
  }

  if (this->load_element_.LoadConfigure(
          global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
          node_attr) == false) {
    LOG_ERROR("Load element config file failed.");
    return false;
  }

  // 加载节点 rune
  TiXmlElement *rune_node = configure_node->FirstChildElement("rune");
  if (NULL == rune_node) {
    LOG_ERROR("Config file (%s) /configure/rune not found.", file.c_str());
    return false;
  }

  node_attr = rune_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/rune[file] not found.", file.c_str());
    return false;
  }

  if (this->load_rune_.LoadConfigure(
          global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
          node_attr) == false) {
    LOG_ERROR("Load rune config file failed.");
    return false;
  }

  // 加载节点 spirit | spirit_facade
  TiXmlElement *spirit_node = configure_node->FirstChildElement("spirit");
  if (NULL == spirit_node) {
    LOG_ERROR("Config file (%s) /configure/spirit not found.", file.c_str());
    return false;
  }

  TiXmlElement *spirit_facade_node = configure_node->FirstChildElement("spirit_facade");
  if (NULL == spirit_facade_node) {
    LOG_ERROR("Config file (%s) /configure/spirit_facade not found.", file.c_str());
    return false;
  }

  node_attr = spirit_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/spirit[file] not found.", file.c_str());
    return false;
  }

  node_attr2 = spirit_facade_node->Attribute("file");
  if (NULL == node_attr2) {
    LOG_ERROR("Config file (%s) /configure/spirit_facade[file] not found.", file.c_str());
    return false;
  }

  if (this->load_spirit_.LoadConfigure(
          global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
          node_attr,
          global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
          node_attr2) == false) {
    LOG_ERROR("Load spirit config file failed.");
    return false;
  }

  // 加载节点 legendary_weapon
  TiXmlElement *legendary_weapon_node = configure_node->FirstChildElement("legendary_weapon");
  if (NULL == legendary_weapon_node) {
    LOG_ERROR("Config file (%s) /configure/legendary_weapon not found.", file.c_str());
    return false;
  }

  node_attr = legendary_weapon_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/legendary_weapon[file] not found.", file.c_str());
    return false;
  }

  if (this->load_legendary_weapon_.LoadConfigure(
          global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
          node_attr) == false) {
    LOG_ERROR("Load legendary_weapon config file failed.");
    return false;
  }

  return true;
}

}  // namespace attr

}  // namespace server

}  // namespace game

