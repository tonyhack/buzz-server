#include "session_server/server/extension/guild/guild.h"

#include <cstddef>
#include <cstdlib>
#include <boost/bind.hpp>

#include "entity/inform_types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "global/server_configure.h"
#include "global/configure/configure.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_guild_save_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_guild_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_guild_protocol_types.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/session_server.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/guild/facade_request.h"

#define MAX_INVITE_CAPACITY 30

namespace session {

namespace server {

namespace guild {

static void EncodeGuildActorSet(const Guild::GuildActorSet &source, std::string *result) {
  char buffer[1024] = "";
  size_t count = 0;

  Guild::GuildActorSet::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    count += snprintf(buffer + count, sizeof(buffer) - count, "%lu,", (*iter)->GetActorID());
  }

  *result = buffer;
}

static void DecodeGuildEvent(const std::string &source, Guild::GuildEventDeque *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    GuildEvent event;
    if (event.Initialize(split_result[i]) == false) {
      LOG_ERROR("GuildEvent(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    result->push_back(event);
  }
}

static void EncodeGuildEvent(const Guild::GuildEventDeque &source, std::string *result) {
  char buffer[10240] = "";
  size_t count = 0;

  Guild::GuildEventDeque::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    const GuildEvent &event = *iter;
    
    count += snprintf(buffer + count, sizeof(buffer) - count, "%d:%s",
        event.GetType(), event.GetTimestamp().c_str());

    for (size_t i = 0; i < event.GetEventVars().size(); ++i) {
      count += snprintf(buffer + count, sizeof(buffer) - count, ":%s",
          event.GetEventVars()[i].c_str());
    }
    count += snprintf(buffer + count, sizeof(buffer) - count, ",");
  }

  *result = buffer;
}

static void DecodeGuildPlayingGroup(const std::string &source,
    Guild::GuildPlayingGroupSet *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    core::int32 playing_group_id = atoi(split_result[i].c_str());

    // 检查公会副本组是否存在
    if (GUILD_CONF()->GetGuildPlayingGroup(playing_group_id) == NULL) {
      LOG_ERROR("playing_group_id(%d) not found in config.", playing_group_id);
      continue;
    }

    result->insert(playing_group_id);
  }
}

static void EncodeGuildPlayingGroup(const Guild::GuildPlayingGroupSet &source,
    std::string *result) {
  char buffer[128] = "";
  size_t count = 0;

  Guild::GuildPlayingGroupSet::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    count += snprintf(buffer + count, sizeof(buffer) - count, "%d,", *iter);
  }

  *result = buffer;
}

