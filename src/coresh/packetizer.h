//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 14:57:36.
// File name: packetizer.h
//
// Description:
// Define class Packetizer.
//

#ifndef __CORESH__PACKETIZER__H
#define __CORESH__PACKETIZER__H

#include "core/base/byte_buffer_dynamic.h"
#include "core/base/noncopyable.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"
#include "coresh/compress/compress_null.h"
#include "coresh/encode/encode_null.h"

namespace coresh {

class Packetizer : public core::Noncopyable {
 public:
  Packetizer();
  ~Packetizer();

  // Get max size of packet/unpacket.
  size_t GetPacketMaxSize(size_t size) const;
  size_t GetUnpacketMaxSize(size_t size) const;

  // Packet/Unpacket a message.
  // Return >0 : successful.
  // Return =0 : with no need for packeting.
  // Return <0 : failure.
  int Packet(const char *in, size_t size);
  int Unpacket(const char *in, size_t size);

  // Set compress/encode type.
  void SetCompressor(CompressType::Type type);
  void SetEncoder(EncodeType::Type type);

  // Get packet/unpacket message and size.
  inline const char *GetPacketMessage(size_t &size) {
    size = this->packet_size_;
    return this->packet_buffer_.HeadPointer();
  }
  inline const char *GetUnpacketMessage(size_t &size) {
    return this->GetPacketMessage(size);
  }

  // Get current compress/encode type.
  inline CompressType::Type GetCompressType() const {
    return this->compressor_->GetType();
  }
  inline EncodeType::Type GetEncodeType() const {
    return this->encoder_->GetType();
  }

  // Get Buffer
  inline core::SimpleByteBufferDynamic *GetBuffer() {
    return &this->packet_buffer_;
  }


 private:
  Compressor *compressor_;
  Encoder *encoder_;

  CompressNull compress_null_;
  EncodeNull encode_null_;

  core::SimpleByteBufferDynamic buffer_;
  core::SimpleByteBufferDynamic packet_buffer_;
  size_t packet_size_;
};

}  // namespace coresh

#endif  // __CORESH__PACKETIZER__H

