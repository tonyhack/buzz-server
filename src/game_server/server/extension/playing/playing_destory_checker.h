//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-24 09:30:58.
// File name: playing_destory_checker.h
//
// Description:
// Define class PlayingDestoryChecker.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__DESTORY__CHECKER__H
#define __GAME__SERVER__PLAYING__PLAYING__DESTORY__CHECKER__H

#include <time.h>

#include <map>

#include "core/timer_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace playing {

class PlayingDestoryChecker : public global::SingletonFactory<PlayingDestoryChecker>,
  public core::TimerHandler {
  friend class global::SingletonFactory<PlayingDestoryChecker>;

  enum { TIMER_ID_CHECK = 0 };

  typedef std::map<core::uint64, time_t> PlayingMap;

 public:
  bool Initialize();
  void Finalize();

  void Add(core::uint64 playing);
  void Remove(core::uint64 playing);

  virtual void OnTimer(core::uint32 id);

 private:
  PlayingDestoryChecker() {}
  ~PlayingDestoryChecker() {}

  PlayingMap playings_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__DESTORY__CHECKER__H

