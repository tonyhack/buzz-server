#ifndef __GAME__SERVER__MAIL__SESSION__MESSAGE__HANDLER__H
#define __GAME__SERVER__MAIL__SESSION__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace mail {

class SessionMessageHandler : public core::Noncopyable {
 public:
  SessionMessageHandler();
  ~SessionMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageMailNotice(const char *data, size_t size);
  void OnMessageMailsendRequest(const char *data, size_t size);
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__MAIL__SESSION__MESSAGE__HANDLE__H

