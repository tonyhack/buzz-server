//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-11 10:58:05.
// File name: playing_request_handler.h
//
// Description:
// Define class PlayingRequestHandler.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__REQUEST__HANDLER__H
#define __SESSION__SERVER__PLAYING__PLAYING__REQUEST__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace playing {

class PlayingRequestHandler : public core::Noncopyable {
 public:
  PlayingRequestHandler();
  ~PlayingRequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestPlayingActorConnectInfo(void *message, size_t size);
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__REQUEST__HANDLER__H

