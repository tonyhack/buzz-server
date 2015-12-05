//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:37:38.
// File name: epoller.h
//
// Description:
// Define class Epoller.
// Simple wrapper of epoll.
//

#ifndef __CORE__EPOLLER__H
#define __CORE__EPOLLER__H

#include <sys/epoll.h>

#include <vector>

#include "core/base/noncopyable.h"

namespace core {

class EventLoop;
class Channel;

class Epoller : public Noncopyable {
 public:
  typedef std::vector<struct epoll_event> EventList;

  Epoller();
  ~Epoller();

  bool Initialize(EventLoop *loop);
  void Finalize();

  // Return numbers of event happened.
  int Poll(struct epoll_event *events,
         int max_events, int timeout_milliseconds = -1);
  void AddChannel(Channel *channel);
  void RemoveChannel(Channel *channel);
  void UpdateChannel(Channel *channel);

 private:
  void Update(int operation, Channel *channel);

  EventLoop *loop_;
  int epoll_fd_;
};

}  // namespace core

#endif  // __CORE__EPOLLER__H

