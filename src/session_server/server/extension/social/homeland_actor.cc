#include "session_server/server/extension/social/homeland_actor.h"

#include <cstddef>
#include <cstdio>
#include <boost/bind.hpp>

#include "entity/functionality_types.h"
#include "entity/homeland_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "database_server/protocol/storage_social_actor_login_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_social_event_types.h"
#include "session_server/server/extension/social/facade_request.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"
#include "session_server/server/extension/social/homeland_battle_manager.h"

namespace session {

namespace server {

namespace social {

HomelandActor::HomelandActor() : social_actor_(NULL),
    upgrade_start_time_(0), upgrade_finish_time_(0),
    temple_level_(0), temple_harvest_times_(0),
    temple_force_harvest_times_(0), temple_next_harvest_time_(0),
    goldmine_level_(0), goldmine_robbed_times_(0),
    goldmine_loss_rate_(0), goldmine_next_harvest_time_(0),
    goldmine_rob_times_(0), goldmine_next_rob_time_(0),
    pool_level_(0), pool_last_harvest_time_(0),
    church_free_pray_times_(0), church_paid_pray_times_(0),
    church_free_refresh_times_(0), church_chest_type_(0),
    tree_level_(0), tree_harvest_times_(0), tree_stage_(0),
    tree_watered_times_(0), tree_next_water_time_(0),
    tree_growing_finish_time_(0), tree_assist_water_times_(0),
    tower1_level_(0), tower2_level_(0),
    stronghold_level_(0), stronghold_next_call_support_time_(0),
    stronghold_support_times_(0) {}

HomelandActor::~HomelandActor() {}

static void DecodeActorIDSet(const std::string &source, HomelandActor::ActorIDSet *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }   

    core::uint64 actor_id = atol(split_result[i].c_str());
    if (NULL == SessionActorManager::GetInstance()->GetBriefActor(actor_id)) {
      LOG_ERROR("actor_id(%lu) does not exist.", actor_id);
      continue;
    }   
    result->insert(actor_id);
  }
}

static void EncodeActorIDSet(const HomelandActor::ActorIDSet &source, std::string *result) {
  char buffer[2048] = ""; 
  size_t count = 0;

  HomelandActor::ActorIDSet::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    count += snprintf(buffer + count, sizeof(buffer) - count, "%lu,", *iter);
  }

  *result = buffer;
}

static void DecodeHomelandSupportedSoul(const std::string &source,
    HomelandActor::HomelandSupportedSoulMap *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }   

    HomelandSupportedSoul data;
    if (data.Initialize(split_result[i]) == false) {
      LOG_ERROR("HomelandSupportedSoul(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    result->insert(std::make_pair(data.GetActorID(), data));
  }
}

static void EncodeHomelandSupportedSoul(const HomelandActor::HomelandSupportedSoulMap &source,
    std::string *result) {
  char buffer[1024] = "";
  size_t count = 0;

  HomelandActor::HomelandSupportedSoulMap::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    const HomelandSupportedSoul &data = iter->second;
    count += data.Serialize(buffer + count, sizeof(buffer) - count);
  }

  *result = buffer;
}

static void DecodeHomelandSupportSoul(const std::string &source,
    HomelandActor::HomelandSupportSoulMap *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    HomelandSupportSoul data;
    if (data.Initialize(split_result[i]) == false) {
      LOG_ERROR("HomelandSupportSoul(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    result->insert(std::make_pair(data.GetSoulID(), data));
  }
}

static void EncodeHomelandSupportSoul(const HomelandActor::HomelandSupportSoulMap &source,
    std::string *result) {
  char buffer[1024] = "";
  size_t count = 0;

  HomelandActor::HomelandSupportSoulMap::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    const HomelandSupportSoul &data = iter->second;
    count += data.Serialize(buffer + count, sizeof(buffer) - count);
  }

  *result = buffer;
}

static void DecodeHomelandInvader(const std::string &source,
    HomelandActor::HomelandInvaderMap *result, HomelandActor::HomelandInvaderSet *index) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    HomelandInvader data;
    if (data.Initialize(split_result[i]) == false) {
      LOG_ERROR("HomelandInvader(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    result->insert(std::make_pair(data.GetActorID(), data));
    index->insert(&(*result)[data.GetActorID()]);
  }
}

static void EncodeHomelandInvader(const HomelandActor::HomelandInvaderMap &source,
    std::string *result) {
  char buffer[2048] = "";
  size_t count = 0;

  HomelandActor::HomelandInvaderMap::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    const HomelandInvader &data = iter->second;
    count += data.Serialize(buffer + count, sizeof(buffer) - count);
  }

  *result = buffer;
}

