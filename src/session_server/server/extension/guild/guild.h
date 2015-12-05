#ifndef __SESSION__SERVER__GUILD__GUILD__H
#define __SESSION__SERVER__GUILD__GUILD__H

#include <ctime>
#include <deque>
#include <set>
#include <string>
#include <vector>
#include <ext/hash_map>

#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"
#include "entity/guild_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "session_server/server/session_server.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_playing_event_types.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_event.h"

namespace database { namespace protocol {
class StorageGuildField;
}}

namespace session {

namespace server {

namespace guild {

class Guild : public core::TimerHandler, public core::Noncopyable {
  enum { TIMER_ID_SAVE = 0 };

 public:
  typedef std::deque<core::uint64> ActorIDDeque;
  typedef std::set<GuildActor *> GuildActorSet;
  typedef __gnu_cxx::hash_map<core::int32, GuildActorSet> GuildActorSetHashmap;
  typedef std::deque<GuildEvent> GuildEventDeque;
  typedef std::set<core::int32> GuildPlayingGroupSet;
  typedef std::set<core::int32> GuildPlayingSet;

  Guild();
  ~Guild();

  bool Initialize(core::uint64 guild_id, const std::string &guild_name,
      core::int32 guild_logo, GuildActor *leader);
  bool Initialize(const database::protocol::StorageGuildField &db_field);
  void Finalize();
  bool CheckDataValid();
  void Save(database::protocol::StorageGuildField &db_field);
  void Save();

  // 广播
  template <typename T>
  void BroadcastMessage(const T &message, gateway::protocol::MessageType::type type);

  // 公会基本属性
  core::uint64 GetID() const { return this->id_; }
  const std::string GetStringID() const;
  const std::string &GetName() const { return this->name_; }
  core::int32 GetLogo() const { return this->logo_; }
  core::int32 GetLevel() const { return this->level_; }
  core::int32 GetPendingMemberNum() const { return this->pending_members_.size(); }
  core::int32 GetMemberNum() const { return this->members_.size(); }
  time_t GetLastSaveTime() const { return this->last_save_time_; }
  GuildActor *GetLeader() const { return this->leader_; }
  void SetLevel(core::int32 new_level, bool synchronize = true);
  core::int64 GetTotalFightScore() const;

  // 公会成员管理
  bool CheckSuppressJoin() const { return this->suppress_join_; }
  void SetSuppressJoin(bool suppress_join, bool synchronize = true);
  bool CheckPendingMemberFull();
  bool CheckMemberFull();
  bool CheckInvitedMember(core::uint64 actor_id);
  void AddInviteMember(core::uint64 actor_id);
  void RemoveInviteMember(core::uint64 actor_id);
  const GuildActorSet &GetPendingMembers() const { return this->pending_members_; }
  const GuildActorSet &GetMembers() const { return this->members_; }
  bool AddPendingMemberDirect(GuildActor *pending_member);
  bool AddMemberDirect(GuildActor *member);
  bool AddPendingMember(GuildActor *pending_member, bool synchronize = true);
  bool RemovePendingMember(GuildActor *pending_member, bool synchronize = true);
  bool AddMember(GuildActor *member, bool synchronize = true);
  bool RemoveMember(GuildActor *member, bool synchronize = true);
  bool CheckAutoDismiss() const;
  void Dismiss();
  bool SetGuildPosition(GuildActor *member, core::int32 guild_position, bool synchronize = true);
  void SetLeader(GuildActor *new_leader, bool synchronize = true);

  // 公会公告
  const std::string &GetAnnouncement() const { return this->announcement_; }
  void SetAnnouncement(const std::string &new_announcement, bool synchronize = true);

  // 公会事件
  const GuildEventDeque &GetEvents() const { return this->events_; }
  void AddEvent(const GuildEvent &event, bool synchronize = true);

  // 公会资源
  core::int32 GetGold() const { return this->gold_; }
  core::int32 GetLightCrystal() const { return this->light_crystal_; }
  core::int32 GetDarkCrystal() const { return this->dark_crystal_; }
  core::int32 GetHolyCrystal() const { return this->holy_crystal_; }
  void SetGold(core::int32 gold, bool synchronize = true);
  void SetLightCrystal(core::int32 light_crystal, bool synchronize = true);
  void SetDarkCrystal(core::int32 dark_crystal, bool synchronize = true);
  void SetHolyCrystal(core::int32 holy_crystal, bool synchronize = true);
  void Donate(GuildActor *member, entity::GuildDonateType::type type, core::int32 num,
      bool synchronize = true);

