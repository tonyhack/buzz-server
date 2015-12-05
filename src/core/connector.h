//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:36:11.
// File name: connector.h
//
// Description:
// Define class Connector.
// This Connector uses non-blocking pattern and must run in a EventLoop.
// Not a thread-safe version.
//

#ifndef __CORE__CONNECTOR__H
#define __CORE__CONNECTOR__H

#include "core/callbacks.h"
#include "core/channel.h"
#include "core/inet_address.h"
#include "core/tcp_socket.h"
#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace core {

class EventLoop;

class Connector : public Noncopyable, public TimerHandler {
 public:
  Connector();
  ~Connector();

  bool Initialize(EventLoop *loop, const InetAddress &server_address, int retry_msec);
  void Finalize();

  virtual void OnTimer(uint32 id);

  // Callback.
  inline void SetNewConnectionCallback(const NewConnectionCallback &callback) {
    this->callback_ = callback;
  }

  // Start and stop.
  void Start();
  void Stop();

  // Support retry timeout.
  void Restart();

 private:
  enum { DISCONNECTED = 0, CONNECTING, CONNECTED, };

  // Connect to server.
  void Connect();

  // Remove channel.
  void RemoveChannel();

  // Delay to remove channel(safe mode).
  void DelayRemoveChannel();

  // Connect complete or error occupy.
  void OnComplete();
  void OnError();

  // Server address.
  InetAddress address_;
  // Socket.
  TCPSocket socket_;
  // Callback.
  NewConnectionCallback callback_;
  // Event loop and channel.
  EventLoop *loop_;
  Channel channel_;
  // state
  uint8 state_;

  int retry_msec_;

  bool retry_flag_;
};

}  // namespace core

#endif  // __CORE__CONNECTOR__H

