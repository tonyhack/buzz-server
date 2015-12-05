#include "session_server/server/extension/guild/guild_actor.h"

#include <cstddef>
#include <cstdio>
#include <algorithm>

#include "coresh/communicator.h"
#include "entity/guild_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_guild_actor_login_types.h"
#include "database_server/protocol/storage_guild_actor_save_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_guild_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_guild_protocol_types.h"
#include "session_server/server/extension_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/guild/guild.h"
#include "session_server/server/extension/guild/guild_event.h"
#include "session_server/server/extension/guild/guild_manager.h"

namespace session {

namespace server {

namespace guild {

static void DecodeGuildSkill(const std::string &source, GuildActor::GuildSkillLevelMap *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    std::vector<std::string> split_result2;
    global::StringSplit(split_result[i].c_str(), ":", split_result2);

    if (split_result2.size() != 2) {
      LOG_ERROR("Guild skill data(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    core::int32 skill_id = atoi(split_result2[0].c_str());
    core::int32 skill_level = atoi(split_result2[1].c_str());

    // 检查技能是否存在
    if (GUILD_CONF()->GetGuildSkill(skill_id, skill_level) == NULL) {
      LOG_ERROR("GuildSkill(%d, %d) not found in config.", skill_id, skill_level);
      continue;
    }

    result->insert(std::make_pair(skill_id, skill_level));
  }
}

static void EncodeGuildSkill(const GuildActor::GuildSkillLevelMap &source, std::string *result) {
  char buff[1024] = "";
  size_t count = 0;

  for (GuildActor::GuildSkillLevelMap::const_iterator iter = source.begin();
       iter != source.end(); ++iter) {
    count += snprintf(buff + count, sizeof(buff) - count, "%d:%d,", iter->first, iter->second);
  }

  *result = buff;
}

static void DecodeGuildBuff(const std::string &source, GuildActor::GuildBuffSet *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    core::int32 buff_id = atoi(split_result[i].c_str());

    // 检查buff是否存在
    if (GUILD_CONF()->GetGuildBuff(buff_id) == NULL) {
      LOG_ERROR("GuildBuff(%d) not found in config.", buff_id);
      continue;
    }

    result->insert(buff_id);
  }
}

static void EncodeGuildBuff(const GuildActor::GuildBuffSet &source, std::string *result) {
  char buff[1024] = "";
  size_t count = 0;

  for (GuildActor::GuildBuffSet::const_iterator iter = source.begin();
       iter != source.end(); ++iter) {
    count += snprintf(buff + count, sizeof(buff) - count, "%d,", *iter);
  }
  
  *result = buff;
}

static void DecodeGuildPlaying(const std::string &source,
    GuildActor::GuildPlayingSet *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    core::int32 playing_template_id = atoi(split_result[i].c_str());

    // 检查公会副本是否存在
    if (GUILD_CONF()->GetGuildPlaying(playing_template_id) == NULL) {
      LOG_ERROR("playing_template_id(%d) not found in config.", playing_template_id);
      continue;
    }

    result->insert(playing_template_id);
  }
}

static void EncodeGuildPlaying(const GuildActor::GuildPlayingSet &source,
    std::string *result) {
  char buffer[256] = "";
  size_t count = 0;

  Guild::GuildPlayingGroupSet::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    count += snprintf(buffer + count, sizeof(buffer) - count, "%d,", *iter);
  }

