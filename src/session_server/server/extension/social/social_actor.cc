#include "session_server/server/extension/social/social_actor.h"

#include <cstddef>
#include <cstdio>
#include <boost/bind.hpp>

#include "entity/functionality_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_social_actor_login_types.h"
#include "database_server/protocol/storage_social_actor_save_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/server/extension_types.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_social_event_types.h"
#include "session_server/server/extension/social/social_actor_manager.h"

namespace session {

namespace server {

namespace social {

static void DecodeActorIDSet(const std::string &source, SocialActor::ActorIDSet *result) {
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

static void EncodeActorIDSet(const SocialActor::ActorIDSet &source, std::string *result) {
  char buffer[2048] = "";
  size_t count = 0;

  SocialActor::ActorIDSet::const_iterator iter = source.begin();
  for (; iter != source.end(); ++iter) {
    count += snprintf(buffer + count, sizeof(buffer) - count, "%lu,", *iter);
  }

  *result = buffer;
}

static void GetContactsInfo(core::uint64 actor_id, entity::ContactsType::type contacts_type,
    gateway::protocol::ContactsData *data) {
  char string_id[32];
  snprintf(string_id, sizeof(string_id), "%lu", actor_id);

  data->__set_actor_id_(string_id);
  data->__set_contacts_type_(contacts_type);


  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_id);
  if (NULL == brief_actor) {
    LOG_ERROR("actor_id(%lu) does not exist.", actor_id);
    return;
  }
  data->__set_actor_name_(brief_actor->GetName());
  data->__set_level_(brief_actor->GetLevel());
  data->__set_gender_(brief_actor->GetGender());
  data->__set_vocation_(brief_actor->GetVocation());

  if (entity::ContactsType::BLACKLIST == contacts_type) {
    return;
  }
  data->__set_map_(brief_actor->GetMap());
  data->__set_last_logout_time_(brief_actor->GetStringLastLogoutTime());

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    data->__set_online_(false);
    return;
  }
  data->__set_online_(true);
  data->__set_signature_(social_actor->GetSignature());
}

static void GetEnemyInfo(core::uint64 actor_id, gateway::protocol::SocialEnemyData *data) {
  char string_id[32];
  snprintf(string_id, sizeof(string_id), "%lu", actor_id);

  data->__set_actor_id_(string_id);
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_id);
  if (NULL == brief_actor) {
    LOG_ERROR("actor_id(%lu) does not exist.", actor_id);
    return;
  }
  data->__set_actor_name_(brief_actor->GetName());
  data->__set_level_(brief_actor->GetLevel());
  data->__set_gender_(brief_actor->GetGender());
  data->__set_vocation_(brief_actor->GetVocation());
  data->__set_map_(brief_actor->GetMap());

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(actor_id);
  if (NULL == social_actor) {
    data->__set_online_(false);
    return;
  }
  data->__set_online_(true);
}

///////////////////////////////////////////////////////////////////////////////

SocialActor::SocialActor() : brief_actor_(NULL), session_actor_(NULL), last_save_time_(0) {}
SocialActor::~SocialActor() {}

bool SocialActor::Initialize(BriefActor *brief_actor,
    const database::protocol::StorageSocialActorField &db_field) {
  if (NULL == brief_actor) {
    LOG_ERROR("brief_actor is null.");
    return false;
  }

  this->brief_actor_ = brief_actor;

  // 联系人初始化
  this->signature_ = db_field.signature_;
  DecodeActorIDSet(db_field.friends_, &this->friends_);
  DecodeActorIDSet(db_field.blacklist_, &this->blacklist_);
  this->contacts_.insert(this->friends_.begin(), this->friends_.end());
  this->contacts_.insert(this->blacklist_.begin(), this->blacklist_.end());

  // 仇人初始化
  DecodeActorIDSet(db_field.enemies_, &this->enemies_);
  
  // 家园初始化
  if (this->homeland_actor_.Initialize(this, db_field) == false) {
    LOG_ERROR("Init HomelandActor(%lu) failed.", this->GetActorID());
    return false;
  }

  // 上次保存时间
  this->last_save_time_ = db_field.last_save_time_;

  return true;
}

void SocialActor::Finalize() {
  this->last_save_time_ = 0;
  this->homeland_actor_.Finalize();
  this->enemy_event_conns_.clear();
  this->enemies_.clear();
  this->contacts_event_conns_.clear();
  this->blacklist_.clear();
  this->friends_.clear();
  this->signature_.clear();
  this->session_actor_ = NULL;
  this->brief_actor_ = NULL;
}

void SocialActor::SendContactsInitToClient() {
  gateway::protocol::MessageSocialContactsInitialize message;
  message.__set_signature_(this->signature_);

  ActorIDSet::const_iterator iter = this->friends_.begin();
  iter = this->friends_.begin();
  for (; iter != this->friends_.end(); ++iter) {
    gateway::protocol::ContactsData data;
    GetContactsInfo(*iter, entity::ContactsType::FRIEND, &data);
    message.contacts_.push_back(data);
  }
  iter = this->blacklist_.begin();
  for (; iter != this->blacklist_.end(); ++iter) {
    gateway::protocol::ContactsData data;
    GetContactsInfo(*iter, entity::ContactsType::BLACKLIST, &data);
    message.contacts_.push_back(data);
  }

  this->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_INITIALIZE);
}


