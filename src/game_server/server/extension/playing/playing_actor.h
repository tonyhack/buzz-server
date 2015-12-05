//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-13 10:43:14.
// File name: playing_actor.h
//
// Description:
// Define class PlayingActor.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__ACTOR__H
#define __GAME__SERVER__PLAYING__PLAYING__ACTOR__H

#include <map>
#include <vector>

#include "core/base/noncopyable.h"
#include "database_server/protocol/storage_playing_actor_login_types.h"
#include "entity/playing_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/playing/maze_actor.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace playing {

class PlayingPage;

class PlayingActor : public core::Noncopyable {
  typedef std::map<core::int32, PlayingPage *> PageMap;

 public:
  PlayingActor();
  ~PlayingActor();

  bool Initialize(GameActor *actor,
      const database::protocol::StoragePlayingActorField &field);
  void Finalize();

  void Save();

  inline GameActor *GetActor() {
    return this->actor_;
  }

  inline core::uint64 GetID() const {
    if(this->actor_) {
      return this->actor_->GetID();
    }
    return 0;
  }

  inline core::uint64 GetPlaying() const {
    return this->playing_;
  }
  inline void SetPlaying(core::uint64 playing) {
    this->playing_ = playing;
  }

  // 是否正在副本中(在副本房间中也算在副本中)
  inline bool CheckPlaying() const {
    return this->GetPlaying() != 0 || this->GetPlayingTeam() != 0;
  }

  // 发送消息
  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    if(this->actor_) {
      this->actor_->SendMessage(message, type);
    }
  }

  inline core::int32 GetAutoPlaying() const {
    return this->auto_playing_;
  }
  inline void SetAutoPlaying(core::int32 playing) {
    this->auto_playing_ = playing;
  }

  inline core::int32 GetAutoPlayingGroup() const {
    return this->auto_playing_group_;
  }
  inline void SetAutoPlayingGroup(core::int32 group) {
    this->auto_playing_group_ = group;
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

  inline core::uint64 GetPlayingTeam() const {
    return this->playing_team_;
  }
  inline void SetPlayingTeam(core::uint64 team) {
    this->playing_team_ = team;
  }

  PlayingPage *GetPage(core::int32 template_id);
  const PlayingPage *GetPage(core::int32 template_id) const;
  bool AddPage(PlayingPage *page);
  PlayingPage *RemovePage(core::int32 template_id);

  // 副本定期清理进度
  void PeriodClean(entity::PlayingPeriodType::type period_type);

  // 同步副本记录
  void SynchronizePage(core::int32 template_id) const;
  // 检测创建副本
  bool CheckCreatePlaying(core::int32 template_id) const;

  // 开启自动副本
  bool StartAutoPlaying(core::int32 template_id);
  bool StartAutoPlayingGroup(core::int32 group);

  // 完成自动副本
  bool FinishAutoPlaying(core::int32 template_id);
  bool FinishAutoPlayingGroup(core::int32 group);

  // 领取奖励
  bool AwardPlaying(core::int32 template_id);
  bool AwardPlayingGroup(core::int32 group);

  inline MazeActor *GetMazeActor() {
    return &this->maze_actor_;
  }

 private:
  GameActor *actor_;

  // 当前所在副本
  core::uint64 playing_;

  // 副本页
  PageMap pages_;

  // 自动进行中的副本
  core::int32 auto_playing_;
  core::int32 auto_playing_group_;
  time_t auto_finish_time_;

  // 地宫
  MazeActor maze_actor_;

  // 副本队伍
  core::uint64 playing_team_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__ACTOR__H

