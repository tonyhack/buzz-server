#include "game_server/server/extension/attr/load_noble_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace attr {

LoadNobleConfigure::LoadNobleConfigure() {}
LoadNobleConfigure::~LoadNobleConfigure() {}

bool LoadNobleConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
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
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    NobleCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_nimbus", (int *)&cell.spend_nimbus_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_nimbus] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_item_id", (int *)&cell.spend_item_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_item_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_item_count", (int *)&cell.spend_item_count_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_item_count] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_gold", (int *)&cell.spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_gold] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_hp_add_value_limit", (int *)&cell.max_hp_add_value_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_hp_add_value_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_hp_add_value_increment", (int *)&cell.max_hp_add_value_increment_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_hp_add_value_increment] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_mp_add_value_limit", (int *)&cell.max_mp_add_value_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_mp_add_value_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_mp_add_value_increment", (int *)&cell.max_mp_add_value_increment_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_mp_add_value_increment] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("physics_attack_add_value_limit",(
            int *)&cell.physics_attack_add_value_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physics_attack_add_value_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("physics_attack_add_value_increment",
          (int *)&cell.physics_attack_add_value_increment_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physics_attack_add_value_increment] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("physics_defence_add_value_limit",
          (int *)&cell.physics_defence_add_value_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physics_defence_add_value_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("physics_defence_add_value_increment",
          (int *)&cell.physics_defence_add_value_increment_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physics_defence_add_value_increment] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("magic_attack_add_value_limit",
          (int *)&cell.magic_attack_add_value_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[magic_attack_add_value_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("magic_attack_add_value_increment",
          (int *)&cell.magic_attack_add_value_increment_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[magic_attack_add_value_increment] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("magic_defence_add_value_limit",
          (int *)&cell.magic_defence_add_value_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[magic_defence_add_value_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("magic_defence_add_value_increment",
          (int *)&cell.magic_defence_add_value_increment_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[magic_defence_add_value_increment] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("dodge_add_value_limit", (int *)&cell.dodge_add_value_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[dodge_add_value_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("dodge_add_value_increment", (int *)&cell.dodge_add_value_increment_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[dodge_add_value_increment] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("crit_add_value_limit", (int *)&cell.crit_add_value_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[crit_add_value_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("crit_add_value_increment", (int *)&cell.crit_add_value_increment_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[crit_add_value_increment] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("ignore_defence_add_value_limit",
          (int *)&cell.ignore_defence_add_value_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[ignore_defence_add_value_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("ignore_defence_add_value_increment",
          (int *)&cell.ignore_defence_add_value_increment_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[ignore_defence_add_value_increment] not found.", file.c_str());
      return false;
    }

    this->cells_store_.push_back(cell);
    this->cells_index_[cell.id_] = this->cells_store_.size() - 1;

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const NobleCell *LoadNobleConfigure::GetNoble(core::int32 id) const {
  NobleIndexHashmap::const_iterator iter = this->cells_index_.find(id);
  if (iter == this->cells_index_.end()) {
    return NULL;
  }
  return &this->cells_store_[iter->second];
}

const NobleCell *LoadNobleConfigure::GetFirstNoble() const {
  if (this->cells_store_.size() >= 1) {
    return &this->cells_store_[0]; 
  }

  return NULL;
}

const NobleCell *LoadNobleConfigure::GetNextNoble(core::int32 id) const {

  NobleIndexHashmap::const_iterator iter = this->cells_index_.find(id);
  if (iter == this->cells_index_.end()) {
    return NULL;
  }
  if (iter->second >= this->cells_store_.size() - 1) {
    return NULL;
  }
  return &this->cells_store_[iter->second + 1];
}

}  // namespace attr

}  // namespace server

}  // namespace game

