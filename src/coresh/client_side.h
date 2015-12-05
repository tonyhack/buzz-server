//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-31 15:28:51.
// File name: client_side.h
//
// Description:
// Define class ClientSide.
//

#ifndef __CORESH__CLIENT__SIDE__H
#define __CORESH__CLIENT__SIDE__H

#include "core/event_loop.h"
#include "core/inet_address.h"
#include "core/tcp_client.h"
#include "core/tcp_connection.h"
#include "core/base/noncopyable.h"
#include "coresh/connect_ready_queue.h"

namespace coresh {

class ClientSide : public core::Noncopyable {
  typedef boost::function<void ()> ConnectCallback;
 public:
  ClientSide();
  ~ClientSide();

  bool Initialize(const core::InetAddress &server_address, const std::string &name,
      core::EventLoop *network_loop, core::EventLoop *application_loop,
      const ConnectReadyQueue::ConnectCallback &connect_ready_callback, bool retry = true);
  void Finalize();

  void Start();
  void Stop();

  inline const core::InetAddress &GetServerAddress() const {
    return this->client_.GetServerAddress();
  }

 private:
  void OnConnection(const core::TCPConnectionPtr &connection);
  void OnMessage(const core::TCPConnectionPtr &connection,
      core::ByteBufferDynamic &buffer, const core::TimeTick &time_tick);
  void OnWriteComplete(const core::TCPConnectionPtr &connection);

  core::TCPClient client_;

  core::EventLoop *network_loop_;
  core::EventLoop *application_loop_;

  ConnectReadyQueue connect_ready_queue_;
};

}  // namespace coresh

#endif  // __CORESH__CLIENT__SIDE__H

