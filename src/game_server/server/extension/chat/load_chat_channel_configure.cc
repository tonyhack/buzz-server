#include "game_server/server/extension/chat/load_chat_channel_configure.h"

#include <cstring>

#include "core/tinyxml/tinyxml.h"
#include "entity/chat_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace chat {

LoadChatChannelConfigure::LoadChatChannelConfigure() {}
LoadChatChannelConfigure::~LoadChatChannelConfigure() {}

bool LoadChatChannelConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  const char *node_attr = NULL;
  ChatChannelCell *cell = NULL;

  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
    return false;
  }

  // 加载节点 data 
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", file.c_str());
    return false;
  }

  // 加载节点集合 cell
  this->cells_.resize(entity::ChatChannelType::MAX);
  size_t cell_count = 0;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    // 检查ID
    node_attr = cell_node->Attribute("string_id");
    if (NULL == node_attr) {
      LOG_ERROR("Config file (%s) /data/cell[string_id] not found.", file.c_str());
      return false;
    }

    core::int32 id;
    if (strcmp(node_attr, "WORLD") == 0) {
      id = entity::ChatChannelType::WORLD;
    } else if (strcmp(node_attr, "SCENE") == 0) {
      id = entity::ChatChannelType::SCENE;
    } else if (strcmp(node_attr, "GUILD") == 0) {
      id = entity::ChatChannelType::GUILD;
    } else if (strcmp(node_attr, "TEAM") == 0) {
      id = entity::ChatChannelType::TEAM;
    } else if (strcmp(node_attr, "WHISPER") == 0) {
      id = entity::ChatChannelType::WHISPER;
    } else if (strcmp(node_attr, "WORLD_VIP") == 0) {
      id = entity::ChatChannelType::WORLD_VIP;
    } else if (strcmp(node_attr, "SYSTEM") == 0) {
      id = entity::ChatChannelType::SYSTEM;
    } else {
      LOG_ERROR("Config file (%s) /data/cell[string_id] is invalid.", file.c_str());
      return false;
    }

    if (this->cells_[id].id_ != 0) {
      LOG_ERROR("Config file (%s) /data/cell[string_id] is duplicate.", file.c_str());
      return false;
    }
    cell = &this->cells_[id];
    cell->id_ = id;

    if (cell_node->Attribute("cooling", (int *)&cell->cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_char", (int *)&cell->max_char_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_char] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_item_id", (int *)&cell->spend_item_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_item_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_item_count", (int *)&cell->spend_item_count_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_item_count] not found.", file.c_str());
      return false;
    }

    ++cell_count;
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const ChatChannelCell *LoadChatChannelConfigure::GetChatChannel(core::int32 id) const {
  if (this->cells_.empty() || (size_t)id > this->cells_.size() - 1) {
    return NULL;
  }
  return &this->cells_[id];
}

}  // namespace chat

}  // namespace server

}  // namespace game

