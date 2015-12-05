//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-18 22:17:55.
// File name: playing_activity_controller.cc
//
// Description:
// Define class PlayingActivityController.
//

#include "session_server/server/extension/playing/playing_activity_controller.h"

#include "global/configure/configure.h"
#include "global/configure/playing_configure.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/session_server.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/server/extension/playing/playing.h"
#include "session_server/server/extension/playing/playing_load_balance.h"
#include "session_server/server/extension/playing/playing_manager.h"
#include "session_server/server/extension/playing/playing_pool.h"

namespace session {

namespace server {

namespace playing {

bool PlayingActivityController::Initialize() {
  return true;
}

void PlayingActivityController::Finalize() {
}

void PlayingActivityController::Start() {
  const global::configure::PlayingConfigure *configure =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingConfigure();

  const global::configure::PlayingConfigure::PlayingMap &playings = configure->GetPlayings();
  global::configure::PlayingConfigure::PlayingMap::const_iterator iterator = playings.begin();
  for (; iterator != playings.end(); ++iterator) {
    if (iterator->second.camp_type_ == entity::PlayingCampType::ACTIVITY) {
      core::uint32 sec = 0;
      if (iterator->second.period_type_ == entity::PlayingPeriodType::DAILY) {
        time_t zero_time = GlobalTimeTick::GetInstance()->GetZeroTime();
        time_t now_time = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
        core::int32 now_sec = now_time - zero_time;
        if (iterator->second.period_begin_time_ > now_sec) {
          sec = iterator->second.period_begin_time_ - now_sec;
        } else {
          sec = 24 * 3600 - now_sec - iterator->second.period_begin_time_;
        }
        SessionServerSingleton::GetInstance().AddTimer(iterator->first, sec * 1000, -1,
            this, "PlayingActivityController::Start");
      }
    }
  }
}

void PlayingActivityController::Stop() {
  const global::configure::PlayingConfigure *configure =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingConfigure();

  const global::configure::PlayingConfigure::PlayingMap &playings = configure->GetPlayings();
  global::configure::PlayingConfigure::PlayingMap::const_iterator iterator = playings.begin();
  for (; iterator != playings.end(); ++iterator) {
    if (iterator->second.camp_type_ == entity::PlayingCampType::ACTIVITY) {
      SessionServerSingleton::GetInstance().RemoveTimer(iterator->first, this);
    }
  }
}

void PlayingActivityController::OnTimer(core::uint32 id) {

  const global::configure::PlayingConfigure *configure =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingConfigure();

  const global::configure::PlayingCell *cell = configure->GetPlaying(id);
  if (cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%d) 失败", id);
    return;
  }

  this->CreatePlaying(id);

  SessionServerSingleton::GetInstance().RemoveTimer(id, this);

  if (cell->period_type_ == entity::PlayingPeriodType::DAILY) {
    SessionServerSingleton::GetInstance().AddTimer(id, 24 * 60 * 60 * 1000, -1,
        this, "PlayingActivityController::OnTimer");
  }
}

void PlayingActivityController::CreatePlaying(core::uint32 id) {
  const global::configure::PlayingConfigure *configure =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingConfigure();

  const global::configure::PlayingCell *cell = configure->GetPlaying(id);
  if (cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%d) 失败", id);
    return;
  }

  // 分配 Playing 对象
  Playing *playing = PlayingPool::GetInstance()->Allocate();
  if(playing == NULL) {
    LOG_ERROR("分配 Playing(%d) 失败", cell->id_);
    return;
  }

  // 初始化 Playing 对象
  if(playing->Initialize(cell->id_) == false) {
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("初始化 Playing(%d) 失败", cell->id_);
    return;
  }

  // Playing对象 加入管理器
  if(PlayingManager::GetInstance()->Add(playing) == false) {
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("Playing(%d) 加入管理器失败", cell->id_);
    return;
  }

  if (this->AddActivity(id, playing->GetID()) == false) {
    PlayingManager::GetInstance()->Remove(playing->GetID());
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    LOG_ERROR("Playing(%d) 加入 activitys_ 失败", cell->id_);
    return;
  }

  // 副本负载均衡到 GameServer
  core::uint32 game_server_id = PlayingLoadBalance::GetInstance()->Allocate();
  SessionTerminal *terminal =
    SessionServerSingleton::GetInstance().GetTerminal(
        global::protocol::ServerType::GAME_SERVER, game_server_id);
  if(terminal != NULL) {
    playing->SetGameServer(game_server_id);
    session::protocol::MessagePlayingCreateSynchronize synchronize;
    synchronize.__set_playing_(playing->GetID());
    synchronize.__set_template_(id);
    terminal->SendGameMessage(synchronize,
        session::protocol::GameMessageType::MESSAGE_PLAYING_CREATE_SYNCHRONIZE);
  } else {
    PlayingLoadBalance::GetInstance()->Deallocate(game_server_id);
    PlayingManager::GetInstance()->Remove(playing->GetID());
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
  }
}

bool PlayingActivityController::AddActivity(core::int32 playing_template, core::uint64 playing) {
  ActivityPlayingMap::const_iterator iterator = this->activitys_.find(playing_template);
  if (iterator != this->activitys_.end()) {
    return false; 
  }

  this->activitys_.insert(std::make_pair(playing_template, playing));

  return true;
}

void PlayingActivityController::RemoveActivity(core::int32 playing_template) {
  this->activitys_.erase(playing_template);
}

core::uint64 PlayingActivityController::GetActivity(core::int32 playing_template) const {
  ActivityPlayingMap::const_iterator iterator = this->activitys_.find(playing_template);
  if (iterator != this->activitys_.end()) {
    return iterator->second; 
  }

  return 0;
}

}  // namespace playing

}  // namespace server

}  // namespace session

