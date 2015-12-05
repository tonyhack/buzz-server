#ifndef __GAME__SERVER__MAIL__MAIL__MESSAGE__HANDLER__H
#define __GAME__SERVER__MAIL__MAIL__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace mail {

class MailMessageHandler : public core::Noncopyable {
 public:
  MailMessageHandler();
  ~MailMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageSendMail(core::uint64 id, const char *data, size_t size);
  void OnMessageEraseMail(core::uint64 id, const char *data, size_t size);
  void OnMessagePickAffix(core::uint64 id, const char *data, size_t size);
  void OnMessageReadMail(core::uint64 id, const char *data, size_t size);
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__MAIL__MAIL__MESSAGE__HANDLE__H
