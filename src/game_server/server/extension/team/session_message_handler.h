//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 21:01:26.
// File name: session_message_handler.h
//
// Description:
// Define class SessionMessageHandler.
//

#ifndef __GAME__SERVER__TEAM__SESSION__MESSAGE__HANDLER__H
#define __GAME__SERVER__TEAM__SESSION__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace team {

class SessionMessageHandler : public core::Noncopyable {
 public:
  SessionMessageHandler();
  ~SessionMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageTeamSynchronize(const char *data, size_t size);
  void OnMessageTeamBuffSynchronize(const char *data, size_t size);
};

}  // namespace team

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEAM__SESSION__MESSAGE__HANDLER__H

