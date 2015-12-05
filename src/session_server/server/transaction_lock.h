#ifndef __SESSION__SERVER__TRANSACTION__LOCK__H
#define __SESSION__SERVER__TRANSACTION__LOCK__H

#include <ctime>
#include <ext/hash_map>

namespace session {

namespace server {

class TransactionLock {
 public:
  typedef __gnu_cxx::hash_map<int, time_t> LockHashmap;

  TransactionLock();
  ~TransactionLock();

  bool CheckLocked(int id) const;
  void Lock(int id, int timeout = 10);
  void Unlock(int id);

 private:
  LockHashmap locks_;
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TRANSACTION__LOCK__H

