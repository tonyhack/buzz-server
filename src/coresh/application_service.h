//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-28 16:23:01.
// File name: application_service.h
//
// Description:
// Define class ApplicationService.
//

#ifndef __CORESH__APPLICATON__SERVICE__H
#define __CORESH__APPLICATON__SERVICE__H

#include <string>

#include <boost/function.hpp>

#include "core/event_loop.h"
#include "core/inet_address.h"
#include "core/loop_thread.h"
#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/connection.h"
#include "coresh/network_service.h"

namespace coresh {

class ApplicationService : public core::Noncopyable {
  typedef boost::function<void (const ConnectionPtr &)> ConnectionCallback;
 public:
  ApplicationService(const core::InetAddress &address, const std::string &name);
  ~ApplicationService();

  // Callbacks.
  inline void SetConnectionCallback(const ConnectionCallback &callback) {
    this->connection_callback_ = callback;
  }
  inline void ResetConnectionCallback() {
    this->connection_callback_.clear();
  }

  inline core::EventLoop *GetLoop() {
    return &this->loop_;
  }
  inline core::EventLoop *AllocateLoop() {
    return this->network_service_.GetNextLoop();
  }

  // Get time tick.
  inline const core::TimeTick &GetTimeTick() const {
    return this->loop_.GetTimeTick();
  }

  // Timer.
  inline void AddTimer(core::uint32 id, core::uint32 msecs, int call_times,
      core::TimerHandler *handler, const std::string &debug) {
    this->loop_.AddTimer(id, msecs, call_times, handler, debug);
  }
  inline void RemoveTimer(core::uint32 id, core::TimerHandler *handler) {
    this->loop_.RemoveTimer(id, handler);
  }

  // Initialize/Finalize.
  bool Initialize();
  void Finalize();

  // Start/Stop.
  void Start();
  void Stop();

 private:
  void OnConnection(const ConnectionPtr &connection);

  NetworkService network_service_;

  core::EventLoop loop_;
  core::LoopThread thread_;

  ConnectionCallback connection_callback_;
};

}  // namespace coresh

#endif  // __CORESH__APPLICATON__SERVICE__H

