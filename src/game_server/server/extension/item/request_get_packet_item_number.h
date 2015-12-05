//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-20 12:32:58.
// File name: request_get_packet_item_number.h
//
// Description:
// Define class RequestGetPacketItemNumber.
//

#ifndef __GAME__SERVER__ITEM__REQUEST__GET__PACKET__ITEM__NUMBER__H
#define __GAME__SERVER__ITEM__REQUEST__GET__PACKET__ITEM__NUMBER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace item {

class RequestGetPacketItemNumber : public core::Noncopyable {
 public:
  RequestGetPacketItemNumber();
  ~RequestGetPacketItemNumber();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__REQUEST__GET__PACKET__ITEM__NUMBER__H