  *result = buffer;
}

///////////////////////////////////////////////////////////////////////////////

GuildActor::GuildActor() : brief_actor_(NULL), session_actor_(NULL), 
  dirty_(false), guild_(NULL), guild_id_(0), guild_position_(0),
  current_contribution_value_(0), total_contribution_value_(0),
  gold_contribution_(0), freedom_dollars_contribution_(0),
  light_crystal_contribution_(0), dark_crystal_contribution_(0),
  holy_crystal_contribution_(0) {}

GuildActor::~GuildActor() {}

bool GuildActor::Initialize(BriefActor *brief_actor,
    const database::protocol::StorageGuildActorField &db_field) {
  if (NULL == brief_actor) {
    LOG_ERROR("brief_actor is null.");
    return false;
  }

  this->brief_actor_ = brief_actor;

  this->guild_id_ = db_field.guild_id_;
  this->guild_position_ = db_field.guild_position_;
  this->current_contribution_value_ = db_field.current_contribution_value_;
  this->total_contribution_value_ = db_field.total_contribution_value_;
  this->gold_contribution_ = db_field.gold_contribution_;
  this->freedom_dollars_contribution_ = db_field.freedom_dollars_contribution_;
  this->light_crystal_contribution_ = db_field.light_crystal_contribution_;
  this->dark_crystal_contribution_ = db_field.dark_crystal_contribution_;
  this->holy_crystal_contribution_ = db_field.holy_crystal_contribution_;

  DecodeGuildSkill(db_field.guild_skills_, &this->guild_skills_);
  DecodeGuildBuff(db_field.guild_buffs_, &this->guild_buffs_);
  DecodeGuildPlaying(db_field.awarded_guild_playings_, &this->awarded_guild_playings_);

  return true;
}

void GuildActor::Finalize() {
  this->awarded_guild_playings_.clear();
  this->guild_buffs_.clear();
  this->effective_guild_skills_.clear();
  this->guild_skills_.clear();
  this->holy_crystal_contribution_ = 0;
  this->dark_crystal_contribution_ = 0;
  this->light_crystal_contribution_ = 0;
  this->freedom_dollars_contribution_ = 0;
  this->gold_contribution_ = 0;
  this->total_contribution_value_ = 0;
  this->current_contribution_value_ = 0;
  this->guild_position_ = 0;
  this->guild_id_ = 0;
  this->joined_guilds_.clear();
  this->guild_ = NULL;
  this->dirty_ = false;
  this->session_actor_ = NULL;
  this->brief_actor_ = NULL;
}

void GuildActor::Online(SessionActor *session_actor) {
  if (NULL == session_actor) {
    LOG_ERROR("session_actor is null.");
    return;
  }

  this->session_actor_ = session_actor;

  // 给 game_server 发送初始化信息
  this->SendInitToGameServer();

  // 未开启功能返回
  if (this->session_actor_->CheckFunctionalityState(
          entity::FunctionalityType::GUILD_MODULE) == false) {
    return;
  }

  // 给 client 发送初始化信息
  this->SendInitToClient();
  this->SendOnlineStateSync(true);
}

void GuildActor::Offline() {
  this->session_actor_ = NULL;
  this->SendOnlineStateSync(false);
}

void GuildActor::SendInitToGameServer() {
  session::protocol::GuildActorData data;
  data.__set_guild_id_(this->GetGuildID());
  data.__set_guild_position_(this->GetGuildPosition());
  data.__set_current_contribution_value_(this->current_contribution_value_);

  // 公会技能
  for (GuildSkillLevelMap::const_iterator iter = this->effective_guild_skills_.begin();
       iter != this->effective_guild_skills_.end(); ++iter) {
    session::protocol::GuildSkillData skill_data;
    skill_data.__set_id_(iter->first);
    skill_data.__set_effective_level_(iter->second);
    data.guild_skills_.push_back(skill_data);
  }
  // 公会BUFF
  for (GuildBuffSet::const_iterator iter = this->guild_buffs_.begin();
       iter != this->guild_buffs_.end(); ++iter) {
    data.guild_buffs_.push_back(*iter);
  }

  session::protocol::MessageGuildActorInitialize message;
  message.__set_actor_id_(this->GetActorID());
  message.__set_data_(data);

  this->SendGameMessage(message,
      session::protocol::GameMessageType::MESSAGE_GUILD_ACTOR_INITIALIZE);
}

void GuildActor::SendInitToClient() {
  gateway::protocol::MessageGuildInitialize message;

  if (NULL == this->guild_) {
    message.__set_guild_id_("0");
  } else {
    message.__set_guild_id_(this->guild_->GetStringID());
    message.__set_guild_name_(this->guild_->GetName());
    message.__set_guild_logo_(this->guild_->GetLogo());
    message.__set_guild_level_(this->guild_->GetLevel());
    message.__set_guild_announcement_(this->guild_->GetAnnouncement());
    message.__set_guild_gold_(this->guild_->GetGold());
    message.__set_guild_light_crystal_(this->guild_->GetLightCrystal());
    message.__set_guild_dark_crystal_(this->guild_->GetDarkCrystal());
    message.__set_guild_holy_crystal_(this->guild_->GetHolyCrystal());
    message.__set_guild_light_attribute_(this->guild_->GetLightAttribute());
    message.__set_guild_dark_attribute_(this->guild_->GetDarkAttribute());
    message.__set_guild_holy_attribute_(this->guild_->GetHolyAttribute());
    message.__set_guild_shop_level_(this->guild_->GetShopLevel());
    message.__set_guild_college_level_(this->guild_->GetCollegeLevel());
    message.__set_guild_barrack_level_(this->guild_->GetBarrackLevel());
    message.__set_suppress_join_(this->guild_->CheckSuppressJoin());
    message.__set_current_contribution_value_(this->current_contribution_value_);

    // 待审核成员信息
    message.__isset.pending_members_ = true;

    const Guild::GuildActorSet &pending_members = this->guild_->GetPendingMembers();
    for (Guild::GuildActorSet::const_iterator iter = pending_members.begin();
         iter != pending_members.end(); ++iter) {
      const GuildActor *guild_actor = *iter;

      gateway::protocol::GuildPendingMemberData data;
      data.__set_actor_id_(guild_actor->GetStringID());
      data.__set_actor_name_(guild_actor->GetName());
      data.__set_online_(guild_actor->CheckOnline());
      data.__set_last_logout_time_(guild_actor->GetStringLastLogoutTime());
      data.__set_level_(guild_actor->GetLevel());
      data.__set_map_(guild_actor->GetMap());

      message.pending_members_.push_back(data);
    }

    // 成员信息
    message.__isset.members_ = true;

    const Guild::GuildActorSet &members = this->guild_->GetMembers();
    for (Guild::GuildActorSet::const_iterator iter = members.begin();
         iter != members.end(); ++iter) {
      const GuildActor *guild_actor = *iter;

      gateway::protocol::GuildMemberData data;
      data.__set_actor_id_(guild_actor->GetStringID());
      data.__set_actor_name_(guild_actor->GetName());
      data.__set_online_(guild_actor->CheckOnline());
      data.__set_last_logout_time_(guild_actor->GetStringLastLogoutTime());
      data.__set_level_(guild_actor->GetLevel());
      data.__set_map_(guild_actor->GetMap());
      data.__set_guild_position_(guild_actor->GetGuildPosition());
      data.__set_total_contribution_value_(guild_actor->GetTotalContributionValue());
      data.__set_gold_contribution_(guild_actor->GetGoldContribution());
      data.__set_freedom_dollars_contribution_(guild_actor->GetFreedomDollarsContribution());
      data.__set_light_crystal_contribution_(guild_actor->GetLightCrystalContribution());
      data.__set_dark_crystal_contribution_(guild_actor->GetDarkCrystalContribution());
      data.__set_holy_crystal_contribution_(guild_actor->GetHolyCrystalContribution());
      data.__set_axe_fight_score_(guild_actor->GetFightScore(entity::FightScoreType::AXE));
      data.__set_shoot_fight_score_(guild_actor->GetFightScore(entity::FightScoreType::SHOOT));
      data.__set_magic_fight_score_(guild_actor->GetFightScore(entity::FightScoreType::MAGIC));

      message.members_.push_back(data);
    }

    // 公会事件
    message.__isset.events_ = true;

    const Guild::GuildEventDeque &events = this->guild_->GetEvents();
    for (Guild::GuildEventDeque::const_iterator iter = events.begin();
         iter != events.end(); ++iter) {
      const GuildEvent &event = *iter;
      gateway::protocol::GuildEventData data;
      data.__set_event_type_((entity::GuildEventType::type)event.GetType());
      data.__set_timestamp_(event.GetTimestamp());
      event.TranslateEventVar(&data.vars_);
      message.events_.push_back(data);
    }

    // 公会技能
    message.__isset.guild_skills_ = true;

    for (GuildSkillLevelMap::const_iterator iter = this->guild_skills_.begin();
         iter != this->guild_skills_.end(); ++iter) {
      core::int32 skill_id = iter->first;

      if (this->effective_guild_skills_.find(skill_id) ==
          this->effective_guild_skills_.end()) {
        LOG_ERROR("GuildSkill(%d) not found in effective skill level list", skill_id);
        continue;
      }
      gateway::protocol::GuildSkillData data;
      data.__set_id_(skill_id);
      data.__set_level_(this->guild_skills_[skill_id]);
      data.__set_effective_level_(this->effective_guild_skills_[skill_id]);
      message.guild_skills_.push_back(data);
    }

    // 公会BUFF
    message.__isset.guild_buffs_ = true;

    for (GuildBuffSet::const_iterator iter = this->guild_buffs_.begin();
         iter != this->guild_buffs_.end(); ++iter) {
      message.guild_buffs_.push_back(*iter);
    }

    // 公会副本
    message.__set_current_playing_id_(global::ToString(this->guild_->GetCurrentPlayingID()));
    message.__set_current_playing_template_id_(this->guild_->GetCurrentPlayingTemplateID());

    message.__isset.opened_playing_groups_ = true;
    const Guild::GuildPlayingGroupSet &opened_playing_groups =
      this->guild_->GetOpenedPlayingGroups();
    for (Guild::GuildPlayingGroupSet::const_iterator iter = opened_playing_groups.begin();
         iter != opened_playing_groups.end(); ++iter) {
      message.opened_playing_groups_.push_back(*iter);
    }

    message.__isset.finished_playings_ = true;
    const Guild::GuildPlayingSet &finished_playings =
      this->guild_->GetFinishedPlayings();
    for (Guild::GuildPlayingSet::const_iterator iter = finished_playings.begin();
         iter != finished_playings.end(); ++iter) {
      message.finished_playings_.push_back(*iter);
    }

    message.__isset.awarded_playings_ = true;
    for (GuildPlayingSet::const_iterator iter = this->awarded_guild_playings_.begin();
         iter != this->awarded_guild_playings_.end(); ++iter) {
      message.awarded_playings_.push_back(*iter);
    }
  }
  this->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_GUILD_INITIALIZE);
}