void SocialActor::ContactsOnline() {
  // Follow 好友信息更新事件
  ActorIDSet::const_iterator iter = this->friends_.begin();
  for (; iter != this->friends_.end(); ++iter) {
    this->FollowContactsInfoUpdateEvent(*iter);
  }

  // 发送信息更新事件
  event::EventSocialContactsInfoUpdate event;
  event.__set_actor_id_(this->GetActorID());
  event.__set_online_(true);
  event.__set_signature_(this->signature_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOCIAL_CONTACTS_INFO_UPDATE, this->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event,
      sizeof(event::EventSocialContactsInfoUpdate));

  // 未开启功能返回
  if (this->session_actor_->CheckFunctionalityState(
          entity::FunctionalityType::CONTACTS_MODULE) == false) {
    return;
  }

  // 发送初始化数据
  this->SendContactsInitToClient();
}

void SocialActor::ContactsOffline() {
  // Unfollow 好友信息更新事件
  ActorIDSet::const_iterator iter = this->friends_.begin();
  for (; iter != this->friends_.end(); ++iter) {
    this->UnfollowContactsInfoUpdateEvent(*iter);
  }

  // 发送信息更新事件
  event::EventSocialContactsInfoUpdate event;
  event.__set_actor_id_(this->GetActorID());
  event.__set_online_(false);
  event.__set_signature_("");
  if (this->brief_actor_ != NULL) {
    event.__set_last_logout_time_(this->brief_actor_->GetStringLastLogoutTime());
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOCIAL_CONTACTS_INFO_UPDATE, this->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event,
      sizeof(event::EventSocialContactsInfoUpdate));
}

void SocialActor::SendEnemyInitToClient() {
  gateway::protocol::MessageSocialEnemyInitialize message;

  ActorIDSet::const_iterator iter = this->enemies_.begin();
  for (; iter != this->enemies_.end(); ++iter) {
    gateway::protocol::SocialEnemyData data;
    GetEnemyInfo(*iter, &data);
    message.enemies_.push_back(data);
  }

  this->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_INITIALIZE);
}

