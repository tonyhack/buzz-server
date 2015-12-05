//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-18 22:08:37.
// File name: playing_activity_controller.h
//
// Description:
// Define class PlayingActivityController.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__ACTIVITY__CONTROLLER__H
#define __SESSION__SERVER__PLAYING__PLAYING__ACTIVITY__CONTROLLER__H

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace playing {

class PlayingActivityController : public global::SingletonFactory<PlayingActivityController>,
  public core::TimerHandler {
  friend class global::SingletonFactory<PlayingActivityController>;

  typedef std::map<core::int32, core::uint64> ActivityPlayingMap;

 public:
  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

  bool AddActivity(core::int32 playing_template, core::uint64 playing);
  void RemoveActivity(core::int32 playing_template);
  core::uint64 GetActivity(core::int32 playing_template) const;

  virtual void OnTimer(core::uint32 id);

 private:
  PlayingActivityController() {}
  ~PlayingActivityController() {}

  void CreatePlaying(core::uint32 id);

  ActivityPlayingMap activitys_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__ACTIVITY__CONTROLLER__H