void GuildActor::SendOnlineStateSync(bool online) {
  if (this->guild_ != NULL) {
    // 同步成员信息
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_online_(online);
    if (online == false) {
      member_sync.data_.__set_last_logout_time_(this->GetStringLastLogoutTime());
    }
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  } else {
    // 同步待审核成员信息
    gateway::protocol::MessageGuildPendingMemberSynchronize pending_member_sync;
    pending_member_sync.__set_type_(gateway::protocol::GuildPendingMemberOperateType::UPDATE);
    pending_member_sync.data_.__set_actor_id_(this->GetStringID());
    pending_member_sync.data_.__set_online_(online);
    if (online == false) {
      pending_member_sync.data_.__set_last_logout_time_(this->GetStringLastLogoutTime());
    }

    for (GuildSet::iterator iter = this->joined_guilds_.begin();
         iter != this->joined_guilds_.end(); ++iter) {
      Guild *guild = *iter;
      if (NULL == guild) {
        continue;
      }
      guild->BroadcastMessage(pending_member_sync,
          gateway::protocol::MessageType::MESSAGE_GUILD_PENDING_MEMBER_SYNCHRONIZE);
    }
  }

}

void GuildActor::SendLevelSync() {
  if (this->guild_ != NULL) {
    // 同步成员信息
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_level_(this->GetLevel());
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  } else {
    // 同步待审核成员信息
    gateway::protocol::MessageGuildPendingMemberSynchronize pending_member_sync;
    pending_member_sync.__set_type_(gateway::protocol::GuildPendingMemberOperateType::UPDATE);
    pending_member_sync.data_.__set_actor_id_(this->GetStringID());
    pending_member_sync.data_.__set_level_(this->GetLevel());

    for (GuildSet::iterator iter = this->joined_guilds_.begin();
         iter != this->joined_guilds_.end(); ++iter) {
      Guild *guild = *iter;
      if (NULL == guild) {
        continue;
      }
      guild->BroadcastMessage(pending_member_sync,
          gateway::protocol::MessageType::MESSAGE_GUILD_PENDING_MEMBER_SYNCHRONIZE);
    }
  }
}