void SocialActor::EnemyOnline() {
  // Follow 仇人信息更新事件
  ActorIDSet::const_iterator iter = this->enemies_.begin();
  for (; iter != this->enemies_.end(); ++iter) {
    this->FollowEnemyInfoUpdateEvent(*iter);
  }

  // 发送信息更新事件
  event::EventSocialEnemyInfoUpdate event;
  event.__set_actor_id_(this->GetActorID());
  event.__set_online_(true);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOCIAL_ENEMY_INFO_UPDATE, this->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event,
      sizeof(event::EventSocialEnemyInfoUpdate));

  // 未开启功能返回
  if (this->session_actor_->CheckFunctionalityState(
          entity::FunctionalityType::CONTACTS_MODULE) == false) {
    return;
  }

  // 发送初始化数据
  this->SendEnemyInitToClient();
}

void SocialActor::EnemyOffline() {
  // Unfollow 仇人信息更新事件
  ActorIDSet::const_iterator iter = this->enemies_.begin();
  for (; iter != this->enemies_.end(); ++iter) {
    this->UnfollowEnemyInfoUpdateEvent(*iter);
  }

  // 发送信息更新事件
  event::EventSocialEnemyInfoUpdate event;
  event.__set_actor_id_(this->GetActorID());
  event.__set_online_(false);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOCIAL_ENEMY_INFO_UPDATE, this->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event,
      sizeof(event::EventSocialEnemyInfoUpdate));
}

void SocialActor::Online(SessionActor *session_actor) {
  if (NULL == session_actor) {
    LOG_ERROR("session_actor is null.");
    return;
  }

  this->session_actor_ = session_actor;

  this->ContactsOnline();
  this->EnemyOnline();
  this->homeland_actor_.Online();
}

void SocialActor::Offline() {
  this->homeland_actor_.Offline();
  this->EnemyOffline();
  this->ContactsOffline();

  this->session_actor_ = NULL;
}

void SocialActor::SendLoadFinishEvent() {
  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(this->GetActorID());
  load_finish.__set_extension_type_(ExtensionType::SOCIAL);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, this->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &load_finish, sizeof(load_finish));
}

void SocialActor::Save() {
  database::protocol::StorageSocialActorSaveRequest request;
  request.__set_actor_id_(this->GetActorID());

  // 联系人
  request.field_.__set_signature_(this->signature_);
  EncodeActorIDSet(this->friends_, &request.field_.friends_);
  EncodeActorIDSet(this->blacklist_, &request.field_.blacklist_);

  // 仇人
  EncodeActorIDSet(this->enemies_, &request.field_.enemies_);

  // 家园
  this->homeland_actor_.Save(request.field_);

  // 上次保存时间
  this->last_save_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  request.field_.__set_last_save_time_(this->last_save_time_);

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_SOCIAL_ACTOR_SAVE,
          this->GetActorID()) == false) {
    LOG_ERROR("Send StorageSocialActorSaveRequest to database server failed.");
    return;
  }
}

bool SocialActor::EnableContactsFunc(bool synchronize) {
  if (synchronize) {
    this->SendContactsInitToClient();
    this->SendEnemyInitToClient();
  }

  return true;
}

bool SocialActor::IsContacts(core::uint64 actor_id) const {
  return this->contacts_.find(actor_id) != this->contacts_.end();
}

bool SocialActor::IsFriend(core::uint64 actor_id) const {
  return this->friends_.find(actor_id) != this->friends_.end();
}

bool SocialActor::IsInBlacklist(core::uint64 actor_id) const {
  return this->blacklist_.find(actor_id) != this->blacklist_.end();
}

void SocialActor::SetSignature(const std::string &signature) {
  this->signature_ = signature;

  // 发送信息更新事件
  event::EventSocialContactsInfoUpdate event;
  event.__set_actor_id_(this->GetActorID());
  event.__set_signature_(this->signature_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SOCIAL_CONTACTS_INFO_UPDATE, this->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event,
      sizeof(event::EventSocialContactsInfoUpdate));
}

