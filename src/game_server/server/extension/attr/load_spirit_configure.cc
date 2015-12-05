#include "game_server/server/extension/attr/load_spirit_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "entity/spirit_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace attr {

LoadSpiritConfigure::LoadSpiritConfigure() {}
LoadSpiritConfigure::~LoadSpiritConfigure() {}

bool LoadSpiritConfigure::LoadConfigure(const std::string &spirit_file,
    const std::string &spirit_facade_file) {
  TiXmlElement *data_node = NULL;
  TiXmlElement *cell_node = NULL;

  // 加载spirit.xml
  TiXmlDocument spirit_doc;
  if (spirit_doc.LoadFile(spirit_file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", spirit_file.c_str());
    return false;
  }
  
  // --加载节点 data 
  data_node = spirit_doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", spirit_file.c_str());
    return false;
  }

  // 加载节点集合 cell
  cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    SpiritCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("physique_add_value",
            (int *)&cell.physique_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physique_add_value] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("strength_add_value",
            (int *)&cell.strength_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[strength_add_value] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("intelligence_add_value",
            (int *)&cell.intelligence_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[intelligence_add_value] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("agile_add_value",
            (int *)&cell.agile_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[agile_add_value] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("spirit_add_value",
            (int *)&cell.spirit_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spirit_add_value] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("move_speed_add_value",
            (int *)&cell.move_speed_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[move_speed_add_value] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_spend_item_id",
            (int *)&cell.upgrade_spend_item_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_spend_item_id] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_spend_item_count",
            (int *)&cell.upgrade_spend_item_count_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_spend_item_count] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("horse_facade_id",
            (int *)&cell.horse_facade_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[horse_facade_id] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("wing_facade_id",
            (int *)&cell.wing_facade_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[wing_facade_id] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("horse_buff_id",
            (int *)&cell.horse_buff_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[horse_buff_id] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("wing_buff_id",
            (int *)&cell.wing_buff_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[wing_buff_id] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("lucky", (int *)&cell.lucky_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[lucky] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_lucky", (int *)&cell.max_lucky_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_lucky] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("lucky_coe", (int *)&cell.lucky_coe_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[lucky_coe] not found.",
                spirit_file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_odds", (int *)&cell.upgrade_odds_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_odds] not found.",
                spirit_file.c_str());
      return false;
    }

    this->spirits_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 加载spirit_facade.xml
  TiXmlDocument spirit_facade_doc;
  if (spirit_facade_doc.LoadFile(spirit_facade_file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", spirit_facade_file.c_str());
    return false;
  }
  
  // --加载节点 data 
  data_node = spirit_facade_doc.FirstChildElement("data");
  if (NULL == data_node) {
    LOG_ERROR("Config file (%s) /data not found.", spirit_facade_file.c_str());
    return false;
  }

  // 加载节点集合 cell
  cell_node = data_node->FirstChildElement("cell");
  while (cell_node) {
    SpiritFacadeCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.",
                spirit_facade_file.c_str());
      return false;
    }
    if (cell_node->Attribute("facade_type", (int *)&cell.facade_type_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[facade_type] not found.",
                spirit_facade_file.c_str());
      return false;
    }
    if (cell.facade_type_ < entity::SpiritFacadeType::MIN ||
        cell.facade_type_ >= entity::SpiritFacadeType::MAX) {
      LOG_ERROR("Config file (%s) /data/cell[facade_type] is invalid.",
                spirit_facade_file.c_str());
      return false;
    }
    if (cell_node->Attribute("physique_add_value",
            (int *)&cell.physique_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[physique_add_value] not found.",
                spirit_facade_file.c_str());
      return false;
    }
    if (cell_node->Attribute("strength_add_value",
            (int *)&cell.strength_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[strength_add_value] not found.",
                spirit_facade_file.c_str());
      return false;
    }
    if (cell_node->Attribute("intelligence_add_value",
            (int *)&cell.intelligence_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[intelligence_add_value] not found.",
                spirit_facade_file.c_str());
      return false;
    }
    if (cell_node->Attribute("agile_add_value",
            (int *)&cell.agile_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[agile_add_value] not found.",
                spirit_facade_file.c_str());
      return false;
    }
    if (cell_node->Attribute("spirit_add_value",
            (int *)&cell.spirit_add_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spirit_add_value] not found.",
                spirit_facade_file.c_str());
      return false;
    }

    this->facades_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const SpiritCell *LoadSpiritConfigure::GetSpirit(core::int32 level) const {
  SpiritHashmap::const_iterator iter = this->spirits_.find(level);
  if (iter == this->spirits_.end()) {
    return NULL;
  }
  return &iter->second;
}

const SpiritFacadeCell *LoadSpiritConfigure::GetSpiritFacade(core::int32 id) const {
  SpiritFacadeHashmap::const_iterator iter = this->facades_.find(id);
  if (iter == this->facades_.end()) {
    return NULL;
  }
  return &iter->second;
}

}  // namespace attr

}  // namespace server

}  // namespace game
