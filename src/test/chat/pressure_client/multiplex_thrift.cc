//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-06 16:30:53.
// File name: multiplex_thrift.cc
//
// Description:
// Define class MultiplexThrift.
//

#include "test/chat/pressure_client/multiplex_thrift.h"

namespace test {

namespace chat {

namespace pressure {

MultiplexThrift MultiplexThriftSinglton::instance_;

MultiplexThrift::MultiplexThrift() {}
MultiplexThrift::~MultiplexThrift() {}

bool MultiplexThrift::Insert(const core::EventLoop *key, global::ThriftPacket *thrift) {
  ThriftMap::iterator iterator = this->thrifts_.find(key);
  if(iterator == this->thrifts_.end()) {
    this->thrifts_.insert(std::make_pair(key, thrift));
    return true;
  }
  return false;
}

global::ThriftPacket *MultiplexThrift::Remove(const core::EventLoop *key) {
  global::ThriftPacket *thrift = NULL;
  ThriftMap::iterator iterator = this->thrifts_.find(key);
  if(iterator != this->thrifts_.end()) {
    thrift = iterator->second;
    this->thrifts_.erase(iterator);
  }
  return thrift;
}

global::ThriftPacket *MultiplexThrift::GetThriftPacket(const core::EventLoop *key) {
  ThriftMap::iterator iterator = this->thrifts_.find(key);
  if(iterator != this->thrifts_.end()) {
    return iterator->second;
  }
  return NULL;
}

}  // namespace pressure

}  // namespace chat

}  // namespace test