  // 公会属性盘
  core::int32 GetLightAttribute() const { return this->light_attribute_; }
  core::int32 GetDarkAttribute() const { return this->dark_attribute_; }
  core::int32 GetHolyAttribute() const { return this->holy_attribute_; }
  void SetLightAttribute(core::int32 light_attribute, bool synchronize = true);
  void SetDarkAttribute(core::int32 dark_attribute, bool synchronize = true);
  void SetHolyAttribute(core::int32 holy_attribute, bool synchronize = true);

  core::int32 GetAttributeSum() const {
    return this->light_attribute_ + this->dark_attribute_ + this->holy_attribute_;
  }

  // 公会商店
  core::int32 GetShopLevel() const { return this->shop_level_; }
  void SetShopLevel(core::int32 new_level, bool synchronize = true);

  // 公会学院
  core::int32 GetCollegeLevel() const { return this->college_level_; }
  void SetCollegeLevel(core::int32 new_level, bool synchronize = true);

  // 公会军营
  core::int32 GetBarrackLevel() const { return this->barrack_level_; }
  void SetBarrackLevel(core::int32 new_level, bool synchronize = true);

  // 公会副本
  const GuildPlayingGroupSet &GetOpenedPlayingGroups() const {
    return this->opened_playing_groups_;
  }
  const GuildPlayingSet &GetFinishedPlayings() const {
    return this->finished_playings_;
  }
  core::uint64 GetCurrentPlayingID() const {
    return this->current_playing_id_;
  }
  core::int32 GetCurrentPlayingTemplateID() const {
    return this->current_playing_template_id_;
  }
  bool CheckPlayingGroupOpened(core::int32 playing_group_id) const;
  bool CheckPlayingFinished(core::int32 playing_template_id) const;
  bool CheckCanCreatePlaying(core::int32 playing_template_id) const;
  void OpenPlayingGroup(core::int32 playing_group_id, bool synchronize = true);
  void SetPlayingFinished(core::int32 playing_template_id, bool synchronize = true);
  void SetCurrentPlaying(core::uint64 playing_id, core::int32 playing_template_id,
      bool synchronize = true);
  void ResetPlaying(bool synchronize = true);

  // 定时器
  virtual void OnTimer(core::uint32 id);
  void DailyClean();
  void WeeklyClean();
  void ClearGuildBuff();

 private:
  void AddTimer();
  void RemoveTimer();
  void InitPositionStatistics();

  void OnEventPlayingEnd(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingDestroy(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  core::uint64 id_;
  std::string name_;
  core::int32 logo_;
  core::int32 level_;
  bool suppress_join_;
  ActorIDDeque invite_members_;
  GuildActorSet pending_members_;
  GuildActorSet members_;
  GuildActorSetHashmap position_statistics_;
  GuildActor* leader_;
  std::string announcement_;
  GuildEventDeque events_;
  core::int32 gold_;
  core::int32 light_crystal_;
  core::int32 dark_crystal_;
  core::int32 holy_crystal_;
  core::int32 light_attribute_;
  core::int32 dark_attribute_;
  core::int32 holy_attribute_;
  core::int32 shop_level_;
  core::int32 college_level_;
  core::int32 barrack_level_;
  GuildPlayingGroupSet opened_playing_groups_;
  GuildPlayingSet finished_playings_;
  core::uint64 current_playing_id_;
  core::int32 current_playing_template_id_;
  coresh::Communicator::Connection playing_end_conn_;
  coresh::Communicator::Connection playing_destroy_conn_;
  time_t last_save_time_;
};

template <typename T>
void Guild::BroadcastMessage(const T &message, gateway::protocol::MessageType::type type) {
  std::vector<core::uint64> actors;

  for (GuildActorSet::iterator iter = this->members_.begin();
       iter != this->members_.end(); ++iter) {
    GuildActor *guild_actor = *iter;
    if (NULL == guild_actor) {
      continue;
    }
    if (guild_actor->CheckOnline() == false) {
      continue;
    }
    actors.push_back(guild_actor->GetActorID());
  }
  SessionServerSingleton::GetInstance().MulticastMessage(message, type,
      &actors[0], actors.size());
}

}  // namespace guild

}  // namespace server

}  // namespace session

#endif

