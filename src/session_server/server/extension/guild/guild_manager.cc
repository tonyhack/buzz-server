#include "session_server/server/extension/guild/guild_manager.h"

#include <cstddef>
#include <boost/bind.hpp>

#include "entity/guild_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/actor_id.h"
#include "global/guild_id.h"
#include "global/configure/configure.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_guild_del_types.h"
#include "database_server/protocol/storage_guild_save_types.h"
#include "database_server/protocol/storage_guild_actor_login_constants.h"
#include "database_server/protocol/storage_guild_actor_login_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_guild_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_guild_protocol_types.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/sync_database_operation_manager.h"
#include "session_server/server/extension/guild/guild.h"
#include "session_server/server/extension/guild/guild_pool.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"
#include "session_server/server/extension/guild/guild_actor_pool.h"

namespace session {

namespace server {

namespace guild {

void GuildManager::AddClearGuildBuffTimer() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  time_t zero_time = GlobalTimeTick::GetInstance()->GetZeroTime();
  time_t next_time;

  if (now < zero_time + MISC_CONF()->guild_clear_buff_time_) {
    next_time = zero_time + MISC_CONF()->guild_clear_buff_time_;
  } else {
    next_time = zero_time + 24 * 60 * 60 + MISC_CONF()->guild_clear_buff_time_;
  }

  SessionServerSingleton::GetInstance().AddTimer(TIMER_ID_CLEAR_GUILD_BUFF, 
      (next_time - now) * 1000, -1, this, "GuildManager::AddClearGuildBuffTimer");
}

void GuildManager::RemoveClearGuildBuffTimer() {
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_CLEAR_GUILD_BUFF, this);
}

void GuildManager::OnTimer(core::uint32 id) {
  if (TIMER_ID_CLEAR_GUILD_BUFF == id) {
    this->ClearGuildBuff();
    this->RemoveClearGuildBuffTimer();
    this->AddClearGuildBuffTimer();
  }
}

bool GuildManager::Initialize() {
  SyncDatabaseOperationManager::GetInstance()->AddLoader(
      boost::bind(&GuildManager::LoadGuilds, this, _1));
  SyncDatabaseOperationManager::GetInstance()->AddSaver(
      boost::bind(&GuildManager::SaveGuilds, this, _1));
  this->max_guild_id_offset_ = 0;

  this->AddClearGuildBuffTimer();

  return true;
}

void GuildManager::Finalize() {
  this->RemoveClearGuildBuffTimer();

  this->max_guild_id_offset_ = 0;
  this->ids_.clear();

  for (GuildMap::iterator iter = this->guilds_.begin();
       iter != this->guilds_.end(); ++iter) {
    iter->second->Finalize();
    GuildPool::GetInstance()->Deallocate(iter->second);
  }

  this->guilds_.clear();
}

Guild *GuildManager::GetGuild(core::uint64 id) {
  GuildMap::iterator iter = this->guilds_.find(id);
  if (iter != this->guilds_.end()) {
    return iter->second;
  }

  return NULL;
}

Guild *GuildManager::GetGuild(const std::string &name) {
  GuildIDHashmap::iterator iter = this->ids_.find(name);
  if (iter != this->ids_.end()) {
    return this->GetGuild(iter->second);
  }

  return NULL;
}

bool GuildManager::CreateGuild(const std::string &guild_name, core::int32 guild_logo,
    GuildActor *leader, bool synchronize) {
  // 检查是否已加入公会
  if (leader->GetGuild() != NULL) {
    LOG_ERROR("already in a guild.");
    return false;
  }

  // 检查公会名是否重复
  if (this->GetGuild(guild_name) != NULL) {
    LOG_ERROR("Guild name already exists.");
    return false;
  }

  // 删除 leader 的所有申请请求
  leader->RemoveAllJoinedGuild();

  // 分配公会ID
  global::ActorID decoded_actor_id(leader->GetActorID());
  global::GuildID decoded_guild_id(decoded_actor_id.zone_, ++this->max_guild_id_offset_);
  core::uint64 guild_id = decoded_guild_id.GetID();

  // 分配公会对象
  Guild *guild = GuildPool::GetInstance()->Allocate();
  if (NULL == guild) {
    LOG_ERROR("Allocate Guild(%lu) from GuildPool failed.", guild_id);
    return false;
  }

  // 初始化
  if (guild->Initialize(guild_id, guild_name, guild_logo, leader) == false) {
    GuildPool::GetInstance()->Deallocate(guild);
    LOG_ERROR("Init Guild(%lu) failed.", guild_id);
    return false;
  }

  // 加入管理器
  if (this->AddGuild(guild) == false) {
    leader->SetGuild(NULL, entity::GuildPositionType::MEMBER);
    guild->Finalize();
    GuildPool::GetInstance()->Deallocate(guild);
    LOG_ERROR("Add Guild(%lu) to GuildManager failed.", guild_id);
    return false;
  }

  // 立即保存公会
  guild->Save();
  leader->Save();

  // 同步 game_server
  session::protocol::MessageGuildInfoSynchronize game_sync;
  game_sync.__set_type_(session::protocol::GuildOperateType::ADD);
  game_sync.data_.__set_id_(guild->GetID());
  game_sync.data_.__set_name_(guild->GetName());
  game_sync.data_.__set_level_(guild->GetLevel());
  game_sync.data_.__set_light_attribute_(guild->GetLightAttribute());
  game_sync.data_.__set_dark_attribute_(guild->GetDarkAttribute());
  game_sync.data_.__set_holy_attribute_(guild->GetHolyAttribute());
  game_sync.data_.__set_shop_level_(guild->GetShopLevel());
  game_sync.data_.__set_college_level_(guild->GetCollegeLevel());
  game_sync.data_.__set_barrack_level_(guild->GetBarrackLevel());
  SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageGuildListSynchronize sync;
    sync.__set_type_(gateway::protocol::GuildListOperateType::ADD);
    sync.data_.__set_guild_id_(guild->GetStringID());
    sync.data_.__set_guild_name_(guild->GetName());
    sync.data_.__set_guild_level_(guild->GetLevel());
    sync.data_.__set_member_num_(guild->GetMemberNum());
    sync.data_.__set_leader_actor_id_(leader->GetStringID());
    sync.data_.__set_leader_actor_name_(leader->GetName());
    sync.data_.__set_leader_online_(leader->CheckOnline());
    sync.data_.__set_suppress_join_(guild->CheckSuppressJoin());
    sync.data_.__set_joined_(false);
    SessionServerSingleton::GetInstance().BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_LIST_SYNCHRONIZE);
  }

  return true;
}