void GuildActor::SendFightScoreSync() {
  if (this->guild_ != NULL) {
    // 同步成员信息
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_axe_fight_score_(this->GetFightScore(entity::FightScoreType::AXE));
    member_sync.data_.__set_shoot_fight_score_(this->GetFightScore(entity::FightScoreType::SHOOT));
    member_sync.data_.__set_magic_fight_score_(this->GetFightScore(entity::FightScoreType::MAGIC));
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }
}

void GuildActor::SendMapSync() {
  if (this->guild_ != NULL) {
    // 同步成员信息
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_map_(this->GetMap());
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  } else {
    // 同步待审核成员信息
    gateway::protocol::MessageGuildPendingMemberSynchronize pending_member_sync;
    pending_member_sync.__set_type_(gateway::protocol::GuildPendingMemberOperateType::UPDATE);
    pending_member_sync.data_.__set_actor_id_(this->GetStringID());
    pending_member_sync.data_.__set_map_(this->GetMap());

    for (GuildSet::iterator iter = this->joined_guilds_.begin();
         iter != this->joined_guilds_.end(); ++iter) {
      Guild *guild = *iter;
      if (NULL == guild) {
        continue;
      }
      guild->BroadcastMessage(pending_member_sync,
          gateway::protocol::MessageType::MESSAGE_GUILD_PENDING_MEMBER_SYNCHRONIZE);
    }
  }
}