static void DecodeHomelandEvent(const std::string &source,
    HomelandActor::HomelandEventDeque *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    HomelandEvent event;
    if (event.Initialize(split_result[i]) == false) {
      LOG_ERROR("HomelandEvent(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    result->push_back(event);
  }
}

static void EncodeHomelandEvent(const HomelandActor::HomelandEventDeque &source,
    std::string *result) {
  char buffer[4096] = "";
  size_t count = 0;

  HomelandActor::HomelandEventDeque::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    const HomelandEvent &event = *iter;

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

static void DecodeSoulData(const std::string &source, HomelandActor::SoulLevelMap *result) {
  std::vector<std::string> split_result;
  global::StringSplit(source.c_str(), ",", split_result);

  for (size_t i = 0; i < split_result.size(); ++i) {
    if (split_result[i].empty()) {
      continue;
    }

    std::vector<std::string> split_result2;
    global::StringSplit(split_result[i].c_str(), ":", split_result2);

    if (split_result2.size() < 2) {
      LOG_ERROR("SoulData(%s) is invalid.", split_result[i].c_str());
      continue;
    }

    core::uint32 soul_id = atol(split_result2[0].c_str());
    core::int32 soul_level = atoi(split_result2[1].c_str());

    if (SOCIAL_CONF()->GetHomelandSoul(soul_id) == NULL) {
      continue;
    }
    result->insert(std::make_pair(soul_id, soul_level));
  }
}

///////////////////////////////////////////////////////////////////////////////

bool HomelandActor::Initialize(SocialActor *social_actor,
    const database::protocol::StorageSocialActorField &db_field) {
  if (NULL == social_actor) {
    LOG_ERROR("SocialActor is null.");
    return false;
  }

  this->social_actor_ = social_actor;
  
  this->upgrade_start_time_ = db_field.homeland_upgrade_start_time_;
  this->upgrade_finish_time_ = db_field.homeland_upgrade_finish_time_;
  this->temple_level_ = db_field.homeland_temple_level_;
  this->temple_harvest_times_ = db_field.homeland_temple_harvest_times_;
  this->temple_force_harvest_times_ = db_field.homeland_temple_force_harvest_times_;
  this->temple_next_harvest_time_ = db_field.homeland_temple_next_harvest_time_;
  this->goldmine_level_ = db_field.homeland_goldmine_level_;
  this->goldmine_robbed_times_ = db_field.homeland_goldmine_robbed_times_;
  this->goldmine_loss_rate_ = db_field.homeland_goldmine_loss_rate_;
  this->goldmine_next_harvest_time_ = db_field.homeland_goldmine_next_harvest_time_;
  this->goldmine_rob_times_ = db_field.homeland_goldmine_rob_times_;
  this->goldmine_next_rob_time_ = db_field.homeland_goldmine_next_rob_time_;
  this->pool_level_ = db_field.homeland_pool_level_;
  this->pool_last_harvest_time_ = db_field.homeland_pool_last_harvest_time_;
  this->church_free_pray_times_ = db_field.homeland_church_free_pray_times_;
  this->church_paid_pray_times_ = db_field.homeland_church_paid_pray_times_;
  this->church_free_refresh_times_ = db_field.homeland_church_free_refresh_times_;
  this->church_chest_type_ = db_field.homeland_church_chest_type_;
  this->tree_level_ = db_field.homeland_tree_level_;
  this->tree_harvest_times_ = db_field.homeland_tree_harvest_times_;
  this->tree_stage_ = db_field.homeland_tree_stage_;
  this->tree_watered_times_ = db_field.homeland_tree_watered_times_;
  this->tree_next_water_time_ = db_field.homeland_tree_next_water_time_;
  this->tree_growing_finish_time_ = db_field.homeland_tree_growing_finish_time_;
  this->tree_assist_water_times_ = db_field.homeland_tree_assist_water_times_;
  DecodeActorIDSet(db_field.homeland_tree_watered_actors_, &this->tree_watered_actors_);
  DecodeActorIDSet(db_field.homeland_tree_stolen_actors_, &this->tree_stolen_actors_);
  this->tower1_level_ = db_field.homeland_tower1_level_;
  this->tower2_level_ = db_field.homeland_tower2_level_;
  this->stronghold_level_ = db_field.homeland_stronghold_level_;
  this->stronghold_next_call_support_time_ = db_field.homeland_stronghold_next_call_support_time_;
  DecodeHomelandSupportedSoul(db_field.homeland_stronghold_supported_souls_,
      &this->stronghold_supported_souls_);
  DecodeHomelandSupportSoul(db_field.homeland_stronghold_support_souls_,
      &this->stronghold_support_souls_);
  this->stronghold_support_times_ = db_field.homeland_stronghold_support_times_;
  DecodeHomelandInvader(db_field.homeland_invaders_,
      &this->invaders_, &this->invaders_time_index_);
  DecodeHomelandEvent(db_field.homeland_events_, &this->events_);
  DecodeSoulData(db_field.soul_data_, &this->souls_);

  // 成长阶段处理
  if (entity::HomelandTreeStageType::GROWING == this->tree_stage_) {
    core::int32 tree_grow_cooling = this->GetTreeGrowingCooling();
    if (0 == tree_grow_cooling) {
      this->MoveTreeToHarvestStage(false);
    } else {
      this->MoveTreeToGrowingStage(tree_grow_cooling, false);
    }
  }

  return true;
}

void HomelandActor::Finalize() {
  // 移除定时器
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_GROWING_FINISH, this);

  this->souls_.clear();
  this->invader_event_conns_.clear();
  this->invaders_time_index_.clear();
  this->invaders_.clear();
  this->friend_event_conns_.clear();
  this->events_.clear();
  this->stronghold_support_times_ = 0;
  this->stronghold_support_souls_.clear();
  this->stronghold_supported_souls_.clear();
  this->stronghold_next_call_support_time_ = 0;
  this->stronghold_level_ = 0;
  this->tower2_level_ = 0;
  this->tower1_level_ = 0;
  this->tree_stolen_actors_.clear();
  this->tree_watered_actors_.clear();
  this->tree_assist_water_times_ = 0;
  this->tree_growing_finish_time_ = 0;
  this->tree_next_water_time_ = 0;
  this->tree_watered_times_ = 0;
  this->tree_stage_ = 0;
  this->tree_harvest_times_ = 0;
  this->tree_level_ = 0;
  this->church_chest_type_ = 0;
  this->church_free_refresh_times_ = 0;
  this->church_paid_pray_times_ = 0;
  this->church_free_pray_times_ = 0;
  this->pool_last_harvest_time_ = 0;
  this->pool_level_ = 0;
  this->goldmine_next_rob_time_ = 0;
  this->goldmine_rob_times_ = 0;
  this->goldmine_next_harvest_time_ = 0;
  this->goldmine_loss_rate_ = 0;
  this->goldmine_robbed_times_ = 0;
  this->goldmine_level_ = 0;
  this->temple_next_harvest_time_ = 0;
  this->temple_force_harvest_times_ = 0;
  this->temple_harvest_times_ = 0;
  this->temple_level_ = 0;
  this->upgrade_finish_time_ = 0;
  this->upgrade_start_time_ = 0;

  this->social_actor_ = NULL;
}

static void SendHomelandFriendSync(core::uint64 actor_id, core::uint64 friend_actor_id) {
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    return;
  }
  if (social_actor->IsFriend(friend_actor_id) == false) {
    LOG_ERROR("Actor(%lu) is not a friend of Actor(%lu).", friend_actor_id, actor_id);
    return;
  }

  SocialActor *friend_social_actor =
    SocialActorManager::GetInstance()->GetFromAll(friend_actor_id);
  if (NULL == friend_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", friend_actor_id);
    return;
  }
  HomelandActor *friend_homeland_actor = friend_social_actor->GetHomelandActor();

  gateway::protocol::MessageHomelandFriendSynchronize sync;
  sync.__set_type_(gateway::protocol::HomelandFriendOperateType::UPDATE);
  sync.data_.__set_actor_id_(friend_social_actor->GetStringID());
  sync.data_.__set_can_visit_(friend_homeland_actor->CheckCanVisit());
  sync.data_.__set_tree_can_water_(
      friend_homeland_actor->CheckTreeCanWater(social_actor->GetActorID()));
  sync.data_.__set_tree_can_steal_(
      friend_homeland_actor->CheckTreeCanSteal(social_actor->GetActorID()));
  social_actor->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_FRIEND_SYNCHRONIZE);
}

static void SendHomelandInvaderSync(core::uint64 actor_id, core::uint64 invader_actor_id) {
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    return;
  }

  SocialActor *invader_social_actor =
    SocialActorManager::GetInstance()->GetFromAll(invader_actor_id);
  if (NULL == invader_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", invader_actor_id);
    return;
  }
  HomelandActor *invader_homeland_actor = invader_social_actor->GetHomelandActor();

  gateway::protocol::MessageHomelandInvaderSynchronize sync;
  sync.__set_type_(gateway::protocol::HomelandInvaderOperateType::UPDATE);
  sync.data_.__set_actor_id_(invader_social_actor->GetStringID());
  sync.data_.__set_can_visit_(invader_homeland_actor->CheckCanVisit());
  sync.data_.__set_can_strike_back_(invader_homeland_actor->CheckGoldmineCanRob());
  social_actor->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_INVADER_SYNCHRONIZE);
}

