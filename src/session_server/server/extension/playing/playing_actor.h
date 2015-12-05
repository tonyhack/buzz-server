//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-13 16:32:45.
// File name: playing_actor.h
//
// Description:
// Define class PlayingActor.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__ACTOR__H
#define __SESSION__SERVER__PLAYING__PLAYING__ACTOR__H

#include "core/base/noncopyable.h"
#include "global/configure/configure.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/extension/playing/playing_team_actor.h"

namespace session {

namespace server {

namespace playing {

class PlayingPage;

class PlayingActor : public core::Noncopyable {
  typedef std::map<core::int32, PlayingPage *> PageMap;

 public:
  PlayingActor();
  ~PlayingActor();

  bool Initialize(SessionActor *actor);
  void Finalize();

  PlayingPage *GetPage(core::int32 template_id);
  const PlayingPage *GetPage(core::int32 template_id) const;
  bool AddPage(PlayingPage *page);
  PlayingPage *RemovePage(core::int32 template_id);

  core::int32 GetPlayingAvailableCount(core::int32 template_id) const;

  inline core::uint64 GetID() const {
    if(this->actor_) {
      return this->actor_->GetActorID();
    }
    return 0;
  }
  inline const std::string GetName() const {
    if(this->actor_) {
      return this->actor_->GetName();
    }
    return std::string("");
  }

  inline core::uint64 GetPlaying() const {
    return this->playing_;
  }
  inline void SetPlaying(core::uint64 playing) {
    this->playing_ = playing;
  }
  inline bool CheckPlaying() const {
    return this->GetPlaying() != 0 || this->playing_team_actor_.GetTeam() != 0;
  }

  inline SessionActor *GetActor() {
    return this->actor_;
  }

  inline bool CheckFunctionalityOpen(core::int32 template_id) const {
    const global::configure::PlayingCell *cell =
      global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
          template_id);
    if (cell == NULL) {
      LOG_ERROR("获取 PlayingCell(%u) 失败", template_id);
      return false;
    }
    return this->CheckFunctionalityOpen(cell->camp_type_);
  }

  inline bool CheckFunctionalityOpen(entity::PlayingCampType::type type) const {
    if (this->actor_ == NULL) {
      return false;
    }
    switch (type) {
      case entity::PlayingCampType::ROOM:
      case entity::PlayingCampType::GUILD:
        return this->actor_->CheckFunctionalityState(entity::FunctionalityType::MULTI_PLAYING);
      case entity::PlayingCampType::PLAYER:
      case entity::PlayingCampType::TEAM:
      default:
        return this->actor_->CheckFunctionalityState(entity::FunctionalityType::PLAYING_MODULE);
    }
  }

  inline PlayingTeamActor *GetPlayingTeamActor() {
    return &this->playing_team_actor_;
  }

  inline void SendMessage(gateway::protocol::MessageType::type type,
      const char *message, size_t size) {
    if(this->actor_) {
      this->actor_->SendMessage(type, message, size);
    }
  }

  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    if (this->actor_) {
      this->actor_->SendMessage(message, type);
    }   
  }

 private:
  SessionActor *actor_;

  core::uint64 playing_;

  PageMap pages_;

  // 副本队伍
  PlayingTeamActor playing_team_actor_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__ACTOR__H

