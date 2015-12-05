#include "global/configure/social_configure.h"

#include <cstddef>

#include "core/tinyxml/tinyxml.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace global {

namespace configure {

SocialConfigure::SocialConfigure() {}
SocialConfigure::~SocialConfigure() {}

bool SocialConfigure::LoadConfigure(const std::string &file) {
  const std::string &config_dir =
      global::ServerConfigureSingleton::GetInstance().GetConfigueDir();
  const char *node_attr = NULL;

  TiXmlDocument doc;
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

  // 加载节点 homeland_temple
  TiXmlElement *homeland_temple_node = configure_node->FirstChildElement("homeland_temple");
  if (NULL == homeland_temple_node) {
    LOG_ERROR("Config file (%s) /configure/homeland_temple not found.", file.c_str());
    return false;
  }
  node_attr = homeland_temple_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/homeland_temple[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadHomelandTemple(config_dir + node_attr) == false) {
    LOG_ERROR("Load homeland temple config file failed.");
    return false;
  }

  // 加载节点 homeland_goldmine
  TiXmlElement *homeland_goldmine_node = configure_node->FirstChildElement("homeland_goldmine");
  if (NULL == homeland_goldmine_node) {
    LOG_ERROR("Config file (%s) /configure/homeland_goldmine not found.", file.c_str());
    return false;
  }
  node_attr = homeland_goldmine_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/homeland_goldmine[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadHomelandGoldmine(config_dir + node_attr) == false) {
    LOG_ERROR("Load homeland goldmine config file failed.");
    return false;
  }

  // 加载节点 homeland_pool
  TiXmlElement *homeland_pool_node = configure_node->FirstChildElement("homeland_pool");
  if (NULL == homeland_pool_node) {
    LOG_ERROR("Config file (%s) /configure/homeland_pool not found.", file.c_str());
    return false;
  }
  node_attr = homeland_pool_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/homeland_pool[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadHomelandPool(config_dir + node_attr) == false) {
    LOG_ERROR("Load homeland pool config file failed.");
    return false;
  }

  // 加载节点 homeland_church_chest
  TiXmlElement *homeland_church_chest_node = configure_node->FirstChildElement("homeland_church_chest");
  if (NULL == homeland_church_chest_node) {
    LOG_ERROR("Config file (%s) /configure/homeland_church_chest not found.", file.c_str());
    return false;
  }
  node_attr = homeland_church_chest_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/homeland_church_chest[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadHomelandChurchChest(config_dir + node_attr) == false) {
    LOG_ERROR("Load homeland pool config file failed.");
    return false;
  }

  // 加载节点 homeland_church_bonus
  TiXmlElement *homeland_church_bonus_node = configure_node->FirstChildElement("homeland_church_bonus");
  if (NULL == homeland_church_bonus_node) {
    LOG_ERROR("Config file (%s) /configure/homeland_church_bonus not found.", file.c_str());
    return false;
  }
  node_attr = homeland_church_bonus_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/homeland_church_bonus[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadHomelandChurchBonus(config_dir + node_attr) == false) {
    LOG_ERROR("Load homeland pool config file failed.");
    return false;
  }

  // 加载节点 homeland_tree
  TiXmlElement *homeland_tree_node = configure_node->FirstChildElement("homeland_tree");
  if (NULL == homeland_tree_node) {
    LOG_ERROR("Config file (%s) /configure/homeland_tree not found.", file.c_str());
    return false;
  }
  node_attr = homeland_tree_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/homeland_tree[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadHomelandTree(config_dir + node_attr) == false) {
    LOG_ERROR("Load homeland tree config file failed.");
    return false;
  }

  // 加载节点 homeland_tower
  TiXmlElement *homeland_tower_node = configure_node->FirstChildElement("homeland_tower");
  if (NULL == homeland_tower_node) {
    LOG_ERROR("Config file (%s) /configure/homeland_tower not found.", file.c_str());
    return false;
  }
  node_attr = homeland_tower_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/homeland_tower[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadHomelandTower(config_dir + node_attr) == false) {
    LOG_ERROR("Load homeland tower config file failed.");
    return false;
  }

  // 加载节点 homeland_stronghold
  TiXmlElement *homeland_stronghold_node = configure_node->FirstChildElement("homeland_stronghold");
  if (NULL == homeland_stronghold_node) {
    LOG_ERROR("Config file (%s) /configure/homeland_stronghold not found.", file.c_str());
    return false;
  }
  node_attr = homeland_stronghold_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/homeland_stronghold[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadHomelandStronghold(config_dir + node_attr) == false) {
    LOG_ERROR("Load homeland stronghold config file failed.");
    return false;
  }

  // 加载节点 homeland_soul
  TiXmlElement *homeland_soul_node = configure_node->FirstChildElement("homeland_soul");
  if (NULL == homeland_soul_node) {
    LOG_ERROR("Config file (%s) /configure/homeland_soul not found.", file.c_str());
    return false;
  }
  node_attr = homeland_soul_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/homeland_soul[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadHomelandSoul(config_dir + node_attr) == false) {
    LOG_ERROR("Load homeland soul config file failed.");
    return false;
  }

  return true;
}

bool SocialConfigure::LoadHomelandTemple(const std::string &file) {
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
    HomelandTempleCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_spend_gold", (int *)&cell.upgrade_spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_spend_gold] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_cooling", (int *)&cell.upgrade_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_harvest_times", (int *)&cell.max_harvest_times_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_harvest_times] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("harvest_gold_reward", (int *)&cell.harvest_gold_reward_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[harvest_gold_reward] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("harvest_cooling", (int *)&cell.harvest_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[harvest_cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_force_harvest_times",
            (int *)&cell.max_force_harvest_times_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_force_harvest_times] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("vip_bonus_force_harvest_times",
            (int *)&cell.vip_bonus_force_harvest_times_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[vip_bonus_force_harvest_times] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("force_harvest_gold_reward",
            (int *)&cell.force_harvest_gold_reward_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[force_harvest_gold_reward] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("force_harvest_min_spend_freedom_dollars",
            (int *)&cell.force_harvest_min_spend_freedom_dollars_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[force_harvest_min_spend_freedom_dollars] not found.",
                file.c_str());
      return false;
    }
    if (cell_node->Attribute("force_harvest_max_spend_freedom_dollars",
            (int *)&cell.force_harvest_max_spend_freedom_dollars_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[force_harvest_max_spend_freedom_dollars] not found.",
                file.c_str());
      return false;
    }
    if (cell_node->Attribute("force_harvest_step_spend_freedom_dollars",
            (int *)&cell.force_harvest_step_spend_freedom_dollars_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[force_harvest_step_spend_freedom_dollars] not found.",
                file.c_str());
      return false;
    }
  
    this->homeland_temples_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 有效性检查
  if (this->GetHomelandTemple(1) == NULL) {
    LOG_ERROR("Config file (%s) no level 1 data.");
    return false;
  }

  return true;
}

bool SocialConfigure::LoadHomelandGoldmine(const std::string &file) {
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
    HomelandGoldmineCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_spend_gold", (int *)&cell.upgrade_spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_spend_gold] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_cooling", (int *)&cell.upgrade_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("harvest_gold_reward", (int *)&cell.harvest_gold_reward_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[harvest_gold_reward] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("harvest_cooling", (int *)&cell.harvest_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[harvest_cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_robbed_times", (int *)&cell.max_robbed_times_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_robbed_times] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("tiny_win_gold_loss_rate",
            (int *)&cell.tiny_win_gold_loss_rate_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[tiny_win_gold_loss_rate] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("great_win_gold_loss_rate",
            (int *)&cell.great_win_gold_loss_rate_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[great_win_gold_loss_rate] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("perfect_win_gold_loss_rate",
            (int *)&cell.perfect_win_gold_loss_rate_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[perfect_win_gold_loss_rate] not found.", file.c_str());
      return false;
    }

    this->homeland_goldmines_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }
  
  // 有效性检查
  if (this->GetHomelandGoldmine(1) == NULL) {
    LOG_ERROR("Config file (%s) no level 1 data.");
    return false;
  }

  return true;
}

bool SocialConfigure::LoadHomelandPool(const std::string &file) {
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
    HomelandPoolCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_spend_gold", (int *)&cell.upgrade_spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_spend_gold] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_cooling", (int *)&cell.upgrade_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("accumulate_exp_reward", (int *)&cell.accumulate_exp_reward_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[accumulate_exp_reward] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("accumulate_nimbus_reward",
            (int *)&cell.accumulate_nimbus_reward_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[accumulate_nimbus_reward] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("accumulate_spend_time",
            (int *)&cell.accumulate_spend_time_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[accumulate_spend_time] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_accumulate_times",
            (int *)&cell.max_accumulate_times_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_accumulate_times] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("double_harvest_spend_item_id",
            (int *)&cell.double_harvest_spend_item_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[double_harvest_spend_item_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("double_harvest_spend_item_count",
            (int *)&cell.double_harvest_spend_item_count_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[double_harvest_spend_item_count] not found.", file.c_str());
      return false;
    }

    // 有效性检查
    if (cell.accumulate_spend_time_ <= 0) {
      LOG_ERROR("Config file (%s) /data/cell[accumulate_spend_time] is invalid.", file.c_str());
      return false;
    }

    this->homeland_pools_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 有效性检查
  if (this->GetHomelandPool(1) == NULL) {
    LOG_ERROR("Config file (%s) no level 1 data.");
    return false;
  }

  return true;
}

bool SocialConfigure::LoadHomelandChurchChest(const std::string &file) {
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
    HomelandChurchChestCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("pray_odds", (int *)&cell.pray_odds_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[pray_odds] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("refresh_odds", (int *)&cell.refresh_odds_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[refresh_odds] not found.", file.c_str());
      return false;
    }

    this->homeland_church_chests_store_.push_back(cell);
    this->homeland_church_chests_index_.insert(
      std::make_pair(cell.id_, this->homeland_church_chests_store_.size() - 1));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool SocialConfigure::LoadHomelandChurchBonus(const std::string &file) {
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
    HomelandChurchBonusCell bonus_cell;
    core::int32 chest_id;

    if (cell_node->Attribute("chest_id", (int *)&chest_id) == false) {
      LOG_ERROR("Config file (%s) /data/cell[chest_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("bonus_id", (int *)&bonus_cell.bonus_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[bonus_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("odds", (int *)&bonus_cell.odds_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[odds] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("item_id", (int *)&bonus_cell.item_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[item_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("item_count", (int *)&bonus_cell.item_count_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[item_count] not found.", file.c_str());
      return false;
    }

    HomelandChurchChestCell *chest_cell = const_cast<HomelandChurchChestCell *>(
        this->GetHomelandChurchChest(chest_id));
    if (NULL == chest_cell) {
      LOG_ERROR("Config file (%s) /data/cell[chest_id] is invalid.", file.c_str());
      return false;
    }

    chest_cell->bonus_set_.push_back(bonus_cell);

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 建立索引
  for (size_t i = 0; i < this->homeland_church_chests_store_.size(); ++i) {
    HomelandChurchChestCell *chest_cell = &this->homeland_church_chests_store_[i];

    for (size_t j = 0; j < chest_cell->bonus_set_.size(); ++j) {
      HomelandChurchBonusCell *bonus_cell = &chest_cell->bonus_set_[j];

      core::uint64 index =
          ((core::uint64)chest_cell->id_ << 32) + (core::uint64)bonus_cell->bonus_id_;

      this->homeland_church_bonus_index_.insert(std::make_pair(index, bonus_cell));
    }
  }

  return true;
}

bool SocialConfigure::LoadHomelandTree(const std::string &file) {
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
    HomelandTreeCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_required_harvest_times",
            (int *)&cell.upgrade_required_harvest_times_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_required_harvest_times] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("growing_required_water_times",
            (int *)&cell.growing_required_water_times_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[growing_required_water_times] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("water_cooling", (int *)&cell.water_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[water_cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("growing_cooling", (int *)&cell.growing_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[growing_cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("harvest_restrict_dollars_reward",
            (int *)&cell.harvest_restrict_dollars_reward_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[harvest_restrict_dollars_reward] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_stolen_times", (int *)&cell.max_stolen_times_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_stolen_times] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("stolen_loss_rate", (int *)&cell.stolen_loss_rate_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[stolen_loss_rate] not found.", file.c_str());
      return false;
    }

    this->homeland_trees_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 有效性检查
  if (this->GetHomelandTree(1) == NULL) {
    LOG_ERROR("Config file (%s) no level 1 data.");
    return false;
  }
  
  return true;
}

bool SocialConfigure::LoadHomelandTower(const std::string &file) {
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
    HomelandTowerCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_spend_gold", (int *)&cell.upgrade_spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_spend_gold] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_cooling", (int *)&cell.upgrade_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("attack", (int *)&cell.attack_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attack] not found.", file.c_str());
      return false;
    }

    this->homeland_towers_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 有效性检查
  if (this->GetHomelandTower(1) == NULL) {
    LOG_ERROR("Config file (%s) no level 1 data.");
    return false;
  }

  return true;
}

bool SocialConfigure::LoadHomelandStronghold(const std::string &file) {
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
    HomelandStrongholdCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_spend_gold", (int *)&cell.upgrade_spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_spend_gold] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("upgrade_cooling", (int *)&cell.upgrade_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[upgrade_cooling] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_support_slot_capacity",
            (int *)&cell.max_support_slot_capacity_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_support_slot_capacity] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("call_support_cooling", (int *)&cell.call_support_cooling_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[call_support_cooling] not found.", file.c_str());
      return false;
    }

    this->homeland_strongholds_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 有效性检查
  if (this->GetHomelandStronghold(1) == NULL) {
    LOG_ERROR("Config file (%s) no level 1 data.");
    return false;
  }

  return true;
}

bool SocialConfigure::LoadHomelandSoul(const std::string &file) {
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
    HomelandSoulCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }

    // 检查 type
    core::int32 type;
    if (cell_node->Attribute("type", (int *)&type) == false) {
      LOG_ERROR("Config file (%s) /data/cell[type] not found.", file.c_str());
      return false;
    }
    if (type < HomelandSoulCell::SoulType::MIN ||
        type >= HomelandSoulCell::SoulType::MAX) {
      LOG_ERROR("Config file (%s) /data/cell[type] is invalid.", file.c_str());
      return false;
    } else {
      cell.type_ = (HomelandSoulCell::SoulType::type)type;
    }

    if (cell_node->Attribute("hp_base", (int *)&cell.hp_base_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[hp_base] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("hp_growth", (int *)&cell.hp_growth_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[hp_growth] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("attack_base", (int *)&cell.attack_base_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attack_base] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("attack_growth", (int *)&cell.attack_growth_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attack_growth] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("dodge_percent", (int *)&cell.dodge_percent_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[dodge_percent] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("crit_percent", (int *)&cell.crit_percent_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[crit_percent] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("crit_effect", (int *)&cell.crit_effect_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[crit_effect] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("special_skill_odds", (int *)&cell.special_skill_odds_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[special_skill_odds] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("special_skill_hurt_coe", (int *)&cell.special_skill_hurt_coe_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[special_skill_hurt_coe] not found.", file.c_str());
      return false;
    }

    this->homeland_souls_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 有效性检查
  if (this->GetHomelandSoul(HomelandSoulCell::SpecialIDType::AXE_ACTOR) == NULL ||
      this->GetHomelandSoul(HomelandSoulCell::SpecialIDType::SHOOT_ACTOR) == NULL ||
      this->GetHomelandSoul(HomelandSoulCell::SpecialIDType::MAGIC_ACTOR) == NULL) {
    LOG_ERROR("Config file (%s) no special id data.");
    return false;
  }

  return true;
}

const HomelandTempleCell *SocialConfigure::GetHomelandTemple(core::int32 level) const {
  HomelandTempleHashmap::const_iterator iter = this->homeland_temples_.find(level);
  if (iter == this->homeland_temples_.end()) {
    return NULL;
  }
  return &iter->second;
}

const HomelandGoldmineCell *SocialConfigure::GetHomelandGoldmine(core::int32 level) const {
  HomelandGoldmineHashmap::const_iterator iter = this->homeland_goldmines_.find(level); 
  if (iter == this->homeland_goldmines_.end()) {
    return NULL;
  }
  return &iter->second;
}

const HomelandPoolCell *SocialConfigure::GetHomelandPool(core::int32 level) const {
  HomelandPoolHashmap::const_iterator iter = this->homeland_pools_.find(level);
  if (iter == this->homeland_pools_.end()) {
    return NULL;
  }
  return &iter->second;
}

const HomelandChurchChestCell *SocialConfigure::GetHomelandChurchChest(core::int32 id) const {
  HomelandChurchChestIndexHashmap::const_iterator iter =
    this->homeland_church_chests_index_.find(id);
  if (iter == this->homeland_church_chests_index_.end()) {
    return NULL;
  }
  return &this->homeland_church_chests_store_[iter->second];
}

const HomelandChurchChestCell *SocialConfigure::GetLastHomelandChurchChest() const {
  if (this->homeland_church_chests_store_.empty()) {
    return NULL;
  }
  return &this->homeland_church_chests_store_.back();
}

const HomelandChurchChestCell *SocialConfigure::GetRandomHomelandChurchChestByPrayOdds(
    core::uint32 random) const {
  core::uint32 odds = 0;

  for (size_t i = 0; i < this->homeland_church_chests_store_.size(); ++i) {
    odds += this->homeland_church_chests_store_[i].pray_odds_;
    if (odds >= random) {
      return &this->homeland_church_chests_store_[i];
    }
  }

  return NULL;
}

const HomelandChurchChestCell *SocialConfigure::GetRandomHomelandChurchChestByRefreshOdds(
    core::uint32 random) const {
  core::uint32 odds = 0;

  for (size_t i = 0; i < this->homeland_church_chests_store_.size(); ++i) {
    odds += this->homeland_church_chests_store_[i].refresh_odds_;
    if (odds >= random) {
      return &this->homeland_church_chests_store_[i];
    }
  }

  return NULL;
}

const HomelandChurchBonusCell *SocialConfigure::GetHomelandChurchBonus(core::int32 chest_id,
    core::int32 bonus_id) const {
  core::uint64 index = ((core::uint64)chest_id << 32) + (core::uint64)bonus_id;

  HomelandChurchBonusIndexHashmap::const_iterator iter = 
    this->homeland_church_bonus_index_.find(index);
  if (iter == this->homeland_church_bonus_index_.end()) {
    return NULL;
  }
  return iter->second;
}

const HomelandChurchBonusCell *SocialConfigure::GetRandomHomelandChurchBonus(core::int32 chest_id,
    core::uint32 random) const {
  const HomelandChurchChestCell *chest_cell = this->GetHomelandChurchChest(chest_id);
  if (NULL == chest_cell) {
    return NULL;
  }

  core::uint32 odds = 0;

  for (size_t i = 0; i < chest_cell->bonus_set_.size(); ++i) {
    odds += chest_cell->bonus_set_[i].odds_;
    if (odds >= random) {
      return &chest_cell->bonus_set_[i];
    }
  }

  return NULL;
}

const HomelandTreeCell *SocialConfigure::GetHomelandTree(core::int32 level) const {
  HomelandTreeHashmap::const_iterator iter = this->homeland_trees_.find(level);
  if (iter == this->homeland_trees_.end()) {
    return NULL;
  }
  return &iter->second;
}

const HomelandTowerCell *SocialConfigure::GetHomelandTower(core::int32 level) const {
  HomelandTowerHashmap::const_iterator iter = this->homeland_towers_.find(level);
  if (iter == this->homeland_towers_.end()) {
    return NULL;
  }
  return &iter->second;
}

const HomelandStrongholdCell *SocialConfigure::GetHomelandStronghold(core::int32 level) const {
  HomelandStrongholdHashmap::const_iterator iter = this->homeland_strongholds_.find(level);
  if (iter == this->homeland_strongholds_.end()) {
    return NULL;
  }
  return &iter->second;
}

const HomelandSoulCell *SocialConfigure::GetHomelandSoul(core::int32 id) const {
  HomelandSoulHashmap::const_iterator iter = this->homeland_souls_.find(id);
  if (iter == this->homeland_souls_.end()) {
    return NULL;
  }
  return &iter->second;
}

}  // namespace configure

}  // namespace global

