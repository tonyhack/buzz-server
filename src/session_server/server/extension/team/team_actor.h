//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 13:45:14.
// File name: team_actor.h
//
// Description:
// Define class TeamActor.
//

#ifndef __SESSION__SERVER__TEAM__TEAM_ACTOR__H
#define __SESSION__SERVER__TEAM__TEAM_ACTOR__H

#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/extension/team/team_commands.h"

namespace session {

namespace server {

namespace team {

class TeamActor : public core::Noncopyable {
 public:
  TeamActor();
  ~TeamActor();

  bool Initialize(SessionActor *actor);
  void Finalize();

  inline SessionActor *GetActor() {
    return this->actor_;
  }

  // 获取 玩家ID
  inline core::uint64 GetID() const {
    if(this->actor_) {
      return this->actor_->GetActorID();
    }
    return 0;
  }

  // 获取 玩家名称
  inline const std::string GetName() const {
    if(this->actor_) {
      return this->actor_->GetName();
    }
    return std::string();
  }

  // 获取玩家等级
  inline core::int32 GetLevel() const {
    if(this->actor_) {
      return this->actor_->GetLevel();
    }
    return 0;
  }

  // 获取玩家所在地图
  inline core::int32 GetMap() const {
    if(this->actor_) {
      return this->actor_->GetMap();
    }
    return 0;
  }

  // 获取/设置 队列ID
  inline core::uint64 GetTeamID() const {
    return this->team_id_;
  }
  inline void SetTeamID(core::uint64 id) {
    this->team_id_ = id;
  }

  // 获取/设置 生命值
  inline core::int32 GetCurrentHp() const {
    return this->current_hp_;
  }
  inline void SetCurrentHp(core::int32 value) {
    this->current_hp_ = value;
  }
  inline core::int32 GetMaxHp() const {
    return this->max_hp_;
  }
  inline void SetMaxHp(core::int32 value) {
    this->max_hp_ = value;
  }

  // 获取/设置 魔法值
  inline core::int32 GetCurrentMp() const {
    return this->current_mp_;
  }
  inline void SetCurrentMp(core::int32 value) {
    this->current_mp_ = value;
  }
  inline core::int32 GetMaxMp() const {
    return this->max_mp_;
  }
  inline void SetMaxMp(core::int32 value) {
    this->max_mp_ = value;
  }

  // 获取/设置 坐标
  inline core::int32 GetLocationX() const {
    return this->x_;
  }
  inline core::int32 GetLocationY() const {
    return this->y_;
  }
  inline void SetLocationX(core::int32 value) {
    this->x_ = value;
  }
  inline void SetLocationY(core::int32 value) {
    this->y_ = value;
  }

  // 获取命令列表
  inline TeamCommands &GetCommands() {
    return this->commands_;
  }

  inline void SendMessage(gateway::protocol::MessageType::type type,
      const char *message, size_t size) {
    if(this->actor_) {
      this->actor_->SendMessage(type, message, size);
    }
  }

  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type);

  // 同步组队BUFF
  void SynchronizeBuff(core::uint32 buff);

 private:
  SessionActor *actor_;

  // 队伍ID
  core::uint64 team_id_;

  // 冗余的玩家生命与魔法
  core::int32 current_hp_;
  core::int32 max_hp_;
  core::int32 current_mp_;
  core::int32 max_mp_;
  core::int32 x_;
  core::int32 y_;

  // 命令列表
  TeamCommands commands_;
};

template <typename T>
void TeamActor::SendMessage(const T &message,
    gateway::protocol::MessageType::type type) {
  if(this->actor_ == NULL) {
    return ;
  }
  this->actor_->SendMessage(message, type);
}

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM_ACTOR__H

