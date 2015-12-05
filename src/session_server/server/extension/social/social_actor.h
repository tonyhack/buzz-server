#ifndef __SESSION__SERVER__SOCIAL__SOCIAL__ACTOR__H
#define __SESSION__SERVER__SOCIAL__SOCIAL__ACTOR__H

#include <ctime>
#include <string>
#include <set>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"
#include "entity/contacts_types.h"
#include "session_server/server/transaction_lock.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/extension/social/homeland_actor.h"

namespace database { namespace protocol {
class StorageSocialActorField;
}}

namespace session {

namespace server {

namespace social {

class SocialActor : public core::Noncopyable {
 public:
  typedef std::set<core::uint64> ActorIDSet;
  typedef std::map<core::uint64, coresh::Communicator::Connection> EventConnectionMap;

  static const core::int32 kHomelandUpgradeLock = -1;

  SocialActor();
  ~SocialActor();

  bool Initialize(BriefActor *brief_actor,
      const database::protocol::StorageSocialActorField &db_field);
  void Finalize();
  void Online(SessionActor *session_actor);
  void Offline();
  void SendLoadFinishEvent();
  void Save();
  bool CheckOnline() const { return this->session_actor_ != NULL; }

  SessionActor *GetActor() const {
    return this->session_actor_;
  }
  core::uint64 GetActorID() const {
    return this->brief_actor_->GetID();
  }
  std::string GetStringID() const {
    return this->brief_actor_->GetStringID();
  }
  const std::string &GetName() const {
    return this->brief_actor_->GetName();
  }
  core::int32 GetLevel() const {
    return this->brief_actor_->GetLevel();
  }
  entity::VocationType::type GetVocation() const {
    return this->brief_actor_->GetVocation();
  }
  entity::GenderType::type GetGender() const {
    return this->brief_actor_->GetGender();
  }
  core::uint16 GetMap() const {
    return this->brief_actor_->GetMap();
  }
  core::int32 GetCurrentFightScore() const {
    return this->brief_actor_->GetCurrentFightScore();
  }
  time_t GetLastLogoutTime() const {
    return this->brief_actor_->GetLastLogoutTime();
  }
  bool CheckFunctionalityState(entity::FunctionalityType::type type) const {
    return this->brief_actor_->CheckFunctionalityState(type);
  }

  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    if (this->session_actor_) {
      session_actor_->SendMessage(message, type);
    }
  }
  template <typename T>
  void SendGameMessage(const T &message, session::protocol::GameMessageType::type type) {
    if (this->session_actor_) {
      session_actor_->SendGameMessage(message, type);
    }
  }

  // 开启功能
  bool EnableContactsFunc(bool synchronize = true);

  bool IsContacts(core::uint64 actor_id) const;
  bool IsFriend(core::uint64 actor_id) const;
  bool IsInBlacklist(core::uint64 actor_id) const;
  const ActorIDSet &GetFriends() const { return this->friends_; }
  inline const std::string &GetSignature() const { return this->signature_; }
  void SetSignature(const std::string &signature);
  bool AddContacts(const std::string &actor_name, entity::ContactsType::type contacts_type,
      bool synchronize = true, bool notify = true);
  bool AddContacts(core::uint64 actor_id, entity::ContactsType::type contacts_type,
      bool synchronize = true, bool notify = true);
  bool RemoveContacts(core::uint64 actor_id, entity::ContactsType::type contacts_type,
      bool synchronize = true);

  bool IsEnemy(core::uint64 actor_id);
  bool AddEnemy(const std::string &actor_name, bool synchronize = true);
  bool AddEnemy(core::uint64 actor_id,  bool synchronize = true);
  bool RemoveEnemy(core::uint64 actor_id, bool synchronize = true);

  // 家园
  HomelandActor *GetHomelandActor() { return &this->homeland_actor_; }

  inline time_t GetLastSaveTime() const { return this->last_save_time_; }

  // 事务锁
  TransactionLock *GetLock() { return &this->lock_; }

  // 每日清理
  void DailyClean(bool synchronize = true);

 private:
  void ContactsOnline();
  void ContactsOffline();
  void SendContactsInitToClient();
  void EnemyOnline();
  void EnemyOffline();
  void SendEnemyInitToClient();

  void FollowContactsInfoUpdateEvent(core::uint64 actor_id);
  void UnfollowContactsInfoUpdateEvent(core::uint64 actor_id);
  void OnEventSocialContactsInfoUpdate(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void FollowEnemyInfoUpdateEvent(core::uint64 actor_id);
  void UnfollowEnemyInfoUpdateEvent(core::uint64 actor_id);
  void OnEventSocialEnemyInfoUpdate(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  BriefActor *brief_actor_;
  SessionActor *session_actor_;

  // 联系人
  std::string signature_;
  ActorIDSet contacts_;
  ActorIDSet friends_;
  ActorIDSet blacklist_;
  EventConnectionMap contacts_event_conns_;
  // 仇人
  ActorIDSet enemies_;
  EventConnectionMap enemy_event_conns_;

  // 家园
  HomelandActor homeland_actor_;

  // 上次保存时间
  time_t last_save_time_;

  // 事务锁
  TransactionLock lock_;
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__SOCIAL__ACTOR__H

