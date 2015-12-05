//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:35:41.
// File name: connection.h
//
// Description: 
// Define class Connection.
//

#ifndef __CORESH__CONNECTION__H
#define __CORESH__CONNECTION__H

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "core/event_loop.h"
#include "core/tcp_connection.h"
#include "core/timer_handler.h"
#include "core/base/byte_buffer_dynamic.h"
#include "core/base/noncopyable.h"
#include "coresh/message_queue.h"

namespace coresh {

typedef boost::weak_ptr<core::TCPConnection> TCPConnectionWeakPtr;

class Connection : public core::Noncopyable, public core::TimerHandler, 
  public boost::enable_shared_from_this<Connection> {
  typedef boost::function<void ()> DisconnectCallback;
  typedef boost::function<void (const char *message, size_t size)> MessageCallback;

  enum { TIMER_READ_MESSAGE_ID = 0, };
  enum { TIMER_READ_MESSAGE_MSEC = 50, };

 public:
  Connection();
  virtual ~Connection();

  bool Allocate();
  void Deallocate();

  bool Initialize(const core::TCPConnectionPtr &connection,
      core::EventLoop *application_loop);
  void Finalize();

  inline void SetDisconnectCallback(const DisconnectCallback &callback) {
    this->disconnect_callback_ = callback;
  }
  inline void ResetDisconnectCallback() {
    this->disconnect_callback_.clear();
  }

  inline void SetMessageCallback(const MessageCallback &callback) {
    this->message_callback_ = callback;
  }
  inline void ResetMessageCallback() {
    this->message_callback_.clear();
  };

  inline void SetConnectStatus(bool status) {
    this->connect_status_ = status;
  }
  inline bool GetConnectStatus() const {
    return this->connect_status_;
  }

  // Invoke by network loop.
  void NetworkDisconnect();
  // Invoke by application loop.
  void ApplicationDisconnect();

  // Invoke by network loop.
  void ReceiveMessage(const core::TCPConnectionPtr &connection,
      core::ByteBufferDynamic &buffer);
  // Invoke by application loop.
  void SendMessage(const char *message, size_t size);

  virtual void OnTimer(core::uint32 id);

 private:
  void OnApplicationDisconnect(const core::TimeTick &time_tick);

  bool OnReadMessageQueue();
  bool OnWriteMessageQueue();

  TCPConnectionWeakPtr connection_;

  MessageQueue read_queue_;
  MessageQueue write_queue_;

  int disconnect_fd_[2];
  core::Channel disconnect_channel_;

  DisconnectCallback disconnect_callback_;
  MessageCallback message_callback_;

  bool connect_status_;
};

typedef boost::shared_ptr<Connection> ConnectionPtr;

}  // namespace coresh

#endif  // __CORESH__CONNECTION__H