void GuildActor::SendLoadFinishEvent() {
  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(this->GetActorID());
  load_finish.__set_extension_type_(ExtensionType::GUILD);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, this->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &load_finish, sizeof(load_finish));
}

void GuildActor::Save(database::protocol::StorageGuildActorField &db_field) {
  db_field.__set_guild_id_(this->guild_id_);
  db_field.__set_guild_position_(this->guild_position_);
  db_field.__set_current_contribution_value_(this->current_contribution_value_);
  db_field.__set_total_contribution_value_(this->total_contribution_value_);
  db_field.__set_gold_contribution_(this->gold_contribution_);
  db_field.__set_freedom_dollars_contribution_(this->freedom_dollars_contribution_);
  db_field.__set_light_crystal_contribution_(this->light_crystal_contribution_);
  db_field.__set_dark_crystal_contribution_(this->dark_crystal_contribution_);
  db_field.__set_holy_crystal_contribution_(this->holy_crystal_contribution_);

  EncodeGuildSkill(this->guild_skills_, &db_field.guild_skills_);
  EncodeGuildBuff(this->guild_buffs_, &db_field.guild_buffs_);
  EncodeGuildPlaying(this->awarded_guild_playings_, &db_field.awarded_guild_playings_);
}

void GuildActor::Save() {
  if (this->CheckDirty() == false) {
    return;
  }

  database::protocol::StorageGuildActorSaveRequest request;
  request.__set_actor_id_(this->GetActorID());
  this->Save(request.field_);

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_GUILD_ACTOR_SAVE,
          this->GetActorID()) == false) {
    LOG_ERROR("Send StorageGuildActorSaveRequest to database server failed.");
    return;
  }

  this->SetDirty(false);
}

bool GuildActor::EnableGuildFunc(bool synchronize) {
  if (synchronize) {
    this->SendInitToClient();
  }

  return true;
}

core::uint64 GuildActor::GetGuildID() const {
  if (this->guild_ != NULL) {
    return this->guild_->GetID();
  }
  return 0;
}

const std::string GuildActor::GetGuildName() const {
  if (this->guild_ != NULL) {
    return this->guild_->GetName();
  }
  return std::string();
}

void GuildActor::SetGuildDirect(Guild *guild) {
  this->guild_ = guild;

  // 更新有效技能等级
  this->UpdateEffectiveGuildSkillLevel(false, false);
}

