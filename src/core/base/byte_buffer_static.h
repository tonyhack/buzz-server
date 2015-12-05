//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:45:39.
// File name: byte_buffer_static.h
//
// Description:
// Define class ByteBufferStatic.
// This is a fixed length buffer.
//

#ifndef __CORE__BYTE__BUFFER__STATIC
#define __CORE__BYTE__BUFFER__STATIC

#include "core/base/logging.h"

namespace core {

// Use stack memory.
template <size_t BufferSize>
class ByteBufferStatic {
 public:
  ByteBufferStatic() {}
  ~ByteBufferStatic() {}

  inline uint8 *BufferPointer() const { return this->buffer_; }

 private:
  uint8 buffer_[BufferSize];
};

// Use heap memory.
class ByteBufferStaticMalloc {
 public:
  explicit ByteBufferStaticMalloc(size_t buffer_size) : buffer_(NULL) {
    this->buffer_ = new uint8[buffer_size];
    if(this->buffer_ == NULL) {
    LOG(FATAL) << "Not enough memory for ByteBufferStaticMalloc";
    }
  }

  ByteBufferStaticMalloc(void *data, size_t size) {
    this->buffer_ = new uint8[size];
    memcpy(this->buffer_, data, size);
  }

  ~ByteBufferStaticMalloc() {
    if(this->buffer_) {
      delete [] this->buffer_;
      this->buffer_ = NULL;
    }
  }

  inline uint8 *BufferPointer() const { return this->buffer_; }

 private:
  uint8 *buffer_;
};

}  // namespace core

#endif  // __CORE__BYTE__BUFFER__STATIC

