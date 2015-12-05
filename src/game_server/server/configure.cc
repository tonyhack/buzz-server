//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-15 16:14:22.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "game_server/server/configure.h"

#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

bool Configure::Initialize() {
  if(this->according_level_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetAccordingLevelFile()) == false) {
    global::LogError("%s:%d (%s) 加载等级类配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_attr_convert_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetAttrConvertFile()) == false) {
    LOG_ERROR("加载 LoadAttrConvertConfigure 失败");
    return false;
  }
  if(this->load_attr_fight_score_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetAttrFightScoreFile()) == false) {
    LOG_ERROR("加载 LoadAttrFightScore 失败");
    return false;
  }
  if(this->load_drop_.LoadDropConfig(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetDropFile()) == false) {
    LOG_ERROR("加载 LoadDropConfigure 失败");
    return false;
  }
  if(this->load_drop_.LoadDropSubConfig(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetSubDropFile()) == false) {
    LOG_ERROR("加载 LoadDropSubConfigure 失败");
    return false;
  }
  if(this->load_vocation_growth_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetVocationGrowthFile()) == false) {
    LOG_ERROR("加载 LoadVocationGrowthConfigure 失败");
    return false;
  }
  if(this->npc_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetNpcFile()) == false) {
    global::LogError("%s:%d (%s) 加载NPC配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_camp_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetCampFile()) == false) {
    global::LogError("%s:%d (%s) 加载 camp 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_functionality_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        global::ServerConfigureSingleton::GetInstance().GetFunctionalityFile()) == false) {
    global::LogError("%s:%d (%s) 加载 functionality open 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

entity::FunctionalityType::type Configure::StrConvertToFunctionality(const char *str) {
  if(str == NULL) {
    LOG_ERROR("功能开启字符串为空");
    return entity::FunctionalityType::MAX;
  }
  if (strcmp("ITEM_COMPOSE", str) == 0) {
    return entity::FunctionalityType::ITEM_COMPOSE;
  } else if(strcmp("EQUIP_INTENSIFY", str) == 0) {
    return entity::FunctionalityType::EQUIP_INTENSIFY;
  } else if(strcmp("GEM_MODULE", str) == 0) {
    return entity::FunctionalityType::GEM_MODULE;
  } else if(strcmp("GEM_SLOT_2", str) == 0) {
    return entity::FunctionalityType::GEM_SLOT_2;
  } else if(strcmp("GEM_SLOT_3", str) == 0) {
    return entity::FunctionalityType::GEM_SLOT_3;
  } else if(strcmp("GEM_SLOT_4", str) == 0) {
    return entity::FunctionalityType::GEM_SLOT_4;
  } else if(strcmp("GEM_SLOT_5", str) == 0) {
    return entity::FunctionalityType::GEM_SLOT_5;
  } else if(strcmp("GEM_SLOT_6", str) == 0) {
    return entity::FunctionalityType::GEM_SLOT_6;
  } else if(strcmp("GEM_SLOT_7", str) == 0) {
    return entity::FunctionalityType::GEM_SLOT_7;
  } else if(strcmp("GEM_SLOT_8", str) == 0) {
    return entity::FunctionalityType::GEM_SLOT_8;
  } else if(strcmp("GEM_SLOT_9", str) == 0) {
    return entity::FunctionalityType::GEM_SLOT_9;
  } else if(strcmp("SOUL_MODULE", str) == 0) {
    return entity::FunctionalityType::SOUL_MODULE;
  } else if(strcmp("SOUL_ALTAR", str) == 0) {
    return entity::FunctionalityType::SOUL_ALTAR;
  } else if(strcmp("SWORD_MODULE", str) == 0) {
    return entity::FunctionalityType::SWORD_MODULE;
  } else if(strcmp("NOBLE_MODULE", str) == 0) {
    return entity::FunctionalityType::NOBLE_MODULE;
  } else if(strcmp("ELEMENT_MODULE", str) == 0) {
    return entity::FunctionalityType::ELEMENT_MODULE;
  } else if(strcmp("SPIRIT_MODULE", str) == 0) {
    return entity::FunctionalityType::SPIRIT_MODULE;
  } else if(strcmp("SPIRIT_UPGRADE", str) == 0) {
    return entity::FunctionalityType::SPIRIT_UPGRADE;
  } else if(strcmp("SPIRIT_WING_FACADE", str) == 0) {
    return entity::FunctionalityType::SPIRIT_WING_FACADE;
  } else if(strcmp("LEGENDARY_WEAPON_MODULE", str) == 0) {
    return entity::FunctionalityType::LEGENDARY_WEAPON_MODULE;
  } else if(strcmp("MAIL_MODULE", str) == 0) {
    return entity::FunctionalityType::MAIL_MODULE;
  } else if(strcmp("CONTACTS_MODULE", str) == 0) {
    return entity::FunctionalityType::CONTACTS_MODULE;
  } else if(strcmp("HOMELAND_MODULE", str) == 0) {
    return entity::FunctionalityType::HOMELAND_MODULE;
  } else if(strcmp("HOMELAND_GOLDMINE", str) == 0) {
    return entity::FunctionalityType::HOMELAND_GOLDMINE;
  } else if(strcmp("HOMELAND_POOL", str) == 0) {
    return entity::FunctionalityType::HOMELAND_POOL;
  } else if(strcmp("HOMELAND_CHURCH", str) == 0) {
    return entity::FunctionalityType::HOMELAND_CHURCH;
  } else if(strcmp("HOMELAND_TREE", str) == 0) {
    return entity::FunctionalityType::HOMELAND_TREE;
  } else if(strcmp("HOMELAND_TOWER", str) == 0) {
    return entity::FunctionalityType::HOMELAND_TOWER;
  } else if(strcmp("HOMELAND_STRONGHOLD", str) == 0) {
    return entity::FunctionalityType::HOMELAND_STRONGHOLD;
  } else if(strcmp("TEAM_MODULE", str) == 0) {
    return entity::FunctionalityType::TEAM_MODULE;
  } else if(strcmp("GUILD_MODULE", str) == 0) {
    return entity::FunctionalityType::GUILD_MODULE;
  } else if(strcmp("TASK_CIRCLE", str) == 0) {
    return entity::FunctionalityType::TASK_CIRCLE;
  } else if(strcmp("TASK_RISK", str) == 0) {
    return entity::FunctionalityType::TASK_RISK;
  } else if(strcmp("TASK_TRIP", str) == 0) {
    return entity::FunctionalityType::TASK_TRIP;
  } else if(strcmp("VOCATION_SET", str) == 0) {
    return entity::FunctionalityType::VOCATION_SET;
  } else if(strcmp("AUCTION_MODULE", str) == 0) {
    return entity::FunctionalityType::AUCTION_MODULE;
  } else if(strcmp("MAZE_MODULE", str) == 0) {
    return entity::FunctionalityType::MAZE_MODULE;
  } else if(strcmp("SOUL_SETING", str) == 0) {
    return entity::FunctionalityType::SOUL_SETING;
  } else if(strcmp("PLAYING_MODULE", str) == 0) {
    return entity::FunctionalityType::PLAYING_MODULE;
  } else if(strcmp("MULTI_PLAYING", str) == 0) {
    return entity::FunctionalityType::MULTI_PLAYING;
  } else if (strcmp("HUNT_TREASURE", str) == 0) {
    return entity::FunctionalityType::HUNT_TREASURE;
  } else if(strcmp("TREASURE", str) == 0) {
    return entity::FunctionalityType::TREASURE;
  } else {
    LOG_ERROR("当前的功能类型错误[%s]", str);
  }
  return entity::FunctionalityType::MAX;
}

entity::FunctionalityType::type Configure::GetServerDefineByFunctionality(core::int32 functionality_id) {
  const char *str = this->GetFunctionality(functionality_id);
  if(str != NULL) {
    return this->StrConvertToFunctionality(str);
  }
  LOG_DEBUG("将功能ID(%d)转换成(%d)", functionality_id, this->StrConvertToFunctionality(str));
  return entity::FunctionalityType::MAX;
}

const char *Configure::GetFunctionality(core::int32 id) {
  return this->load_functionality_.GetFunctionalityStr(id);
}

void Configure::Finalize() {}

}  // namespace server

}  // namespace game

