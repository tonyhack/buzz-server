//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-14 16:41:48.
// File name: multiplex_thrift.h
//
// Description:
// Define class MultiplexThrift.
//

#ifndef __GLOBAL__MULTIPLEX__THRIFT__H
#define __GLOBAL__MULTIPLEX__THRIFT__H

#include <map>

#include "core/event_loop.h"
#include "core/base/noncopyable.h"
#include "global/thrift_packet.h"

namespace global {

class MultiplexThrift : public core::Noncopyable {
  typedef std::map<core::EventLoop *, global::ThriftPacket *> ThriftPacketMap;

 public:
  MultiplexThrift() {}
  ~MultiplexThrift() {}

  bool Insert(core::EventLoop *key, global::ThriftPacket *packet) {
    ThriftPacketMap::iterator iterator = this->packets_.find(key);
    if(iterator == this->packets_.end()) {
      this->packets_.insert(std::make_pair(key, packet));
      return true;
    }
    return false;
  }

  global::ThriftPacket *Remove(core::EventLoop *key) {
    global::ThriftPacket *packet = NULL;
    ThriftPacketMap::iterator iterator = this->packets_.find(key);
    if(iterator != this->packets_.end()) {
      packet = iterator->second;
      this->packets_.erase(iterator);
    }
    return packet;
  }

  global::ThriftPacket *GetThriftPacket(core::EventLoop *key) {
    ThriftPacketMap::iterator iterator = this->packets_.find(key);
    if(iterator != this->packets_.end()) {
      return iterator->second;
    }
    return NULL;
  }

 private:
  ThriftPacketMap packets_;
};

}  // namespace global

#endif  // __GLOBAL__MULTIPLEX__THRIFT__H

