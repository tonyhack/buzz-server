//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-06 14:41:51.
// File name: pressure_client_set.h
//
// Description:
// Define class PressureClientSet.
//

#ifndef __TEST__CHAT__PRESSURE__PRESSURE__CLIENT__SET__H
#define __TEST__CHAT__PRESSURE__PRESSURE__CLIENT__SET__H

#include <map>
#include <string>

#include "core/event_loop_thread_group.h"
#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "global/random_generator.h"
#include "test/chat/pressure_client/pressure_client.h"

namespace test {

namespace chat {

namespace pressure {

class PressureClientSet : public core::Noncopyable {
  typedef std::map<std::string, PressureClientPtr> ClientMap;

 public:
  PressureClientSet(const core::InetAddress &server_address,
      const std::string &template_name);
  ~PressureClientSet();

  bool Allocate();
  void Deallocate();

  bool Initialize(size_t client_number);
  void Finalize();

  void Start();
  void Stop();

  global::RandomGenerator &GetRandomGenerator() {
    return this->random_;
  }

 private:
  std::string template_name_;

  core::InetAddress server_address_;
  core::EventLoopThreadGroup *thread_group_;

  ClientMap clients_;

  global::RandomGenerator random_;
};

class PressureClientSetSingleton : public core::Noncopyable {
 public:
  PressureClientSetSingleton() {}
  ~PressureClientSetSingleton() {}

  static void SetInstance(PressureClientSet *instance) {
    PressureClientSetSingleton::instance_ = instance;
  }

  static PressureClientSet &GetInstance() {
    return *PressureClientSetSingleton::instance_;
  }

 private:
  static PressureClientSet *instance_;
};

}  // namespace pressure

}  // namespace chat

}  // namespace test

#endif  // __TEST__CHAT__PRESSURE__PRESSURE__CLIENT__SET__H

