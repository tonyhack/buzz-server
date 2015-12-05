//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-04 11:25:52.
// File name: check_session_queue.cc
//
// Description:
// Define class CheckSessionQueue.
//

#include "gateway_server/server/check_session_queue.h"

#include "gateway_server/server/gateway_user.h"

namespace gateway {

namespace server {

CheckSessionQueue::CheckSessionQueue() {}
CheckSessionQueue::~CheckSessionQueue() {}

void CheckSessionQueue::Initialize() {
  this->sequence_ = 0;
}
void CheckSessionQueue::Finalize() {}

bool CheckSessionQueue::Insert(GatewayUser *user, core::uint64 &sequence) {
  if(user == NULL) {
    return false;
  }
  ++this->sequence_;
  if(this->sequence_ == 0) {
    ++this->sequence_;
  }
  sequence = this->sequence_;
  Queue::const_iterator iterator = this->queue_.find(sequence);
  if(iterator != this->queue_.end()) {
    return false;
  }
  this->queue_.insert(std::make_pair(sequence, user));
  return true;
}

GatewayUser *CheckSessionQueue::Remove(core::uint64 sequence) {
  Queue::iterator iterator = this->queue_.find(sequence);
  if(iterator == this->queue_.end()) {
    return NULL;
  }
  GatewayUser *user = iterator->second;
  this->queue_.erase(iterator);
  return user;
}

}  // namespace server

}  // namespace gateway

