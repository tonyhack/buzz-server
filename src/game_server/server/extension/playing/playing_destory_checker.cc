//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-24 09:47:44.
// File name: playing_destory_checker.cc
//
// Description:
// Define class PlayingDestoryChecker.
//

#include "game_server/server/extension/playing/playing_destory_checker.h"

#include "game_server/server/game_server.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/playing/playing.h"
#include "game_server/server/extension/playing/playing_manager.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace playing {

bool PlayingDestoryChecker::Initialize() {
  const global::configure::PlayingConfigure *playing_configure =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingConfigure();
  if(playing_configure == NULL) {
    LOG_ERROR("获取 PlayingConfigure 失败");
    return false;
  }

  GameServerSingleton::GetInstance().AddTimer(TIMER_ID_CHECK,
      playing_configure->kPlayingDestoryCheckMsec_, -1, this,
      "PlayingDestoryChecker::Initialize");

  return true;
}

void PlayingDestoryChecker::Finalize() {
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_CHECK, this);
}

void PlayingDestoryChecker::Add(core::uint64 playing) {
  if(this->playings_.find(playing) == this->playings_.end()) {
    this->playings_.insert(std::make_pair(playing,
          GlobalTimeTick::GetInstance()->TimeTick().GetRealSec()));
  }
}

void PlayingDestoryChecker::Remove(core::uint64 playing) {
  this->playings_.erase(playing);
}

void PlayingDestoryChecker::OnTimer(core::uint32 id) {
  const global::configure::PlayingConfigure *playing_configure =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingConfigure();
  if(playing_configure == NULL) {
    LOG_ERROR("获取 PlayingConfigure 失败");
    return;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  PlayingMap::iterator iterator = this->playings_.begin();
  for(; iterator != this->playings_.end();) {
    if(now > iterator->second && now - iterator->second >
        playing_configure->kPlayingDestoryTimeout_) {
      Playing *playing = PlayingManager::GetInstance()->Get(iterator->first);
      if(playing) {
        LOG_DEBUG("PlayingDestoryChecker 销毁副本(%lu)", playing->GetID());
        playing->RequireDestory();
      }
      this->playings_.erase(iterator++);
    } else {
      ++iterator;
    }
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

