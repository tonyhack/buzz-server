#ifndef __SESSION__SERVER__GUILD__GUILD__ACTOR__H
#define __SESSION__SERVER__GUILD__GUILD__ACTOR__H

#include <map>
#include <set>
#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/guild_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor.h"

namespace database { namespace protocol {
class StorageGuildActorField;
}}

namespace session {

namespace server {

namespace guild {

class Guild;

class GuildActor : public core::Noncopyable {
 public:
  typedef std::set<Guild *> GuildSet;
  typedef std::map<core::int32, core::int32> GuildSkillLevelMap;
  typedef std::set<core::int32> GuildBuffSet;
  typedef std::set<core::int32> GuildPlayingSet;

  GuildActor();
  ~GuildActor();

  bool Initialize(BriefActor *brief_actor,
      const database::protocol::StorageGuildActorField &db_field);
  void Finalize();
  void Online(SessionActor *session_actor);
  void Offline();
  void SendInitToGameServer();
  void SendLoadFinishEvent();
  void Save(database::protocol::StorageGuildActorField &db_field);
  void Save();

  bool CheckDirty() const { return this->dirty_; }
  void SetDirty(bool dirty = true) { this->dirty_ = dirty; }
  bool CheckOnline() const { return this->session_actor_ != NULL; }
  void SendOnlineStateSync(bool online);
  void SendLevelSync();
  void SendMapSync();
  void SendFightScoreSync();

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
  core::uint16 GetMap() const {
    return this->brief_actor_->GetMap();
  }
  core::int32 GetCurrentFightScore() const {
    return this->brief_actor_->GetCurrentFightScore();
  }
  core::int32 GetFightScore(entity::FightScoreType::type type) const {
    return this->brief_actor_->GetFightScore(type);
  }
  std::string GetStringLastLogoutTime() const {
    return this->brief_actor_->GetStringLastLogoutTime();
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

  // 开启模块功能
  bool EnableGuildFunc(bool synchronize = true);

  Guild *GetGuild() const { return this->guild_; }
  core::uint64 GetGuildID() const;
  const std::string GetGuildName() const;
  void SetGuildDirect(Guild *guild);
  void SetGuild(Guild *guild, entity::GuildPositionType::type guild_position,
      bool synchronize = true);

  core::int32 GetJoinedGuildNum() const { return this->joined_guilds_.size(); }
  bool CheckJoinedGuild(Guild *guild) const;
  bool AddJoinedGuild(Guild *guild, bool synchronize = true);
  bool RemoveJoinedGuild(Guild *guild, bool synchronize = true);
  void RemoveAllJoinedGuild(bool synchronize = true);

  core::int32 GetGuildPosition() const { return this->guild_position_; }
  void SetGuildPosition(core::int32 guild_position, bool synchronize = true);

  // 公会贡献
  core::int32 GetCurrentContributionValue() const { return this->current_contribution_value_; }
  core::int32 GetTotalContributionValue() const { return this->total_contribution_value_; }
  core::int32 GetGoldContribution() const { return this->gold_contribution_; }
  core::int32 GetFreedomDollarsContribution() const { return this->freedom_dollars_contribution_; }
  core::int32 GetLightCrystalContribution() const { return this->light_crystal_contribution_; }
  core::int32 GetDarkCrystalContribution() const { return this->dark_crystal_contribution_; }
  core::int32 GetHolyCrystalContribution() const { return this->holy_crystal_contribution_; }
  void AddContribution(core::int32 contribution, bool synchronize = true);
  void DelContribution(core::int32 contribution, bool synchronize = true);
  void AddGoldContribution(core::int32 contribution, bool synchronize = true);
  void AddFreedomDollarsContribution(core::int32 contribution, bool synchronize = true);
  void AddLightCrystalContribution(core::int32 contribution, bool synchronize = true);
  void AddDarkCrystalContribution(core::int32 contribution, bool synchronize = true);
  void AddHolyCrystalContribution(core::int32 contribution, bool synchronize = true);

  // 公会技能
  core::int32 GetGuildSkillLevel(core::int32 id) const;
  void SetGuildSkillLevel(core::int32 id, core::int32 level, bool synchronize = true);
  void UpdateEffectiveGuildSkillLevel(core::int32 id,
      bool synchronize_game = true, bool synchronize_client = true);
  void UpdateEffectiveGuildSkillLevel(bool synchronize_game = true, bool synchronize_client = true);

  // 公会BUFF
  core::int32 GetGuildBuffNum() const { return this->guild_buffs_.size(); }
  bool CheckGuildBuffExists(core::int32 id) const;
  bool AddGuildBuff(core::int32 id, bool synchronize = true);
  void ClearGuildBuff(bool synchronize = true);

  // 公会副本
  bool CheckPlayingAwarded(core::int32 playing_template_id) const;
  void SetPlayingAwarded(core::int32 playing_template_id);

  // 每周清理
  void WeeklyClean();

 private:
  void SendInitToClient();

  BriefActor *brief_actor_;
  SessionActor *session_actor_;

  bool dirty_;
  Guild *guild_;
  GuildSet joined_guilds_; // 申请的公会
  core::uint64 guild_id_; // 退出公会时guild_id_保留, guild_指针将为空
  core::int32 guild_position_;
  core::int32 current_contribution_value_;
  core::int32 total_contribution_value_;
  core::int32 gold_contribution_;
  core::int32 freedom_dollars_contribution_;
  core::int32 light_crystal_contribution_;  
  core::int32 dark_crystal_contribution_;
  core::int32 holy_crystal_contribution_;
  GuildSkillLevelMap guild_skills_;
  GuildSkillLevelMap effective_guild_skills_;
  GuildBuffSet guild_buffs_;
  GuildPlayingSet awarded_guild_playings_;
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__GUILD__ACTOR__H

