#include "global/configure/guild_configure.h"

#include <cstddef>
#include <algorithm>

#include "core/tinyxml/tinyxml.h"
#include "entity/guild_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace global {

namespace configure {

GuildConfigure::GuildConfigure() {}
GuildConfigure::~GuildConfigure() {}

bool GuildConfigure::LoadConfigure(const std::string &file) {
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

  // 加载节点 guild
  TiXmlElement *guild_node = configure_node->FirstChildElement("guild");
  if (NULL == guild_node) {
    LOG_ERROR("Config file (%s) /configure/guild not found.", file.c_str());
    return false;
  }
  node_attr = guild_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuild(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild config file failed.");
    return false;
  }

  // 加载节点 guild_position
  TiXmlElement *guild_position_node = configure_node->FirstChildElement("guild_position");
  if (NULL == guild_position_node) {
    LOG_ERROR("Config file (%s) /configure/guild_position not found.", file.c_str());
    return false;
  }
  node_attr = guild_position_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_position[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildPosition(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild position config file failed.");
    return false;
  }

  // 加载节点 guild_logo
  TiXmlElement *guild_logo_node = configure_node->FirstChildElement("guild_logo");
  if (NULL == guild_logo_node) {
    LOG_ERROR("Config file (%s) /configure/guild_logo not found.", file.c_str());
    return false;
  }
  node_attr = guild_logo_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_logo[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildLogo(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild logo config file failed.");
    return false;
  }

  // 加载节点 guild_shop
  TiXmlElement *guild_shop_node = configure_node->FirstChildElement("guild_shop");
  if (NULL == guild_shop_node) {
    LOG_ERROR("Config file (%s) /configure/guild_shop not found.", file.c_str());
    return false;
  }
  node_attr = guild_shop_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_shop[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildShop(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild shop config file failed.");
    return false;
  }

  // 加载节点 guild_shop_item
  TiXmlElement *guild_shop_item_node = configure_node->FirstChildElement("guild_shop_item");
  if (NULL == guild_shop_item_node) {
    LOG_ERROR("Config file (%s) /configure/guild_shop_item not found.", file.c_str());
    return false;
  }
  node_attr = guild_shop_item_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_shop_item[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildShopItem(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild shop item config file failed.");
    return false;
  }

  // 加载节点 guild_college
  TiXmlElement *guild_college_node = configure_node->FirstChildElement("guild_college");
  if (NULL == guild_college_node) {
    LOG_ERROR("Config file (%s) /configure/guild_college not found.", file.c_str());
    return false;
  }
  node_attr = guild_college_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_college[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildCollege(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild college config file failed.");
    return false;
  }

  // 加载节点 guild_skill
  TiXmlElement *guild_skill_node = configure_node->FirstChildElement("guild_skill");
  if (NULL == guild_skill_node) {
    LOG_ERROR("Config file (%s) /configure/guild_skill not found.", file.c_str());
    return false;
  }
  node_attr = guild_skill_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_skill[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildSkill(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild skill config file failed.");
    return false;
  }

  // 加载节点 guild_buff
  TiXmlElement *guild_buff_node = configure_node->FirstChildElement("guild_buff");
  if (NULL == guild_buff_node) {
    LOG_ERROR("Config file (%s) /configure/guild_buff not found.", file.c_str());
    return false;
  }
  node_attr = guild_buff_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_buff[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildBuff(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild buff config file failed.");
    return false;
  }

  // 加载节点 guild_pray
  TiXmlElement *guild_pray_node = configure_node->FirstChildElement("guild_pray");
  if (NULL == guild_pray_node) {
    LOG_ERROR("Config file (%s) /configure/guild_pray not found.", file.c_str());
    return false;
  }
  node_attr = guild_pray_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_pray[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildPray(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild pray config file failed.");
    return false;
  }

  // 加载节点 guild_barrack
  TiXmlElement *guild_barrack_node = configure_node->FirstChildElement("guild_barrack");
  if (NULL == guild_barrack_node) {
    LOG_ERROR("Config file (%s) /configure/guild_barrack not found.", file.c_str());
    return false;
  }
  node_attr = guild_barrack_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_barrack[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildBarrack(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild barrack config file failed.");
    return false;
  }

  // 加载节点 guild_playing_group
  TiXmlElement *guild_playing_group_node =
    configure_node->FirstChildElement("guild_playing_group");
  if (NULL == guild_playing_group_node) {
    LOG_ERROR("Config file (%s) /configure/guild_playing_group not found.", file.c_str());
    return false;
  }
  node_attr = guild_playing_group_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_playing_group[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildPlayingGroup(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild playing group config file failed.");
    return false;
  }

  // 加载节点 guild_playing
  TiXmlElement *guild_playing_node = configure_node->FirstChildElement("guild_playing"); 
  if (NULL == guild_playing_node) {
    LOG_ERROR("Config file (%s) /configure/guild_playing not found.", file.c_str());
    return false;
  }
  node_attr = guild_playing_node->Attribute("file");
  if (NULL == node_attr) {
    LOG_ERROR("Config file (%s) /configure/guild_playing[file] not found.", file.c_str());
    return false;
  }
  if (this->LoadGuildPlaying(config_dir + node_attr) == false) {
    LOG_ERROR("Load guild playing config file failed.");
    return false;
  }

  return true;
}

bool GuildConfigure::LoadGuild(const std::string &file) {
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
    GuildCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_members", (int *)&cell.max_members_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_members] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("maintenance_fee", (int *)&cell.maintenance_fee_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[maintenance_fee] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("maintenance_fee_limit",
            (int *)&cell.maintenance_fee_limit_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[maintenance_fee_limit] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_gold", (int *)&cell.spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_gold] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_light_crystal", (int *)&cell.spend_light_crystal_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_light_crystal] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_dark_crystal", (int *)&cell.spend_dark_crystal_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_dark_crystal] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_holy_crystal", (int *)&cell.spend_holy_crystal_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_holy_crystal] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_shop_level", (int *)&cell.max_shop_level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_shop_level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_college_level", (int *)&cell.max_college_level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_college_level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_barrack_level", (int *)&cell.max_barrack_level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_barrack_level] not found.", file.c_str());
      return false;
    }

    this->guilds_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool GuildConfigure::LoadGuildPosition(const std::string &file) {
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
    GuildPositionCell cell;
    core::int32 i;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("seq", (int *)&cell.seq_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[seq] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("max_num", (int *)&cell.max_num_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[max_num] not found.", file.c_str());
      return false;
    }

    if (cell_node->Attribute("authority_suppress_join", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_suppress_join] not found.", file.c_str());
      return false;
    }
    cell.authority_suppress_join_ = i;

    if (cell_node->Attribute("authority_invite", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_invite] not found.", file.c_str());
      return false;
    }
    cell.authority_invite_ = i;

    if (cell_node->Attribute("authority_audit_pending_member", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_audit_pending_member] not found.", file.c_str());
      return false;
    }
    cell.authority_audit_pending_member_ = i;

    if (cell_node->Attribute("authority_leave_guild", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_leave_guild] not found.", file.c_str());
      return false;
    }
    cell.authority_leave_guild_ = i;

    if (cell_node->Attribute("authority_kick_member", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_kick_member] not found.", file.c_str());
      return false;
    }
    cell.authority_kick_member_ = i;

    if (cell_node->Attribute("authority_set_member_position", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_set_member_position] not found.", file.c_str());
      return false;
    }
    cell.authority_set_member_position_ = i;

    if (cell_node->Attribute("authority_give_up_position", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_give_up_position] not found.", file.c_str());
      return false;
    }
    cell.authority_give_up_position_ = i;

    if (cell_node->Attribute("authority_edit_announcement", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_edit_announcement] not found.", file.c_str());
      return false;
    }
    cell.authority_edit_announcement_ = i;

    if (cell_node->Attribute("authority_chat", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_chat] not found.", file.c_str());
      return false;
    }
    cell.authority_chat_ = i;

    if (cell_node->Attribute("authority_donate", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_donate] not found.", file.c_str());
      return false;
    }
    cell.authority_donate_ = i;

    if (cell_node->Attribute("authority_upgrade_facility", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_upgrade_facility] not found.", file.c_str());
      return false;
    }
    cell.authority_upgrade_facility_ = i;

    if (cell_node->Attribute("authority_shop_buy_item", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_shop_buy_item] not found.", file.c_str());
      return false;
    }
    cell.authority_shop_buy_item_ = i;

    if (cell_node->Attribute("authority_college_lean_skill", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_college_lean_skill] not found.", file.c_str());
      return false;
    }
    cell.authority_college_lean_skill_ = i;

    if (cell_node->Attribute("authority_idol_pray", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_idol_pray] not found.", file.c_str());
      return false;
    }
    cell.authority_idol_pray_ = i;

    if (cell_node->Attribute("authority_open_playing", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_open_playing] not found.", file.c_str());
      return false;
    }
    cell.authority_open_playing_ = i;

    if (cell_node->Attribute("authority_create_playing", (int *)&i) == false) {
      LOG_ERROR("Config file (%s) /data/cell[authority_create_playing] not found.", file.c_str());
      return false;
    }
    cell.authority_create_playing_ = i;

    this->guild_positions_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  // 检查必须值是否配置
  if (this->GetGuildPosition(entity::GuildPositionType::MEMBER) == NULL) {
    LOG_ERROR("Config file (%s) GuildPosition::MEMBER is not in config."); 
    return false;
  }
  if (this->GetGuildPosition(entity::GuildPositionType::LEADER) == NULL) {
    LOG_ERROR("Config file (%s) GuildPosition::LEADER is not in config."); 
    return false;
  }
  
  return true;
}

bool GuildConfigure::LoadGuildLogo(const std::string &file) {
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
    core::int32 id;

    if (cell_node->Attribute("id", (int *)&id) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }

    this->guild_logos_.insert(id);

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool GuildConfigure::LoadGuildShop(const std::string &file) {
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
    GuildShopCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_gold", (int *)&cell.spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_gold] not found.", file.c_str());
      return false;
    }

    this->guild_shops_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool GuildConfigure::LoadGuildShopItem(const std::string &file) {
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
    GuildShopItemCell cell;

    if (cell_node->Attribute("item_id", (int *)&cell.item_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[item_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("required_guild_shop_level",
            (int *)&cell.required_guild_shop_level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[required_guild_shop_level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("required_guild_attribute_sum",
            (int *)&cell.required_guild_attribute_sum_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[required_guild_attribute_sum] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_contribution", (int *)&cell.spend_contribution_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_contribution] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_freedom_dollars", (int *)&cell.spend_freedom_dollars_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_freedom_dollars] not found.", file.c_str());
      return false;
    }

    this->guild_shop_items_.insert(std::make_pair(cell.item_id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool GuildConfigure::LoadGuildCollege(const std::string &file) {
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
    GuildCollegeCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_gold", (int *)&cell.spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_gold] not found.", file.c_str());
      return false;
    }

    this->guild_colleges_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool GuildConfigure::LoadGuildSkill(const std::string &file) {
  TiXmlDocument doc;
  std::set<core::int32> guild_skills;

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
    GuildSkillCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("required_guild_college_level",
            (int *)&cell.required_guild_college_level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[required_guild_college_level] not found.", file.c_str());
      return false;
    }
    core::int32 attr_id;
    if (cell_node->Attribute("attr", (int *)&attr_id) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attr] not found.", file.c_str());
      return false;
    }
    if (attr_id < GuildSkillCell::AttrType::MIN ||
        attr_id >= GuildSkillCell::AttrType::MAX) {
      LOG_ERROR("Config file (%s) /data/cell[attr] is invalid.", file.c_str());
      return false;
    } else {
      cell.attr_type_ = (GuildSkillCell::AttrType::type)attr_id;
    }

    if (cell_node->Attribute("attr_value", (int *)&cell.attr_value_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[attr_value] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_contribution", (int *)&cell.spend_contribution_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_contribution] not found.", file.c_str());
      return false;
    }

    // 技能ID
    guild_skills.insert(cell.id_);

    // 技能
    core::uint64 index = ((core::uint64)cell.id_ << 32) + (core::uint64)cell.level_;
    this->guild_skills_.insert(std::make_pair(index, cell));

    // 技能最大等级
    index = ((core::uint64)cell.required_guild_college_level_ << 32) + (core::uint64)cell.id_;
    GuildSkillMaxLevelHashmap::iterator iter =
      this->guild_skill_max_levels_.find(index);
    if (iter == this->guild_skill_max_levels_.end()) {
      this->guild_skill_max_levels_.insert(std::make_pair(index, cell.level_));
    } else {
      iter->second = std::max(iter->second, cell.level_);
    }

    cell_node = cell_node->NextSiblingElement("cell");
  }

  for (std::set<core::int32>::const_iterator iter = guild_skills.begin();
       iter != guild_skills.end(); ++iter) {
    core::uint64 index = ((core::uint64)1 << 32) + (core::uint64)(*iter);
      GuildSkillMaxLevelHashmap::const_iterator iter2 =
        this->guild_skill_max_levels_.find(index);
      if (iter2 == this->guild_skill_max_levels_.end()) {
        this->guild_skill_max_levels_.insert(std::make_pair(index, 0));
      }
  }

  for (core::int32 i = 2; ; ++i) {
    if (this->GetGuildCollege(i) == NULL) {
      break;
    }

    for (std::set<core::int32>::const_iterator iter = guild_skills.begin();
         iter != guild_skills.end(); ++iter) {
      core::uint64 prev_index = ((core::uint64)(i - 1) << 32) + (core::uint64)(*iter);
      core::uint64 index = ((core::uint64)i << 32) + (core::uint64)(*iter);
      GuildSkillMaxLevelHashmap::const_iterator iter2 =
        this->guild_skill_max_levels_.find(index);
      if (iter2 == this->guild_skill_max_levels_.end()) {
        this->guild_skill_max_levels_.insert(
            std::make_pair(index, this->guild_skill_max_levels_[prev_index]));
      } else {
        this->guild_skill_max_levels_[index] =
          std::max(this->guild_skill_max_levels_[index],
                   this->guild_skill_max_levels_[prev_index]);
      }
    }
  }

  return true;
}

bool GuildConfigure::LoadGuildBuff(const std::string &file) {
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
    GuildBuffCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("buff_id", (int *)&cell.buff_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[buff_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("required_light_attribute",
            (int *)&cell.required_light_attribute_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[required_light_attribute] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("required_dark_attribute",
            (int *)&cell.required_dark_attribute_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[required_dark_attribute] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("required_holy_attribute",
            (int *)&cell.required_holy_attribute_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[required_holy_attribute] not found.", file.c_str());
      return false;
    }

    this->guild_buffs_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool GuildConfigure::LoadGuildPray(const std::string &file) {
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
    GuildPrayCell cell;

    if (cell_node->Attribute("pray_times", (int *)&cell.pray_times_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[pray_times] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_contribution", (int *)&cell.spend_contribution_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_contribution] not found.", file.c_str());
      return false;
    }

    this->guild_prays_.insert(std::make_pair(cell.pray_times_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool GuildConfigure::LoadGuildBarrack(const std::string &file) {
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
    GuildBarrackCell cell;

    if (cell_node->Attribute("level", (int *)&cell.level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_gold", (int *)&cell.spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_gold] not found.", file.c_str());
      return false;
    }

    this->guild_barracks_.insert(std::make_pair(cell.level_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool GuildConfigure::LoadGuildPlayingGroup(const std::string &file) {
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
    GuildPlayingGroupCell cell;

    if (cell_node->Attribute("id", (int *)&cell.id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("required_guild_level",
            (int *)&cell.required_guild_level_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[required_guild_level] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("spend_gold", (int *)&cell.spend_gold_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[spend_gold] not found.", file.c_str());
      return false;
    }

    this->guild_playing_groups_.insert(std::make_pair(cell.id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

bool GuildConfigure::LoadGuildPlaying(const std::string &file) {
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
    GuildPlayingCell cell;

    if (cell_node->Attribute("playing_template_id",
            (int *)&cell.playing_template_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[playing_template_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("group_id", (int *)&cell.group_id_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[group_id] not found.", file.c_str());
      return false;
    }
    if (cell_node->Attribute("depend", (int *)&cell.depend_) == false) {
      LOG_ERROR("Config file (%s) /data/cell[depend] not found.", file.c_str());
      return false;
    }

    // 解析奖励
    {
      const char *awards = cell_node->Attribute("awards");
      if (NULL == awards) {
        LOG_ERROR("Config file (%s) /data/cell[awards] not found.", file.c_str());
        return false;
      }

      std::vector<std::string> split_result;
      global::StringSplit(awards, "+", split_result);
      for (size_t i = 0; i < split_result.size(); ++i) {
        if (split_result[i].empty()) {
          continue;
        }
        std::vector<std::string> split_result2;
        global::StringSplit(split_result[i].c_str(), ":", split_result2);

        if (split_result2.size() != 2) {
          LOG_ERROR("Config file (%s) /data/cell[awards] is invalid.", file.c_str());
          return false;
        }

        entity::AddItemFields field;
        field.__set_item_template_(atoi(split_result2[0].c_str()));
        field.__set_number_(atoi(split_result2[1].c_str()));
        field.__set_bind_(true);
        field.__set_intensify_level_(0);
        cell.awards_.push_back(field);
      }
    }

    this->guild_playings_.insert(std::make_pair(cell.playing_template_id_, cell));

    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

const GuildCell *GuildConfigure::GetGuild(core::int32 level) const {
  GuildHashmap::const_iterator iter = this->guilds_.find(level);
  if (iter == this->guilds_.end()) {
    return NULL;
  }
  return &iter->second;
}

const GuildPositionCell *GuildConfigure::GetGuildPosition(core::int32 id) const {
  GuildPositionHashmap::const_iterator iter = this->guild_positions_.find(id);
  if (iter == this->guild_positions_.end()) {
    return NULL;
  }
  return &iter->second;
}

bool GuildConfigure::CheckGuildLogoExist(core::int32 id) const {
  if (this->guild_logos_.find(id) != this->guild_logos_.end()) {
    return true;
  } else {
    return false;
  }
}

const GuildShopCell *GuildConfigure::GetGuildShop(core::int32 level) const {
  GuildShopHashmap::const_iterator iter = this->guild_shops_.find(level);
  if (iter == this->guild_shops_.end()) {
    return NULL;
  }
  return &iter->second;
}

const GuildShopItemCell *GuildConfigure::GetGuildShopItem(core::uint32 item_id) const {
  GuildShopItemHashmap::const_iterator iter = this->guild_shop_items_.find(item_id);
  if (iter == this->guild_shop_items_.end()) {
    return NULL;
  }
  return &iter->second;
}

const GuildCollegeCell *GuildConfigure::GetGuildCollege(core::int32 level) const {
  GuildCollegeHashmap::const_iterator iter = this->guild_colleges_.find(level);
  if (iter == this->guild_colleges_.end()) {
    return NULL;
  }
  return &iter->second;
}

const GuildSkillCell *GuildConfigure::GetGuildSkill(core::int32 id, core::int32 level) const {
  core::uint64 index = ((core::uint64)id << 32) + (core::uint64)level;
  GuildSkillHashmap::const_iterator iter = this->guild_skills_.find(index);
  if (iter == this->guild_skills_.end()) {
    return NULL;
  }
  return &iter->second;
}

core::int32 GuildConfigure::GetMaxSkillLevelInCollegeLevel(core::int32 college_level,
    core::int32 skill_id) const {
  core::uint64 index = ((core::uint64)college_level << 32) + (core::uint64)skill_id;
  GuildSkillMaxLevelHashmap::const_iterator iter = this->guild_skill_max_levels_.find(index);
  if (iter == this->guild_skill_max_levels_.end()) {
    return 0;
  }
  return iter->second;
}

const GuildBuffCell *GuildConfigure::GetGuildBuff(core::int32 id) const {
  GuildBuffHashmap::const_iterator iter = this->guild_buffs_.find(id);
  if (iter == this->guild_buffs_.end()) {
    return NULL;
  }
  return &iter->second;
}

const GuildPrayCell *GuildConfigure::GetGuildPray(core::int32 pray_times) const {
  GuildPrayHashmap::const_iterator iter = this->guild_prays_.find(pray_times);
  if (iter == this->guild_prays_.end()) {
    return NULL;
  }
  return &iter->second;
}

const GuildBarrackCell *GuildConfigure::GetGuildBarrack(core::int32 level) const {
  GuildBarrackHashmap::const_iterator iter = this->guild_barracks_.find(level);
  if (iter == this->guild_barracks_.end()) {
    return NULL;
  }
  return &iter->second;
}

const GuildPlayingGroupCell *GuildConfigure::GetGuildPlayingGroup(core::int32 id) const {
  GuildPlayingGroupHashmap::const_iterator iter = this->guild_playing_groups_.find(id);
  if (iter == this->guild_playing_groups_.end()) {
    return NULL;
  }
  return &iter->second;
}

const GuildPlayingCell *GuildConfigure::GetGuildPlaying(core::int32 playing_template_id) const {
  GuildPlayingHashmap::const_iterator iter = this->guild_playings_.find(playing_template_id);
  if (iter == this->guild_playings_.end()) {
    return NULL;
  }
  return &iter->second;
}

}  // namespace configure

}  // namespace global

