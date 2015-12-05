//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-06 16:24:23.
// File name: multiplex_thrift.h
//
// Description:
// Define class MultiplexThrift.
//

#ifndef __TEST__CHAT__PRESSURE__MULTIPLEX__THRIFT__H
#define __TEST__CHAT__PRESSURE__MULTIPLEX__THRIFT__H

#include <map>

#include "core/event_loop.h"
#include "core/base/noncopyable.h"
#include "global/thrift_packet.h"

namespace test {

namespace chat {

namespace pressure {

class MultiplexThrift : public core::Noncopyable {
  typedef std::map<const core::EventLoop *, global::ThriftPacket *> ThriftMap;

 public:
  MultiplexThrift();
  ~MultiplexThrift();

  bool Insert(const core::EventLoop *key, global::ThriftPacket *thrift);
  global::ThriftPacket *Remove(const core::EventLoop *key);

  global::ThriftPacket *GetThriftPacket(const core::EventLoop *key);

 private:
  ThriftMap thrifts_;
};

class MultiplexThriftSinglton : public core::Noncopyable {
 public:
  MultiplexThriftSinglton() {}
  ~MultiplexThriftSinglton() {}

  static MultiplexThrift &GetInstance() {
    return MultiplexThriftSinglton::instance_;
  }

 private:
  static MultiplexThrift instance_;
};

}  // namespace pressure

}  // namespace chat

}  // namespace test

#endif  // __TEST__CHAT__PRESSURE__MULTIPLEX__THRIFT__H