void GuildManager::DismissGuild(core::uint64 id, bool synchronize) {
  LOG_INFO("DismissGuild(%lu)", id);

  Guild *guild = this->RemoveGuild(id);
  if (NULL == guild) {
    LOG_ERROR("Remove Guild(%lu) from GuildManager failed.", id);
    return;
  }

  guild->Dismiss();
  guild->Finalize();
  GuildPool::GetInstance()->Deallocate(guild);

  // 同步 game_server
  session::protocol::MessageGuildInfoSynchronize game_sync;
  game_sync.__set_type_(session::protocol::GuildOperateType::REMOVE);
  game_sync.data_.__set_id_(id);
  SessionServerSingleton::GetInstance().BroadcastGameMessage(game_sync,
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);

  // 同步 client
  if (synchronize) {
    char string_id[32];

    gateway::protocol::MessageGuildListSynchronize sync;
    sync.__set_type_(gateway::protocol::GuildListOperateType::REMOVE);
    snprintf(string_id, sizeof(string_id), "%lu", id);
    sync.data_.__set_guild_id_(string_id);
    SessionServerSingleton::GetInstance().BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_LIST_SYNCHRONIZE);
  }

  // 发送删除公会数据库请求
  database::protocol::StorageGuildDelRequest request;
  request.__set_guild_id_(id);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_GUILD_DEL, id) == false) {
    LOG_ERROR("Send StorageGuildDelRequest to database server failed.");
    return;
  }
}

