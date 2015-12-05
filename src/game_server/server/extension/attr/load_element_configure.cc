#include "game_server/server/extension/attr/load_element_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "game_server/server/extension/attr/configure.h"

namespace game {

namespace server {

namespace attr {

LoadElementConfigure::LoadElementConfigure() {}
LoadElementConfigure::~LoadElementConfigure() {}

bool LoadElementConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;

  if (doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed", file.c_str());
    return false;
  }

  // 加载节点 data 
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", file.c_str());
    return false;
  }

  core::int32 max_hp_add_value = 0;
  core::int32 max_mp_add_value = 0;
  core::int32 physics_attack_add_value = 0;
  core::int32 physics_defence_add_value = 0;
  core::int32 magic_attack_add_value = 0;
  core::int32 magic_defence_add_value = 0;
  core::int32 dodge_add_value = 0;
  core::int32 crit_add_value = 0;
  core::int32 ignore_defence_add_value = 0;

  // 加载节点集合 cell
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    ElementCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("attr_bonus_id", (int *)&cell.attr_bonus_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attr_bonus_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_war_soul", (int *)&cell.spend_war_soul_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_war_soul] not found.", file.c_str());
      return false;
    }

    const AttrBonusCell *attr_bonus_cell = Configure::GetInstance()->GetAttrBonus(cell.attr_bonus_id_);
    if (NULL == attr_bonus_cell) {
      LOG_ERROR("Config file (%s) /data/cell[attr_bonus_id](%d) is invalid.",
                file.c_str(), cell.attr_bonus_id_);
      return false;
    }
    max_hp_add_value += attr_bonus_cell->max_hp_add_value_;
    max_mp_add_value += attr_bonus_cell->max_mp_add_value_;
    physics_attack_add_value += attr_bonus_cell->physics_attack_add_value_;
    physics_defence_add_value += attr_bonus_cell->physics_defence_add_value_;
    magic_attack_add_value += attr_bonus_cell->magic_attack_add_value_;
    magic_defence_add_value += attr_bonus_cell->magic_defence_add_value_;
    dodge_add_value += attr_bonus_cell->dodge_add_value_;
    crit_add_value += attr_bonus_cell->crit_add_value_;
    ignore_defence_add_value += attr_bonus_cell->ignore_defence_add_value_;

    cell.max_hp_add_value_ = max_hp_add_value;
    cell.max_mp_add_value_ = max_mp_add_value;
    cell.physics_attack_add_value_ = physics_attack_add_value;
    cell.physics_defence_add_value_ = physics_defence_add_value;
    cell.magic_attack_add_value_ = magic_attack_add_value;
    cell.magic_defence_add_value_ = magic_defence_add_value;
    cell.dodge_add_value_ = dodge_add_value;
    cell.crit_add_value_ = crit_add_value;
    cell.ignore_defence_add_value_ = ignore_defence_add_value;

    this->cells_store_.push_back(cell);
    this->cells_index_[cell.id_] = this->cells_store_.size() - 1;

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const ElementCell *LoadElementConfigure::GetElement(core::int32 id) const {
  ElementIndexHashmap::const_iterator iter = this->cells_index_.find(id);
  if (iter == this->cells_index_.end()) {
    return NULL;
  }
  return &this->cells_store_[iter->second];
}

const ElementCell *LoadElementConfigure::GetNextElement(core::int32 id) const {
  if (0 == id && this->cells_store_.size() >= 1) {
    return &this->cells_store_[0];
  }

  ElementIndexHashmap::const_iterator iter = this->cells_index_.find(id);
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

