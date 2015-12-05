//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-04 11:20:27.
// File name: check_session_queue.h
//
// Description:
// Define class CheckSessionQueue.
//

#ifndef __GATEWAY__SERVER__CHECK__SESSION__QUEUE__H
#define __GATEWAY__SERVER__CHECK__SESSION__QUEUE__H

#include <map>

#include "global/sequence_allocator.h"
#include "global/singleton_factory.h"
#include "global/thrift_packet.h"

namespace gateway {

namespace server {

class GatewayUser;

class CheckSessionQueue : public global::SingletonFactory<CheckSessionQueue> {
  friend class global::SingletonFactory<CheckSessionQueue>;
  typedef std::map<core::uint64, GatewayUser *> Queue;

 public:
  CheckSessionQueue();
  ~CheckSessionQueue();

  void Initialize();
  void Finalize();

  bool Insert(GatewayUser *user, core::uint64 &sequence);
  GatewayUser *Remove(core::uint64 sequence);

  Queue queue_;
  core::uint64 sequence_;
  global::ThriftPacket thrift_;
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__CHECK__SESSION__QUEUE__H

