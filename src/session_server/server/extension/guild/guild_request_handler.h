#ifndef __SESSION__SERVER__GUILD__GUILD__REQUEST__HANDLER__H
#define __SESSION__SERVER__GUILD__GUILD__REQUEST__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace guild {

class GuildRequestHandler : public core::Noncopyable {
 public:
  GuildRequestHandler();
  ~GuildRequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestGuildGetID(void *message, size_t size);
  int OnRequestGuildAddContribution(void *message, size_t size);
  int OnRequestGuildCheckAuthorityChat(void *message, size_t size);
  int OnRequestGuildAddAttribute(void *message, size_t size);
  int OnRequestGuildGetMembers(void *message, size_t size);
  int OnRequestGuildCheckCanCreatePlaying(void *message, size_t size);
  int OnRequestGuildGetCurrentPlaying(void *message, size_t size);
  int OnRequestGuildSetCurrentPlaying(void *message, size_t size);
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__GUILD__REQUEST__HANDLER__H