void GuildActor::SetGuild(Guild *guild, entity::GuildPositionType::type guild_position,
    bool synchronize) {
  this->guild_ = guild;
  this->guild_position_ = guild_position;

  if (guild != NULL) {
    if (this->guild_id_ != guild->GetID() &&
        this->guild_id_ != 0) {
      // 清理贡献值
      this->current_contribution_value_ = 0;
      this->total_contribution_value_ = 0;
      this->gold_contribution_ = 0;
      this->freedom_dollars_contribution_ = 0;
      this->light_crystal_contribution_ = 0;
      this->dark_crystal_contribution_ = 0;
      this->holy_crystal_contribution_ = 0;
    }

    // 设置公会ID
    this->guild_id_ = guild->GetID();
  }

  // 更新有效技能等级
  this->UpdateEffectiveGuildSkillLevel(true, false);
  // 清理BUFF
  this->guild_buffs_.clear();
  // 设置脏标志
  this->SetDirty();
  // 保存数据
  this->Save();

  // 同步 game_server
  session::protocol::MessageGuildActorSynchronize game_sync;
  game_sync.__set_actor_id_(this->GetActorID());
  game_sync.__set_guild_id_(this->GetGuildID());
  game_sync.__set_guild_name_(this->GetGuildName());
  game_sync.__set_current_contribution_value_(this->current_contribution_value_);
  this->SendGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_ACTOR_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    this->SendInitToClient();
  }
}

bool GuildActor::CheckJoinedGuild(Guild *guild) const {
  return this->joined_guilds_.find(guild) != this->joined_guilds_.end();
}

bool GuildActor::AddJoinedGuild(Guild *guild, bool synchronize) {
  if (this->guild_ != NULL) {
    LOG_ERROR("already in a guild.");
    return false;
  }
  if (NULL == guild) {
    LOG_ERROR("guild is null.");
    return false;
  }
  if (this->CheckJoinedGuild(guild)) {
    LOG_ERROR("already joined this guild.");
    return false;
  }

  if (guild->AddPendingMember(this) == false) {
    LOG_ERROR("Guild(%lu) AddPendingMember(%lu) failed.",
              guild->GetID(), this->GetActorID());
    return false;
  }

  this->joined_guilds_.insert(guild);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildListSynchronize sync;
    sync.__set_type_(gateway::protocol::GuildListOperateType::UPDATE);
    sync.data_.__set_guild_id_(guild->GetStringID());
    sync.data_.__set_joined_(true);
    this->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_LIST_SYNCHRONIZE);
  }

  return true;
}

bool GuildActor::RemoveJoinedGuild(Guild *guild, bool synchronize) {
  if (NULL == guild) {
    LOG_ERROR("guild is null.");
    return false;
  }
  if (this->CheckJoinedGuild(guild) == false) {
    LOG_ERROR("Not joined this guild.");
    return false;
  }

  if (guild->RemovePendingMember(this) == false) {
    LOG_ERROR("Guild(%lu) RemovePendingMember(%lu) failed.",
              guild->GetID(), this->GetActorID());
    return false;
  }

  this->joined_guilds_.erase(guild);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildListSynchronize sync;
    sync.__set_type_(gateway::protocol::GuildListOperateType::UPDATE);
    sync.data_.__set_guild_id_(guild->GetStringID());
    sync.data_.__set_joined_(false);
    this->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_LIST_SYNCHRONIZE);
  }

  return true;
}

void GuildActor::RemoveAllJoinedGuild(bool synchronize) {
  for (GuildSet::iterator iter = this->joined_guilds_.begin();
       iter != this->joined_guilds_.end();) {
    this->RemoveJoinedGuild(*iter++, synchronize);
  }
}

void GuildActor::SetGuildPosition(core::int32 guild_position, bool synchronize) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }
  this->guild_position_ = guild_position;
  this->SetDirty();
  this->Save();

  // 同步 game_server
  session::protocol::MessageGuildActorSynchronize game_sync;
  game_sync.__set_actor_id_(this->GetActorID());
  game_sync.__set_guild_position_(this->GetGuildPosition());
  this->SendGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_ACTOR_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildMemberSynchronize client_sync;
    client_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    client_sync.data_.__set_actor_id_(this->GetStringID());
    client_sync.data_.__set_guild_position_(this->guild_position_);
    this->guild_->BroadcastMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }
}

void GuildActor::AddContribution(core::int32 contribution, bool synchronize) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }

  this->current_contribution_value_ += contribution;
  this->total_contribution_value_ += contribution;
  this->SetDirty();

  // 同步 game_server
  session::protocol::MessageGuildActorSynchronize game_sync;
  game_sync.__set_actor_id_(this->GetActorID());
  game_sync.__set_current_contribution_value_(this->current_contribution_value_);
  this->SendGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_ACTOR_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize guild_sync;
    guild_sync.__set_current_contribution_value_(this->current_contribution_value_);
    this->SendMessage(guild_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);

    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_total_contribution_value_(this->total_contribution_value_);
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }
}

