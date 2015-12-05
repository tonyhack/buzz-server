//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-07 10:21:02.
// File name: load_sword_configure.cc
//
// Description:
// Define class LoadSwordConfigure.
//

#include "game_server/server/extension/attr/load_sword_configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension/attr/configure.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace attr {

LoadSwordConfigure::LoadSwordConfigure() {}
LoadSwordConfigure::~LoadSwordConfigure() {}

bool LoadSwordConfigure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
    return false;
  }

  // 加载节点 data
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("Config file(%s) /data not found");
    return false;
  }

  core::int32 attr_bonus_id = 0;

  core::int32 max_hp_add_value = 0;
  core::int32 max_mp_add_value = 0;
  core::int32 physics_attack_add_value = 0;
  core::int32 physics_defence_add_value = 0;
  core::int32 magic_attack_add_value = 0;
  core::int32 magic_defence_add_value = 0;
  core::int32 dodge_add_value = 0;
  core::int32 crit_add_value = 0;
  core::int32 ignore_defence_add_value = 0;

  SwordCell::BuffVector buffs;

  // 加载节点集合 cell
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    SwordCell cell;
    SwordBuff buff;

    if(cell_node->Attribute("step", (int *)(&cell.step_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[step] not found.", file.c_str());
      return false;
    }
    if(this->swords_.find(cell.step_) != this->swords_.end()) {
      LOG_ERROR("Config file (%s) multiple define step(%u).", file.c_str(), cell.step_);
      return false;
    }
    if(cell_node->Attribute("attr_bonus_id", (int *)(&attr_bonus_id)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attr_bonus_id] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("max_level", (int *)(&cell.max_level_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_level] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("active_hours", (int *)(&cell.active_hours_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[active_hours] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("buff", (int *)(&buff.buff_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[buff] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("buff_odds", (int *)(&buff.buff_odds_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[buff_odds] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("buff_cooling", (int *)(&buff.buff_cooling_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[buff_cooling] not found.", file.c_str());
      return false;
    }
    core::int32 target_type = 0;
    if(cell_node->Attribute("target_type", (int *)(&target_type)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[target_type] not found.", file.c_str());
      return false;
    }
    buff.target_type_ = (entity::BuffTargetType::type)target_type;
    if(cell_node->Attribute("material", (int *)(&cell.spend_item_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[material] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("material_number", (int *)(&cell.spend_item_number_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[material_number] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("gold", (int *)(&cell.spend_gold_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[gold] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("lucky", (int *)(&cell.lucky_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[lucky] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("max_lucky", (int *)(&cell.max_lucky_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_lucky] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("lucky_coe", (int *)(&cell.lucky_coe_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[lucky_coe] not found.", file.c_str());
      return false;
    }
    if(cell_node->Attribute("upgrade_odds", (int *)(&cell.upgrade_odds_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_odds] not found.", file.c_str());
      return false;
    }
    // 得到当前的属性增长配置
    const AttrBonusCell *attr_bonus_cell =
      Configure::GetInstance()->GetAttrBonus(attr_bonus_id);
    if(attr_bonus_cell == NULL) {
      LOG_ERROR("Config file (%s) /data/cell[attr_bonus_id](%d) is invalid.",
          file.c_str(), attr_bonus_id);
      return false;
    }
    // 每次累加之前的加成值
    max_hp_add_value += attr_bonus_cell->max_hp_add_value_;
    max_mp_add_value += attr_bonus_cell->max_mp_add_value_;
    physics_attack_add_value += attr_bonus_cell->physics_attack_add_value_;
    physics_defence_add_value += attr_bonus_cell->physics_defence_add_value_;
    magic_attack_add_value += attr_bonus_cell->magic_attack_add_value_;
    magic_defence_add_value += attr_bonus_cell->magic_defence_add_value_;
    dodge_add_value += attr_bonus_cell->dodge_add_value_;
    crit_add_value += attr_bonus_cell->crit_add_value_;
    ignore_defence_add_value += attr_bonus_cell->ignore_defence_add_value_;

    // 设置为累计加成值
    cell.max_hp_add_value_ = max_hp_add_value;
    cell.max_mp_add_value_ = max_mp_add_value;
    cell.physics_attack_add_value_ = physics_attack_add_value;
    cell.physics_defence_add_value_ = physics_defence_add_value;
    cell.magic_attack_add_value_ = magic_attack_add_value;
    cell.magic_defence_add_value_ = magic_defence_add_value;
    cell.dodge_add_value_ = dodge_add_value;
    cell.crit_add_value_ = crit_add_value;
    cell.ignore_defence_add_value_ = ignore_defence_add_value;

    // BUFF列表
    if(buff.buff_ != 0) {
      buffs.push_back(buff);
    }
    cell.buffs_ = buffs;

    this->swords_.insert(std::make_pair(cell.step_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool LoadSwordConfigure::LoadLevelConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    LOG_ERROR("Load config file (%s) failed.", file.c_str());
    return false;
  }

  // 加载节点 data
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    LOG_ERROR("Config file(%s) /data not found");
    return false;
  }

  core::int32 attr_bonus_id = 0;

  core::int32 max_hp_add_value = 0;
  core::int32 max_mp_add_value = 0;
  core::int32 physics_attack_add_value = 0;
  core::int32 physics_defence_add_value = 0;
  core::int32 magic_attack_add_value = 0;
  core::int32 magic_defence_add_value = 0;
  core::int32 dodge_add_value = 0;
  core::int32 crit_add_value = 0;
  core::int32 ignore_defence_add_value = 0;

  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    SwordLevelCell cell;

    if(cell_node->Attribute("level", (int *)(&cell.level_)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if(this->levels_.find(cell.level_) != this->levels_.end()) {
      LOG_ERROR("Config file (%s) multiple define step(%u).", file.c_str(), cell.level_);
      return false;
    }
    if(cell_node->Attribute("attr_bonus_id", (int *)(&attr_bonus_id)) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attr_bonus_id] not found.", file.c_str());
      return false;
    }

    // 得到当前的属性增长配置
    const AttrBonusCell *attr_bonus_cell =
      Configure::GetInstance()->GetAttrBonus(attr_bonus_id);
    if(attr_bonus_cell == NULL) {
      LOG_ERROR("Config file (%s) /data/cell[attr_bonus_id](%d) is invalid.",
          file.c_str(), attr_bonus_id);
      return false;
    }
    // 每次累加之前的加成值
    max_hp_add_value += attr_bonus_cell->max_hp_add_value_;
    max_mp_add_value += attr_bonus_cell->max_mp_add_value_;
    physics_attack_add_value += attr_bonus_cell->physics_attack_add_value_;
    physics_defence_add_value += attr_bonus_cell->physics_defence_add_value_;
    magic_attack_add_value += attr_bonus_cell->magic_attack_add_value_;
    magic_defence_add_value += attr_bonus_cell->magic_defence_add_value_;
    dodge_add_value += attr_bonus_cell->dodge_add_value_;
    crit_add_value += attr_bonus_cell->crit_add_value_;
    ignore_defence_add_value += attr_bonus_cell->ignore_defence_add_value_;

    // 设置为累计加成值
    cell.max_hp_add_value_ = max_hp_add_value;
    cell.max_mp_add_value_ = max_mp_add_value;
    cell.physics_attack_add_value_ = physics_attack_add_value;
    cell.physics_defence_add_value_ = physics_defence_add_value;
    cell.magic_attack_add_value_ = magic_attack_add_value;
    cell.magic_defence_add_value_ = magic_defence_add_value;
    cell.dodge_add_value_ = dodge_add_value;
    cell.crit_add_value_ = crit_add_value;
    cell.ignore_defence_add_value_ = ignore_defence_add_value;

    this->levels_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const SwordCell *LoadSwordConfigure::GetSword(core::int32 id) const {
  SwordMap::const_iterator iterator = this->swords_.find(id);
  if(iterator == this->swords_.end()) {
    return NULL;
  }

  return &iterator->second;
}

const SwordLevelCell *LoadSwordConfigure::GetLevel(core::int32 level) const {
  SwordLevelHashmap::const_iterator iterator = this->levels_.find(level);
  if(iterator == this->levels_.end()) {
    return NULL;
  }

  return &iterator->second;
}

}  // namespace attr

}  // namespace server

}  // namespace game

