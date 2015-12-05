//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-28 14:48:17.
// File name: connection_pool.h
//
// Description:
// Define class ConnectionPool.
//

#ifndef __CORESH__CONNECTION__POOL__H
#define __CORESH__CONNECTION__POOL__H

#include <list>
#include <set>

#include "core/base/mutex.h"
#include "coresh/connection.h"

namespace coresh {

class ConnectionPool : public core::Noncopyable {
  typedef std::list<ConnectionPtr> ConnectionList;
  typedef std::set<ConnectionPtr>ConnectionSet;

 public:
  ConnectionPool();
  ~ConnectionPool();

  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  ConnectionPtr Allocate();
  void Deallocate(const ConnectionPtr &connection);

 private:
  void Extend();

  ConnectionList free_connections_;
  ConnectionSet busy_connections_;

  size_t extend_number_;
  core::Mutex mutex_;
};

class ConnectionPoolSingleton : public core::Noncopyable {
 public:
  ConnectionPoolSingleton() {}
  ~ConnectionPoolSingleton() {}

  static inline ConnectionPool &GetInstance() {
    return ConnectionPoolSingleton::pool_;
  }

 private:
  static ConnectionPool pool_;
};

}  // namespace coresh

#endif  // __CORESH__CONNECTION__POOL__H