bool GuildManager::AddGuild(Guild *guild) {
  if (NULL == guild) {
    return false;
  }
  if (this->guilds_.find(guild->GetID()) != this->guilds_.end()) {
    return false;
  }
  if (this->ids_.find(guild->GetName()) != this->ids_.end()) {
    return false;
  }

  this->guilds_.insert(std::make_pair(guild->GetID(), guild));
  this->ids_.insert(std::make_pair(guild->GetName(), guild->GetID()));

  return true;
}

Guild *GuildManager::RemoveGuild(core::uint64 id) {
  GuildMap::iterator iter1 = this->guilds_.find(id);
  if (iter1 == this->guilds_.end()) {
    return NULL;
  }
  Guild *guild = iter1->second;
  if (NULL == guild) {
    LOG_ERROR("Guild is null.");
    return NULL;
  }
  this->guilds_.erase(iter1);

  GuildIDHashmap::iterator iter2 = this->ids_.find(guild->GetName());
  if (iter2 != this->ids_.end()) {
    this->ids_.erase(iter2);
  } else {
    LOG_ERROR("Guild(%s) is not in id table.", guild->GetName().c_str());
  }

  return guild;
}

void GuildManager::DailyClean() {
  for (GuildMap::iterator iter = this->guilds_.begin();
       iter != this->guilds_.end();) {
    GuildMap::iterator iter2 = iter;
    ++iter2;

    Guild *guild = iter->second;
    if (guild != NULL) {
      if (guild->CheckAutoDismiss()) {
        this->DismissGuild(guild->GetID());
      } else {
        guild->DailyClean();
      }
    }

    iter = iter2;
  }
}

void GuildManager::WeeklyClean() {
  for (GuildMap::iterator iter = this->guilds_.begin();
       iter != this->guilds_.end();) {
    Guild *guild = iter->second;
    if (guild != NULL) {
      guild->WeeklyClean();
    }
  }
}

void GuildManager::ClearGuildBuff() {
  for (GuildMap::iterator iter = this->guilds_.begin();
       iter != this->guilds_.end(); ++iter) {
    Guild *guild = iter->second;
    if (guild != NULL) {
      guild->ClearGuildBuff();
    }
  }
}

