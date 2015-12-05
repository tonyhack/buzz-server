//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-13 11:03:51.
// File name: global_packet.h
//
// Description:
// Define class GlobalPacket.
//

#ifndef __GLOBAL__GLOBAL__PACKET__H
#define __GLOBAL__GLOBAL__PACKET__H

#include "core/base/noncopyable.h"
#include "global/thrift_packet.h"

namespace global {

class GlobalPacket : public core::Noncopyable {
 public:
  GlobalPacket() {}
  ~GlobalPacket() {}

  static void SetPacket(ThriftPacket *thrift) {
    GlobalPacket::thrift_ = thrift;
  }
  static ThriftPacket *GetPacket() {
    return GlobalPacket::thrift_;
  }

 private:
  static ThriftPacket *thrift_;
};

}  // namespace global

#endif  // __GLOBAL__GLOBAL__PACKET__H

