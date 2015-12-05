#include "session_server/server/extension/social/homeland_battle_manager.h"

#include <algorithm>
#include <vector>

#include "entity/homeland_types.h"
#include "entity/vocation_types.h"
#include "global/types.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/homeland_battle.h"

#define TOWER_UNIT_ID 0
#define ATTACKER_ACTOR_UNIT_ID 1
#define DEFENDER_ACTOR_UNIT_ID 2

namespace session {

namespace server {

namespace social {

HomelandBattleResult::HomelandBattleResult() :
    result_(entity::HomelandBattleResultType::LOSE),
    commander_battle_attacker_win_(false), soul_battle_attacker_win_(false),
    attacker_soul_alive_(0), defender_soul_alive_(0) {}

HomelandBattleResult::~HomelandBattleResult() {}

///////////////////////////////////////////////////////////////////////////////

bool HomelandBattleManager::Initialize() {
  return true;
}

void HomelandBattleManager::Finalize() {
  this->ladder_index_.clear();
  this->ladder_.clear();
}

void HomelandBattleManager::AddToBattleLadder(SocialActor *social_actor) {
  if (social_actor->GetHomelandActor()->CheckGoldmineCanRob() == false) {
    return;
  }

  // 删除索引
  LadderEntryMap::iterator iter = this->ladder_.find(social_actor->GetActorID());
  if (iter != this->ladder_.end()) {
    this->ladder_index_.erase(&iter->second);
  }

  LadderEntry entry;
  entry.actor_id_ = social_actor->GetActorID();
  entry.level_ = social_actor->GetLevel();
  entry.random_perturb_ = ExtensionManager::GetInstance()->GetRandom().Random();

  this->ladder_[social_actor->GetActorID()] = entry;
  this->ladder_index_.insert(&this->ladder_[social_actor->GetActorID()]);
}

void HomelandBattleManager::RemoveFromBattleLadder(core::uint64 actor_id) {
  LadderEntryMap::iterator iter = this->ladder_.find(actor_id);
  if (this->ladder_.end() == iter) {
    return;
  }

  this->ladder_index_.erase(&this->ladder_[actor_id]);
  this->ladder_.erase(actor_id);
}

core::uint64 HomelandBattleManager::GetBattleTarget(core::uint64 actor_id) {
  LadderEntryMap::iterator iter = this->ladder_.find(actor_id);
  if (this->ladder_.end() == iter) {
    LOG_ERROR("Actor(%lu) not found in ladder.", actor_id);
    return 0;
  }

  LadderEntrySet::const_iterator index_iter = this->ladder_index_.find(&iter->second);
  if (this->ladder_index_.end() == index_iter) {
    LOG_ERROR("Actor(%lu) not found in ladder index.", actor_id);
    return 0;
  }

  std::vector<core::uint64> result_pool;

  // 向前找5个玩家
  do {
    if (this->ladder_index_.begin() == index_iter) {
      break;
    }

    LadderEntrySet::const_iterator search_iter = index_iter;
    --search_iter;

    for (int i = 0; i < 5; ++i, --search_iter) {
      result_pool.push_back((*search_iter)->actor_id_);
      if (search_iter == this->ladder_index_.begin()) {
        break;
      }
    }

  } while(0);

  // 向后找5个玩家
  do {

    LadderEntrySet::const_iterator search_iter = index_iter;
    ++search_iter;

    for (int i = 0; i < 5; ++i, ++search_iter) {
      if (this->ladder_index_.end() == search_iter) {
        break;
      }
      result_pool.push_back((*search_iter)->actor_id_);
    }

  } while(0);

  if (result_pool.empty()) {
    return 0;
  } else {
    std::random_shuffle(result_pool.begin(), result_pool.end());
    return result_pool[0];
  }
}

static bool InitSoulUnitProp(
    core::uint32 soul_id, core::int32 soul_level, core::int32 fight_score,
    HomelandBattleUnit *unit, gateway::protocol::HomelandBattleUnitData *client_unit) {
  const global::configure::HomelandSoulCell *cell =
    SOCIAL_CONF()->GetHomelandSoul(soul_id);
  if (NULL == cell) {
    LOG_ERROR("HomelandSoul(%u) not found in config.", soul_id);
    return false;
  }

  unit->hp_ = cell->hp_base_ + cell->hp_growth_ * soul_level +
      fight_score * MISC_CONF()->homeland_battle_hp_buff_coe_ / 10000;
  unit->attack_ = cell->attack_base_ + cell->attack_growth_ * soul_level +
      fight_score * MISC_CONF()->homeland_battle_attack_buff_coe_ / 10000;
  unit->dodge_percent_ = cell->dodge_percent_;
  unit->crit_percent_ = cell->crit_percent_;
  unit->crit_effect_ = cell->crit_effect_;

  if (cell->type_ == global::configure::HomelandSoulCell::SoulType::AXE) {
    unit->special_skill_type_ = HomelandBattleUnit::SpecialSkillType::AXE;
  } else if (cell->type_ == global::configure::HomelandSoulCell::SoulType::SHOOT) {
    unit->special_skill_type_ = HomelandBattleUnit::SpecialSkillType::SHOOT;
  } else if (cell->type_ == global::configure::HomelandSoulCell::SoulType::MAGIC) {
    unit->special_skill_type_ = HomelandBattleUnit::SpecialSkillType::MAGIC;
  } else {
    LOG_ERROR("SoulType is invalid.");
    return false;
  }

  unit->special_skill_odds_ = cell->special_skill_odds_;
  unit->special_skill_hurt_coe_ = cell->special_skill_hurt_coe_;

  client_unit->__set_soul_id_(soul_id);
  client_unit->__set_hp_(unit->hp_);

  return true;
}

static bool InitActorUnitProp(SocialActor *social_actor,
    HomelandBattleUnit *unit, gateway::protocol::HomelandBattleUnitData *client_unit) {
  core::uint32 soul_id = 0;
  core::uint32 soul_level = social_actor->GetLevel();

  if (entity::VocationType::AXE == social_actor->GetVocation()) {
    soul_id = global::configure::HomelandSoulCell::SpecialIDType::AXE_ACTOR;
  } else if (entity::VocationType::SHOOT == social_actor->GetVocation()) {
    soul_id = global::configure::HomelandSoulCell::SpecialIDType::SHOOT_ACTOR;
  } else if (entity::VocationType::MAGIC == social_actor->GetVocation()) {
    soul_id = global::configure::HomelandSoulCell::SpecialIDType::MAGIC_ACTOR;
  } else {
    LOG_ERROR("Actor(%lu)'s vocation is invalid.");
    return false;
  }

  if (InitSoulUnitProp(soul_id, soul_level, social_actor->GetCurrentFightScore(),
          unit, client_unit) == false) {
    LOG_ERROR("InitSoulUnitProp failed.");
    return false;
  }

  unit->row_ = 0;
  unit->column_ = 0;
  client_unit->__set_row_(unit->row_);
  client_unit->__set_column_(unit->column_);

  return true;
}

static bool SetSoulPosition(std::vector<HomelandBattleUnit> *units,
    std::vector<gateway::protocol::HomelandBattleUnitData> *client_units) {
  // 打乱元素
  std::vector<HomelandBattleUnit *> unit_ptrs;
  unit_ptrs.resize(units->size());
  for (size_t i = 0; i < units->size(); ++i) {
    unit_ptrs[i] = &(*units)[i];
  }
  std::random_shuffle(unit_ptrs.begin(), unit_ptrs.end());

  core::int32 axe_soul_count = 0;  
  core::int32 magic_soul_count = 0;
  core::int32 shoot_soul_count = 0;

  for (size_t i = 0; i < unit_ptrs.size(); ++i) {
    HomelandBattleUnit *unit = unit_ptrs[i];

    if (HomelandBattleUnit::SpecialSkillType::AXE == unit->special_skill_type_) {
      // 战士型英灵放第一排
      unit->row_ = 0;
      unit->column_ = axe_soul_count++;
    } else if (HomelandBattleUnit::SpecialSkillType::MAGIC == unit->special_skill_type_) {
      // 法师型英灵放第二排
      unit->row_ = 1;
      unit->column_ = magic_soul_count++;
    } else if (HomelandBattleUnit::SpecialSkillType::SHOOT == unit->special_skill_type_) {
      // 弓手型英灵放第三排
      unit->row_ = 2;
      unit->column_ = shoot_soul_count++;
    } else {
      LOG_ERROR("Unit special skill type is invalid.");
      return false;
    }
  }

  for (size_t i = 0; i < units->size(); ++i) {
    HomelandBattleUnit *unit = &(*units)[i];
    gateway::protocol::HomelandBattleUnitData *client_unit = &(*client_units)[unit->id_];

    client_unit->__set_row_(unit->row_);
    client_unit->__set_column_(unit->column_);
  }

  return true;
}

static void AddBattleUnit(HomelandBattle::HomelandBattleUnitMap *units,
    HomelandBattleUnit *unit) {
  core::uint64 index = ((core::uint64)unit->row_ << 32) + (core::uint64)unit->column_;
  units->insert(std::make_pair(index, unit));
}

bool HomelandBattleManager::DoBattle(SocialActor *attacker, SocialActor *defender,
    HomelandBattleResult *battle_result, bool synchronize) {
  gateway::protocol::MessageHomelandBattleFightDataNotify notify;
  notify.__set_target_actor_name_(defender->GetName());
  notify.__set_target_gender_(defender->GetGender());

  HomelandBattleUnit attacker_actor_unit;
  HomelandBattleUnit defender_actor_unit;
  std::vector<HomelandBattleUnit> attacker_soul_units;
  std::vector<HomelandBattleUnit> defender_soul_units;
  core::int32 global_soul_id = DEFENDER_ACTOR_UNIT_ID;
  core::int32 attacker_actor_aoe_hurt = 0;
  core::int32 defender_actor_aoe_hurt = 0;
  core::int32 tower_aoe_hurt = 0;

  // 设置单位
  // --炮塔
  {
    gateway::protocol::HomelandBattleUnitData data;
    data.__set_unit_id_(TOWER_UNIT_ID);
    data.__set_side_(entity::HomelandBattleSideType::DEFENCE);
    data.__set_unit_type_(entity::HomelandBattleUnitType::TOWER);
    notify.units_.push_back(data);

  }
  // --攻击方主角
  {
    gateway::protocol::HomelandBattleUnitData data;
    data.__set_unit_id_(ATTACKER_ACTOR_UNIT_ID);
    data.__set_side_(entity::HomelandBattleSideType::ATTACK);
    data.__set_unit_type_(entity::HomelandBattleUnitType::ACTOR);

    attacker_actor_unit.id_ = data.unit_id_;
    if (InitActorUnitProp(attacker, &attacker_actor_unit, &data) == false) {
      LOG_ERROR("InitActorUnitProp for attacker failed.");
      return false;
    }

    notify.units_.push_back(data);
  }
  // --防御方主角
  {
    gateway::protocol::HomelandBattleUnitData data;
    data.__set_unit_id_(DEFENDER_ACTOR_UNIT_ID);
    data.__set_side_(entity::HomelandBattleSideType::DEFENCE);
    data.__set_unit_type_(entity::HomelandBattleUnitType::ACTOR);

    defender_actor_unit.id_ = data.unit_id_;
    if (InitActorUnitProp(defender, &defender_actor_unit, &data) == false) {
      LOG_ERROR("InitActorUnitProp for defender failed.");
      return false;
    }

    notify.units_.push_back(data);
  }
  // --攻击方英灵
  {
    const HomelandActor::SoulLevelMap &souls = attacker->GetHomelandActor()->GetSouls();
    for (HomelandActor::SoulLevelMap::const_iterator iter = souls.begin(); 
         iter != souls.end(); ++iter) {
      gateway::protocol::HomelandBattleUnitData data;
      data.__set_unit_id_(++global_soul_id);
      data.__set_side_(entity::HomelandBattleSideType::ATTACK);
      data.__set_unit_type_(entity::HomelandBattleUnitType::SOUL);

      HomelandBattleUnit unit;
      unit.id_ = data.unit_id_;
      if (InitSoulUnitProp(iter->first, iter->second,
              attacker->GetCurrentFightScore(), &unit, &data) == false) {
        LOG_ERROR("InitSoulUnitProp(%u) for attacker failed.", iter->first);
        return false;
      }

      attacker_soul_units.push_back(unit);
      notify.units_.push_back(data);
    }

    // 攻击方英灵设置位置
    if (SetSoulPosition(&attacker_soul_units, &notify.units_) == false) {
      LOG_ERROR("SetSoulPosition for attacker failed.");
      return false;
    }
  }
  // --防御方英灵
  {
    const HomelandActor::SoulLevelMap &souls = defender->GetHomelandActor()->GetSouls();
    for (HomelandActor::SoulLevelMap::const_iterator iter = souls.begin(); 
         iter != souls.end(); ++iter) {
      gateway::protocol::HomelandBattleUnitData data;
      data.__set_unit_id_(++global_soul_id);
      data.__set_side_(entity::HomelandBattleSideType::DEFENCE);
      data.__set_unit_type_(entity::HomelandBattleUnitType::SOUL);

      HomelandBattleUnit unit;
      unit.id_ = data.unit_id_;
      if (InitSoulUnitProp(iter->first, iter->second,
              defender->GetCurrentFightScore(), &unit, &data) == false) {
        LOG_ERROR("InitSoulUnitProp(%u) for defender failed.", iter->first);
        return false;
      }

      defender_soul_units.push_back(unit);
      notify.units_.push_back(data);
    }

    // 加入援助的英灵
    std::vector<core::uint64> supported_actors;
    const HomelandActor::HomelandSupportedSoulMap &supported_souls =
        defender->GetHomelandActor()->GetStrongholdSupportedSouls(); 
    for (HomelandActor::HomelandSupportedSoulMap::const_iterator iter = supported_souls.begin();
         iter != supported_souls.end(); ++iter) {
      const HomelandSupportedSoul *soul = &iter->second;

      gateway::protocol::HomelandBattleUnitData data;
      data.__set_unit_id_(++global_soul_id);
      data.__set_side_(entity::HomelandBattleSideType::DEFENCE);
      data.__set_unit_type_(entity::HomelandBattleUnitType::SOUL);

      HomelandBattleUnit unit;
      unit.id_ = data.unit_id_;
      if (InitSoulUnitProp(soul->GetSoulID(), soul->GetSoulLevel(),
              defender->GetCurrentFightScore(), &unit, &data) == false) {
        LOG_ERROR("InitSoulUnitProp(%u) for defender failed.", soul->GetSoulID());
        return false;
      }

      defender_soul_units.push_back(unit);
      notify.units_.push_back(data);

      supported_actors.push_back(iter->first);
      if (supported_actors.size() >= (size_t)MISC_CONF()->homeland_max_battle_support_soul_) {
        break;
      }
    }

    // 援助英灵返回
    for (size_t i = 0; i < supported_actors.size(); ++i) {
      defender->GetHomelandActor()->RemoveStrongholdSupportedSoul(supported_actors[i]);
    }

    // 防御方英灵设置位置
    if (SetSoulPosition(&defender_soul_units, &notify.units_) == false) {
      LOG_ERROR("SetSoulPosition for defender failed.");
      return false;
    }
  }

  // 计算AOE伤害值
  {
    for (core::int32 i = 1; i <= 2; ++i) {
      const global::configure::HomelandTowerCell *cell =
        SOCIAL_CONF()->GetHomelandTower(defender->GetHomelandActor()->GetTowerLevel(i));
      if (NULL == cell) {
        LOG_ERROR("HomelandTowerCell(%d) not found in config.",
                  defender->GetHomelandActor()->GetTowerLevel(i));
        return false;
      }
      tower_aoe_hurt += cell->attack_;
    }

    attacker_actor_aoe_hurt = attacker_actor_unit.attack_ *
        attacker_actor_unit.special_skill_hurt_coe_ / global::kPercentDenom;
    defender_actor_aoe_hurt = defender_actor_unit.attack_ *
        defender_actor_unit.special_skill_hurt_coe_ / global::kPercentDenom;
  }

  // 主将战
  {

    HomelandBattle::HomelandBattleUnitMap left_units;
    AddBattleUnit(&left_units, &attacker_actor_unit);

    HomelandBattle::HomelandBattleUnitMap right_units;
    AddBattleUnit(&right_units, &defender_actor_unit);

    core::int32 left_alive = 0;
    core::int32 right_alive = 0;

    HomelandBattle battle(&left_units, &right_units, &notify.commander_battle_);
    battle.DoBattle(MISC_CONF()->homeland_max_commander_battle_round_,
        &left_alive, &right_alive);

    battle_result->commander_battle_attacker_win_ = (0 == right_alive);
  }

  // 英灵战
  {
    HomelandBattle::HomelandBattleUnitMap left_units;
    for (size_t i = 0; i < attacker_soul_units.size(); ++i) {
      AddBattleUnit(&left_units, &attacker_soul_units[i]);
    }

    HomelandBattle::HomelandBattleUnitMap right_units;
    for (size_t i = 0; i < defender_soul_units.size(); ++i) {
      AddBattleUnit(&right_units, &defender_soul_units[i]);
    }

    core::int32 left_alive = 0;
    core::int32 right_alive = 0;

    HomelandBattle battle(&left_units, &right_units, &notify.soul_battle_);

    do {
      // 胜利方主角AOE
      if (battle_result->commander_battle_attacker_win_) {
        battle.DoRightAoe(attacker_actor_unit.id_, entity::HomelandBattleUnitType::ACTOR,
            attacker_actor_aoe_hurt, &left_alive, &right_alive);
        if (0 == right_alive) {
          break;
        }
      } else {
        battle.DoLeftAoe(defender_actor_unit.id_, entity::HomelandBattleUnitType::ACTOR,
            defender_actor_aoe_hurt, &left_alive, &right_alive);
        if (0 == left_alive) {
          break;
        }
      }

      // 炮塔AOE
      battle.DoLeftAoe(TOWER_UNIT_ID, entity::HomelandBattleUnitType::TOWER,
          tower_aoe_hurt, &left_alive, &right_alive);
      if (0 == left_alive) {
        break;
      }

      // 英灵战斗
      battle.DoBattle(MISC_CONF()->homeland_max_soul_battle_round_, &left_alive, &right_alive);

    } while(0);

    battle_result->attacker_soul_alive_ = left_alive;
    battle_result->defender_soul_alive_ = right_alive;

    // 战斗结果评价
    if (right_alive > 0) {
      battle_result->result_ = entity::HomelandBattleResultType::LOSE;
    } else if (attacker_soul_units.empty()) {
      if (battle_result->commander_battle_attacker_win_) {
        battle_result->result_ = entity::HomelandBattleResultType::PERFECT_WIN;
      } else {
        battle_result->result_ = entity::HomelandBattleResultType::LOSE;
      }
    } else if (left_alive * 100 / (core::int32)attacker_soul_units.size() >= 80 &&
               battle_result->commander_battle_attacker_win_) {
      battle_result->result_ = entity::HomelandBattleResultType::PERFECT_WIN;
    } else if (left_alive * 100 / (core::int32)attacker_soul_units.size() >= 30) {
      battle_result->result_ = entity::HomelandBattleResultType::GREAT_WIN;
    } else if (left_alive > 0) {
      battle_result->result_ = entity::HomelandBattleResultType::TINY_WIN;
    } else {
      battle_result->result_ = entity::HomelandBattleResultType::LOSE;
    }

    if (entity::HomelandBattleResultType::LOSE == battle_result->result_) {
      battle_result->soul_battle_attacker_win_ = false;
    } else {
      battle_result->soul_battle_attacker_win_ = true;
    }

    notify.__set_battle_result_(battle_result->result_);
  }

  if (synchronize) {
    attacker->SendMessage(notify,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_BATTLE_FIGHT_DATA_NOTIFY);
  }

  return true;
}

}  // namespace social

}  // namespace server

}  // namespace session