bool SocialActor::AddContacts(const std::string &actor_name,
    entity::ContactsType::type contacts_type, bool synchronize, bool notify) {
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_name);
  if (NULL == brief_actor) {
    LOG_WARNING("actor_name(%s) does not exist.", actor_name.c_str());
    return false;
  }
  core::uint64 actor_id = brief_actor->GetID();

  return this->AddContacts(actor_id, contacts_type, synchronize, notify);
}

bool SocialActor::AddContacts(core::uint64 actor_id,
    entity::ContactsType::type contacts_type, bool synchronize, bool notify) {
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_id);
  if (NULL == brief_actor) {
    LOG_WARNING("Actor(%lu) does not exist.", actor_id);
    return false;
  }

  // 检查是否自己加自己
  if (this->GetActorID() == actor_id) {
    LOG_WARNING("Can not add yourself.");
    return false;
  }

  // 检查是否已在联系人中
  if (this->IsContacts(actor_id)) {
    LOG_WARNING("actor_name(%lu) is already in contacts list.", actor_id);
    return false;
  }

  if (entity::ContactsType::FRIEND == contacts_type) {
    // 检查好友添加上限
    if (this->friends_.size() >= (size_t)MISC_CONF()->contacts_max_friend_capacity_) {
      LOG_WARNING("Contacts_list[friends] is full.");
      return false;
    }
    this->friends_.insert(actor_id);
    this->contacts_.insert(actor_id);

  } else if (entity::ContactsType::BLACKLIST == contacts_type) {
    // 检查黑名单添加上限
    if (this->blacklist_.size() >= (size_t)MISC_CONF()->contacts_max_blacklist_capacity_) {
      LOG_WARNING("Contacts_list[blacklist] is full.");
      return false;
    }
    this->blacklist_.insert(actor_id);
    this->contacts_.insert(actor_id);

  } else {
    LOG_ERROR("Invalid contacts_type.");
    return false;
  }

  // Follow 好友信息更新事件
  if (entity::ContactsType::FRIEND == contacts_type) {
    this->FollowContactsInfoUpdateEvent(actor_id);
  }

  // 同步数据
  if (synchronize) {
    gateway::protocol::MessageSocialContactsSynchronize sync;
    sync.__set_type_(gateway::protocol::ContactsOperateType::ADD);
    GetContactsInfo(actor_id, contacts_type, &sync.contacts_);
    this->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_SYNCHRONIZE);
  }

  // 发送加好友通知
  if (notify) {
    if (entity::ContactsType::FRIEND == contacts_type) {
      SocialActor *target_social_actor = SocialActorManager::GetInstance()->Get(actor_id);
      // 对方在线并且不是联系人
      if (target_social_actor != NULL &&
          target_social_actor->IsContacts(this->GetActorID()) == false) {
        gateway::protocol::MessageSocialContactsAddNotify notify;
        notify.__set_actor_name_(this->GetName());
        target_social_actor->SendMessage(notify,
            gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_ADD_NOTIFY);
      }
    }
  }

  // 新增家园好友
  this->homeland_actor_.AddHomelandFriend(actor_id);

  return true;
}

bool SocialActor::RemoveContacts(core::uint64 actor_id, entity::ContactsType::type contacts_type,
    bool synchronize) {
  if (entity::ContactsType::FRIEND == contacts_type) {
    if (this->IsFriend(actor_id) == false) {
      LOG_WARNING("Actor(%lu) is not friend.", actor_id);
      return false;
    }
    this->friends_.erase(actor_id);
    this->contacts_.erase(actor_id);

  } else if (entity::ContactsType::BLACKLIST == contacts_type) {
    if (this->IsInBlacklist(actor_id) == false) {
      LOG_WARNING("Actor(%lu) is not in blacklist.", actor_id);
      return false;
    }
    this->blacklist_.erase(actor_id);
    this->contacts_.erase(actor_id);

  } else {
    LOG_ERROR("Invalid contacts_type.");
    return false;
  }

  // Unfollow 好友信息更新事件
  if (entity::ContactsType::FRIEND == contacts_type) {
    this->UnfollowContactsInfoUpdateEvent(actor_id);
  }

  // 同步数据
  if (synchronize) {
    char string_id[32];
    snprintf(string_id, sizeof(string_id), "%lu", actor_id);

    gateway::protocol::MessageSocialContactsSynchronize sync;
    sync.__set_type_(gateway::protocol::ContactsOperateType::REMOVE);
    sync.contacts_.__set_actor_id_(string_id);
    sync.contacts_.__set_contacts_type_(contacts_type);
    this->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_SYNCHRONIZE);
  }

  // 移除家园好友
  this->homeland_actor_.RemoveHomelandFriend(actor_id);

  return true;
}