static GuildActor *LoadGuildActor(mysqlpp::Connection &connection, core::uint64 actor_id) {
  // 检查缓存, 已存在直接返回
  GuildActor *guild_actor = GuildActorManager::GetInstance()->GetFromCache(actor_id);
  if (guild_actor != NULL) {
    return guild_actor;
  }

  mysqlpp::Query query(&connection, false);
  database::protocol::StorageGuildActorField db_field;

  query << "CALL GAME_GUILD_ACTOR_LOGIN(" << actor_id << ")";
  mysqlpp::StoreQueryResult result = query.store();

  if (query.errnum() > 0) {
    LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
              query.str().c_str(), query.error());
    return NULL;
  }
  while (query.more_results()) {
    query.store_next();
  }

  if (result.num_rows() != 1) {
    LOG_ERROR("Query rows is not 1.");
    return NULL;
  }

  for (size_t pos = 0; pos < database::protocol::GuildActorField::MAX; ++pos) {
    int index = result.field_num(
        database::protocol::g_storage_guild_actor_login_constants.kGuildActorFieldStr[pos]);
    if (-1 == index) {
      LOG_ERROR("Get result [%s] failed.",
          database::protocol::g_storage_guild_actor_login_constants.kGuildActorFieldStr[pos].c_str());
      return NULL;
    }

    switch (pos) {
      case database::protocol::GuildActorField::GUILD_ID:
        db_field.__set_guild_id_(atol(result[0][index].data()));
        break;
      case database::protocol::GuildActorField::GUILD_POSITION:
        db_field.__set_guild_position_(atoi(result[0][index].data()));
        break;
      case database::protocol::GuildActorField::CURRENT_CONTRIBUTION_VALUE:
        db_field.__set_current_contribution_value_(atoi(result[0][index].data()));
        break;
      case database::protocol::GuildActorField::TOTAL_CONTRIBUTION_VALUE:
        db_field.__set_total_contribution_value_(atoi(result[0][index].data()));
        break;
      case database::protocol::GuildActorField::GOLD_CONTRIBUTION:
        db_field.__set_gold_contribution_(atoi(result[0][index].data()));
        break;
      case database::protocol::GuildActorField::FREEDOM_DOLLARS_CONTRIBUTION:
        db_field.__set_freedom_dollars_contribution_(atoi(result[0][index].data()));
        break;
      case database::protocol::GuildActorField::LIGHT_CRYSTAL_CONTRIBUTION:
        db_field.__set_light_crystal_contribution_(atoi(result[0][index].data()));
        break;
      case database::protocol::GuildActorField::DARK_CRYSTAL_CONTRIBUTION:
        db_field.__set_dark_crystal_contribution_(atoi(result[0][index].data()));
        break;
      case database::protocol::GuildActorField::HOLY_CRYSTAL_CONTRIBUTION:
        db_field.__set_holy_crystal_contribution_(atoi(result[0][index].data()));
        break;
      case database::protocol::GuildActorField::GUILD_SKILLS:
        db_field.__set_guild_skills_(result[0][index].data());
        break;
      case database::protocol::GuildActorField::GUILD_BUFFS:
        db_field.__set_guild_buffs_(result[0][index].data());
        break;
      case database::protocol::GuildActorField::AWARDED_GUILD_PLAYINGS:
        db_field.__set_awarded_guild_playings_(result[0][index].data());
        break;
      default:
        break;
    }
  }

  BriefActor *brief_actor =
    SessionActorManager::GetInstance()->GetBriefActor(actor_id);
  if (NULL == brief_actor) {
    LOG_ERROR("Get BriefActor(%lu) from SessionActorManager failed.", actor_id);
    return NULL;
  }

  // 分配公会玩家对象
  guild_actor = GuildActorPool::GetInstance()->Allocate();
  if (NULL == guild_actor) {
    LOG_ERROR("Allocate GuildActor(%lu) from GuildActorPool failed.", actor_id);
    return NULL;
  }

  // 初始化
  if (guild_actor->Initialize(brief_actor, db_field) == false) {
    GuildActorPool::GetInstance()->Deallocate(guild_actor);
    LOG_ERROR("Init GuildActor(%lu) failed.", actor_id);
    return NULL;
  }

  // 放入缓存中
  if (GuildActorManager::GetInstance()->AddToCache(guild_actor) == false) {
    guild_actor->Finalize();
    GuildActorPool::GetInstance()->Deallocate(guild_actor);
    LOG_ERROR("Add GuildActor(%lu) to GuildActorManager failed.", actor_id);
    return NULL;
  }

  return guild_actor;
}

bool GuildManager::LoadGuildActors(mysqlpp::Connection &connection, Guild *guild,
    const GuildActorVector &pending_members, const GuildActorVector &members) {
  for (size_t i = 0; i < pending_members.size(); ++i) {
    GuildActor *guild_actor = LoadGuildActor(connection, pending_members[i]);
    if (NULL == guild_actor) {
      LOG_ERROR("Load GuildActor(%lu) failed.", pending_members[i]);
      return false;
    }

    if (guild_actor->AddJoinedGuild(guild, false) == false) {
      LOG_ERROR("GuildActor(%lu) AddJoinedGuild failed." ,pending_members[i]);
      return false;
    }
  }

  for (size_t i = 0; i < members.size(); ++i) {
    GuildActor *guild_actor = LoadGuildActor(connection, members[i]);
    if (NULL == guild_actor) {
      LOG_ERROR("Load GuildActor(%lu) failed.", members[i]);
      return false;
    }
    if (guild->AddMemberDirect(guild_actor) == false) {
      LOG_ERROR("AddMember(%lu) failed.", members[i]);
      return false;
    }
  }

  return true;
}

