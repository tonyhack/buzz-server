//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:45:18.
// File name: byte_buffer_dynamic.h
//
// Description:
// Define class ByteBufferDynamic.
//
// This is a dynamic length buffer, using for socket to receive/send data.
// This buffer contains two pointers, you can use these pointers to read/write
// data from/to it. Before you write any data to buffer, you should check the
// buffer is enough using function - "PreWrite" (this function can reallocate
// the buffer when the buffer is not enough), After reading/writing, you should
// shift the buffer using the function - "ReadShift/WriteShift".
// This ByteBufferDynamic supports ordering read and random read, but only
// supports ordering write.
//

#ifndef __CORE__BYTE__BUFFER__DYNAMIC__H
#define __CORE__BYTE__BUFFER__DYNAMIC__H

#include <assert.h>
#include <cstddef>
#include <vector>

#include "core/base/types.h"

namespace core{

// A wrapper of vector<char>
class SimpleByteBufferDynamic {
  typedef std::vector<char> BufferVector;
 public:
  SimpleByteBufferDynamic() {}
  ~SimpleByteBufferDynamic() {}

  inline bool PreWrite(int size) {
    if(this->buffer_.size() < (uint32)size)
      this->buffer_.resize(size);
    return true;
  }
  inline char *RandomPointer(int position) {
    return &this->buffer_[position];
  }
  inline char *HeadPointer() {
    return &this->buffer_[0];
  }
  inline size_t GetSize() const {
    return this->buffer_.size();
  }
  inline size_t GetCapacitySize() const {
    return this->buffer_.capacity();
  }

 private:
  BufferVector buffer_;
};


class ByteBufferDynamic {
  // buffer typedefs.
  typedef std::vector<char> BufferVector;
  typedef BufferVector::iterator BufferVectorIterator;

 public:
  explicit ByteBufferDynamic(int unit_buffer_size)
    : buffer_size_(0), read_index_(0), write_index_(0),
    unit_buffer_size_(unit_buffer_size), allocate_count_(0),
    kmax_count_(kint32max / unit_buffer_size_) {}

  ~ByteBufferDynamic() {}

  inline void Initialize() {
    this->read_index_ = 0;
    this->write_index_ = 0;
    this->allocate_count_ = 0;
  }

  inline void Finalize() {
    this->ReduceBufferSize();
    this->read_index_ = 0;
    this->write_index_ = 0;
    this->allocate_count_ = 0;
  }

  // Check the buffer is enough or reallocate the buffer.
  bool PreWrite(const int size);

  // Get position of read/write index.
  inline int ReadIndex() const {
    return this->read_index_;
  }
  inline int WriteIndex() const {
    return this->write_index_;
  }

  // Reduce the size of buffer.
  void ReduceBufferSize();

  void Reset() {
    this->read_index_ = this->write_index_ = 0;
  }

  ////////////////////////////////////////////////////////////
  // This methods give an ordering read/write scheme.
  // Get the pointer of read/write position.
  inline const char *ReadPointer() const {
    return &this->buffer_[this->read_index_];
  }
  inline char *WritePointer() {
    return &this->buffer_[this->write_index_];
  }

  // Get size of read/write.
  inline int ReadSize() const {
    assert(this->allocate_count_ < kmax_count_);
    return this->write_index_ - this->read_index_;
  }
  inline int WriteSize() const {
    assert(this->allocate_count_ < kmax_count_);
    return this->buffer_size_ - this->write_index_;
  }

  // shift the buffer after reading/writing.
  void ReadShift(int size);
  void WriteShift(int size);
  ////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////
  // This methods give an random read scheme.
  // Get the cosnt pointer of given position.
  inline const char *RandomReadPointer(int position) const {
    return &this->buffer_[position];
  }
  // Get the read size of given position.
  inline int RandomReadSize(int position) const {
    assert(this->allocate_count_ < kmax_count_);
    return this->write_index_ - position;
  }
  // Shift the buffer of given size.
  void RandomReadShift(int position, int size);
  ////////////////////////////////////////////////////////////


  inline int GetUnitBufferSize() const { return this->unit_buffer_size_; }

 private:
  // the real shift.
  void Shift(int read_size, int write_size);

  BufferVector buffer_;
  int buffer_size_;
  int read_index_;
  int write_index_;

  // EveryTime need to allocate the buffer, this value is the append length.
  int unit_buffer_size_;

  int allocate_count_;
  const int kmax_count_;
};

}  // namespace core

#endif  // __CORE__BYTE__BUFFER__DYNAMIC__H

