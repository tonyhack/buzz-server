//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-27 13:18:56.
// File name: request_daily_pass_handler.h
//
// Description:
// Define class RequestPlayingHandler.
//

#ifndef __GAME__SERVER__PLAYING__REQUEST__PLAYING__HANDLER__H
#define __GAME__SERVER__PLAYING__REQUEST__PLAYING__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace playing {

class RequestPlayingHandler : public core::Noncopyable {
 public:
  RequestPlayingHandler();
  ~RequestPlayingHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestPlayingSynchronizeScore(void *message, size_t size);
  int OnRequestPlayingSynchronizeRanking(void *message, size_t size);
  int OnRequestPlayingInitializeActor(void *message, size_t size);
  int OnRequestPlayingComplete(void *message, size_t size);
  int OnRequestPlayingActorComplete(void *message, size_t size);
  int OnRequestPlayingFailure(void *message, size_t size);
  int OnRequestPlayingUpdateRecord(void *message, size_t size);
  int OnRequestPlayingUpdateResult(void *message, size_t size);
  int OnRequestPlayingAddAward(void *message, size_t size);
  int OnRequestPlayingClearComplete(void *message, size_t size);
  int OnRequestPlayingIncreaseComplete(void *message, size_t size);
  int OnRequestPlayingActorIncreaseComplete(void *message, size_t size);
  int OnRequestPlayingAwardActor(void *message, size_t size);
  int OnRequestPlayingID(void *message, size_t size);
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__REQUEST__PLAYING__HANDLER__H

