//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:35:06.
// File name: thrift_packet.h
//
// Description: 
// Define class ThriftPacket.
//

#ifndef __GLOBAL__THRIFT__PACKET__H
#define __GLOBAL__THRIFT__PACKET__H

#include <boost/shared_ptr.hpp>

#include <thrift/Thrift.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>

#include "core/base/noncopyable.h"

using apache::thrift::transport::TMemoryBuffer;
using apache::thrift::protocol::TBinaryProtocol;
using apache::thrift::TException;

namespace global {

class ThriftPacket : public core::Noncopyable {
 public:
  ThriftPacket() :
    deserialize_buffer_(new (std::nothrow) TMemoryBuffer()),
    deserialize_protocol_(new (std::nothrow) TBinaryProtocol(deserialize_buffer_)),
    serialize_buffer_(new (std::nothrow) TMemoryBuffer()),
    serialize_protocol_(new (std::nothrow) TBinaryProtocol(serialize_buffer_)) {}

  ~ThriftPacket() {}

  template <typename T>
  inline bool Deserialize(T &instance, const std::string &message) {
    return this->Deserialize<T>(instance, message.data(), message.size());
  }

  template <typename T>
  inline bool Deserialize(T &instance, const char *msg, size_t size) {
    try {
      this->deserialize_buffer_->resetBuffer((uint8_t *)msg, size/*, TMemoryBuffer::COPY*/);
      this->deserialize_protocol_->setStringSizeLimit(size);
      this->deserialize_protocol_->setContainerSizeLimit(size);
      if(instance.read(this->deserialize_protocol_.get()) > 0) {
        return true;
      } else {
        return false;
      }
    } catch(TException &ex) {
      return false;
    }
  }

  template <typename T>
  inline const char *Serialize(const T &instance, size_t &size) {
    this->serialize_buffer_->resetBuffer();
    const char *message = NULL;
    try {
      if((instance.write(this->serialize_protocol_.get())) > 0) {
        this->serialize_buffer_->getBuffer((uint8_t **)&message, (uint32_t *)&size);
      } else {
        return NULL;
      }
      return (const char *)message;
    } catch(TException &ex) {
      return NULL;
    }
  }

 private:
  boost::shared_ptr<TMemoryBuffer> deserialize_buffer_;
  boost::shared_ptr<TBinaryProtocol> deserialize_protocol_;

  boost::shared_ptr<TMemoryBuffer> serialize_buffer_;
  boost::shared_ptr<TBinaryProtocol> serialize_protocol_;
};

typedef boost::shared_ptr<ThriftPacket> ThriftPacketPtr;

}  // namespace global

#endif  // __GLOBAL__THRIFT__PACKET__H