static void DecodeGuildPlaying(const std::string &source,
    Guild::GuildPlayingSet *result) {
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

static void EncodeGuildPlaying(const Guild::GuildPlayingSet &source,
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

Guild::Guild() : id_(0), logo_(0), level_(0), suppress_join_(false), leader_(NULL),
  gold_(0), light_crystal_(0), dark_crystal_(0), holy_crystal_(0),
  light_attribute_(0), dark_attribute_(0), holy_attribute_(0),
  shop_level_(0), college_level_(0), barrack_level_(0),
  current_playing_id_(0), current_playing_template_id_(0),
  last_save_time_(0) {}

Guild::~Guild() {}

bool Guild::Initialize(core::uint64 guild_id, const std::string &guild_name,
    core::int32 guild_logo, GuildActor *leader) {
  if (NULL == leader) {
    LOG_ERROR("leader is null.");
    return false;
  }

  this->id_ = guild_id;
  this->name_ = guild_name;
  this->logo_ = guild_logo;
  this->level_ = 1;
  this->gold_ = MISC_CONF()->guild_init_gold_;
  this->shop_level_ = 0;
  this->college_level_ = 0;
  this->barrack_level_ = 0;
  this->current_playing_id_ = 0;
  this->current_playing_template_id_ = 0;

  this->AddTimer();
  this->InitPositionStatistics();

  this->members_.insert(leader);
  this->position_statistics_[entity::GuildPositionType::LEADER].insert(leader);
  this->leader_ = leader;

  leader->SetGuild(this, entity::GuildPositionType::LEADER);

  return true;
}

bool Guild::Initialize(const database::protocol::StorageGuildField &db_field) {
  this->id_ = db_field.id_;
  this->name_ = db_field.name_; 
  this->logo_ = db_field.logo_;
  this->level_ = db_field.level_;
  this->suppress_join_ = db_field.suppress_join_;
  this->announcement_ = db_field.announcement_;
  this->gold_ = db_field.gold_;
  this->light_crystal_ = db_field.light_crystal_;
  this->dark_crystal_ = db_field.dark_crystal_;
  this->holy_crystal_ = db_field.holy_crystal_;
  this->light_attribute_ = db_field.light_attribute_;
  this->dark_attribute_ = db_field.dark_attribute_;
  this->holy_attribute_ = db_field.holy_attribute_;
  this->shop_level_ = db_field.shop_level_;
  this->college_level_ = db_field.college_level_;
  this->barrack_level_ = db_field.barrack_level_;
  DecodeGuildEvent(db_field.events_, &this->events_);
  DecodeGuildPlayingGroup(db_field.opened_playing_groups_, &this->opened_playing_groups_);
  DecodeGuildPlaying(db_field.finished_playings_, &this->finished_playings_);
  this->current_playing_id_ = 0;
  this->current_playing_template_id_ = 0;
  this->last_save_time_ = db_field.last_save_time_;

  this->AddTimer();
  this->InitPositionStatistics();

  return true;
}

void Guild::Finalize() {
  this->RemoveTimer();

  this->last_save_time_ = 0;
  this->SetCurrentPlaying(0, 0, false);
  this->finished_playings_.clear();
  this->opened_playing_groups_.clear();
  this->barrack_level_ = 0;
  this->college_level_ = 0;
  this->shop_level_ = 0;
  this->holy_attribute_ = 0;
  this->dark_attribute_ = 0;
  this->light_attribute_ = 0;
  this->holy_crystal_ = 0;
  this->dark_crystal_ = 0;
  this->light_crystal_ = 0;
  this->gold_ = 0;
  this->events_.clear();
  this->announcement_.clear();
  this->leader_ = NULL;
  this->position_statistics_.clear();
  this->members_.clear();
  this->pending_members_.clear();
  this->invite_members_.clear();
  this->suppress_join_ = false;
  this->level_ = 0;
  this->logo_ = 0;
  this->name_.clear();
  this->id_ = 0;
}

bool Guild::CheckDataValid() {
  if (NULL == this->leader_) {
    LOG_ERROR("Guild has no leader.");
    return false;
  }

  if (this->position_statistics_[entity::GuildPositionType::LEADER].size() != 1) {
    LOG_ERROR("leader_count(%zd) is incorrect",
              this->position_statistics_[entity::GuildPositionType::LEADER].size());
    return false;
  }

  return true;
}

void Guild::Save(database::protocol::StorageGuildField &db_field) {
  db_field.__set_id_(this->id_);
  db_field.__set_name_(this->name_);
  db_field.__set_logo_(this->logo_);
  db_field.__set_level_(this->level_);
  db_field.__set_suppress_join_(this->suppress_join_);
  EncodeGuildActorSet(this->pending_members_, &db_field.pending_members_);
  EncodeGuildActorSet(this->members_, &db_field.members_);
  db_field.__set_announcement_(this->announcement_);
  EncodeGuildEvent(this->events_, &db_field.events_);
  db_field.__set_gold_(this->gold_);
  db_field.__set_light_crystal_(this->light_crystal_);
  db_field.__set_dark_crystal_(this->dark_crystal_);
  db_field.__set_holy_crystal_(this->holy_crystal_);
  db_field.__set_light_attribute_(this->light_attribute_);
  db_field.__set_dark_attribute_(this->dark_attribute_);
  db_field.__set_holy_attribute_(this->holy_attribute_);
  db_field.__set_shop_level_(this->shop_level_);
  db_field.__set_college_level_(this->college_level_);
  db_field.__set_barrack_level_(this->barrack_level_);
  EncodeGuildPlayingGroup(this->opened_playing_groups_, &db_field.opened_playing_groups_);
  EncodeGuildPlaying(this->finished_playings_, &db_field.finished_playings_);
  this->last_save_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  db_field.__set_last_save_time_(this->last_save_time_);
}

void Guild::Save() {
  database::protocol::StorageGuildSaveRequest request;
  this->Save(request.field_);

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_GUILD_SAVE,
          this->GetID()) == false) {
    LOG_ERROR("Send StorageGuildSaveRequest to database server failed.");
    return;
  }
}

void Guild::AddTimer() {
  // 注册一个定时器，用于定时保存数据
  core::uint32 random = ExtensionManager::GetInstance()->GetRandom().Random(1000);

  SessionServerSingleton::GetInstance().AddTimer(TIMER_ID_SAVE,
      global::ServerConfigureSingleton::GetInstance().GetAutoSave().actor_ * 1000 + random,
      -1, this, "Guild::AddTimer");
}

void Guild::RemoveTimer() {
  // 删除数据保存定时器
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_SAVE, this);
}

void Guild::OnTimer(core::uint32 id) {
  if (TIMER_ID_SAVE == id) { 
    this->Save();
    this->RemoveTimer();
    this->AddTimer();
  }
}

