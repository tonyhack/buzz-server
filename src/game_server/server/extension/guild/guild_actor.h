#ifndef __GAME__SERVER__GUILD__GUILD__ACTOR__H
#define __GAME__SERVER__GUILD__GUILD__ACTOR__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "game_server/server/game_actor.h"

namespace session { namespace protocol {
class GuildActorData;
}}

namespace game {

namespace server {

namespace guild {

class GuildActor : public core::Noncopyable {
 public:
  typedef std::map<core::int32, core::int32> GuildSkillLevelMap;
  typedef std::set<core::int32> GuildBuffSet;

  GuildActor();
  ~GuildActor();

  bool Initialize(GameActor *game_actor, const session::protocol::GuildActorData &data);
  void InitializeGuildBuff();
  void Finalize();
  void SendLoadFinishEvent();

  GameActor *GetActor() const {
    return this->game_actor_;
  }
  core::uint64 GetID() const {
    return this->game_actor_->GetID();
  }
  std::string GetStringID() const {
    return this->game_actor_->GetStringID();
  }
  const std::string &GetName() const {
    return this->game_actor_->GetName();
  }

  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    this->game_actor_->SendMessage(message, type);
  }

  core::uint64 GetGuildID() const {
    return this->guild_id_;
  }
  void SetGuildID(core::uint64 guild_id) {
    this->guild_id_ = guild_id;
  }

  std::string GetGuildName() const;

  core::int32 GetGuildPosition() const {
    return this->guild_position_;
  }
  void SetGuildPosition(core::int32 guild_position) {
    this->guild_position_ = guild_position;
  }

  core::int32 GetCurrentContributionValue() const {
    return this->current_contribution_value_;
  }
  void SetCurrentContributionValue(core::int32 current_contribution_value) {
    this->current_contribution_value_ = current_contribution_value;
  }

  void SetGuildSkillLevel(core::int32 id, core::int32 level, bool synchronize = true);
  void ClearGuildSkill(bool synchronize = true);

  bool CheckGuildBuffExists(core::int32 id) const;
  bool AddGuildBuff(core::int32 id);
  bool RemoveGuildBuff(core::int32 id);
  void ClearGuildBuff();

  void UpdateGuildSkillFightScore(bool synchronize = true);

 private:
  void AddGuildSkillAttr(core::int32 id, core::int32 level, bool synchronize = true);
  void RemoveGuildSkillAttr(core::int32 id, core::int32 level, bool synchronize = true);

  GameActor *game_actor_;
  core::uint64 guild_id_;
  core::int32 guild_position_;
  core::int32 current_contribution_value_;
  GuildSkillLevelMap effective_guild_skills_;
  GuildBuffSet guild_buffs_;
};

}  // namespace guild

}  // namespace server

}  // namespace game 

#endif  // __GAME__SERVER__GUILD__GUILD__ACTOR__H