void HomelandActor::SendInitToClient() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  gateway::protocol::MessageHomelandInitialize message;
  message.__set_upgrade_cooling_(this->GetUpgradeCooling(now));
  message.__set_upgrade_cooling_red_(this->CheckUpgradeCoolingRed(now));
  message.__set_temple_level_(this->temple_level_);
  message.__set_temple_harvest_times_(this->temple_harvest_times_);
  message.__set_temple_force_harvest_times_(this->temple_force_harvest_times_);
  message.__set_temple_harvest_cooling_(this->GetTempleHarvestCooling(now));
  message.__set_goldmine_level_(this->goldmine_level_);
  message.__set_goldmine_harvest_cooling_(this->GetGoldmineHarvestCooling(now));
  message.__set_goldmine_robbed_times_(this->goldmine_robbed_times_);
  message.__set_goldmine_loss_rate_(this->goldmine_loss_rate_);
  message.__set_goldmine_rob_times_(this->goldmine_rob_times_);
  message.__set_goldmine_rob_cooling_(this->GetGoldmineRobCooling(now));
  message.__set_pool_level_(this->pool_level_);
  message.__set_pool_accumulate_time_(this->GetPoolAccumulateTime(now));
  message.__set_church_free_pray_times_(this->church_free_pray_times_);
  message.__set_church_paid_pray_times_(this->church_paid_pray_times_);
  message.__set_church_free_refresh_times_(this->church_free_refresh_times_);
  message.__set_church_chest_type_(this->church_chest_type_);
  message.__set_tree_level_(this->tree_level_);
  message.__set_tree_harvest_times_(this->tree_harvest_times_);
  message.__set_tree_stage_((entity::HomelandTreeStageType::type)this->tree_stage_);
  message.__set_tree_watered_times_(this->tree_watered_times_);
  message.__set_tree_stolen_times_(this->GetTreeStolenTimes());
  message.__set_tree_water_cooling_(this->GetTreeWaterCooling(now));
  message.__set_tree_growing_cooling_(this->GetTreeGrowingCooling(now));
  message.__set_tree_assist_water_times_(this->tree_assist_water_times_);
  message.__set_tower1_level_(this->tower1_level_);
  message.__set_tower2_level_(this->tower2_level_);
  message.__set_stronghold_level_(this->stronghold_level_);
  message.__set_stronghold_call_support_cooling_(this->GetStrongholdCallSupportCooling(now));
  message.__set_stronghold_support_times_(this->stronghold_support_times_);

  // 军团据点被支援英灵
  const HomelandSupportedSoulMap &supported_souls = this->GetStrongholdSupportedSouls();
  for (HomelandSupportedSoulMap::const_iterator iter = supported_souls.begin();
       iter != supported_souls.end(); ++iter) {
    const HomelandSupportedSoul &soul = iter->second;
    gateway::protocol::HomelandSupportedSoulData data;

    BriefActor *brief_actor =
      SessionActorManager::GetInstance()->GetBriefActor(soul.GetActorID());
    if (NULL == brief_actor) {
      LOG_ERROR("Actor(%lu) does not exist.", soul.GetActorID());
      continue;
    }

    data.__set_actor_id_(brief_actor->GetStringID());
    data.__set_actor_name_(brief_actor->GetName());
    data.__set_soul_id_(soul.GetSoulID());
    data.__set_soul_level_(soul.GetSoulLevel());
    data.__set_leave_cooling_(soul.GetLeaveCooling(now));

    message.stronghold_supported_souls_.push_back(data);
  }

  // 军团据点支援英灵
  const HomelandSupportSoulMap &support_souls = this->GetStrongholdSupportSouls();
  for (HomelandSupportSoulMap::const_iterator iter = support_souls.begin();
       iter != support_souls.end(); ++iter) {
    const HomelandSupportSoul &soul = iter->second;
    gateway::protocol::HomelandSupportSoulData data;

    BriefActor *brief_actor =
      SessionActorManager::GetInstance()->GetBriefActor(soul.GetSupportActorID());
    if (NULL == brief_actor) {
      LOG_ERROR("Actor(%lu) does not exist.", soul.GetSupportActorID());
      continue;
    }

    data.__set_soul_id_(soul.GetSoulID());
    data.__set_actor_id_(brief_actor->GetStringID());
    data.__set_actor_name_(brief_actor->GetName());
    data.__set_cooling_(soul.GetNextSupportCooling(now));

    message.stronghold_support_souls_.push_back(data);
  }

  // 家园好友
  const SocialActor::ActorIDSet &friends = this->social_actor_->GetFriends();
  for (SocialActor::ActorIDSet::const_iterator iter = friends.begin();
       iter != friends.end(); ++iter) {
    gateway::protocol::HomelandFriendData data;
    char string_id[32];
    snprintf(string_id, sizeof(string_id), "%lu", *iter);
    data.__set_actor_id_(string_id);

    SocialActor *friend_social_actor = SocialActorManager::GetInstance()->GetFromAll(*iter);
    if (NULL == friend_social_actor) {
      // 加载离线玩家
      SocialActorManager::GetInstance()->OfflineLoad(*iter,
          boost::bind(&SendHomelandFriendSync, this->social_actor_->GetActorID(), *iter));
      data.__set_can_visit_(false);
      data.__set_tree_can_water_(false);
      data.__set_tree_can_steal_(false);
    } else {
      HomelandActor *friend_homeland_actor = friend_social_actor->GetHomelandActor();
      data.__set_can_visit_(friend_homeland_actor->CheckCanVisit());
      data.__set_tree_can_water_(
          friend_homeland_actor->CheckTreeCanWater(this->social_actor_->GetActorID()));
      data.__set_tree_can_steal_(
          friend_homeland_actor->CheckTreeCanSteal(this->social_actor_->GetActorID()));
    }

    message.friends_.push_back(data);
  }

  // 家园入侵者
  for (HomelandInvaderSet::const_iterator iter = this->invaders_time_index_.begin();
       iter != this->invaders_time_index_.end(); ++iter) {
    core::uint64 invader_actor_id = (*iter)->GetActorID();

    BriefActor *invader_brief_actor =
      SessionActorManager::GetInstance()->GetBriefActor(invader_actor_id);
    if (NULL == invader_brief_actor) {
      LOG_ERROR("Actor(%lu) does not exist.", invader_actor_id);
      continue;
    }

    gateway::protocol::HomelandInvaderData data;
    data.__set_actor_id_(invader_brief_actor->GetStringID());
    data.__set_actor_name_(invader_brief_actor->GetName());
    data.__set_level_(invader_brief_actor->GetLevel());

    SocialActor *invader_social_actor =
        SocialActorManager::GetInstance()->GetFromAll(invader_actor_id);
    if (NULL == invader_social_actor) {
      // 加载离线玩家
      SocialActorManager::GetInstance()->OfflineLoad(invader_actor_id,
          boost::bind(&SendHomelandInvaderSync, this->social_actor_->GetActorID(), invader_actor_id));
      data.__set_can_visit_(false);
      data.__set_can_strike_back_(false);
    } else {
      HomelandActor *invader_homeland_actor = invader_social_actor->GetHomelandActor();
      data.__set_can_visit_(invader_homeland_actor->CheckCanVisit());
      data.__set_can_strike_back_(invader_homeland_actor->CheckGoldmineCanRob());
    }

    message.invaders_.push_back(data);
  }

  // 告示板
  for (HomelandEventDeque::const_iterator iter = this->events_.begin();
       iter != this->events_.end(); ++iter) {
    const HomelandEvent &event = *iter;
    gateway::protocol::HomelandEventData data;

    data.__set_event_type_((entity::HomelandEventType::type)event.GetType());
    data.__set_timestamp_(event.GetTimestamp());
    event.TranslateEventVar(&data.vars_);

    message.events_.push_back(data);
  }

  this->social_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_INITIALIZE);
}

void HomelandActor::Online() {
  // 未开启功能返回
  if (this->social_actor_->CheckFunctionalityState(
          entity::FunctionalityType::HOMELAND_MODULE) == false) {
    return;
  }

  // Follow 家园好友信息更新事件
  const SocialActor::ActorIDSet &friends = this->social_actor_->GetFriends();
  for (SocialActor::ActorIDSet::const_iterator iter = friends.begin();
       iter != friends.end(); ++iter) {
    this->FollowHomelandFriendInfoUpdateEvent(*iter);
  }

  // Follow 家园侵入者信息更新事件
  for (HomelandInvaderMap::const_iterator iter = this->invaders_.begin();
       iter != this->invaders_.end(); ++iter) {
    this->FollowHomelandInvaderInfoUpdateEvent(iter->second.GetActorID());
  }

  // 移除失效的侵入者
  this->RemoveExpiredInvader(false);

  // 发送初始化数据
  this->SendInitToClient();
}

void HomelandActor::Offline() {
  // Unfollow 家园入侵者信息更新事件
  for (HomelandInvaderMap::const_iterator iter = this->invaders_.begin();
       iter != this->invaders_.end(); ++iter) {
    this->UnfollowHomelandInvaderInfoUpdateEvent(iter->second.GetActorID());
  }

  // Unfollow 家园好友信息更新事件
  if (this->social_actor_->CheckFunctionalityState(
          entity::FunctionalityType::HOMELAND_MODULE) == false) {
    return;
  }
  const SocialActor::ActorIDSet &friends = this->social_actor_->GetFriends();
  for (SocialActor::ActorIDSet::const_iterator iter = friends.begin();
       iter != friends.end(); ++iter) {
    this->UnfollowHomelandFriendInfoUpdateEvent(*iter);
  }
}

void HomelandActor::Save(database::protocol::StorageSocialActorField &db_field) {
  db_field.__set_homeland_upgrade_start_time_(this->upgrade_start_time_);
  db_field.__set_homeland_upgrade_finish_time_(this->upgrade_finish_time_);
  db_field.__set_homeland_temple_level_(this->temple_level_);
  db_field.__set_homeland_temple_harvest_times_(this->temple_harvest_times_);
  db_field.__set_homeland_temple_force_harvest_times_(this->temple_force_harvest_times_);
  db_field.__set_homeland_temple_next_harvest_time_(this->temple_next_harvest_time_);
  db_field.__set_homeland_goldmine_level_(this->goldmine_level_);
  db_field.__set_homeland_goldmine_robbed_times_(this->goldmine_robbed_times_);
  db_field.__set_homeland_goldmine_loss_rate_(this->goldmine_loss_rate_);
  db_field.__set_homeland_goldmine_next_harvest_time_(this->goldmine_next_harvest_time_);
  db_field.__set_homeland_goldmine_rob_times_(this->goldmine_rob_times_);
  db_field.__set_homeland_goldmine_next_rob_time_(this->goldmine_next_rob_time_);
  db_field.__set_homeland_pool_level_(this->pool_level_);
  db_field.__set_homeland_pool_last_harvest_time_(this->pool_last_harvest_time_);
  db_field.__set_homeland_church_free_pray_times_(this->church_free_pray_times_);
  db_field.__set_homeland_church_paid_pray_times_(this->church_paid_pray_times_);
  db_field.__set_homeland_church_free_refresh_times_(this->church_free_refresh_times_);
  db_field.__set_homeland_church_chest_type_(this->church_chest_type_);
  db_field.__set_homeland_tree_level_(this->tree_level_);
  db_field.__set_homeland_tree_harvest_times_(this->tree_harvest_times_);
  db_field.__set_homeland_tree_stage_(this->tree_stage_);
  db_field.__set_homeland_tree_watered_times_(this->tree_watered_times_);
  db_field.__set_homeland_tree_next_water_time_(this->tree_next_water_time_);
  db_field.__set_homeland_tree_growing_finish_time_(this->tree_growing_finish_time_);
  db_field.__set_homeland_tree_assist_water_times_(this->tree_assist_water_times_);
  EncodeActorIDSet(this->tree_watered_actors_, &db_field.homeland_tree_watered_actors_);
  EncodeActorIDSet(this->tree_stolen_actors_, &db_field.homeland_tree_stolen_actors_);
  db_field.__set_homeland_tower1_level_(this->tower1_level_);
  db_field.__set_homeland_tower2_level_(this->tower2_level_);
  db_field.__set_homeland_stronghold_level_(this->stronghold_level_);
  db_field.__set_homeland_stronghold_next_call_support_time_(this->stronghold_next_call_support_time_);
  EncodeHomelandSupportedSoul(this->GetStrongholdSupportedSouls(),
      &db_field.homeland_stronghold_supported_souls_);
  EncodeHomelandSupportSoul(this->GetStrongholdSupportSouls(),
      &db_field.homeland_stronghold_support_souls_);
  db_field.__set_homeland_stronghold_support_times_(this->stronghold_support_times_);
  EncodeHomelandInvader(this->invaders_, &db_field.homeland_invaders_);
  EncodeHomelandEvent(this->events_, &db_field.homeland_events_);
}

