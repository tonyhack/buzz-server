//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:42:07.
// File name: tcp_server.h
//
// Description:
// Define class TCPServer.
//

#ifndef __CORE__TCP__SERVER__H
#define __CORE__TCP__SERVER__H

#include <map>
#include <string>

#include "core/acceptor.h"
#include "core/callbacks.h"
#include "core/inet_address.h"
#include "core/tcp_connection.h"
#include "core/base/noncopyable.h"

namespace core {

class EventLoop;
class EventLoopThreadGroup;
// class TCPConnection;

class TCPServer : public Noncopyable {
  typedef std::map<std::string, TCPConnectionPtr> ConnectionList;

 public:
  TCPServer(const InetAddress &address, const std::string &name);
  ~TCPServer();

  bool Initialize(EventLoop *loop, EventLoopThreadGroup *thread_group);
  void Finalize();

  bool Start();
  void Stop();

  inline void SetConnectionCallback(const ConnectionCallback &callback) {
    this->connection_callback_ = callback;
  }
  inline void SetMessageCallback(const MessageCallback &callback) {
    this->message_callback_ = callback;
  }
  inline void SetWriteCallback(const WriteCompleteCallback &callback) {
    this->write_complete_callback_ = callback;
  }

  inline const InetAddress &GetAddress() const { return this->local_address_; }

 private:
  void NewTCPConnection(int sockfd, const InetAddress &peer);
  void RemoveTCPConnection(const TCPConnectionPtr &connection);
  void RemoveInThreadLoop(const TCPConnectionPtr &connection);
  void RemoveAll();

  std::string name_;
  EventLoop *loop_;
  Acceptor acceptor_;
  InetAddress local_address_;

  EventLoopThreadGroup *thread_group_;

  // Callbacks.
  ConnectionCallback connection_callback_;
  MessageCallback message_callback_;
  WriteCompleteCallback write_complete_callback_;

  ConnectionList connections_;

  core::uint32 connection_id_;
};

}  // namespace core

#endif  // __CORE__TCP__SERVER__H

