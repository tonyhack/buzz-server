//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:32:08.
// File name: acceptor.h
//
// Description:
// Define class Acceptor.
// This acceptor uses non-blocking pattern.
//

#ifndef __CORE__ACCEPTOR__H
#define __CORE__ACCEPTOR__H

#include "core/callbacks.h"
#include "core/channel.h"
#include "core/tcp_socket.h"
#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"

namespace core {

class EventLoop;
class InetAddress;

class Acceptor : public Noncopyable {
 public:
  Acceptor();
  ~Acceptor();

  bool Initialize(const InetAddress &listen_address, EventLoop *loop);
  void Finalize();

  // Callback.
  inline void SetNewConnectionCallback(const NewConnectionCallback &callback) {
    this->callback_ = callback;
  }

  // Listen and accept.
  bool Listen();
  void Accept(const TimeTick &time_tick);

  inline InetAddress &GetAddress() { return this->local_address_; }

  inline EventLoop *GetEventLoop() { return this->loop_; }

 private:
  TCPSocket socket_;
  NewConnectionCallback callback_;
  InetAddress local_address_;
  EventLoop *loop_;
  Channel channel_;
};

}  // namespace core

#endif  // __CORE__ACCEPTOR__H

