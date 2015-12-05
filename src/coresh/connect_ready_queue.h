//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:35:49.
// File name: connect_ready_queue.h
//
// Description: 
// Define class ConnectReadyQueue.
//

#ifndef __CORESH__READY__QUEUE__H
#define __CORESH__READY__QUEUE__H

#include <boost/function.hpp>

#include "core/channel.h"
#include "core/event_loop.h"
#include "core/event_pipe.h"
#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"
#include "coresh/atomic_counter.h"
#include "coresh/connection.h"
#include "coresh/nonblocking_queue.h"

namespace coresh {

class ConnectReadyQueue : public core::Noncopyable {
 public:
  typedef boost::function<void (const ConnectionPtr &)> ConnectCallback;

  ConnectReadyQueue();
  ~ConnectReadyQueue();

  bool Initialize(core::EventLoop *receiver);
  void Finalize();

  inline void SetConnectCallback(const ConnectCallback &callback) {
    this->connect_callback_ = callback;
  }
  inline void ResetConnectionCallback() {
    this->connect_callback_.clear();
  }

  // Invoke by sender event loop thread.
  void OnReceive(const ConnectionPtr &connection);

 private:
  // Callback in receiver event loop thread.
  void OnReadCallback(const core::TimeTick &time_tick);
  void SendEvent();

  NonblockingQueue<ConnectionPtr> queue_;

  core::Channel event_channel_;
  int event_fd_[2];

  AtomicCounter event_counter_;

  ConnectCallback connect_callback_;
};

}  // namespace coresh

#endif  // __CORESH__READY__QUEUE__H