const std::string Guild::GetStringID() const {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%lu", this->id_);
  return std::string(buffer);
}

void Guild::InitPositionStatistics() {
  const global::configure::GuildConfigure::GuildPositionHashmap &guild_position_cells =
    GUILD_CONF()->GetAllGuildPosition();

  global::configure::GuildConfigure::GuildPositionHashmap::const_iterator iter =
    guild_position_cells.begin();
  for (; iter != guild_position_cells.end(); ++iter) {
    const global::configure::GuildPositionCell &cell = iter->second;
    position_statistics_.insert(std::make_pair(cell.id_, GuildActorSet()));
  }
}

void Guild::SetLevel(core::int32 new_level, bool synchronize) {
  this->level_ = new_level;

  // 同步 game_server
  session::protocol::MessageGuildInfoSynchronize game_sync;
  game_sync.__set_type_(session::protocol::GuildOperateType::UPDATE);
  game_sync.data_.__set_id_(this->id_);
  game_sync.data_.__set_level_(this->level_);
  SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize guild_sync;
    guild_sync.__set_guild_level_(this->level_);
    this->BroadcastMessage(guild_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

core::int64 Guild::GetTotalFightScore() const {
  core::int64 total_fight_score = 0;

  for (GuildActorSet::const_iterator iter = this->members_.begin();
       iter != this->members_.end(); ++iter) {
    GuildActor *guild_actor = *iter;
    if (guild_actor != NULL) {
      total_fight_score += guild_actor->GetCurrentFightScore();
    }
  }

  return total_fight_score;
}

void Guild::SetSuppressJoin(bool suppress_join, bool synchronize) {
  this->suppress_join_ = suppress_join;

  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize guild_sync;
    guild_sync.__set_suppress_join_(this->suppress_join_);
    this->BroadcastMessage(guild_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);

    gateway::protocol::MessageGuildListSynchronize guild_list_sync;
    guild_list_sync.__set_type_(gateway::protocol::GuildListOperateType::UPDATE);
    guild_list_sync.data_.__set_guild_id_(this->GetStringID());
    guild_list_sync.data_.__set_suppress_join_(this->suppress_join_);
    SessionServerSingleton::GetInstance().BroadcastMessage(guild_list_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_LIST_SYNCHRONIZE);
  }
}

bool Guild::CheckPendingMemberFull() {
  return this->GetPendingMemberNum() >= MISC_CONF()->guild_max_pending_member_capacity_;
}

bool Guild::CheckMemberFull() {
  const global::configure::GuildCell *cell = GUILD_CONF()->GetGuild(this->level_);
  if (NULL == cell) {
    LOG_ERROR("GuildCell(%d) not found in config.", this->level_);
    return true;
  }
  return this->GetMemberNum() >= cell->max_members_;
}

bool Guild::CheckInvitedMember(core::uint64 actor_id) {
  for (size_t i = 0; i < this->invite_members_.size(); ++i) {
    if (this->invite_members_[i] == actor_id) {
      return true;
    }
  }
  return false;
}

void Guild::AddInviteMember(core::uint64 actor_id) {
  // 已存在则不添加
  if (this->CheckInvitedMember(actor_id)) {
    return;
  }

  // 超过上限, 删掉最早的邀请
  if (this->invite_members_.size() >= MAX_INVITE_CAPACITY) {
    this->invite_members_.pop_front();
  }
  this->invite_members_.push_back(actor_id);
}

void Guild::RemoveInviteMember(core::uint64 actor_id) {
  for (size_t i = 0; i < this->invite_members_.size(); ++i) {
    if (this->invite_members_[i] == actor_id) {
      this->invite_members_.erase(this->invite_members_.begin() + i);
      return;
    }
  }
}

bool Guild::AddPendingMemberDirect(GuildActor *pending_member) {
  if (NULL == pending_member) {
    LOG_ERROR("pending_member is null.");
    return false;
  }
  if (this->pending_members_.find(pending_member) != this->pending_members_.end()) {
    LOG_ERROR("GuildActor(%lu) is already in this guild's pending member list.",
              pending_member->GetActorID());
    return false;
  }
  this->pending_members_.insert(pending_member);

  return true;
}

bool Guild::AddMemberDirect(GuildActor *member) {
  if (NULL == member) {
    LOG_ERROR("member is null.");
    return false;
  }
  if (this->members_.find(member) != this->members_.end()) {
    LOG_ERROR("GuildActor(%lu) is already in this guild's member list.",
              member->GetActorID());
    return false;
  }
  if (this->position_statistics_.find(member->GetGuildPosition()) ==
      this->position_statistics_.end()) {
    LOG_ERROR("GuildActor(%lu)'s position is invalid.", member->GetActorID());
    return false;
  }

  member->SetGuildDirect(this);
  this->members_.insert(member);
  this->position_statistics_[member->GetGuildPosition()].insert(member);

  if (entity::GuildPositionType::LEADER == member->GetGuildPosition()) {
    this->leader_ = member;
  }

  return true;
}

bool Guild::AddPendingMember(GuildActor *pending_member, bool synchronize) {
  if (NULL == pending_member) {
    LOG_ERROR("pending_member is null.");
    return false;
  }
  if (this->pending_members_.find(pending_member) != this->pending_members_.end()) {
    LOG_ERROR("GuildActor(%lu) is already in this guild's pending member list.",
              pending_member->GetActorID());
    return false;
  }
  this->pending_members_.insert(pending_member);

  if (synchronize) {
    gateway::protocol::MessageGuildPendingMemberSynchronize sync;
    sync.__set_type_(gateway::protocol::GuildPendingMemberOperateType::ADD);
    sync.data_.__set_actor_id_(pending_member->GetStringID());
    sync.data_.__set_actor_name_(pending_member->GetName());
    sync.data_.__set_online_(pending_member->CheckOnline());
    sync.data_.__set_last_logout_time_(pending_member->GetStringLastLogoutTime());
    sync.data_.__set_level_(pending_member->GetLevel());
    sync.data_.__set_map_(pending_member->GetMap());
    this->BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_PENDING_MEMBER_SYNCHRONIZE);
  }

  return true;
}

