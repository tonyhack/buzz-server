//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-28 15:03:43.
// File name: connection_pool.cc
//
// Description:
// Define class ConnectionPool.
//

#include "coresh/connection_pool.h"

#include "core/async_logging_singleton.h"
#include "core/base/auto_mutex.h"

namespace coresh {

ConnectionPool ConnectionPoolSingleton::pool_;

ConnectionPool::ConnectionPool() : extend_number_(0) {}

ConnectionPool::~ConnectionPool() {}

bool ConnectionPool::Initialize(size_t initial_number, size_t extend_number) {
  this->extend_number_ = extend_number;
  size_t pos = 0;
  for(; pos < initial_number; ++pos) {
    ConnectionPtr connection(new (std::nothrow) Connection());
    if(connection && connection->Allocate()) {
      this->free_connections_.push_back(connection);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate connection failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
  }
  return true;
}

void ConnectionPool::Finalize() {
  ConnectionList::iterator free = this->free_connections_.begin();
  for(; free != this->free_connections_.end(); ++free) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_DEBUG,
        "free connection[reference count=%d].", (*free).use_count());
    (*free)->Deallocate();
  }
  ConnectionSet::iterator busy = this->busy_connections_.begin();
  for(; busy != this->busy_connections_.end(); ++busy) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) One allocated connection[reference count=%d].",
        __FILE__, __LINE__, __FUNCTION__, (*busy).use_count());
    (*busy)->Deallocate();
  }
  this->free_connections_.clear();
  this->busy_connections_.clear();
}

ConnectionPtr ConnectionPool::Allocate() {
  ConnectionPtr connection;
  core::AutoMutex auto_mutex(&this->mutex_);
  if(this->free_connections_.empty()) {
    Extend();
  }
  ConnectionList::iterator iterator = this->free_connections_.begin();
  if(iterator != this->free_connections_.end()) {
    connection = *iterator;
    this->free_connections_.erase(iterator);
    this->busy_connections_.insert(connection);
  }
  return connection;
}

void ConnectionPool::Deallocate(const ConnectionPtr &connection) {
  core::AutoMutex auto_mutex(&this->mutex_);
  ConnectionSet::iterator iterator = this->busy_connections_.find(connection);
  if(iterator != this->busy_connections_.end()) {
    this->busy_connections_.erase(iterator);
    this->free_connections_.push_back(connection);
    connection->Finalize();
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deallocate a non-busy connection.", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ConnectionPool::Extend() {
  size_t pos = 0;
  for(; pos < this->extend_number_; ++pos) {
    ConnectionPtr connection(new (std::nothrow) Connection());
    if(connection && connection->Allocate()) {
      this->free_connections_.push_back(connection);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate/Initialize connection failed.",
          __FILE__, __LINE__, __FUNCTION__);
    }
  }
}

}  // namespace coresh

