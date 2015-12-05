//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 09:31:55.
// File name: session_message_handler.h
//
// Description:
// Define SessionMessageHandler.
//

#ifndef __GAME__SERVER__PLAYING__SESSION__MESSAGE__HANDLER__H
#define __GAME__SERVER__PLAYING__SESSION__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace playing {

class SessionMessageHandler : public core::Noncopyable {
 public:
  SessionMessageHandler();
  ~SessionMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessagePlayingCreateResponse(const char *data, size_t size);
  void OnMessagePlayingCreateSynchronize(const char *data, size_t size);
  void OnMessagePlayingCreateFinishResponse(const char *data, size_t size);
  void OnMessagePlayingDestoryResponse(const char *data, size_t size);
  void OnMessagePlayingEndResponse(const char *data, size_t size);
  void OnMessagePlayingTeamSynchronize(const char *data, size_t size);

  void SendMessagePlayingCreateFinishFailed(core::uint64 id, core::int8 failure = 0);
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__SESSION__MESSAGE__HANDLER__H