void GuildActor::DelContribution(core::int32 contribution, bool synchronize) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }

  this->current_contribution_value_ -= contribution;
  this->SetDirty();

  // 同步 game_server
  session::protocol::MessageGuildActorSynchronize game_sync;
  game_sync.__set_actor_id_(this->GetActorID());
  game_sync.__set_current_contribution_value_(this->current_contribution_value_);
  this->SendGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_ACTOR_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize guild_sync;
    guild_sync.__set_current_contribution_value_(this->current_contribution_value_);
    this->SendMessage(guild_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void GuildActor::AddGoldContribution(core::int32 contribution, bool synchronize) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }

  this->gold_contribution_ += contribution;
  this->SetDirty();

  if (synchronize) {
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_gold_contribution_(this->gold_contribution_);
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }
}

void GuildActor::AddFreedomDollarsContribution(core::int32 contribution, bool synchronize) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }

  this->freedom_dollars_contribution_ += contribution;
  this->SetDirty();

  if (synchronize) {
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_freedom_dollars_contribution_(this->freedom_dollars_contribution_);
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }
}

void GuildActor::AddLightCrystalContribution(core::int32 contribution, bool synchronize) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }

  this->light_crystal_contribution_ += contribution;
  this->SetDirty();

  if (synchronize) {
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_light_crystal_contribution_(this->light_crystal_contribution_);
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }
}

void GuildActor::AddDarkCrystalContribution(core::int32 contribution, bool synchronize) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }

  this->dark_crystal_contribution_ += contribution;
  this->SetDirty();

  if (synchronize) {
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_dark_crystal_contribution_(this->dark_crystal_contribution_);
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }
}

void GuildActor::AddHolyCrystalContribution(core::int32 contribution, bool synchronize) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }

  this->holy_crystal_contribution_ += contribution;
  this->SetDirty();

  if (synchronize) {
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::UPDATE);
    member_sync.data_.__set_actor_id_(this->GetStringID());
    member_sync.data_.__set_holy_crystal_contribution_(this->holy_crystal_contribution_);
    this->guild_->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }
}

core::int32 GuildActor::GetGuildSkillLevel(core::int32 id) const {
  GuildSkillLevelMap::const_iterator iter = this->guild_skills_.find(id);
  if (iter == this->guild_skills_.end()) {
    return 0;
  } else {
    return iter->second;
  }
}

void GuildActor::SetGuildSkillLevel(core::int32 id, core::int32 level, bool synchronize) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }

  bool added = false;

  if (this->guild_skills_.find(id) == this->guild_skills_.end()) {
    added = true;
  }

  this->guild_skills_[id] = level;
  this->SetDirty();

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSkillSynchronize sync;
    if (added) {
      sync.__set_type_(gateway::protocol::GuildSkillOperateType::ADD);
    } else {
      sync.__set_type_(gateway::protocol::GuildSkillOperateType::UPDATE);
    }
    sync.data_.__set_id_(id);
    sync.data_.__set_level_(level);
    this->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SKILL_SYNCHRONIZE);
  }

  // 更新有效等级
  this->UpdateEffectiveGuildSkillLevel(id, synchronize);
}

