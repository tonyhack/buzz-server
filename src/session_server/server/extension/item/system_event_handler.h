#ifndef __SESSION__SERVER__ITEM__SYSTEM__EVENT__HANDLER__H
#define __SESSION__SERVER__ITEM__SYSTEM__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace session {

namespace server {

namespace item {

class SystemEventHandler : public core::Noncopyable {
 public:
  SystemEventHandler();
  ~SystemEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventSystemServerLogin(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_server_login_; 
};

}  // namespace item

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__ITEM__SYSTEM__EVENT__HANDLER__H

