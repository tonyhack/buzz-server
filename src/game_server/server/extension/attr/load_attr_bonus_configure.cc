#include "game_server/server/extension/attr/load_attr_bonus_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace attr {

LoadAttrBonusConfigure::LoadAttrBonusConfigure() {}
LoadAttrBonusConfigure::~LoadAttrBonusConfigure() {}

bool LoadAttrBonusConfigure::LoadConfigure(const std::string &file) {
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
    AttrBonusCell cell;

    if (cell_node->Attribute("attr_bonus_id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attr_bonus_id_] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("physique_add_value", (int *)&cell.physique_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physique_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("strength_add_value", (int *)&cell.strength_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[strength_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("intelligence_add_value", (int *)&cell.intelligence_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[intelligence_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("agile_add_value", (int *)&cell.agile_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[agile_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spirit_add_value", (int *)&cell.spirit_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spirit_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_hp_add_value", (int *)&cell.max_hp_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_hp_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_mp_add_value", (int *)&cell.max_mp_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_mp_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("physics_attack_add_value", (int *)&cell.physics_attack_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physics_attack_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("physics_defence_add_value", (int *)&cell.physics_defence_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physics_defence_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("magic_attack_add_value", (int *)&cell.magic_attack_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[magic_attack_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("magic_defence_add_value", (int *)&cell.magic_defence_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[magic_defence_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("dodge_add_value", (int *)&cell.dodge_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[dodge_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("crit_add_value", (int *)&cell.crit_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[crit_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("ignore_defence_add_value", (int *)&cell.ignore_defence_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[ignore_defence_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("move_speed_add_value", (int *)&cell.move_speed_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[move_speed_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("dodge_percent_add_value", (int *)&cell.dodge_percent_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[dodge_percent_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("crit_percent_add_value", (int *)&cell.crit_percent_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[crit_percent_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("ignore_defence_percent_add_value",
          (int *)&cell.ignore_defence_percent_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[ignore_defence_percent_add_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("physics_hurt_reduce_add_percent",
          (int *)&cell.physics_hurt_reduce_add_percent_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physics_hurt_reduce_add_percent] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("magic_hurt_reduce_add_percent",
          (int *)&cell.magic_hurt_reduce_add_percent_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[t not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("hurt_reduce_add_percent", (int *)&cell.hurt_reduce_add_percent_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[hurt_reduce_add_percent] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("hurt_random_percent_add", (int *)&cell.hurt_random_percent_add_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[hurt_random_percent_add] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("cure_percent_add", (int *)&cell.cure_percent_add_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[cure_percent_add] not found.", file.c_str());
      return false;
    }

    this->cells_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const AttrBonusCell *LoadAttrBonusConfigure::GetAttrBonus(core::int32 id) const {
  AttrBonusHashmap::const_iterator iter = this->cells_.find(id);
  if (iter == this->cells_.end()) {
    return NULL;
  }
  return &iter->second;
}


}  // namespace attr

}  // namespace server

}  // namespace game

