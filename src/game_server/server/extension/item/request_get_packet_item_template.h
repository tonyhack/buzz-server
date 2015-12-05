//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-22 21:02:26.
// File name: request_get_packet_item_template.h
//
// Description:
//

#ifndef __GAME__SERVER__ITEM__REQUEST__GET__PACKET__ITEM__TEMPLATE__H
#define __GAME__SERVER__ITEM__REQUEST__GET__PACKET__ITEM__TEMPLATE__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace item {

class RequestGetPacketItemTemplate : public core::Noncopyable {
 public:
  RequestGetPacketItemTemplate();
  ~RequestGetPacketItemTemplate();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__REQUEST__GET__PACKET__ITEM__TEMPLATE__H