bool SocialActor::IsEnemy(core::uint64 actor_id) {
  return this->enemies_.find(actor_id) != this->enemies_.end();
}

bool SocialActor::AddEnemy(const std::string &actor_name, bool synchronize) {
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_name);
  if (NULL == brief_actor) {
    LOG_WARNING("actor_name(%s) does not exist.", actor_name.c_str());
    return false;
  }
  core::uint64 actor_id = brief_actor->GetID();

  return this->AddEnemy(actor_id, synchronize);
}

bool SocialActor::AddEnemy(core::uint64 actor_id,  bool synchronize) {
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_id);
  if (NULL == brief_actor) {
    LOG_WARNING("actor_name(%lu) does not exist.", actor_id);
    return false;
  }

  // 检查是否自己加自己
  if (this->GetActorID() == actor_id) {
    LOG_WARNING("Can not add yourself.");
    return false;
  }

  // 检查是否已在仇人列表中
  if (this->IsEnemy(actor_id)) {
    LOG_WARNING("actor_name(%lu) is already in enemy list.", actor_id);
    return false;
  }

  // 检查仇人添加上限
  if (this->enemies_.size() >= (size_t)MISC_CONF()->social_enemy_max_capacity_) {
    LOG_WARNING("Enemy list is full.");
    return false;
  }
  this->enemies_.insert(actor_id);

  // Follow 仇人信息更新事件
  this->FollowEnemyInfoUpdateEvent(actor_id);

  // 同步数据
  if (synchronize) {
    gateway::protocol::MessageSocialEnemySynchronize sync;
    sync.__set_type_(gateway::protocol::SocialEnemyOperateType::ADD);
    GetEnemyInfo(actor_id, &sync.enemy_);
    this->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_SYNCHRONIZE);
  }

  return true;
}

bool SocialActor::RemoveEnemy(core::uint64 actor_id, bool synchronize) {
  if (this->IsEnemy(actor_id) == false) {
    LOG_WARNING("Actor(%lu) is not in enemy list.", actor_id);
    return false;
  }
  this->enemies_.erase(actor_id);

  // Unfollow 仇人信息更新事件
  this->UnfollowEnemyInfoUpdateEvent(actor_id);

  // 同步数据
  if (synchronize) {
    char string_id[32];
    snprintf(string_id, sizeof(string_id), "%lu", actor_id);

    gateway::protocol::MessageSocialEnemySynchronize sync;
    sync.__set_type_(gateway::protocol::SocialEnemyOperateType::REMOVE);
    sync.enemy_.__set_actor_id_(string_id);
    this->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_SYNCHRONIZE);
  }

  return true;
}

void SocialActor::FollowContactsInfoUpdateEvent(core::uint64 actor_id) {
  coresh::Communicator::Connection conn =
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_SOCIAL_CONTACTS_INFO_UPDATE, actor_id,
        event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&SocialActor::OnEventSocialContactsInfoUpdate, this, _1, _2, _3, _4));
  this->contacts_event_conns_.insert(std::make_pair(actor_id, conn));
}