bool Guild::RemovePendingMember(GuildActor *pending_member, bool synchronize) {
  if (this->pending_members_.find(pending_member) == this->pending_members_.end()) {
    LOG_ERROR("GuildActor(%lu) is not in this guild's pending member list.",
              pending_member->GetActorID());
    return false;
  }
  this->pending_members_.erase(pending_member);

  if (synchronize) {
    gateway::protocol::MessageGuildPendingMemberSynchronize sync;
    sync.__set_type_(gateway::protocol::GuildPendingMemberOperateType::REMOVE);
    sync.data_.__set_actor_id_(pending_member->GetStringID());
    this->BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_PENDING_MEMBER_SYNCHRONIZE);
  }

  return true;
}

bool Guild::AddMember(GuildActor *member, bool synchronize) {
  if (NULL == member) {
    LOG_ERROR("member is null.");
    return false;
  }
  if (this->members_.find(member) != this->members_.end()) {
    LOG_ERROR("GuildActor(%lu) is already in this guild.");  
    return false;
  }

  this->members_.insert(member);
  this->position_statistics_[entity::GuildPositionType::MEMBER].insert(member);

  member->SetGuild(this, entity::GuildPositionType::MEMBER);

  if (synchronize) {
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::ADD);
    member_sync.data_.__set_actor_id_(member->GetStringID());
    member_sync.data_.__set_actor_name_(member->GetName());
    member_sync.data_.__set_online_(member->CheckOnline());
    member_sync.data_.__set_last_logout_time_(member->GetStringLastLogoutTime());
    member_sync.data_.__set_level_(member->GetLevel());
    member_sync.data_.__set_map_(member->GetMap());
    member_sync.data_.__set_guild_position_(member->GetGuildPosition());
    member_sync.data_.__set_total_contribution_value_(member->GetTotalContributionValue());
    member_sync.data_.__set_gold_contribution_(member->GetGoldContribution());
    member_sync.data_.__set_freedom_dollars_contribution_(member->GetFreedomDollarsContribution());
    member_sync.data_.__set_light_crystal_contribution_(member->GetLightCrystalContribution());
    member_sync.data_.__set_dark_crystal_contribution_(member->GetDarkCrystalContribution());
    member_sync.data_.__set_holy_crystal_contribution_(member->GetHolyCrystalContribution());
    member_sync.data_.__set_axe_fight_score_(member->GetFightScore(entity::FightScoreType::AXE));
    member_sync.data_.__set_shoot_fight_score_(member->GetFightScore(entity::FightScoreType::SHOOT));
    member_sync.data_.__set_magic_fight_score_(member->GetFightScore(entity::FightScoreType::MAGIC));
    this->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }

  return true;
}

bool Guild::RemoveMember(GuildActor *member, bool synchronize) {
  if (this->members_.find(member) == this->members_.end()) {
    LOG_ERROR("GuildActor(%lu) is not in this guild.");  
    return false;
  }
  if (this->position_statistics_.find(member->GetGuildPosition()) ==
      this->position_statistics_.end()) {
    LOG_ERROR("GuildActor(%lu)'s position is invalid.", member->GetActorID());
    return false;
  }

  this->members_.erase(member);
  this->position_statistics_[member->GetGuildPosition()].erase(member);

  member->SetGuild(NULL, entity::GuildPositionType::MEMBER);

  if (synchronize) {
    gateway::protocol::MessageGuildMemberSynchronize member_sync;
    member_sync.__set_type_(gateway::protocol::GuildMemberOperateType::REMOVE);
    member_sync.data_.__set_actor_id_(member->GetStringID());
    this->BroadcastMessage(member_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_MEMBER_SYNCHRONIZE);
  }

  return true;
}

