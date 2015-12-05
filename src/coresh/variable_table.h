//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 14:13:41.
// File name: variable_table.h
//
// Description:
// Define class VariableTable.
//

#ifndef __CORESH__VARIABLE__TABLE__H
#define __CORESH__VARIABLE__TABLE__H

#include <stddef.h>

#include <string>

#include "core/base/noncopyable.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"

namespace coresh {

class VariableTable : public core::Noncopyable {
 public:
  VariableTable() {
    max_protocol_size_ = 0;
    max_message_size_ = 0;
    message_queue_initial_size_ = 0;
    connection_pool_initial_number_ = 0;
    connection_pool_extend_number_ = 0;
  }

  ~VariableTable() {}

  inline size_t GetMaxProtocolSize() const {
    return this->max_protocol_size_;
  }
  inline void SetMaxProtocolSize(size_t size) {
    this->max_protocol_size_ = size;
  }

  inline size_t GetMaxMessageSize() const {
    return this->max_message_size_;
  }
  inline void SetMaxMessageSize(size_t size) {
    this->max_message_size_ = size;
  }

  inline size_t GetMessageQueueInitialSize() const {
    return this->message_queue_initial_size_;
  }
  inline void SetMessageQueueInitialSize(size_t size) {
    this->message_queue_initial_size_ = size;
  }

  inline size_t GetConnectionPoolInitialNumber() const {
    return this->connection_pool_initial_number_;
  }
  inline void SetConnectionPoolInitialNumber(size_t size) {
    this->connection_pool_initial_number_ = size;
  }

  inline size_t GetConnectionPoolExtendNumber() const {
    return this->connection_pool_extend_number_;
  }
  inline void SetConnectionPoolExtendNumber(size_t size) {
    this->connection_pool_extend_number_ = size;
  }

  inline size_t GetNetworkThreadNumber() const {
    return this->network_thread_number_;
  }
  inline void SetNetworkThreadNumber(size_t number) {
    this->network_thread_number_ = number;
  }

  inline CompressType::Type GetCompressType() const {
    return this->compress_type_;
  }
  inline void SetCompressType(CompressType::Type type) {
    this->compress_type_ = type;
  }

  inline EncodeType::Type GetEncodeType() const {
    return this->encode_type_;
  }
  inline void SetEncodeType(EncodeType::Type type) {
    this->encode_type_ = type;
  }

  inline const std::string &GetCoreConfigure() const {
    return this->core_configure_;
  }
  inline void SetCoreConfigure(const std::string &configure) {
    this->core_configure_ = configure;
  }

 private:
  // max protocol size(exclusive of message head, after encoding and compressing).
  size_t max_protocol_size_;
  // max application message size.
  size_t max_message_size_;
  // initial size of message queue.
  size_t message_queue_initial_size_;
  // Initial number of connection pool.
  size_t connection_pool_initial_number_;
  // Extend number of connection pool.
  size_t connection_pool_extend_number_;
  // Network thread number.
  size_t network_thread_number_;
  // Message compress type.
  CompressType::Type compress_type_;
  // Message encode type.
  EncodeType::Type encode_type_;
  // Core configure file.
  std::string core_configure_;
};

class VariableTableSingleton : public core::Noncopyable {
 public:
  VariableTableSingleton() {}
  ~VariableTableSingleton() {}

  static inline VariableTable &GetInstance() {
    return VariableTableSingleton::variables_;
  }

 private:
  static VariableTable variables_;
};

}  // namespace coresh

#endif  // __CORESH__VARIABLE__TABLE__H