void SocialActor::UnfollowContactsInfoUpdateEvent(core::uint64 actor_id) {
  EventConnectionMap::const_iterator iter = this->contacts_event_conns_.find(actor_id);
  if (iter == this->contacts_event_conns_.end()) {
    LOG_ERROR("Event connection(%lu) not found.", actor_id);
    return;
  }
  coresh::CommunicatorSingleton::GetInstance().Unfollow(iter->second);
  this->contacts_event_conns_.erase(actor_id);
}

void SocialActor::OnEventSocialContactsInfoUpdate(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventSocialContactsInfoUpdate) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventSocialContactsInfoUpdate *event = (event::EventSocialContactsInfoUpdate *)message;

  char string_id[32];
  snprintf(string_id, sizeof(string_id), "%lu", event->actor_id_);

  gateway::protocol::MessageSocialContactsSynchronize sync;
  sync.__set_type_(gateway::protocol::ContactsOperateType::UPDATE);
  sync.contacts_.__set_actor_id_(string_id);
  sync.contacts_.__set_contacts_type_(entity::ContactsType::FRIEND);

  if (event->__isset.online_) {
    sync.contacts_.__set_online_(event->online_);
  }
  if (event->__isset.level_) {
    sync.contacts_.__set_level_(event->level_);
  }
  if (event->__isset.gender_) {
    sync.contacts_.__set_gender_(event->gender_);
  }
  if (event->__isset.vocation_) {
    sync.contacts_.__set_vocation_(event->vocation_);
  }
  if (event->__isset.map_) {
    sync.contacts_.__set_map_(event->map_);
  }
  if (event->__isset.last_logout_time_) {
    sync.contacts_.__set_last_logout_time_(event->last_logout_time_);
  }
  if (event->__isset.signature_) {
    sync.contacts_.__set_signature_(event->signature_);
  }

  this->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_SYNCHRONIZE);
}

void SocialActor::FollowEnemyInfoUpdateEvent(core::uint64 actor_id) {
  coresh::Communicator::Connection conn =
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_SOCIAL_ENEMY_INFO_UPDATE, actor_id,
        event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&SocialActor::OnEventSocialEnemyInfoUpdate, this, _1, _2, _3, _4));
  this->enemy_event_conns_.insert(std::make_pair(actor_id, conn));
}

void SocialActor::UnfollowEnemyInfoUpdateEvent(core::uint64 actor_id) {
  EventConnectionMap::const_iterator iter = this->enemy_event_conns_.find(actor_id);
  if (iter == this->enemy_event_conns_.end()) {
    LOG_ERROR("Event connection(%lu) not found.", actor_id);
    return;
  }
  coresh::CommunicatorSingleton::GetInstance().Unfollow(iter->second);
  this->enemy_event_conns_.erase(actor_id);
}

void SocialActor::OnEventSocialEnemyInfoUpdate(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventSocialEnemyInfoUpdate) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventSocialEnemyInfoUpdate *event = (event::EventSocialEnemyInfoUpdate *)message;

  char string_id[32];
  snprintf(string_id, sizeof(string_id), "%lu", event->actor_id_);

  gateway::protocol::MessageSocialEnemySynchronize sync;
  sync.__set_type_(gateway::protocol::SocialEnemyOperateType::UPDATE);
  sync.enemy_.__set_actor_id_(string_id);

  if (event->__isset.online_) {
    sync.enemy_.__set_online_(event->online_);
  }
  if (event->__isset.level_) {
    sync.enemy_.__set_level_(event->level_);
  }
  if (event->__isset.gender_) {
    sync.enemy_.__set_gender_(event->gender_);
  }
  if (event->__isset.vocation_) {
    sync.enemy_.__set_vocation_(event->vocation_);
  }
  if (event->__isset.map_) {
    sync.enemy_.__set_map_(event->map_);
  }

  this->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_ENEMY_SYNCHRONIZE);
}

void SocialActor::DailyClean(bool synchronize) {
  this->homeland_actor_.DailyClean(synchronize);
}

}  // namespace social

}  // namespace server

}  // namespace session