bool GuildManager::LoadGuilds(mysqlpp::Connection &connection) {
  mysqlpp::Query query(&connection, false);

  query << "CALL GAME_GUILD_LOAD()";
  mysqlpp::StoreQueryResult result = query.store();

  if (query.errnum() > 0) {
    LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
              query.str().c_str(), query.error());
    return false;
  }
  while (query.more_results()) {
    query.store_next();
  }
  query.reset();

  const char *fields[] = {
    "id",
    "name",
    "logo",
    "level",
    "suppress_join",
    "pending_members",
    "members",
    "announcement",
    "events",
    "gold",
    "light_crystal",
    "dark_crystal",
    "holy_crystal",
    "light_attribute",
    "dark_attribute",
    "holy_attribute",
    "shop_level",
    "college_level",
    "barrack_level",
    "opened_playing_groups",
    "finished_playings",
    "last_save_time"
  };

  for (size_t i = 0; i < sizeof(fields) / sizeof(char *); ++i) {
    if ((size_t)result.field_num(fields[i]) != i) { 
      LOG_ERROR("Database field order is incorrect.");
      return false;
    }
  }

  for (size_t row = 0; row < result.num_rows(); ++row) {
    database::protocol::StorageGuildField db_field;
    int idx = 0;
    db_field.__set_id_(atol(result[row][idx++].data()));
    db_field.__set_name_(result[row][idx++].data());
    db_field.__set_logo_(atoi(result[row][idx++].data()));
    db_field.__set_level_(atoi(result[row][idx++].data()));
    db_field.__set_suppress_join_(atoi(result[row][idx++].data()));
    db_field.__set_pending_members_(result[row][idx++].data());
    db_field.__set_members_(result[row][idx++].data());
    db_field.__set_announcement_(result[row][idx++].data());
    db_field.__set_events_(result[row][idx++].data());
    db_field.__set_gold_(atoi(result[row][idx++].data()));
    db_field.__set_light_crystal_(atoi(result[row][idx++].data()));
    db_field.__set_dark_crystal_(atoi(result[row][idx++].data()));
    db_field.__set_holy_crystal_(atoi(result[row][idx++].data()));
    db_field.__set_light_attribute_(atoi(result[row][idx++].data()));
    db_field.__set_dark_attribute_(atoi(result[row][idx++].data()));
    db_field.__set_holy_attribute_(atoi(result[row][idx++].data()));
    db_field.__set_shop_level_(atoi(result[row][idx++].data()));
    db_field.__set_college_level_(atoi(result[row][idx++].data()));
    db_field.__set_barrack_level_(atoi(result[row][idx++].data()));
    db_field.__set_opened_playing_groups_(result[row][idx++].data());
    db_field.__set_finished_playings_(result[row][idx++].data());
    db_field.__set_last_save_time_(atol(result[row][idx++].data()));

    // 分配公会对象
    Guild *guild = GuildPool::GetInstance()->Allocate();
    if (NULL == guild) {
      LOG_ERROR("Allocate Guild(%lu) from GuildPool failed.", db_field.id_);
      return false;
    }

    // 初始化
    if (guild->Initialize(db_field) == false) {
      GuildPool::GetInstance()->Deallocate(guild);
      LOG_ERROR("Init Guild(%lu) failed.", db_field.id_);
      return false;
    }

    // 加入管理器
    if (this->AddGuild(guild) == false) {
      guild->Finalize();
      GuildPool::GetInstance()->Deallocate(guild);
      LOG_ERROR("Add Guild(%lu) to GuildManager failed.", db_field.id_);
      return false;
    }

    // 读入公会成员
    GuildActorVector pending_members;
    GuildActorVector members;
    std::vector<std::string> split_result;

    global::StringSplit(db_field.pending_members_.c_str(), ",", split_result);
    for (size_t i = 0; i < split_result.size(); ++i) {
      if (split_result[i].empty()) {
        continue;
      }
      core::uint64 actor_id = atol(split_result[i].c_str());
      pending_members.push_back(actor_id);
    }

    split_result.clear();
    global::StringSplit(db_field.members_.c_str(), ",", split_result);
    for (size_t i = 0; i < split_result.size(); ++i) {
      if (split_result[i].empty()) {
        continue;
      }
      core::uint64 actor_id = atol(split_result[i].c_str());
      members.push_back(actor_id);
    }

    if (this->LoadGuildActors(connection, guild, pending_members, members) == false) {
      this->RemoveGuild(guild->GetID());
      guild->Finalize();
      GuildPool::GetInstance()->Deallocate(guild);
      LOG_ERROR("Load guild actors for Guild(%lu) failed.", db_field.id_);
      return false;
    }

    // 校验公会数据正确性
    if (guild->CheckDataValid() == false) {
      this->RemoveGuild(guild->GetID());
      guild->Finalize();
      GuildPool::GetInstance()->Deallocate(guild);
      LOG_ERROR("Guild(%lu) data is invalid.", db_field.id_);
      return false;
    }

    // 设置 max_offset
    global::GuildID decoded_guild_id(db_field.id_);
    if (this->max_guild_id_offset_ < decoded_guild_id.offset_) {
      this->max_guild_id_offset_ = decoded_guild_id.offset_;
    }

    time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
    time_t zero_time = GlobalTimeTick::GetInstance()->GetZeroTime();

    // 跨天清理
    if (zero_time >= guild->GetLastSaveTime()) {
      LOG_INFO("Startup daily clean for Guild(%lu).", guild->GetID());

      if (guild->CheckAutoDismiss()) {
        this->DismissGuild(guild->GetID());
        continue;
      } else {
        guild->DailyClean();
      }
    }
    // 跨周清理
    time_t this_week_clean_time = GlobalTimeTick::GetInstance()->GetWeekTime(1, 0, 0, 0, now);
    if (guild->GetLastSaveTime() < this_week_clean_time &&
        now >= this_week_clean_time) {
      LOG_INFO("Startup weekly clean for Guild(%lu).", guild->GetID());
      guild->WeeklyClean();
    }

    // 清理公会BUFF
    time_t last_clear_buff_time =
      zero_time + MISC_CONF()->guild_clear_buff_time_ - 24 * 60 * 60;
    if (last_clear_buff_time >= guild->GetLastSaveTime()) {
      LOG_INFO("Startup clear buff for Guild(%lu).", guild->GetID());
      guild->ClearGuildBuff();
    }
  }

  return true;
}

