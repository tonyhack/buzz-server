#include "game_server/server/extension/guild/guild_actor.h"

#include <cstddef>

#include "global/logging.h"
#include "global/configure/configure.h"
#include "global/configure/guild_configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_guild_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_guild_protocol_types.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/guild/facade_request.h"
#include "game_server/server/extension/guild/guild.h"
#include "game_server/server/extension/guild/guild_manager.h"

namespace game {

namespace server {

namespace guild {

GuildActor::GuildActor() : game_actor_(NULL),
  guild_id_(0), current_contribution_value_(0) {}

GuildActor::~GuildActor() {}

bool GuildActor::Initialize(GameActor *game_actor,
    const session::protocol::GuildActorData &data) {
  if (NULL == game_actor) {
    LOG_ERROR("game_actor is null.");
    return false;
  }

  this->game_actor_ = game_actor;

  this->guild_id_ = data.guild_id_;
  this->guild_position_ = data.guild_position_;
  this->current_contribution_value_ = data.current_contribution_value_;

  // 公会技能
  for (size_t i = 0; i < data.guild_skills_.size(); ++i) {
    const session::protocol::GuildSkillData &skill_data = data.guild_skills_[i];
    this->effective_guild_skills_[skill_data.id_] = skill_data.effective_level_;
    this->AddGuildSkillAttr(skill_data.id_, skill_data.effective_level_, false);
  }

  // 公会BUFF
  for (size_t i = 0; i < data.guild_buffs_.size(); ++i) {
    if (GUILD_CONF()->GetGuildBuff(data.guild_buffs_[i]) == NULL) {
      LOG_ERROR("GuildBuff(%d) not found in config.", data.guild_buffs_[i]);
      continue;
    }
    this->guild_buffs_.insert(data.guild_buffs_[i]);
  }

  // 更新战斗力
  if (!this->effective_guild_skills_.empty()) {
    this->UpdateGuildSkillFightScore(false);
  }

  return true; 
}

void GuildActor::InitializeGuildBuff() {
  for (GuildBuffSet::const_iterator iter = this->guild_buffs_.begin();
       iter != this->guild_buffs_.end(); ++iter) {
    const global::configure::GuildBuffCell *cell =
      GUILD_CONF()->GetGuildBuff(*iter);
    if (NULL == cell) {
      LOG_ERROR("GuildBuff(%d) not found in config.", *iter);
      continue;
    }

    if (FacadeRequest::GetInstance()->AddBuff(this->GetID(), cell->buff_id_) == false) {
      LOG_ERROR("Add buff(%u) failed", cell->buff_id_);
      continue;
    }
  }
}

void GuildActor::Finalize() {
  this->guild_buffs_.clear();
  this->effective_guild_skills_.clear();
  this->current_contribution_value_ = 0;
  this->guild_position_ = 0;
  this->guild_id_ = 0;
  this->game_actor_ = NULL;
}

std::string GuildActor::GetGuildName() const {
  if (0 == this->guild_id_) {
    return std::string();
  } else {
    Guild *guild = GuildManager::GetInstance()->GetGuild(this->guild_id_);
    if (NULL == guild) {
      LOG_ERROR("Get Guild(%lu) from GuildManager failed.", this->guild_id_);
      return std::string();
    } else {
      return guild->GetName();
    }
  }
}

void GuildActor::SendLoadFinishEvent() {
  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(this->GetID());
  load_finish.__set_extension_type_(ExtensionType::GUILD);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, this->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));
  LOG_DEBUG("玩家(%lu) GUILD扩展加载成功", this->GetID());
}

void GuildActor::AddGuildSkillAttr(core::int32 id, core::int32 level, bool synchronize) {
  if (this->game_actor_ == NULL) {
    LOG_ERROR("GameActor is null.");
    return;
  }
  if (0 == level) {
    return;
  }

  LOG_DEBUG("AddGuildSkillAttr(%d, %d)", id, level);

  const global::configure::GuildSkillCell *cell = GUILD_CONF()->GetGuildSkill(id, level);
  if (cell == NULL) {
    LOG_ERROR("GuildSkillCell(%d, %d) not found in config.", id, level);
    return;
  }

  switch(cell->attr_type_) {
    case global::configure::GuildSkillCell::AttrType::PHYSIQUE:
      this->game_actor_->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) +
            cell->attr_value_, synchronize);
      break;
    case global::configure::GuildSkillCell::AttrType::STRENGTH:
      this->game_actor_->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE) +
            cell->attr_value_, synchronize);
      break;
    case global::configure::GuildSkillCell::AttrType::INTELLIGENCE:
      this->game_actor_->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE) +
            cell->attr_value_, synchronize);
      break;
    case global::configure::GuildSkillCell::AttrType::AGILE:
      this->game_actor_->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE) +
            cell->attr_value_, synchronize);
      break;
    case global::configure::GuildSkillCell::AttrType::SPIRIT:
      this->game_actor_->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE) +
            cell->attr_value_, synchronize);
      break;
    default:
      break;
  }
}