bool Guild::CheckAutoDismiss() const
{
  const global::configure::GuildCell *cell = GUILD_CONF()->GetGuild(this->level_);
  if (NULL == cell) {
    LOG_ERROR("GuildCell(%d) not found in config.", this->level_);
    return false;
  }

  return this->GetGold() < -1 * cell->maintenance_fee_limit_;
}

void Guild::Dismiss() {
  // 发送解散通知
  gateway::protocol::MessageGuildDismissNotify notify;
  this->BroadcastMessage(notify,
      gateway::protocol::MessageType::MESSAGE_GUILD_DISMISS_NOTIFY);

  // 发送解散邮件
  for (GuildActorSet::iterator iter = this->members_.begin();
       iter != this->members_.end(); ++iter) {
    GuildActor *guild_actor = *iter;
    if (guild_actor != NULL) {
      if (FacadeRequest::GetInstance()->SendDismissMail(guild_actor->GetActorID()) == false) {
        LOG_ERROR("SendDismissMail to actor(%lu) failed.", guild_actor->GetActorID());
        continue;
      }
    }
  }

  // 移除所有未审核成员
  for (GuildActorSet::iterator iter = this->pending_members_.begin();
       iter != this->pending_members_.end();) {
    this->RemovePendingMember(*iter++, false);
  }

  // 移除所有成员
  for (GuildActorSet::iterator iter = this->members_.begin();
       iter != this->members_.end();) {
    this->RemoveMember(*iter++, false);
  }
}

bool Guild::SetGuildPosition(GuildActor *member, core::int32 guild_position, bool synchronize) {
  if (NULL == member) {
    LOG_ERROR("member is null.");
    return false;
  }
  if (this->position_statistics_.find(member->GetGuildPosition()) ==
      this->position_statistics_.end()) {
    LOG_ERROR("GuildActor(%lu)'s position is invalid.", member->GetGuildPosition());
    return false;
  }
  if (this->position_statistics_.find(guild_position) ==
      this->position_statistics_.end()) {
    LOG_ERROR("New guild position(%d) is invalid.", guild_position);
    return false;
  }

  const global::configure::GuildPositionCell *cell =
    GUILD_CONF()->GetGuildPosition(guild_position);
  if (NULL == cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_position);
    return false;
  }

  if (guild_position != entity::GuildPositionType::MEMBER &&
      this->position_statistics_[guild_position].size() >= (size_t)cell->max_num_) {
    LOG_WARNING("Guild postion(%d) is full.", guild_position);
    return false;
  }

  this->position_statistics_[member->GetGuildPosition()].erase(member);
  this->position_statistics_[guild_position].insert(member);
  
  member->SetGuildPosition(guild_position, synchronize);

  return true;
}

void Guild::SetLeader(GuildActor *new_leader, bool synchronize) {
  if (NULL == new_leader || NULL == this->leader_) {
    LOG_ERROR("leader is null.");
    return;
  }
  if (this->position_statistics_.find(new_leader->GetGuildPosition()) ==
      this->position_statistics_.end()) {
    LOG_ERROR("GuildActor(%lu)'s position is invalid.", new_leader->GetGuildPosition());
    return;
  }
  GuildActor *old_leader = this->leader_;

  this->position_statistics_[entity::GuildPositionType::LEADER].erase(old_leader);
  this->position_statistics_[new_leader->GetGuildPosition()].erase(new_leader);
  this->position_statistics_[entity::GuildPositionType::LEADER].insert(new_leader);
  this->position_statistics_[entity::GuildPositionType::MEMBER].insert(old_leader);

  this->leader_ = new_leader;

  new_leader->SetGuildPosition(entity::GuildPositionType::LEADER, synchronize);
  old_leader->SetGuildPosition(entity::GuildPositionType::MEMBER, synchronize);
}

