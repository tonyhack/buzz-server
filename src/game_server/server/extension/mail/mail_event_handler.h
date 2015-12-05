#ifndef __GAME__SERVER__MAIL__MAIL__EVENT__HANDLER__H
#define __GAME__SERVER__MAIL__MAIL__EVENT__HANDLER__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace mail {

class MailEventHandler : public core::Noncopyable {
 public:
  MailEventHandler();
  ~MailEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnLoginEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnLoginOutEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_login_;
  coresh::Communicator::Connection conn_actor_logout_;
};

}  // namespace mail

}  // namespace server

}  // namespace game


#endif  // __GAME__SERVER__MAIL__MAIL__EVENT__HANDLE__H
