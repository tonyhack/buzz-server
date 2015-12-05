//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 15:46:34.
// File name: packetizer.cc
//
// Description:
// Define class Packetizer.
//

#include "coresh/packetizer.h"

#include "core/async_logging_singleton.h"
#include "coresh/protocol_format.h"

namespace coresh {

Packetizer::Packetizer()
  : compressor_(NULL), encoder_(NULL), packet_size_(0) {}

Packetizer::~Packetizer() {}

size_t Packetizer::GetPacketMaxSize(size_t size) const {
  size = this->encoder_->GetEncodeMaxSize(size);
  size = this->compressor_->GetCompressMaxSize(size);
  return size;
}

size_t Packetizer::GetUnpacketMaxSize(size_t size) const {
  size = this->compressor_->GetUncompressMaxSize(size);
  size = this->encoder_->GetUnencodeMaxSize(size);
  return size;
}

int Packetizer::Packet(const char *in, size_t size) {
  // Packet buffer resize.
  size_t packet_size = this->GetPacketMaxSize(size);
  this->packet_buffer_.PreWrite(packet_size);
  // encode buffer resize.
  size_t encode_size = this->encoder_->GetEncodeMaxSize(size);
  this->buffer_.PreWrite(encode_size);
  // Encode.
  int retcode = this->encoder_->Encode(in, size,
      this->buffer_.HeadPointer(), encode_size);
  if(retcode < 0) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Packet failed, encode error.",
        __FILE__, __LINE__, __FUNCTION__);
    return retcode;
  }
  // Compress.
  if(retcode == 0) {
    retcode = this->compressor_->Compress(in, size,
        this->packet_buffer_.HeadPointer(), packet_size);
  } else {
    retcode = this->compressor_->Compress(this->buffer_.HeadPointer(), encode_size,
        this->packet_buffer_.HeadPointer(), packet_size);
  }
  if(retcode > 0) {
    this->packet_size_ = packet_size;
  } else if(retcode < 0) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Packet failed, compress error.",
        __FILE__, __LINE__, __FUNCTION__);
    return retcode;
  }
  return retcode;
}


int Packetizer::Unpacket(const char *in, size_t size) {
  // Packet buffer resize.
  size_t unpacket_size = this->GetUnpacketMaxSize(size);
  this->buffer_.PreWrite(unpacket_size);
  // Uncompress buffer resize
  size_t uncompress_size = this->compressor_->GetUncompressMaxSize(size);
  // Uncompress.
  int retcode = this->compressor_->Uncompress(in, size,
      this->buffer_.HeadPointer(), uncompress_size);
  if(retcode < 0) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Unpacket failed, uncompress error.",
        __FILE__, __LINE__, __FUNCTION__);
    return retcode;
  }
  // Unencode.
  if(retcode == 0) {
    retcode = this->encoder_->Unecode(in, size,
        this->packet_buffer_.HeadPointer(), unpacket_size);
  } else {
    retcode = this->encoder_->Unecode(this->buffer_.HeadPointer(), uncompress_size,
        this->packet_buffer_.HeadPointer(), unpacket_size);
  }
  if(retcode > 0) {
    this->packet_size_ = packet_size_;
  } else if(retcode < 0) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Unpacket failed, unencode error.",
        __FILE__, __LINE__, __FUNCTION__);
    return retcode;
  }
  return retcode;
}

void Packetizer::SetCompressor(CompressType::Type type) {
  switch(type) {
    case CompressType::TYPE_NULL:
      this->compressor_ = &this->compress_null_;
      break;
    default:
      this->compressor_ = &this->compress_null_;
      break;
  }
}

void Packetizer::SetEncoder(EncodeType::Type type) {
  switch(type) {
    case EncodeType::TYPE_NULL:
      this->encoder_ = &this->encode_null_;
      break;
    default:
      this->encoder_ = &this->encode_null_;
      break;
  }
}

}  // namespace coresh