void GuildActor::RemoveGuildSkillAttr(core::int32 id, core::int32 level, bool synchronize) {
  if (this->game_actor_ == NULL) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  LOG_DEBUG("RemoveGuildSkillAttr(%d, %d)", id, level);

  const global::configure::GuildSkillCell *cell = GUILD_CONF()->GetGuildSkill(id, level);
  if (cell == NULL) {
    LOG_ERROR("GuildSkillCell(%d, %d) not found in config.", id, level);
    return;
  }

  switch(cell->attr_type_) {
    case global::configure::GuildSkillCell::AttrType::PHYSIQUE:
      this->game_actor_->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE) -
            cell->attr_value_, synchronize);
      break;
    case global::configure::GuildSkillCell::AttrType::STRENGTH:
      this->game_actor_->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE) -
            cell->attr_value_, synchronize);
      break;
    case global::configure::GuildSkillCell::AttrType::INTELLIGENCE:
      this->game_actor_->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE) -
            cell->attr_value_, synchronize);
      break;
    case global::configure::GuildSkillCell::AttrType::AGILE:
      this->game_actor_->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE) -
            cell->attr_value_, synchronize);
      break;
    case global::configure::GuildSkillCell::AttrType::SPIRIT:
      this->game_actor_->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
        this->game_actor_->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE) -
            cell->attr_value_, synchronize);
      break;
    default:
      break;
  }
}

void GuildActor::SetGuildSkillLevel(core::int32 id, core::int32 level, bool synchronize) {
  if (this->effective_guild_skills_.find(id) == this->effective_guild_skills_.end()) {
    this->effective_guild_skills_[id] = level;
    this->AddGuildSkillAttr(id, level, synchronize);
  } else {
    this->RemoveGuildSkillAttr(id, this->effective_guild_skills_[id], synchronize);
    this->effective_guild_skills_[id] = level;
    this->AddGuildSkillAttr(id, level, synchronize);
  }

  // 更新战斗力
  this->UpdateGuildSkillFightScore(synchronize);
}

void GuildActor::ClearGuildSkill(bool synchronize) {
  for (GuildSkillLevelMap::const_iterator iter = this->effective_guild_skills_.begin();
       iter != this->effective_guild_skills_.end(); ++iter) {
    this->RemoveGuildSkillAttr(iter->first, iter->second, synchronize);
  }

  this->effective_guild_skills_.clear();

  // 更新战斗力
  this->UpdateGuildSkillFightScore(synchronize);
}

bool GuildActor::CheckGuildBuffExists(core::int32 id) const {
  if (this->guild_buffs_.find(id) != this->guild_buffs_.end()) {
    return true;
  }
  return false;
}

bool GuildActor::AddGuildBuff(core::int32 id) {
  if (this->CheckGuildBuffExists(id)) {
    LOG_ERROR("GuildBuff(%d) is already exists.", id);
    return false;
  }

  const global::configure::GuildBuffCell *cell =
    GUILD_CONF()->GetGuildBuff(id);
  if (NULL == cell) {
    LOG_ERROR("GuildBuff(%d) not found in config.", id);
    return false;
  }

  if (FacadeRequest::GetInstance()->AddBuff(this->GetID(), cell->buff_id_) == false) {
    LOG_ERROR("Add buff(%u) failed", cell->buff_id_);
    return false;
  }
  this->guild_buffs_.insert(id);

  return true;
}

bool GuildActor::RemoveGuildBuff(core::int32 id) {
  if (this->CheckGuildBuffExists(id) == false) {
    LOG_ERROR("GuildBuff(%d) not exists.", id);
    return false;
  }

  const global::configure::GuildBuffCell *cell =
    GUILD_CONF()->GetGuildBuff(id);
  if (NULL == cell) {
    LOG_ERROR("GuildBuff(%d) not found in config.", id);
    return false;
  }

  if (FacadeRequest::GetInstance()->RemoveBuff(this->GetID(), cell->buff_id_) == false) {
    LOG_ERROR("Remove buff(%u) failed", cell->buff_id_);
    return false;
  }
  this->guild_buffs_.erase(id);

  return true;
}

void GuildActor::ClearGuildBuff() {
  for (GuildBuffSet::iterator iter = this->guild_buffs_.begin();
       iter != this->guild_buffs_.end();) {
    GuildBuffSet::iterator iter2 = iter;
    ++iter2;

    if (this->RemoveGuildBuff(*iter) == false) {
      LOG_ERROR("Remove GuildBuff(%d) failed.", *iter);
    }

    iter = iter2;
  }
}

static void AddAttributeToCalculator(FightScoreCalculator *calc,
    global::configure::GuildSkillCell::AttrType::type type, core::int32 value) {
  switch (type) {
    case global::configure::GuildSkillCell::AttrType::PHYSIQUE:
      calc->physique_ += value;
      break;
    case global::configure::GuildSkillCell::AttrType::STRENGTH:
      calc->strength_ += value;
      break;
    case global::configure::GuildSkillCell::AttrType::INTELLIGENCE:
      calc->intelligence_ += value;
      break;
    case global::configure::GuildSkillCell::AttrType::AGILE:
      calc->agile_ += value;
      break;
    case global::configure::GuildSkillCell::AttrType::SPIRIT:
      calc->spirit_ += value;
      break;
    default:
      break;
  }
}

void GuildActor::UpdateGuildSkillFightScore(bool synchronize) {
  if (this->game_actor_ == NULL) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  FightScoreCalculator calc;

  for (GuildSkillLevelMap::const_iterator iter = this->effective_guild_skills_.begin();
       iter != this->effective_guild_skills_.end(); ++iter) {
    // 过滤掉0级技能
    if (0 == iter->second) {
      continue;
    }

    const global::configure::GuildSkillCell *cell =
      GUILD_CONF()->GetGuildSkill(iter->first, iter->second);
    if (cell == NULL) {
      LOG_ERROR("GuildSkillCell(%d, %d) not found in config.",
                iter->first, iter->second);
      continue;
    }
    AddAttributeToCalculator(&calc, cell->attr_type_, cell->attr_value_);
  }

  this->game_actor_->SetSubFightScore(entity::SubFightScoreType::GUILD_SKILL,
      calc.GetFightScore(), synchronize);
}

}  // namespace guild

}  // namespace server

}  // namespace game

