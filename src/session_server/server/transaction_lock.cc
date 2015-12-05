#include "session_server/server/transaction_lock.h"

#include "session_server/server/global_time_tick.h"

namespace session {

namespace server {

TransactionLock::TransactionLock() {}
TransactionLock::~TransactionLock() {}

bool TransactionLock::CheckLocked(int id) const {
  LockHashmap::const_iterator iter = this->locks_.find(id);
  if (iter == this->locks_.end()) {
    return false;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if (now < iter->second) {
    return true;
  } else {
    return false;
  }
}

void TransactionLock::Lock(int id, int timeout) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  this->locks_[id] = now + timeout;
}

void TransactionLock::Unlock(int id) {
  this->locks_.erase(id);
}

}  // namespace server

}  // namespace session

