#include "game_server/server/load_attr_fight_score_configure.h"

#include <cstring>

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"

namespace game {

namespace server {

AttrFightScoreCell::AttrFightScoreCell() :
  physique_(0),
  strength_(0),
  intelligence_(0),
  agile_(0),
  spirit_(0),
  max_hp_(0),
  max_mp_(0),
  physics_attack_(0),
  physics_defence_(0),
  magic_attack_(0),
  magic_defence_(0),
  dodge_(0),
  crit_(0),
  ignore_defence_(0),
  move_speed_(0),
  dodge_percent_add_value_(0),
  crit_percent_add_value_(0),
  crit_effect_percent_add_value_(0),
  ignore_defence_percent_add_value_(0),
  physics_hurt_reduce_add_percent_(0),
  magic_hurt_reduce_add_percent_(0),
  hurt_reduce_add_percent_(0),
  hurt_random_percent_add_(0),
  cure_percent_add_(0) {}

AttrFightScoreCell::~AttrFightScoreCell() {}

///////////////////////////////////////////////////////////////////////////////

bool LoadAttrFightScoreConfigure::LoadConfigure(const std::string &file) {
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
    core::int32 fight_score;
    if (cell_node->Attribute("fight_score", (int *)&fight_score) == false) {
      LOG_ERROR("Config file (%s) /data/cell[fight_score] not found.", file.c_str());
      return false;
    }

    const char *node_attr = cell_node->Attribute("string_id");
    if (NULL == node_attr) {
      LOG_ERROR("Config file (%s) /data/cell[string_id] not found.", file.c_str());
      return false;
    }
    if (strcmp(node_attr, "PHYSIQUE") == 0) {
      this->cell.physique_ = fight_score;
    } else if (strcmp(node_attr, "STRENGTH") == 0) {
      this->cell.strength_ = fight_score;
    } else if (strcmp(node_attr, "INTELLIGENCE") == 0) {
      this->cell.intelligence_ = fight_score;
    } else if (strcmp(node_attr, "AGILE") == 0) {
      this->cell.agile_ = fight_score;
    } else if (strcmp(node_attr, "SPIRIT") == 0) {
      this->cell.spirit_ = fight_score;
    } else if (strcmp(node_attr, "MAX_HP") == 0) {
      this->cell.max_hp_ = fight_score;
    } else if (strcmp(node_attr, "MAX_MP") == 0) {
      this->cell.max_mp_ = fight_score;
    } else if (strcmp(node_attr, "PHYSICS_ATTACK") == 0) {
      this->cell.physics_attack_ = fight_score;
    } else if (strcmp(node_attr, "PHYSICS_DEFENCE") == 0) {
      this->cell.physics_defence_ = fight_score;
    } else if (strcmp(node_attr, "MAGIC_ATTACK") == 0) {
      this->cell.magic_attack_ = fight_score;
    } else if (strcmp(node_attr, "MAGIC_DEFENCE") == 0) {
      this->cell.magic_defence_ = fight_score;
    } else if (strcmp(node_attr, "DODGE") == 0) {
      this->cell.dodge_ = fight_score;
    } else if (strcmp(node_attr, "CRIT") == 0) {
      this->cell.crit_ = fight_score;
    } else if (strcmp(node_attr, "IGNORE_DEFENCE") == 0) {
      this->cell.ignore_defence_ = fight_score;
    } else if (strcmp(node_attr, "MOVE_SPEED") == 0) {
      this->cell.move_speed_ = fight_score;
    } else if (strcmp(node_attr, "DODGE_PERCENT_ADD_VALUE") == 0) {
      this->cell.dodge_percent_add_value_ = fight_score;
    } else if (strcmp(node_attr, "CRIT_PERCENT_ADD_VALUE") == 0) {
      this->cell.crit_percent_add_value_ = fight_score;
    } else if (strcmp(node_attr, "CRIT_EFFECT_PERCENT_ADD_VALUE") == 0) {
      this->cell.crit_effect_percent_add_value_ = fight_score;
    } else if (strcmp(node_attr, "IGNORE_DEFENCE_PERCENT_ADD_VALUE") == 0) {
      this->cell.ignore_defence_percent_add_value_ = fight_score;
    } else if (strcmp(node_attr, "PHYSICS_HURT_REDUCE_ADD_PERCENT") == 0) {
      this->cell.physics_hurt_reduce_add_percent_ = fight_score;
    } else if (strcmp(node_attr, "MAGIC_HURT_REDUCE_ADD_PERCENT") == 0) {
      this->cell.magic_hurt_reduce_add_percent_ = fight_score;
    } else if (strcmp(node_attr, "HURT_REDUCE_ADD_PERCENT") == 0) {
      this->cell.hurt_reduce_add_percent_ = fight_score;
    } else if (strcmp(node_attr, "HURT_RANDOM_PERCENT_ADD") == 0) {
      this->cell.hurt_random_percent_add_ = fight_score;
    } else if (strcmp(node_attr, "CURE_PERCENT_ADD") == 0) {
      this->cell.cure_percent_add_ = fight_score;
    } else if (strcmp(node_attr, "SKILL_LEVEL") == 0) {
      this->cell.skill_level_ = fight_score;
    } else {
      LOG_ERROR("Config file (%s) /data/cell[string_id] is invalid.", file.c_str());
      return false;
    }

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

}  // namespace server

}  // namespace game

