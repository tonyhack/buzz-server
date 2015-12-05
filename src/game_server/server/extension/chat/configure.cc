#include "game_server/server/extension/chat/configure.h"

#include <cstddef>

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace chat {

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  const char *node_attr = NULL;

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

  // 加载节点 chat_channel 
  TiXmlElement *chat_channel_node = configure_node->FirstChildElement("chat_channel");
  if (NULL == chat_channel_node) {
    LOG_ERROR("Config file (%s) /configure/chat_channel not found.", file.c_str());
    return false;
  }

  node_attr = chat_channel_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/chat_channel[file] not found.", file.c_str());
    return false;
  }

  if (this->load_chat_channel_.LoadConfigure(
          global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
          node_attr) == false) {
    LOG_ERROR("Load chat_channel config file failed.");
    return false;
  }

  return true;
}

}  // namespace chat

}  // namespace server

}  // namespace game

