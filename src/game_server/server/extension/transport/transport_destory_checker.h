//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-10 01:10:25.
// File name: transport_destory_checker.h
//
// Description:
// Define class TransportDestoryChecker.
//

#ifndef __GAME__SERVER__TRANSPORT__TRANSPORT__DESTORY__CHECKER__H
#define __GAME__SERVER__TRANSPORT__TRANSPORT__DESTORY__CHECKER__H

#include <time.h>

#include <map>

#include "core/timer_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace transport {

class TransportDestoryChecker : public global::SingletonFactory<TransportDestoryChecker>,
  public core::TimerHandler {

  friend class global::SingletonFactory<TransportDestoryChecker>;

  enum { TIMER_ID_CHECK = 0, };

  typedef std::map<core::uint64, time_t> TransportMap;

 public:
  bool Initialize();
  void Finalize();

  void Add(core::uint64 transport_npc);
  void Remove(core::uint64 transport_npc);

  virtual void OnTimer(core::uint32 id);

 private:
  TransportDestoryChecker() {}
  ~TransportDestoryChecker() {}

  TransportMap transports_;
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__TRANSPORT__DESTORY__CHECKER__H

