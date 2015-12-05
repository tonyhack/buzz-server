//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:36:31.
// File name: network_service.h
//
// Description: 
// Define class NetworkService.
//

#ifndef __CORESH__NETWORK__SERVICE__H
#define __CORESH__NETWORK__SERVICE__H

#include "core/tcp_server.h"
#include "core/event_loop_thread_group.h"
#include "core/tcp_connection.h"
#include "core/base/noncopyable.h"
#include "coresh/connect_ready_queue.h"

namespace coresh {

class NetworkService : public core::Noncopyable {
  typedef boost::shared_ptr<core::EventLoopThreadGroup> EventLoopThreadGroupPtr;

 public:
  NetworkService(const core::InetAddress &address, const std::string &name);
  ~NetworkService();

  bool Initialize(size_t thread_number, core::EventLoop *application_loop,
      const ConnectReadyQueue::ConnectCallback &connect_ready_callback);
  void Finalize();

  void Start();
  void Stop();

  core::EventLoop *GetNextLoop() {
    return this->thread_pool_->GetNextLoop();
  }

 private:
  void OnConnection(const core::TCPConnectionPtr &connection);
  void OnMessage(const core::TCPConnectionPtr &connection,
      core::ByteBufferDynamic &buffer, const core::TimeTick &time_tick);
  void OnWriteComplete(const core::TCPConnectionPtr &connection);

  EventLoopThreadGroupPtr thread_pool_;
  core::TCPServer server_;

  ConnectReadyQueue connect_ready_queue_;

  core::EventLoop *application_loop_;
};

}  // namespace coresh

#endif  // __CORESH__NETWORK__SERVICE__H

