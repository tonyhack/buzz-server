#include "game_server/server/extension/attr/load_legendary_weapon_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "game_server/server/extension/attr/configure.h"

namespace game {

namespace server {

namespace attr {

LoadLegendaryWeaponConfigure::LoadLegendaryWeaponConfigure() {}
LoadLegendaryWeaponConfigure::~LoadLegendaryWeaponConfigure() {}

bool LoadLegendaryWeaponConfigure::LoadConfigure(const std::string &file) {
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
    LegendaryWeaponCell weapon_cell;

    if (cell_node->Attribute("id", (int *)&weapon_cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }

    // 碎片1
    {
      LegendaryWeaponPieceCell piece_cell;

      if (cell_node->Attribute("piece1_item_id", (int *)&piece_cell.item_id_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece1_item_id] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("piece1_item_count", (int *)&piece_cell.item_count_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece1_item_count] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("piece1_exp_reward", (int *)&piece_cell.exp_reward_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece1_exp_reward] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("piece1_nimbus_reward", (int *)&piece_cell.nimbus_reward_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece1_nimbus_reward] not found.", file.c_str());
        return false;
      }

      weapon_cell.pieces_.push_back(piece_cell);
    }
    // 碎片2
    {
      LegendaryWeaponPieceCell piece_cell;

      if (cell_node->Attribute("piece2_item_id", (int *)&piece_cell.item_id_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece2_item_id] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("piece2_item_count", (int *)&piece_cell.item_count_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece2_item_count] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("piece2_exp_reward", (int *)&piece_cell.exp_reward_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece2_exp_reward] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("piece2_nimbus_reward", (int *)&piece_cell.nimbus_reward_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece2_nimbus_reward] not found.", file.c_str());
        return false;
      }

      weapon_cell.pieces_.push_back(piece_cell);
    }
    // 碎片3
    {
      LegendaryWeaponPieceCell piece_cell;

      if (cell_node->Attribute("piece3_item_id", (int *)&piece_cell.item_id_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece3_item_id] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("piece3_item_count", (int *)&piece_cell.item_count_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece3_item_count] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("piece3_exp_reward", (int *)&piece_cell.exp_reward_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece3_exp_reward] not found.", file.c_str());
        return false;
      }
      if (cell_node->Attribute("piece3_nimbus_reward", (int *)&piece_cell.nimbus_reward_) == false) {
        LOG_ERROR("Config file (%s) /data/cell[piece3_nimbus_reward] not found.", file.c_str());
        return false;
      }

      weapon_cell.pieces_.push_back(piece_cell);
    }

    if (cell_node->Attribute("attr_bonus_id", (int *)&weapon_cell.attr_bonus_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attr_bonus_id] not found.", file.c_str());
      return false;
    }
    if (Configure::GetInstance()->GetAttrBonus(weapon_cell.attr_bonus_id_) == NULL) {
      LOG_ERROR("Config file (%s) /data/cell[attr_bonus_id] is invalid.", file.c_str());
      return false;
    }

    this->weapons_.insert(std::make_pair(weapon_cell.id_, weapon_cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const LegendaryWeaponCell *LoadLegendaryWeaponConfigure::GetLegendaryWeapon(
    core::int32 id) const {
  LegendaryWeaponHashmap::const_iterator iter = this->weapons_.find(id);
  if (iter == this->weapons_.end()) {
    return NULL;
  }
  return &iter->second;
}

}  // namespace attr

}  // namespace server 

}  // namespace game