void GuildActor::UpdateEffectiveGuildSkillLevel(core::int32 id,
    bool synchronize_game, bool synchronize_client) {
  if (NULL == this->guild_) {
    LOG_ERROR("not in a guild.");
    return;
  }

  if (this->guild_skills_.find(id) == this->guild_skills_.end()) {
    LOG_ERROR("GuildSkill(%d) not found.", id);
    return;
  }

  core::int32 effective_level =
    std::min(GUILD_CONF()->GetMaxSkillLevelInCollegeLevel(this->guild_->GetCollegeLevel(), id),
             this->guild_skills_[id]);
  bool changed = false;

  if (this->effective_guild_skills_.find(id) ==
      this->effective_guild_skills_.end()) {
    this->effective_guild_skills_[id] = effective_level;
    changed = true;
  } else {
    if (this->effective_guild_skills_[id] != effective_level) {
      this->effective_guild_skills_[id] = effective_level;
      changed = true;
    }
  }

  // 同步 game_server
  if (changed && synchronize_game) {
    session::protocol::MessageGuildSkillSynchronize game_sync;
    game_sync.__set_actor_id_(this->GetActorID());
    game_sync.__set_type_(session::protocol::GuildSkillOperateType::UPDATE);
    game_sync.data_.__set_id_(id);
    game_sync.data_.__set_effective_level_(effective_level);
    this->SendGameMessage(game_sync,
        session::protocol::GameMessageType::MESSAGE_GUILD_SKILL_SYNCHRONIZE);
  }
  
  // 同步 client
  if (changed && synchronize_client) {
    gateway::protocol::MessageGuildSkillSynchronize client_sync;
    client_sync.__set_type_(gateway::protocol::GuildSkillOperateType::UPDATE);
    client_sync.data_.__set_id_(id);
    client_sync.data_.__set_effective_level_(effective_level);
    this->SendMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SKILL_SYNCHRONIZE);
  }
}

void GuildActor::UpdateEffectiveGuildSkillLevel(bool synchronize_game, bool synchronize_client) {
  if (NULL == this->guild_) {
    this->effective_guild_skills_.clear();

    // 同步 game_server
    if (synchronize_game) {
      session::protocol::MessageGuildSkillSynchronize game_sync;
      game_sync.__set_actor_id_(this->GetActorID());
      game_sync.__set_type_(session::protocol::GuildSkillOperateType::CLEAR);
      this->SendGameMessage(game_sync,
          session::protocol::GameMessageType::MESSAGE_GUILD_SKILL_SYNCHRONIZE);
    }

    return;
  }

  for (GuildSkillLevelMap::const_iterator iter = this->guild_skills_.begin();
       iter != this->guild_skills_.end(); ++iter) {
    core::int32 skill_id = iter->first;
    this->UpdateEffectiveGuildSkillLevel(skill_id, synchronize_game, synchronize_client);
  }
}

bool GuildActor::CheckGuildBuffExists(core::int32 id) const {
  if (this->guild_buffs_.find(id) != this->guild_buffs_.end()) {
    return true;
  }
  return false;
}

bool GuildActor::AddGuildBuff(core::int32 id, bool synchronize) {
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

  this->guild_buffs_.insert(id);
  this->SetDirty();

  // 同步 game_server
  session::protocol::MessageGuildBuffSynchronize game_sync;
  game_sync.__set_actor_id_(this->GetActorID());
  game_sync.__set_type_(session::protocol::GuildBuffOperateType::ADD);
  game_sync.__set_guild_buff_id_(id);
  this->SendGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_BUFF_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildBuffSynchronize client_sync;
    client_sync.__set_type_(gateway::protocol::GuildBuffOperateType::ADD);
    client_sync.__set_guild_buff_id_(id);
    this->SendMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_BUFF_SYNCHRONIZE);
  }

  return true;
}

void GuildActor::ClearGuildBuff(bool synchronize) {
  this->guild_buffs_.clear();
  this->SetDirty();

  // 同步 game_server
  session::protocol::MessageGuildBuffSynchronize game_sync;
  game_sync.__set_actor_id_(this->GetActorID());
  game_sync.__set_type_(session::protocol::GuildBuffOperateType::CLEAR);
  game_sync.__set_guild_buff_id_(0);
  this->SendGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_BUFF_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildBuffSynchronize client_sync;
    client_sync.__set_type_(gateway::protocol::GuildBuffOperateType::CLEAR);
    client_sync.__set_guild_buff_id_(0);
    this->SendMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_BUFF_SYNCHRONIZE);
  }
}

bool GuildActor::CheckPlayingAwarded(core::int32 playing_template_id) const {
  return this->awarded_guild_playings_.find(playing_template_id) != 
         this->awarded_guild_playings_.end();
}

void GuildActor::SetPlayingAwarded(core::int32 playing_template_id) {
  this->awarded_guild_playings_.insert(playing_template_id);
}

void GuildActor::WeeklyClean() {
  this->awarded_guild_playings_.clear();
}

}  // namespace guild

}  // namespace server

}  // namespace session

