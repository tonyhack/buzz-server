//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 03:05:15.
// File name: request_transport_handler.h
//
// Description:
// Define class RequestTransportHandler.
//

#ifndef __GAME__SERVER__TRANSPORT__REQUEST__TRANSPORT__HANDLER__H
#define __GAME__SERVER__TRANSPORT__REQUEST__TRANSPORT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace transport {

class RequestTransportHandler : public core::Noncopyable {
 public:
  RequestTransportHandler();
  ~RequestTransportHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestTransportSummon(void *message, size_t size);
  int OnRequestTransportRemove(void *message, size_t size);
  int OnRequestTransportCurrentNpc(void *message, size_t size);
  int OnRequestTransportAround(void *message, size_t size);
  int OnRequestTransportGetSnapshot(void *message, size_t size);
  int OnRequestTransportSummonFromSnapshot(void *message, size_t size);
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__REQUEST__TRANSPORT__HANDLER__H

