//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 14:22:40.
// File name: message_thread_pool.h
//
// Description:
// Define class MessageThreadPool.
//

#ifndef __LOGIN__SERVER__MESSAGE__THREAD__POOL__H
#define __LOGIN__SERVER__MESSAGE__THREAD__POOL__H

#include <map>

#include "core/event_loop_thread_group.h"
#include "core/base/noncopyable.h"
#include "login_server_informal/server/mysql_client.h"
#include "global/thrift_packet.h"

namespace login {

namespace server {

class MessageThreadPool : public core::Noncopyable {
  typedef std::map<const core::EventLoop *, global::ThriftPacketPtr> PacketMap;
  typedef std::map<const core::EventLoop *, MysqlClientPtr> MysqlMap;

 public:
  MessageThreadPool();
  ~MessageThreadPool();

  bool Initialize(size_t thread_number, const std::string &name, bool timer = false);

  void Start();
  void Stop();

  inline core::EventLoop *GetNextLoop() {
    return this->thread_pool_.GetNextLoop();
  }

  inline global::ThriftPacketPtr GetPacket(const core::EventLoop *loop) {
    PacketMap::iterator iterator = this->packets_.find(loop);
    if(iterator != this->packets_.end()) {
      return iterator->second;
    } else {
      return global::ThriftPacketPtr();
    }
  }

  inline MysqlClientPtr GetMysql(const core::EventLoop *loop) {
    MysqlMap::iterator iterator = this->mysqls_.find(loop);
    if(iterator != this->mysqls_.end()) {
      return iterator->second;
    } else {
      return MysqlClientPtr();
    }
  }

  inline core::EventLoopThreadGroup *GetEventLoopThreadGroup() {
    return &this->thread_pool_;
  }

 private:
  core::EventLoopThreadGroup thread_pool_;

  PacketMap packets_;
  MysqlMap mysqls_;
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__MESSAGE__THREAD__POOL__H

