//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-12 00:35:30.
// File name: playing_team.h
//
// Description:
// Define class PlayingTeam.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__TEAM__H
#define __SESSION__SERVER__PLAYING__PLAYING__TEAM__H

#include "core/timer_handler.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/configure/configure.h"
#include "global/configure/playing_configure.h"
#include "session_server/server/session_server.h"
#include "session_server/server/extension/playing/playing_actor.h"

namespace session {

namespace server {

namespace playing {

class PlayingActor;
class PlayingTeamActor;

class PlayingTeam : public core::TimerHandler {
  enum { TIMER_ID_READY = 0, };

 public:
  typedef std::vector<PlayingTeamActor *> MemberVector;

  PlayingTeam();
  ~PlayingTeam();

  bool Initialize(core::int32 playing_template, core::uint64 id);
  void Finalize();

  bool AddMember(PlayingTeamActor *actor);
  bool RemoveMember(PlayingTeamActor *actor);
  bool SetCaptain(PlayingTeamActor *actor);
  bool SetNewCaptain(PlayingTeamActor *actor);
  const std::string GetName() const;
  bool Dismiss();

  bool CheckReady() const;

  bool GotoReady();
  bool GotoPlay();

  void Synchronize();
  void Synchronize(PlayingActor *actor);

  void SyncGameServer();

  virtual void OnTimer(core::uint32 id);

  inline core::int32 GetSize() const {
    return (core::int32)this->members_.size();
  }

  inline core::uint64 GetID() const {
    return this->id_;
  }
  inline void SetID(core::uint64 id) {
    this->id_ = id;
  }

  inline core::int32 GetMinFightScore() const {
    return this->min_fight_score_;
  }
  inline void SetMinFightScore(core::int32 score) {
    this->min_fight_score_ = score;
  }

  inline entity::PlayingTeamStatus::type GetStatus() const {
    return this->status_;
  }
  inline void SetStatus(entity::PlayingTeamStatus::type status) {
    this->status_ = status;
  }

  inline core::int32 GetCapacity() const {
    return this->capacity_;
  }
  inline void SetCapacity(core::int32 capacity) {
    this->capacity_ = capacity;
  }

  inline bool GetAutoOpen() const {
    return this->auto_open_;
  }
  inline void SetAutoOpen(bool value) {
    this->auto_open_ = value;
  }

  inline core::int32 GetPlayingTemplate() const {
    return this->playing_template_;
  }
  inline void SetPlayingTeamplate(core::int32 playing_template) {
    this->playing_template_ = playing_template;
  }

  inline core::uint64 GetPlaying() const {
    return this->playing_;
  }
  inline void SetPlaying(core::uint64 playing) {
    this->playing_ = playing;
  }

  inline bool CheckMemberFull() const {
    if (this->GetCapacity() < global::configure::PlayingConfigure::kTeamMaxMembers_) {
      return this->GetSize() >= this->GetCapacity();
    } else {
      return this->GetSize() >= global::configure::PlayingConfigure::kTeamMaxMembers_;
    }
  }

  inline const MemberVector &GetMembers() const {
    return this->members_;
  }

  template <typename T>
  void BroadcastMessage(const T &message, gateway::protocol::MessageType::type type);

 private:
  MemberVector members_;
  core::uint64 id_;
  core::int32 min_fight_score_;
  entity::PlayingTeamStatus::type status_;
  core::int32 capacity_;
  bool auto_open_;

  core::uint64 playing_;
  core::int32 playing_template_;
};

template <typename T>
void PlayingTeam::BroadcastMessage(const T &message, gateway::protocol::MessageType::type type) {
  global::ThriftPacket *thrift =
    SessionServerSingleton::GetInstance().GetThrift();
  if(thrift == NULL) {
    LOG_ERROR("%s:%d (%s) Get thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    PlayingTeamActor *team_actor = *iterator;
    if(team_actor == NULL) {
      continue;
    }
    PlayingActor *actor = team_actor->GetActor();
    if (actor == NULL) {
      continue;
    }
    size_t size = 0;
    const char *message1 = thrift->Serialize(message, size);
    if(message1 == NULL) {
      LOG_ERROR("%s:%d (%s) Serialize message failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    actor->SendMessage(type, message1, size);
  }
}

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__TEAM__H