void Guild::SetAnnouncement(const std::string &new_announcement, bool synchronize) {
  this->announcement_ = new_announcement;

  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize sync;
    sync.__set_guild_announcement_(this->announcement_);
    this->BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::AddEvent(const GuildEvent &event, bool synchronize) {
  if (this->events_.size() >= (size_t)MISC_CONF()->guild_max_event_capacity_ &&
      !this->events_.empty()) {
    this->events_.pop_front();
  }
  this->events_.push_back(event);

  if (synchronize) {
    gateway::protocol::MessageGuildEventSynchronize sync;
    sync.__set_type_(gateway::protocol::GuildEventOperateType::ADD);
    sync.data_.__set_event_type_((entity::GuildEventType::type)event.GetType());
    sync.data_.__set_timestamp_(event.GetTimestamp());
    event.TranslateEventVar(&sync.data_.vars_);
    this->BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_EVENT_SYNCHRONIZE);
  }
}

void Guild::SetGold(core::int32 gold, bool synchronize) {
  this->gold_ = gold;

  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize sync;
    sync.__set_guild_gold_(this->gold_);
    this->BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::SetLightCrystal(core::int32 light_crystal, bool synchronize) {
  this->light_crystal_ = light_crystal;
  
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize sync;
    sync.__set_guild_light_crystal_(this->light_crystal_);
    this->BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::SetDarkCrystal(core::int32 dark_crystal, bool synchronize) {
  this->dark_crystal_ = dark_crystal;

  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize sync;
    sync.__set_guild_dark_crystal_(this->dark_crystal_);
    this->BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::SetHolyCrystal(core::int32 holy_crystal, bool synchronize) {
  this->holy_crystal_ = holy_crystal;

  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize sync;
    sync.__set_guild_holy_crystal_(this->holy_crystal_);
    this->BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::Donate(GuildActor *member, entity::GuildDonateType::type type,
    core::int32 num, bool synchronize) {
  core::int32 contribution = 0;

  if (entity::GuildDonateType::GOLD == type) {
    // 捐献金币
    contribution = MISC_CONF()->guild_gold_unit_contribution_ * num;
    member->AddGoldContribution(num, synchronize);
    this->SetGold(this->gold_ + num, synchronize);

  } else if (entity::GuildDonateType::FREEDOM_DOLLARS == type) {
    // 捐献钻石
    contribution = MISC_CONF()->guild_freedom_dollars_unit_contribution_ * num;
    member->AddFreedomDollarsContribution(num, synchronize);
    this->SetGold(this->gold_ + MISC_CONF()->guild_freedom_dollars_unit_gold_ * num, synchronize);

  } else if (entity::GuildDonateType::LIGHT_CRYSTAL == type) {
    // 捐献光晶石
    contribution = MISC_CONF()->guild_light_crystal_unit_contribution_ * num;
    member->AddLightCrystalContribution(num, synchronize);
    this->SetLightCrystal(this->light_crystal_ + num, synchronize);

  } else if (entity::GuildDonateType::DARK_CRYSTAL == type) {
    // 捐献暗晶石
    contribution = MISC_CONF()->guild_dark_crystal_unit_contribution_ * num;
    member->AddDarkCrystalContribution(num, synchronize);
    this->SetDarkCrystal(this->dark_crystal_ + num, synchronize);

  } else if (entity::GuildDonateType::HOLY_CRYSTAL == type) {
    // 捐献圣晶石
    contribution = MISC_CONF()->guild_holy_crystal_unit_contribution_ * num;
    member->AddHolyCrystalContribution(num, synchronize);
    this->SetHolyCrystal(this->holy_crystal_ + num, synchronize);

  } else {
    LOG_ERROR("Invalid donate type.");
    return;
  }

  // 增加贡献值
  member->AddContribution(contribution, synchronize);

  // 公会捐赠公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::GUILD_DONATE);
  inform.params_.push_back(member->GetStringID());
  inform.params_.push_back(member->GetName());
  inform.params_.push_back(global::ToString(type));
  inform.params_.push_back(global::ToString(num));
  inform.params_.push_back(global::ToString(contribution));
  this->BroadcastMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
}

void Guild::SetLightAttribute(core::int32 light_attribute, bool synchronize) {
  this->light_attribute_ = light_attribute;

  // 同步 game_server
  session::protocol::MessageGuildInfoSynchronize game_sync;
  game_sync.__set_type_(session::protocol::GuildOperateType::UPDATE);
  game_sync.data_.__set_id_(this->id_);
  game_sync.data_.__set_light_attribute_(this->light_attribute_);
  SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize client_sync;
    client_sync.__set_guild_light_attribute_(this->light_attribute_);
    this->BroadcastMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::SetDarkAttribute(core::int32 dark_attribute, bool synchronize) {
  this->dark_attribute_ = dark_attribute;

  // 同步 game_server
  session::protocol::MessageGuildInfoSynchronize game_sync;
  game_sync.__set_type_(session::protocol::GuildOperateType::UPDATE);
  game_sync.data_.__set_id_(this->id_);
  game_sync.data_.__set_dark_attribute_(this->dark_attribute_);
  SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize client_sync;
    client_sync.__set_guild_dark_attribute_(this->dark_attribute_);
    this->BroadcastMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::SetHolyAttribute(core::int32 holy_attribute, bool synchronize) {
  this->holy_attribute_ = holy_attribute;

  // 同步 game_server
  session::protocol::MessageGuildInfoSynchronize game_sync;
  game_sync.__set_type_(session::protocol::GuildOperateType::UPDATE);
  game_sync.data_.__set_id_(this->id_);
  game_sync.data_.__set_holy_attribute_(this->holy_attribute_);
  SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize client_sync;
    client_sync.__set_guild_holy_attribute_(this->holy_attribute_);
    this->BroadcastMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::SetShopLevel(core::int32 new_level, bool synchronize) {
  this->shop_level_ = new_level;

  // 同步 game_server
  session::protocol::MessageGuildInfoSynchronize game_sync;
  game_sync.__set_type_(session::protocol::GuildOperateType::UPDATE);
  game_sync.data_.__set_id_(this->id_);
  game_sync.data_.__set_shop_level_(this->shop_level_);
  SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize client_sync;
    client_sync.__set_guild_shop_level_(this->shop_level_);
    this->BroadcastMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::SetCollegeLevel(core::int32 new_level, bool synchronize) {
  this->college_level_ = new_level;

  // 更新公会成员的技能有效等级
  for (GuildActorSet::iterator iter = this->members_.begin();
       iter != this->members_.end(); ++iter) {
    GuildActor *guild_actor = *iter;
    if (guild_actor != NULL) {
      guild_actor->UpdateEffectiveGuildSkillLevel();
    }
  }

  // 同步 game_server
  session::protocol::MessageGuildInfoSynchronize game_sync;
  game_sync.__set_type_(session::protocol::GuildOperateType::UPDATE);
  game_sync.data_.__set_id_(this->id_);
  game_sync.data_.__set_college_level_(this->college_level_);
  SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize client_sync;
    client_sync.__set_guild_college_level_(this->college_level_);
    this->BroadcastMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::SetBarrackLevel(core::int32 new_level, bool synchronize) {
  this->barrack_level_ = new_level;

  // 同步 game_server
  session::protocol::MessageGuildInfoSynchronize game_sync;
  game_sync.__set_type_(session::protocol::GuildOperateType::UPDATE);
  game_sync.data_.__set_id_(this->id_);
  game_sync.data_.__set_barrack_level_(this->barrack_level_);
  SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize client_sync;
    client_sync.__set_guild_barrack_level_(this->barrack_level_);
    this->BroadcastMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

bool Guild::CheckPlayingGroupOpened(core::int32 playing_group_id) const {
  return this->opened_playing_groups_.find(playing_group_id) !=
         this->opened_playing_groups_.end();
}

bool Guild::CheckPlayingFinished(core::int32 playing_template_id) const {
  return this->finished_playings_.find(playing_template_id) !=
         this->finished_playings_.end();
}

bool Guild::CheckCanCreatePlaying(core::int32 playing_template_id) const {
  // 重置前2小时无法创建副本
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  time_t this_week_clean_time = GlobalTimeTick::GetInstance()->GetWeekTime(1, 0, 0, 0, now);
  time_t next_clean_time;
  if (now < this_week_clean_time) {
    next_clean_time = this_week_clean_time;
  } else {
    next_clean_time = this_week_clean_time + 7 * 24 * 60 * 60;
  }
  if (next_clean_time - now <= 2 * 60 * 60) {
    return false;
  }

  // 检查当前是否有正在进行中的副本
  if (this->current_playing_id_ != 0 || this->current_playing_template_id_ != 0) {
    LOG_WARNING("Another playing in progress.");
    return false;
  }

  // 检查副本是否存在
  const global::configure::GuildPlayingCell *cell =
      GUILD_CONF()->GetGuildPlaying(playing_template_id);
  if (NULL == cell) {
    LOG_WARNING("playing_template_id not found in config.");
    return false;
  }

  // 检查副本是否已完成
  if (this->CheckPlayingFinished(playing_template_id)) {
    LOG_WARNING("Playing already finished.");
    return false;
  }

  // 检查副本组是否已开启
  if (this->CheckPlayingGroupOpened(cell->group_id_) == false) {
    LOG_WARNING("Required playing group is not opened.");
    return false;
  }

  // 检查副本前置是否完成
  if (cell->depend_ != 0 && this->CheckPlayingFinished(cell->depend_) == false) {
    LOG_WARNING("depend playing is not finished.");
    return false;
  }

  return true;
}

void Guild::OpenPlayingGroup(core::int32 playing_group_id, bool synchronize) {
  // 检查副本组是否存在
  if (GUILD_CONF()->GetGuildPlayingGroup(playing_group_id) == NULL) {
    LOG_ERROR("playing_group_id(%d) is invalid.", playing_group_id);
    return;
  }

  this->opened_playing_groups_.insert(playing_group_id);

  if (synchronize) {
    gateway::protocol::MessageGuildPlayingGroupOpenNotify notify;
    notify.__set_playing_group_id_(playing_group_id);
    this->BroadcastMessage(notify,
        gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_GROUP_OPEN_NOTIFY);
  }
}

void Guild::SetPlayingFinished(core::int32 playing_template_id, bool synchronize) {
  // 检查副本是否存在
  if (GUILD_CONF()->GetGuildPlaying(playing_template_id) == NULL) {
    LOG_ERROR("playing_template_id(%d) is invalid.", playing_template_id);
    return;
  }

  this->finished_playings_.insert(playing_template_id);

  if (synchronize) {
    gateway::protocol::MessageGuildPlayingFinishedNotify notify;
    notify.__set_playing_template_id_(playing_template_id);
    this->BroadcastMessage(notify,
        gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_FINISHED_NOTIFY);
  }
}

void Guild::SetCurrentPlaying(core::uint64 playing_id, core::int32 playing_template_id,
    bool synchronize) {
  if (this->current_playing_id_ != 0) {
    // Unfollow 副本结束事件
    coresh::CommunicatorSingleton::GetInstance().Unfollow(this->playing_end_conn_);
    // Unfollow 副本销毁事件
    coresh::CommunicatorSingleton::GetInstance().Unfollow(this->playing_destroy_conn_);
  }

  this->current_playing_id_ = playing_id;
  this->current_playing_template_id_ = playing_template_id;

  if (this->current_playing_id_ != 0) {
    // Follow 副本结束事件
    this->playing_end_conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_PLAYING_END, this->current_playing_id_,
        event::ChannelType::CHANNEL_PLAYING,
        boost::bind(&Guild::OnEventPlayingEnd, this, _1, _2, _3, _4));
    // Follow 副本销毁事件
    this->playing_destroy_conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_PLAYING_DESTROY, this->current_playing_id_,
        event::ChannelType::CHANNEL_PLAYING,
        boost::bind(&Guild::OnEventPlayingDestroy, this, _1, _2, _3, _4));
  }

  if (synchronize) {
    gateway::protocol::MessageGuildSynchronize sync;
    sync.__set_current_playing_id_(global::ToString(this->current_playing_id_));
    sync.__set_current_playing_template_id_(this->current_playing_template_id_);
    this->BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_SYNCHRONIZE);
  }
}

void Guild::OnEventPlayingEnd(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventPlayingEnd) ||
      channel_type != event::ChannelType::CHANNEL_PLAYING) {
    LOG_ERROR("Invalid params");
    return;
  }

  event::EventPlayingEnd *event = (event::EventPlayingEnd *)message;

  if ((core::uint64)event->playing_id_ == this->current_playing_id_) {
    this->SetPlayingFinished(event->playing_template_id_);
  }
}

void Guild::OnEventPlayingDestroy(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventPlayingDestroy) ||
      channel_type != event::ChannelType::CHANNEL_PLAYING) {
    LOG_ERROR("Invalid params");
    return;
  }

  event::EventPlayingDestroy *event = (event::EventPlayingDestroy *)message;

  if ((core::uint64)event->playing_id_ == this->current_playing_id_) {
    this->SetCurrentPlaying(0, 0);
  }
}

void Guild::ResetPlaying(bool synchronize) {
  this->opened_playing_groups_.clear();
  this->finished_playings_.clear();
  this->SetCurrentPlaying(0, 0, false);

  if (synchronize) {
    gateway::protocol::MessageGuildPlayingResetNotify notify;
    this->BroadcastMessage(notify,
        gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_RESET_NOTIFY);
  }
}

void Guild::DailyClean() {
  const global::configure::GuildCell *cell = GUILD_CONF()->GetGuild(this->level_);
  if (NULL == cell) {
    LOG_ERROR("GuildCell(%d) not found in config.", this->level_);
    return;
  }

  this->SetGold(this->gold_ - cell->maintenance_fee_);
}

void Guild::WeeklyClean() {
  this->ResetPlaying();
}

void Guild::ClearGuildBuff() {
  for (GuildActorSet::iterator iter = this->members_.begin();
       iter != this->members_.end(); ++iter) {
    GuildActor *guild_actor = *iter;
    if (guild_actor != NULL) {
      guild_actor->ClearGuildBuff();
    }
  }
}

}  // namespace guild

}  // namespace server

}  // namespace session

