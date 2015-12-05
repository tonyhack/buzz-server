//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:41:48.
// File name: tcp_connection.h
//
// Description:
// Define class TCPConnection.
//

#ifndef __CORE__TCP__CONNECTION__H
#define __CORE__TCP__CONNECTION__H

#include <string>

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "core/callbacks.h"
#include "core/channel.h"
#include "core/inet_address.h"
#include "core/tcp_socket.h"
#include "core/base/bound_queue.h"
#include "core/base/byte_buffer_dynamic.h"
#include "core/base/mutex.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace core {

class EventLoop;
class TimeTick;

class TCPConnection : public Noncopyable,
  public boost::enable_shared_from_this<TCPConnection> {
 public:
  TCPConnection(int sockfd);
  ~TCPConnection();

  bool Initialize(EventLoop *loop, const std::string &name,
         const InetAddress &local, const InetAddress &peer);
  void Finalize();

  // Get.
  EventLoop *GetLoop() { return this->loop_; }
  const EventLoop *GetLoop() const { return this->loop_; }
  const std::string &GetName() const { return this->name_; }
  const InetAddress &GetLocalAddress() const { return this->local_address_; }
  const InetAddress &GetPeerAddress() const { return this->peer_address_; }
  bool CheckConnected() const { return this->state_ == CONNECTED; }

  // Set callbacks.
  inline void SetConnectionCallback(const ConnectionCallback &callback) {
    this->connection_callback_ = callback;
  }
  inline void SetMessageCallback(const MessageCallback &callback) {
    this->message_callback_ = callback;
  }
  inline void SetWriteCompleteCallback(const WriteCompleteCallback &callback) {
    this->write_complete_callback_ = callback;
  }
  inline void SetCloseCallback(const CloseCallback &callback) {
    this->close_callback_ = callback;
  }

  inline boost::any &GetContext() { return this->context_; }
  inline void ClearContext() {
    if(this->context_.empty() == false) { this->context_ = boost::any(); }
  }

  void ConnectionEstablished();
  void ConnectionDestoryed();

  // Shut down socket.
  void Shutdown();

  // Send data.
  void SendMessage(const char *message, size_t size);

  void SendMessage(const std::string &message) {
    this->SendMessage(message.data(), message.size());
  }

  // receive uses callback --- the member "message_callback_".

 private:
  enum { DISCONNECTED = 0, CONNECTING, CONNECTED, DISCONNECTING, };

  inline void SetState(uint8 state) { this->state_ = state; }

  void OnRead(const TimeTick &time_tick);
  void OnWrite();
  void OnClose();
  void OnError();

  // For other threads' sending message.
  void CacheMessage(const void *message, size_t size);
  void AbsorbCache();

  void RequireShutdown();
  void Disconnect();

  // Basic data.
  std::string name_;
  uint8 state_;

  // Communication tools.
  EventLoop *loop_;
  Channel channel_;
  TCPSocket socket_;

  // Address.
  InetAddress local_address_;
  InetAddress peer_address_;

  // Buffers.
  ByteBufferDynamic send_buffer_;
  ByteBufferDynamic receive_buffer_;

  // For other threads' sending.
  BoundQueue cache_;

  // Mutex.
  mutable Mutex mutex_;

  // Callbacks.
  //
  // Invoke when connection established/destroyed.
  ConnectionCallback connection_callback_;
  // Invoke when recieving any data from socket.
  MessageCallback message_callback_;
  // Invoke when sending all data in the send buffer.
  WriteCompleteCallback write_complete_callback_;
  // Invoke when closed(in tcp server).
  CloseCallback close_callback_;

  boost::any context_;
};

typedef boost::shared_ptr<TCPConnection> TCPConnectionPtr;

}  // namespace core

#endif  // __CORE__TCP__CONNECTION__H