bool GuildManager::SaveGuilds(mysqlpp::Connection &connection) {
  GuildMap::iterator iter = this->guilds_.begin();
  for (; iter != this->guilds_.end(); ++iter) {
    Guild *guild = iter->second;
    if (NULL == guild) {
      LOG_ERROR("guild is null.");
      continue;
    }

    database::protocol::StorageGuildField db_field;
    guild->Save(db_field);

    mysqlpp::Query query(&connection, false);

    // Escape string.
    query.escape_string(&db_field.name_);
    query.escape_string(&db_field.announcement_);

    query << "CALL GAME_GUILD_SAVE("
          << db_field.id_ << ", "
          << "'" << db_field.name_ << "', "
          << db_field.logo_ << ", "
          << db_field.level_ << ", "
          << db_field.suppress_join_ << ", "
          << "'" << db_field.pending_members_ << "', "
          << "'" << db_field.members_ << "', "
          << "'" << db_field.announcement_ << "', "
          << "'" << db_field.events_ << "', "
          << db_field.gold_ << ", "
          << db_field.light_crystal_ << ", "
          << db_field.dark_crystal_ << ", "
          << db_field.holy_crystal_ << ", "
          << db_field.light_attribute_ << ", "
          << db_field.dark_attribute_ << ", "
          << db_field.holy_attribute_ << ", "
          << db_field.shop_level_ << ", "
          << db_field.college_level_ << ", "
          << db_field.barrack_level_ << ", "
          << "'" << db_field.opened_playing_groups_ << "', "
          << "'" << db_field.finished_playings_ << "', "
          << db_field.last_save_time_
          << ")";

    mysqlpp::StoreQueryResult result = query.store();

    if (query.errnum() > 0) {
      LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
                query.str().c_str(), query.error());
      return false;
    }
    while (query.more_results()) {
      query.store_next();
    }
    query.reset();
  }
  
  return true;
}

}  // namespace guild

}  // namespace server

}  // namespace session

