//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-23 13:04:32.
// File name: guild_request_handler.h
//
// Description:
// Define class GuildRequestHandler.
//

#ifndef __GAME__SERVER__GUILD__GUILD__REQUEST__HANDLER__H
#define __GAME__SERVER__GUILD__GUILD__REQUEST__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace guild {

class GuildRequestHandler : public core::Noncopyable {
 public:
  GuildRequestHandler();
  ~GuildRequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestGuildGetActorGuild(void *message, size_t size);
  int OnRequestGuildGetActorGuildName(void *message, size_t size);
};

}  // namespace guild

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GUILD__GUILD__REQUEST__HANDLER__H