bool HomelandActor::EnableHomelandFunc(bool synchronize) {
  this->temple_level_ = 1;

  // Follow 家园好友信息更新事件
  const SocialActor::ActorIDSet &friends = this->social_actor_->GetFriends();
  for (SocialActor::ActorIDSet::const_iterator iter = friends.begin();
       iter != friends.end(); ++iter) {
    this->FollowHomelandFriendInfoUpdateEvent(*iter);
  }

  if (synchronize) {
    this->SendInitToClient();
  }

  // 发送信息更新事件
  {
    event::EventHomelandFriendInfoUpdate event;
    event.__set_actor_id_(this->social_actor_->GetActorID());
    event.__set_can_visit_(true);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_HOMELAND_FRIEND_INFO_UPDATE, this->social_actor_->GetActorID(),
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
  {
    event::EventHomelandInvaderInfoUpdate event;
    event.__set_actor_id_(this->social_actor_->GetActorID());
    event.__set_can_visit_(true);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_HOMELAND_INVADER_INFO_UPDATE, this->social_actor_->GetActorID(),
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }

  return true;
}

bool HomelandActor::EnableHomelandGoldmineFunc(bool synchronize) {
  this->SetGoldmineLevel(1, synchronize);

  // 加入对战表
  HomelandBattleManager::GetInstance()->AddToBattleLadder(this->social_actor_);
  
  return true;
}

bool HomelandActor::EnableHomelandPoolFunc(bool synchronize) {
  this->pool_level_ = 1;
  this->ClearPoolAccmulateTime(false);

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_pool_level_(this->pool_level_);
    sync.__set_pool_accumulate_time_(0);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  return true;
}

bool HomelandActor::EnableHomelandTreeFunc(bool synchronize) {
  this->tree_level_ = 1;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_level_(this->tree_level_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  event::EventHomelandFriendInfoUpdate event;
  event.__set_actor_id_(this->social_actor_->GetActorID());
  event.__set_tree_can_water_(true);
  event.__set_tree_can_steal_(false);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_HOMELAND_FRIEND_INFO_UPDATE, this->social_actor_->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

  return true;
}

bool HomelandActor::EnableHomelandTowerFunc(bool synchronize) {
  this->tower1_level_ = 1;
  this->tower2_level_ = 1;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tower1_level_(this->tower1_level_);
    sync.__set_tower2_level_(this->tower2_level_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  return true;
}

bool HomelandActor::EnableHomelandStrongholdFunc(bool synchronize) {
  this->stronghold_level_ = 1;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_stronghold_level_(this->stronghold_level_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////

bool HomelandActor::CheckCanVisit() const {
  return this->temple_level_ > 0;
}

core::int32 HomelandActor::GetUpgradeCooling(time_t now) const {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->upgrade_finish_time_ > now) {
    return this->upgrade_finish_time_ - now;
  } else {
    return 0;
  }
}

bool HomelandActor::CheckUpgradeCoolingRed(time_t now) const {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->upgrade_start_time_ > now) {
    return true;
  } else {
    return false;
  }
}

void HomelandActor::AddUpgradeCooling(core::int32 cooling_time, bool synchronize) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  if (this->upgrade_finish_time_ > now) {
    this->upgrade_finish_time_ += cooling_time;
  } else {
    this->upgrade_finish_time_ = now + cooling_time;
  }

  if (this->upgrade_finish_time_ - now >= MISC_CONF()->homeland_max_upgrade_cooling_) {
    this->upgrade_start_time_ = this->upgrade_finish_time_;
  }

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_upgrade_cooling_(this->GetUpgradeCooling(now));
    sync.__set_upgrade_cooling_red_(this->CheckUpgradeCoolingRed(now));
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::ClearUpgradeCooling(bool synchronize) {
  this->upgrade_start_time_ = 0;
  this->upgrade_finish_time_ = 0;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_upgrade_cooling_(0);
    sync.__set_upgrade_cooling_red_(false);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

bool HomelandActor::CheckUpgradeCooling() const {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  return this->CheckUpgradeCoolingRed(now) == false &&
         this->GetUpgradeCooling(now) < MISC_CONF()->homeland_max_upgrade_cooling_;
}

///////////////////////////////////////////////////////////////////////////////

void HomelandActor::SetTempleLevel(core::int32 new_level, bool synchronize) {
  this->temple_level_ = new_level;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_temple_level_(this->temple_level_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  if (this->tree_level_ > 0) {
    this->TreeTryUpgrade(synchronize);
  }
}

core::int32 HomelandActor::GetTempleHarvestCooling(time_t now) const {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->temple_next_harvest_time_ > now) {
    return this->temple_next_harvest_time_ - now;
  } else {
    return 0;
  }
}

void HomelandActor::SetTempleHarvestCooling(core::int32 cooling_time, bool synchronize) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  this->temple_next_harvest_time_ = now + cooling_time;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_temple_harvest_cooling_(cooling_time);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::ClearTempleHarvestCooling(bool synchronize) {
  this->temple_next_harvest_time_ = 0;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_temple_harvest_cooling_(0);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::AddTempleHarvestTimes(bool synchronize) {
  ++this->temple_harvest_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_temple_harvest_times_(this->temple_harvest_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::AddTempleForceHarvestTimes(bool synchronize) {
  ++this->temple_force_harvest_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_temple_force_harvest_times_(this->temple_force_harvest_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

///////////////////////////////////////////////////////////////////////////////

void HomelandActor::SetGoldmineLevel(core::int32 new_level, bool synchronize) {
  this->goldmine_level_ = new_level;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_goldmine_level_(this->goldmine_level_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

core::int32 HomelandActor::GetGoldmineHarvestCooling(time_t now) const {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->goldmine_next_harvest_time_ > now) {
    return this->goldmine_next_harvest_time_ - now;
  } else {
    return 0;
  }
}

void HomelandActor::SetGoldmineHarvestCooling(core::int32 cooling_time, bool synchronize) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  this->goldmine_next_harvest_time_ = now + cooling_time;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_goldmine_harvest_cooling_(cooling_time);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

bool HomelandActor::CheckGoldmineCanRob() const {
  if (0 == this->temple_level_) {
    return false;
  }
  if (0 == this->goldmine_level_) {
    return false;
  }

  const global::configure::HomelandGoldmineCell *cell =
    SOCIAL_CONF()->GetHomelandGoldmine(this->goldmine_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandGoldmineCell(%d) not found in config.", this->goldmine_level_);
    return false;
  }

  if (this->goldmine_robbed_times_ >= cell->max_robbed_times_) {
    return false;
  }

  return true;
}

core::int32 HomelandActor::GetGoldmineRobCooling(time_t now) const {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->goldmine_next_rob_time_ > now) {
    return this->goldmine_next_rob_time_ - now;
  } else {
    return 0;
  }
}

void HomelandActor::SetGoldmineRobCooling(core::int32 cooling_time, bool synchronize) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  this->goldmine_next_rob_time_ = now + cooling_time;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_goldmine_rob_cooling_(cooling_time);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::AddGoldmineRobTimes(bool synchronize) {
  ++this->goldmine_rob_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_goldmine_rob_times_(this->goldmine_rob_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::AddGoldmineRobbedTimes(bool synchronize) {
  ++this->goldmine_robbed_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_goldmine_robbed_times_(this->goldmine_robbed_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  const global::configure::HomelandGoldmineCell *cell =
    SOCIAL_CONF()->GetHomelandGoldmine(this->goldmine_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandGoldmineCell(%d) not found in config.", this->goldmine_level_);
    return;
  }

  if (this->goldmine_robbed_times_ >= cell->max_robbed_times_) {
    // 发送信息更新事件
    event::EventHomelandInvaderInfoUpdate event;
    event.__set_actor_id_(this->social_actor_->GetActorID());
    event.__set_can_strike_back_(false);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_HOMELAND_INVADER_INFO_UPDATE, this->social_actor_->GetActorID(),
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
    // 从对战表移除
    HomelandBattleManager::GetInstance()->RemoveFromBattleLadder(this->social_actor_->GetActorID());
  }
}

void HomelandActor::AddGoldmineLossRate(core::int32 rate, bool synchronize) {
  this->goldmine_loss_rate_ += rate;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_goldmine_loss_rate_(this->goldmine_loss_rate_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::ResetGoldmine(bool synchronize) {
  this->goldmine_robbed_times_ = 0;
  this->goldmine_loss_rate_ = 0;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_goldmine_robbed_times_(this->goldmine_robbed_times_);
    sync.__set_goldmine_loss_rate_(this->goldmine_loss_rate_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  // 发送信息更新事件
  event::EventHomelandInvaderInfoUpdate event;
  event.__set_actor_id_(this->social_actor_->GetActorID());
  event.__set_can_strike_back_(true);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_HOMELAND_INVADER_INFO_UPDATE, this->social_actor_->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  // 加入对战表
  HomelandBattleManager::GetInstance()->AddToBattleLadder(this->social_actor_);
}

///////////////////////////////////////////////////////////////////////////////

void HomelandActor::SetPoolLevel(core::int32 new_level, bool synchronize) {
  this->pool_level_ = new_level;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_pool_level_(this->pool_level_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

core::int32 HomelandActor::GetPoolAccumulateTime(time_t now) const {
  if (0 == this->pool_level_) {
    return 0;
  }

  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  core::int32 accumulate_time;

  if (now > this->pool_last_harvest_time_) {
    accumulate_time = now - this->pool_last_harvest_time_;
    // 取上限
    const global::configure::HomelandPoolCell *cell =
      SOCIAL_CONF()->GetHomelandPool(this->pool_level_);
    if (NULL == cell) {
      LOG_ERROR("HomelandPoolCell(%d) not found in config.", this->pool_level_);
      return 0;
    }

    core::int32 max_accumulate_time =
      cell->accumulate_spend_time_ * cell->max_accumulate_times_;
    accumulate_time = std::min(accumulate_time, std::max(0, max_accumulate_time));

  } else {
    accumulate_time = 0;
  }

  return accumulate_time;
}

void HomelandActor::ClearPoolAccmulateTime(bool synchronize) {
  this->pool_last_harvest_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec(); 

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_pool_accumulate_time_(0);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

///////////////////////////////////////////////////////////////////////////////

bool HomelandActor::CheckChurchInPrayStage() const {
  return 0 == this->church_chest_type_;
}

bool HomelandActor::CheckChurchInHarvestStage() const {
  return this->church_chest_type_ != 0;
}

void HomelandActor::MoveChurchToPrayStage(bool synchronize) {
  this->church_chest_type_ = 0;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_church_chest_type_(this->church_chest_type_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::MoveChurchToHarvestStage(core::int32 chest_type, bool synchronize) {
  this->church_chest_type_ = chest_type;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_church_chest_type_(this->church_chest_type_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::AddChurchFreePrayTimes(bool synchronize) {
  ++this->church_free_pray_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_church_free_pray_times_(this->church_free_pray_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::AddChurchPaidPrayTimes(bool synchronize) {
  ++this->church_paid_pray_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_church_paid_pray_times_(this->church_paid_pray_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::AddChurchFreeRefreshTimes(bool synchronize) {
  ++this->church_free_refresh_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_church_free_refresh_times_(this->church_free_refresh_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

///////////////////////////////////////////////////////////////////////////////

void HomelandActor::AddTreeHarvestTimes(bool synchronize) {
  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(this->tree_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.", this->tree_level_);
    return;
  }

  if (this->tree_harvest_times_ < cell->upgrade_required_harvest_times_) {
    ++this->tree_harvest_times_;
  }

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_harvest_times_(this->tree_harvest_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  this->TreeTryUpgrade(synchronize);
}

void HomelandActor::TreeTryUpgrade(bool synchronize) {
  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(this->tree_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.", this->tree_level_);
    return;
  }

  if (this->tree_harvest_times_ >= cell->upgrade_required_harvest_times_ &&
      SOCIAL_CONF()->GetHomelandTree(this->tree_level_ + 1) != NULL &&
      this->tree_level_ < this->temple_level_) {
    ++this->tree_level_;
    this->tree_harvest_times_ = 0;

    if (synchronize) {
      gateway::protocol::MessageHomelandSynchronize sync;
      sync.__set_tree_level_(this->tree_level_);
      sync.__set_tree_harvest_times_(this->tree_harvest_times_);
      this->social_actor_->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
    }
  }
}

core::int32 HomelandActor::GetTreeWaterCooling(time_t now) const {
  if (this->tree_stage_ != entity::HomelandTreeStageType::WATERING) {
    return 0;
  }

  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->tree_next_water_time_ > now) {
    return this->tree_next_water_time_ - now;
  } else {
    return 0;
  }
}

void HomelandActor::SetTreeWaterCooling(core::int32 cooling_time, bool synchronize) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  this->tree_next_water_time_ = now + cooling_time;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_water_cooling_(cooling_time);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

core::int32 HomelandActor::GetTreeGrowingCooling(time_t now) const {
  if (this->tree_stage_ != entity::HomelandTreeStageType::GROWING) {
    return 0;
  }

  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->tree_growing_finish_time_ > now) {
    return this->tree_growing_finish_time_ - now;
  } else {
    return 0;
  }
}

void HomelandActor::SetTreeGrowingCooling(core::int32 cooling_time, bool synchronize) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  this->tree_growing_finish_time_ = now + cooling_time;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_growing_cooling_(cooling_time);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::MoveTreeToGrowingStage(core::int32 growing_time, bool synchronize) {
  this->tree_stage_ = entity::HomelandTreeStageType::GROWING;
  this->SetTreeGrowingCooling(growing_time, false);

  SessionServerSingleton::GetInstance().AddTimer(TIMER_ID_GROWING_FINISH,
      growing_time * 1000, 1, this, "HomelandActor::MoveTreeToGrowingStage");

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_stage_((entity::HomelandTreeStageType::type)this->tree_stage_);
    sync.__set_tree_growing_cooling_(growing_time);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  // 发送信息更新事件
  event::EventHomelandFriendInfoUpdate event;
  event.__set_actor_id_(this->social_actor_->GetActorID());
  event.__set_tree_can_water_(false);
  event.__set_tree_can_steal_(false);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_HOMELAND_FRIEND_INFO_UPDATE, this->social_actor_->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void HomelandActor::MoveTreeToHarvestStage(bool synchronize) {
  this->tree_stage_ = entity::HomelandTreeStageType::HARVEST;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_stage_((entity::HomelandTreeStageType::type)this->tree_stage_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  // 发送信息更新事件
  event::EventHomelandFriendInfoUpdate event;
  event.__set_actor_id_(this->social_actor_->GetActorID());
  event.__set_tree_can_water_(false);
  event.__set_tree_can_steal_(true);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_HOMELAND_FRIEND_INFO_UPDATE, this->social_actor_->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void HomelandActor::MoveTreeToWateringStage(bool synchronize) {
  this->tree_stage_ = entity::HomelandTreeStageType::WATERING;
  this->tree_stolen_actors_.clear();
  this->tree_watered_times_ = 0;
  this->tree_next_water_time_ = 0;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_stage_((entity::HomelandTreeStageType::type)this->tree_stage_);
    sync.__set_tree_watered_times_(this->tree_watered_times_);
    sync.__set_tree_stolen_times_(this->GetTreeStolenTimes());
    sync.__set_tree_water_cooling_(0);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  // 发送信息更新事件
  event::EventHomelandFriendInfoUpdate event;
  event.__set_actor_id_(this->social_actor_->GetActorID());
  event.__set_tree_can_water_(true);
  event.__set_tree_can_steal_(false);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_HOMELAND_FRIEND_INFO_UPDATE, this->social_actor_->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}


bool HomelandActor::CheckTreeCanWater(core::uint64 actor_id) const {
  if (0 == this->temple_level_) {
    return false;
  }

  if (0 == this->tree_level_) {
    return false;
  }

  if (this->tree_stage_ != entity::HomelandTreeStageType::WATERING) {
    return false;
  }

  if (actor_id != this->social_actor_->GetActorID()) {
    if (this->tree_watered_actors_.find(actor_id) !=
        this->tree_watered_actors_.end()) {
      return false;
    }
  }

  return true;
}

bool HomelandActor::CheckTreeCanSteal(core::uint64 actor_id) const {
  if (0 == this->temple_level_) {
    return false;
  }

  if (0 == this->tree_level_) {
    return false;
  }

  if (this->tree_stage_ != entity::HomelandTreeStageType::HARVEST) {
    return false;
  }

  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(this->tree_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.", this->tree_level_);
    return false;
  }

  if (this->GetTreeStolenTimes() >= cell->max_stolen_times_) {
    return false;
  }

  if (this->social_actor_->GetActorID() == actor_id) {
    return false;
  }

  if (this->tree_stolen_actors_.find(actor_id) != this->tree_stolen_actors_.end()) {
    return false;
  }

  return true;
}

void HomelandActor::AddTreeWaterdTimes(core::uint64 actor_id, bool synchronize) {
  if (actor_id != this->social_actor_->GetActorID()) {
    this->tree_watered_actors_.insert(actor_id);
  }

  ++this->tree_watered_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_watered_times_(this->tree_watered_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::AddTreeStolenTimes(core::uint64 actor_id, bool synchronize) {
  this->tree_stolen_actors_.insert(actor_id);

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_stolen_times_(this->GetTreeStolenTimes());
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }

  const global::configure::HomelandTreeCell *cell =
    SOCIAL_CONF()->GetHomelandTree(this->tree_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandTreeCell(%d) not found in config.", this->tree_level_);
    return;
  }

  if (this->GetTreeStolenTimes() >= cell->max_stolen_times_) {
    // 发送信息更新事件
    event::EventHomelandFriendInfoUpdate event;
    event.__set_actor_id_(this->social_actor_->GetActorID());
    event.__set_tree_can_steal_(false);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_HOMELAND_FRIEND_INFO_UPDATE, this->social_actor_->GetActorID(),
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
}

void HomelandActor::AddTreeAssistWaterTimes(bool synchronize) {
  ++this->tree_assist_water_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_tree_assist_water_times_(this->tree_assist_water_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

///////////////////////////////////////////////////////////////////////////////

core::int32 HomelandActor::GetTowerLevel(core::int32 tower_id) const {
  if (1 == tower_id) {
    return this->tower1_level_;
  } else if (2 == tower_id) {
    return this->tower2_level_;
  } else {
    LOG_ERROR("Tower id is invalid.");
    return -1;
  }
}

void HomelandActor::SetTowerLevel(core::int32 tower_id, core::int32 new_level,
    bool synchronize) {
  if (1 == tower_id) {
    this->tower1_level_ = new_level;
  } else if (2 == tower_id) {
    this->tower2_level_ = new_level;
  } else {
    LOG_ERROR("Tower id is invalid.");
    return;
  }

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    if (1 == tower_id) {
      sync.__set_tower1_level_(this->tower1_level_);
    } else {
      sync.__set_tower2_level_(this->tower2_level_);
    }
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

///////////////////////////////////////////////////////////////////////////////

void HomelandActor::SetStrongholdLevel(core::int32 new_level, bool synchronize) {
  this->stronghold_level_ = new_level;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_stronghold_level_(this->stronghold_level_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

core::int32 HomelandActor::GetStrongholdCallSupportCooling(time_t now) const {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->stronghold_next_call_support_time_ > now) {
    return this->stronghold_next_call_support_time_ - now;
  } else {
    return 0;
  }
}

void HomelandActor::SetStrongholdCallSupportCooling(core::int32 cooling_time, bool synchronize) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  this->stronghold_next_call_support_time_ = now + cooling_time;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_stronghold_call_support_cooling_(cooling_time);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

const HomelandActor::HomelandSupportedSoulMap &HomelandActor::GetStrongholdSupportedSouls() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  for (HomelandSupportedSoulMap::iterator iter = this->stronghold_supported_souls_.begin();
       iter != this->stronghold_supported_souls_.end();) {
    HomelandSupportedSoul &soul = iter->second; 

    if (soul.GetLeaveCooling(now) == 0) {
      this->stronghold_supported_souls_.erase(iter++);
    } else {
      ++iter;
    }
  }

  return this->stronghold_supported_souls_;
}

const HomelandActor::HomelandSupportSoulMap &HomelandActor::GetStrongholdSupportSouls() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  for (HomelandSupportSoulMap::iterator iter = this->stronghold_support_souls_.begin();
       iter != this->stronghold_support_souls_.end();) {
    HomelandSupportSoul &soul = iter->second;

    if (soul.GetNextSupportCooling(now) == 0) {
      this->stronghold_support_souls_.erase(iter++);
    } else {
      ++iter;
    }
  }

  return this->stronghold_support_souls_;
}

bool HomelandActor::CheckStrongholdCanSupport(core::uint64 actor_id) {
  if (FacadeRequest::GetInstance()->CheckInSameGuild(
          this->social_actor_->GetActorID(), actor_id) == false) {
    return false;
  }

  const global::configure::HomelandStrongholdCell *cell =
    SOCIAL_CONF()->GetHomelandStronghold(this->stronghold_level_);
  if (NULL == cell) {
    LOG_ERROR("HomelandStrongholdCell(%d) not found in config.", this->stronghold_level_);
    return false;
  }

  const HomelandSupportedSoulMap &supported_souls = this->GetStrongholdSupportedSouls();
  if (supported_souls.size() >= (size_t)cell->max_support_slot_capacity_) {
    return false;
  }

  if (supported_souls.find(actor_id) != supported_souls.end()) {
    return false;
  }

  return true;
}

bool HomelandActor::CheckSoulCanSupport(core::uint32 soul_id) const {
  if (this->stronghold_support_times_ >=
      MISC_CONF()->homeland_max_stronghold_support_times_) {
    return false;
  }

  if (this->souls_.find(soul_id) == this->souls_.end()) {
    return false;
  }

  HomelandSupportSoulMap::const_iterator iter =
    this->stronghold_support_souls_.find(soul_id);
  if (iter == this->stronghold_support_souls_.end()) {
    return true;
  }

  const HomelandSupportSoul &soul = iter->second;
  if (soul.GetNextSupportCooling() == 0) {
    return true;
  }

  return false;
}

void HomelandActor::AddStrongholdSupportedSoul(core::uint64 actor_id,
    core::uint32 soul_id, core::int32 soul_level, bool synchronize) {
  BriefActor *target_brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_id);
  if (NULL == target_brief_actor) {
    LOG_ERROR("actor_id(%lu) does not exist.", actor_id);
    return;
  }

  HomelandSupportedSoulMap::iterator iter =
    this->stronghold_supported_souls_.find(actor_id);
  if (iter != this->stronghold_supported_souls_.end()) {
    iter->second.SetSoulID(soul_id);
    iter->second.SetSoulLevel(soul_level);
    iter->second.SetLeaveCooling(MISC_CONF()->homeland_stronghold_support_cooling_);
  } else {
    HomelandSupportedSoul soul(actor_id, soul_id, soul_level);
    soul.SetLeaveCooling(MISC_CONF()->homeland_stronghold_support_cooling_);
    this->stronghold_supported_souls_.insert(std::make_pair(actor_id, soul));
  }

  if (synchronize) {
    gateway::protocol::MessageHomelandSupportedSoulSynchronize soul_sync;
    soul_sync.__set_type_(gateway::protocol::HomelandSupportedSoulOperateType::ADD);
    soul_sync.data_.__set_actor_id_(target_brief_actor->GetStringID());
    soul_sync.data_.__set_actor_name_(target_brief_actor->GetName());
    soul_sync.data_.__set_soul_id_(soul_id);
    soul_sync.data_.__set_soul_level_(soul_level);
    soul_sync.data_.__set_leave_cooling_(MISC_CONF()->homeland_stronghold_support_cooling_);
    this->social_actor_->SendMessage(soul_sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SUPPORTED_SOUL_SYNCHRONIZE);
  }
}

void HomelandActor::RemoveStrongholdSupportedSoul(core::uint64 actor_id, bool synchronize) {
  HomelandSupportedSoulMap::iterator iter =
    this->stronghold_supported_souls_.find(actor_id);
  if (this->stronghold_supported_souls_.end() == iter) {
    LOG_ERROR("Actor(%lu)'s soul not found in supported soul list.", actor_id);
    return;
  }

  this->stronghold_supported_souls_.erase(actor_id);

  if (synchronize) {
    char string_id[32];

    gateway::protocol::MessageHomelandSupportedSoulSynchronize soul_sync;
    soul_sync.__set_type_(gateway::protocol::HomelandSupportedSoulOperateType::REMOVE);
    snprintf(string_id, sizeof(string_id), "%lu", actor_id);
    soul_sync.data_.__set_actor_id_(string_id);
    this->social_actor_->SendMessage(soul_sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SUPPORTED_SOUL_SYNCHRONIZE);
  }
}

void HomelandActor::AddStrongholdSupportSoul(core::uint32 soul_id, core::uint64 actor_id,
    bool synchronize) {
  BriefActor *target_brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_id);
  if (NULL == target_brief_actor) {
    LOG_ERROR("actor_id(%lu) does not exist.", actor_id);
    return;
  }

  HomelandSupportSoulMap::iterator iter =
    this->stronghold_support_souls_.find(soul_id);
  if (iter != this->stronghold_support_souls_.end()) {
    iter->second.SetSupportActorID(actor_id);
    iter->second.SetNextSupportCooling(MISC_CONF()->homeland_stronghold_support_cooling_);
  } else {
    HomelandSupportSoul soul(soul_id, actor_id);
    soul.SetNextSupportCooling(MISC_CONF()->homeland_stronghold_support_cooling_);
    this->stronghold_support_souls_.insert(std::make_pair(soul_id, soul));
  }

  ++this->stronghold_support_times_;

  if (synchronize) {
    gateway::protocol::MessageHomelandSupportSoulSynchronize soul_sync;
    soul_sync.__set_type_(gateway::protocol::HomelandSupportSoulOperateType::ADD);
    soul_sync.data_.__set_soul_id_(soul_id);
    soul_sync.data_.__set_actor_id_(target_brief_actor->GetStringID());
    soul_sync.data_.__set_actor_name_(target_brief_actor->GetName());
    soul_sync.data_.__set_cooling_(MISC_CONF()->homeland_stronghold_support_cooling_);
    this->social_actor_->SendMessage(soul_sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SUPPORT_SOUL_SYNCHRONIZE);

    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_stronghold_support_times_(this->stronghold_support_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

///////////////////////////////////////////////////////////////////////////////

void HomelandActor::AddEvent(const HomelandEvent &event, bool synchronize) {
  if (this->events_.size() >= (size_t)MISC_CONF()->homeland_max_event_capacity_ &&
      !this->events_.empty()) {
    this->events_.pop_front();
  }
  this->events_.push_back(event);

  if (synchronize) {
    gateway::protocol::MessageHomelandEventSynchronize sync;
    sync.__set_type_(gateway::protocol::HomelandEventOperateType::ADD);
    sync.data_.__set_event_type_((entity::HomelandEventType::type)event.GetType());
    sync.data_.__set_timestamp_(event.GetTimestamp());
    event.TranslateEventVar(&sync.data_.vars_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_EVENT_SYNCHRONIZE);
  }
}

///////////////////////////////////////////////////////////////////////////////

void HomelandActor::AddHomelandFriend(core::uint64 actor_id, bool synchronize) {
  // 未开启功能返回
  if (this->social_actor_->CheckFunctionalityState(
          entity::FunctionalityType::HOMELAND_MODULE) == false) {
    return;
  }

  // Follow 家园好友信息更新事件
  this->FollowHomelandFriendInfoUpdateEvent(actor_id);

  if (synchronize) {
    gateway::protocol::MessageHomelandFriendSynchronize sync;
    sync.__set_type_(gateway::protocol::HomelandFriendOperateType::ADD);
    char string_id[32];
    snprintf(string_id, sizeof(string_id), "%lu", actor_id);
    sync.data_.__set_actor_id_(string_id);

    SocialActor *friend_social_actor = SocialActorManager::GetInstance()->GetFromAll(actor_id);
    if (NULL == friend_social_actor) {
      // 加载离线玩家
      SocialActorManager::GetInstance()->OfflineLoad(actor_id,
          boost::bind(SendHomelandFriendSync, this->social_actor_->GetActorID(), actor_id));
      sync.data_.__set_can_visit_(false);
      sync.data_.__set_tree_can_water_(false);
      sync.data_.__set_tree_can_steal_(false);
    } else {
      HomelandActor *friend_homeland_actor = friend_social_actor->GetHomelandActor();
      sync.data_.__set_can_visit_(friend_homeland_actor->CheckCanVisit());
      sync.data_.__set_tree_can_water_(
          friend_homeland_actor->CheckTreeCanWater(this->social_actor_->GetActorID()));
      sync.data_.__set_tree_can_steal_(
          friend_homeland_actor->CheckTreeCanSteal(this->social_actor_->GetActorID()));
    }
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_FRIEND_SYNCHRONIZE);
  }
}

void HomelandActor::RemoveHomelandFriend(core::uint64 actor_id, bool synchronize) {
  // 未开启功能返回
  if (this->social_actor_->CheckFunctionalityState(
          entity::FunctionalityType::HOMELAND_MODULE) == false) {
    return;
  }

  // Unfollow 家园好友信息更新事件
  this->UnfollowHomelandFriendInfoUpdateEvent(actor_id);

  if (synchronize) {
    gateway::protocol::MessageHomelandFriendSynchronize sync;
    sync.__set_type_(gateway::protocol::HomelandFriendOperateType::REMOVE);
    char string_id[32];
    snprintf(string_id, sizeof(string_id), "%lu", actor_id);
    sync.data_.__set_actor_id_(string_id);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_FRIEND_SYNCHRONIZE);
  }
}

void HomelandActor::FollowHomelandFriendInfoUpdateEvent(core::uint64 actor_id) {
  coresh::Communicator::Connection conn =
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_HOMELAND_FRIEND_INFO_UPDATE, actor_id,
        event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&HomelandActor::OnEventHomelandFriendInfoUpdate, this, _1, _2, _3, _4));
  this->friend_event_conns_.insert(std::make_pair(actor_id, conn));
}

void HomelandActor::UnfollowHomelandFriendInfoUpdateEvent(core::uint64 actor_id) {
  EventConnectionMap::const_iterator iter = this->friend_event_conns_.find(actor_id);
  if (iter == this->friend_event_conns_.end()) {
    LOG_ERROR("Event connection(%lu) not found.", actor_id);
    return;
  }
  coresh::CommunicatorSingleton::GetInstance().Unfollow(iter->second);
  this->friend_event_conns_.erase(actor_id);
}

void HomelandActor::OnEventHomelandFriendInfoUpdate(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventHomelandFriendInfoUpdate) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventHomelandFriendInfoUpdate *event = (event::EventHomelandFriendInfoUpdate *)message;

  SocialActor *friend_social_actor = SocialActorManager::GetInstance()->GetFromAll(event->actor_id_);
  if (NULL == friend_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", event->actor_id_);
    return;
  }
  HomelandActor *friend_homeland_actor = friend_social_actor->GetHomelandActor();

  gateway::protocol::MessageHomelandFriendSynchronize sync;
  sync.__set_type_(gateway::protocol::HomelandFriendOperateType::UPDATE);
  char string_id[32];
  snprintf(string_id, sizeof(string_id), "%lu", event->actor_id_);
  sync.data_.__set_actor_id_(string_id);

  if (event->__isset.can_visit_) {
    sync.data_.__set_can_visit_(event->can_visit_);
  }
  if (event->__isset.tree_can_water_) {
    if (false == event->tree_can_water_) {
      sync.data_.__set_tree_can_water_(false);
    } else {
      sync.data_.__set_tree_can_water_(
          friend_homeland_actor->CheckTreeCanWater(this->social_actor_->GetActorID()));
    }
  }
  if (event->__isset.tree_can_steal_) {
    if (false == event->tree_can_steal_) {
      sync.data_.__set_tree_can_steal_(false);
    } else {
      sync.data_.__set_tree_can_steal_(
          friend_homeland_actor->CheckTreeCanSteal(this->social_actor_->GetActorID()));
    }
  }

  this->social_actor_->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_FRIEND_SYNCHRONIZE);
}

///////////////////////////////////////////////////////////////////////////////

bool HomelandActor::IsInvader(core::uint64 actor_id) const {
  return this->invaders_.find(actor_id) != this->invaders_.end();
}

void HomelandActor::AddInvader(core::uint64 actor_id, bool synchronize) {
  BriefActor *invader_brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_id);
  if (NULL == invader_brief_actor) {
    LOG_ERROR("Actor(%lu) does not exist.", actor_id);
    return;
  }

  if (this->invaders_.size() >= (size_t)MISC_CONF()->homeland_max_invader_capacity_ &&
      !this->invaders_.empty()) {
    this->RemoveInvader((*this->invaders_time_index_.begin())->GetActorID());
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  HomelandInvader invader(actor_id, now);

  this->invaders_[actor_id] = invader;
  this->invaders_time_index_.insert(&this->invaders_[actor_id]);

  // Follow 家园侵入者信息更新事件
  this->FollowHomelandInvaderInfoUpdateEvent(actor_id);

  if (synchronize) {
    gateway::protocol::MessageHomelandInvaderSynchronize sync;
    sync.__set_type_(gateway::protocol::HomelandInvaderOperateType::ADD);
    sync.data_.__set_actor_id_(invader_brief_actor->GetStringID());
    sync.data_.__set_actor_name_(invader_brief_actor->GetName());
    sync.data_.__set_level_(invader_brief_actor->GetLevel());

    SocialActor *invader_social_actor =
        SocialActorManager::GetInstance()->GetFromAll(actor_id);
    if (NULL == invader_social_actor) {
      // 加载离线玩家
      SocialActorManager::GetInstance()->OfflineLoad(actor_id,
          boost::bind(&SendHomelandInvaderSync, this->social_actor_->GetActorID(), actor_id));
      sync.data_.__set_can_visit_(false);
      sync.data_.__set_can_strike_back_(false);
    } else {
      HomelandActor *invader_homeland_actor = invader_social_actor->GetHomelandActor();
      sync.data_.__set_can_visit_(invader_homeland_actor->CheckCanVisit());
      sync.data_.__set_can_strike_back_(invader_homeland_actor->CheckGoldmineCanRob());
    }
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_INVADER_SYNCHRONIZE);
  }
}

void HomelandActor::RemoveInvader(core::uint64 actor_id, bool synchronize) {
  if (this->IsInvader(actor_id) == false) {
    LOG_ERROR("Actor(%lu) is not invader.", actor_id);
    return;
  }

  this->invaders_time_index_.erase(&this->invaders_[actor_id]);
  this->invaders_.erase(actor_id);

  // Unfollow 家园侵入者信息更新事件
  this->UnfollowHomelandInvaderInfoUpdateEvent(actor_id);

  if (synchronize) {
    gateway::protocol::MessageHomelandInvaderSynchronize sync;
    sync.__set_type_(gateway::protocol::HomelandInvaderOperateType::REMOVE);
    char string_id[32];
    snprintf(string_id, sizeof(string_id), "%lu", actor_id);
    sync.data_.__set_actor_id_(string_id);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_INVADER_SYNCHRONIZE);
  }
}

void HomelandActor::RemoveExpiredInvader(bool synchronize) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  // 移除48小时之前侵入的玩家
  for (HomelandInvaderMap::iterator iter = this->invaders_.begin();
       iter != this->invaders_.end();) {
    HomelandInvaderMap::iterator iter2 = iter;
    ++iter2;

    if (iter->second.GetInvadeTime() + 48 * 60 * 60 < now) {
      this->RemoveInvader(iter->second.GetActorID(), synchronize);
    }

    iter = iter2;
  }
}

void HomelandActor::FollowHomelandInvaderInfoUpdateEvent(core::uint64 actor_id) {
  coresh::Communicator::Connection conn =
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_HOMELAND_INVADER_INFO_UPDATE, actor_id,
        event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&HomelandActor::OnEventHomelandInvaderInfoUpdate, this, _1, _2, _3, _4));
  this->invader_event_conns_.insert(std::make_pair(actor_id, conn));
}

void HomelandActor::UnfollowHomelandInvaderInfoUpdateEvent(core::uint64 actor_id) {
  EventConnectionMap::const_iterator iter = this->invader_event_conns_.find(actor_id);
  if (iter == this->invader_event_conns_.end()) {
    LOG_ERROR("Event connection(%lu) not found.", actor_id);
    return;
  }
  coresh::CommunicatorSingleton::GetInstance().Unfollow(iter->second);
  this->invader_event_conns_.erase(actor_id);
}

void HomelandActor::OnEventHomelandInvaderInfoUpdate(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventHomelandInvaderInfoUpdate) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventHomelandInvaderInfoUpdate *event = (event::EventHomelandInvaderInfoUpdate *)message;

  SocialActor *invader_social_actor = SocialActorManager::GetInstance()->GetFromAll(event->actor_id_);
  if (NULL == invader_social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", event->actor_id_);
    return;
  }
  HomelandActor *invader_homeland_actor = invader_social_actor->GetHomelandActor();

  gateway::protocol::MessageHomelandInvaderSynchronize sync;
  sync.__set_type_(gateway::protocol::HomelandInvaderOperateType::UPDATE);
  char string_id[32];
  snprintf(string_id, sizeof(string_id), "%lu", event->actor_id_);
  sync.data_.__set_actor_id_(string_id);

  if (event->__isset.level_) {
    sync.data_.__set_level_(event->level_);
  }
  if (event->__isset.can_visit_) {
    sync.data_.__set_can_visit_(event->can_visit_);
  }
  if (event->__isset.can_strike_back_) {
    sync.data_.__set_can_strike_back_(event->can_strike_back_);
  }

  this->social_actor_->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_HOMELAND_INVADER_SYNCHRONIZE);
}

///////////////////////////////////////////////////////////////////////////////

core::int32 HomelandActor::GetSoulLevel(core::uint32 soul_id) const {
  SoulLevelMap::const_iterator iter = this->souls_.find(soul_id);
  if (iter == this->souls_.end()) {
    LOG_ERROR("Get Soul(%u)'s level failed.", soul_id);
    return -1;
  }
  return iter->second;
}

void HomelandActor::UpdateSoulData(core::uint32 soul_id, core::int32 soul_level) {
  if (SOCIAL_CONF()->GetHomelandSoul(soul_id) == NULL) {
    return;
  }
  this->souls_[soul_id] = soul_level;
}

///////////////////////////////////////////////////////////////////////////////

void HomelandActor::DailyClean(bool synchronize) {
  this->temple_harvest_times_ = 0;
  this->temple_force_harvest_times_ = 0;
  this->goldmine_rob_times_ = 0;
  this->church_free_pray_times_ = 0;
  this->church_paid_pray_times_ = 0;
  this->church_free_refresh_times_ = 0;
  this->tree_assist_water_times_ = 0;
  this->tree_watered_actors_.clear();
  this->stronghold_support_times_ = 0;

  if (synchronize) {
    gateway::protocol::MessageHomelandSynchronize sync;
    sync.__set_temple_harvest_times_(this->temple_harvest_times_);
    sync.__set_temple_force_harvest_times_(this->temple_force_harvest_times_);
    sync.__set_goldmine_rob_times_(this->goldmine_rob_times_);
    sync.__set_church_free_pray_times_(this->church_free_pray_times_);
    sync.__set_church_paid_pray_times_(this->church_paid_pray_times_);
    sync.__set_church_free_refresh_times_(this->church_free_refresh_times_);
    sync.__set_tree_assist_water_times_(this->tree_assist_water_times_);
    sync.__set_stronghold_support_times_(this->stronghold_support_times_);
    this->social_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_HOMELAND_SYNCHRONIZE);
  }
}

void HomelandActor::OnTimer(core::uint32 id) {
  if (TIMER_ID_GROWING_FINISH == id) {
    this->MoveTreeToHarvestStage();
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

