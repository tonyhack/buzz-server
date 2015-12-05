//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:44:39.
// File name: bound_queue.cc
//
// Description:
// Define class BoundQueue.
// Muti-thread safe.
//

#include "core/base/bound_queue.h"

#include "core/base/logging.h"

namespace core {

BoundQueue::BoundQueue(size_t max_size, int blocking_pattern) 
  : queue_(max_size + 1), size_(max_size),
  write_pointer_(0), read_pointer_(0),
  blocking_pattern_(blocking_pattern),
  terminate_flag_(false) {}

BoundQueue::~BoundQueue() {}

int BoundQueue::Add(const char *data, size_t size) {
  if(size > this->size_) {
    CoreLog(ERROR, "%s:%d (%s) Error, gives an error size(%d)",
        __FILE__, __LINE__, __FUNCTION__, size);
    return kErrorCodeErrorSize;
  }

  AutoMutex auto_mutex(&this->mutex_not_full_);

  // Save the read pointer, in case consumer threads' altering.
  size_t read_pointer = this->read_pointer_;
  size_t need_size = sizeof(BoundQueueElement) + size;
  size_t free_size = 0;
  // Get the free size.
  RING_QUEUE_FREE_SIZE(this->size_, read_pointer,
      this->write_pointer_, free_size);

  // Separate blocking/non-blocking pattern to check ring's full.
  if(this->blocking_pattern_ & BLOCKING_PRODUCER) {
    while(free_size < need_size) {
      this->condition_not_full_.Wait(&this->mutex_not_full_);
      if(this->CheckTerminate()) return kErrorCodeComplete;
      read_pointer = this->read_pointer_;
      RING_QUEUE_FREE_SIZE(this->size_, read_pointer,
          this->write_pointer_, free_size);
    }
  } else {
    if(free_size < need_size) return kErrorCodeFull;
  }

  // Get the real pointer of ring.
  char *ring_pointer = (char *)this->queue_.BufferPointer();
  // Get the free size after write_pointer_.
  size_t after_write_pointer_size = 0;
  RING_QUEUE_FREE_SIZE_AFTER_WRITE_POINTER(
      this->size_, read_pointer,
      this->write_pointer_, after_write_pointer_size);

  if(after_write_pointer_size >= need_size) {
    // Give the size of data at the begin position.
    BoundQueueElement *temp_data
      = (BoundQueueElement *)(&(ring_pointer[this->write_pointer_]));
    temp_data->data_size_ = size;
    memcpy(temp_data->data_pointer_, data, size);
    // This position write_pointer_'s altering must behind the copying.
    this->write_pointer_ += need_size;
  } else {
    // Not enough, copy to the end position and copy the left to the begin position.
    if(after_write_pointer_size >= sizeof(BoundQueueElement)) {
      // The size after the write position have enough space to hold the size of data.
      BoundQueueElement *temp_data =
        (BoundQueueElement *)(&(ring_pointer[this->write_pointer_]));
      temp_data->data_size_ = size;
      after_write_pointer_size -= sizeof(BoundQueueElement);
      memcpy(temp_data->data_pointer_, data, after_write_pointer_size);
      size_t second_part_size = size - after_write_pointer_size;
      memcpy(ring_pointer, &data[after_write_pointer_size], second_part_size);
      this->write_pointer_ = second_part_size;
    } else {
      // Not enough, copy the first part of "size" to the write position, then copy the
      // second part to the begin position, at last, copy the data.
      memcpy(&(ring_pointer[this->write_pointer_]),
          (char *)(&size), after_write_pointer_size);
      size_t second_part_size = sizeof(BoundQueueElement) - after_write_pointer_size;
      memcpy(ring_pointer, &(((char *)(&size))[after_write_pointer_size]),
          second_part_size);
      memcpy(&ring_pointer[second_part_size], data, size);
      // This position write_pointer_'s altering must behind the copying action.
      this->write_pointer_ = need_size - after_write_pointer_size;
    }
  }

  // Blocking pattern.
  if(this->blocking_pattern_ & BLOCKING_CONSUMER) {
    // AutoMutex auto_mutex(&this->mutex_not_empty_);
    this->condition_not_empty_.Signal();
  }

  return size;
}

int BoundQueue::Remove(char *data, size_t size) {
  AutoMutex auto_mutex(&this->mutex_not_empty_);

  // Save the write pointer, in case producer threads' altering.
  size_t write_pointer = this->write_pointer_;
  size_t can_read_size = 0;

  RING_QUEUE_READ_SIZE(this->size_, this->read_pointer_,
      write_pointer, can_read_size);

  // Separate blocking/non-blocking pattern to check ring's empty.
  if(this->blocking_pattern_ & BLOCKING_CONSUMER) {
    // AutoMutex auto_mutex(&this->mutex_not_empty_);
    while(can_read_size == 0) {
      if(this->CheckTerminate()) return kErrorCodeComplete;
      this->condition_not_empty_.Wait(&this->mutex_not_empty_);
      write_pointer = this->write_pointer_;
      // Get the can read size again.
      RING_QUEUE_READ_SIZE(this->size_, this->read_pointer_,
          write_pointer, can_read_size);
    }
  } else {
    if(can_read_size == 0) {
      return kErrorCodeEmpty;
    }
  }

  if(can_read_size < sizeof(BoundQueueElement)) {
    CoreLog(ERROR, "%s:%d (%s) Error, error can read size(%d)",
        __FILE__, __LINE__, __FUNCTION__, can_read_size);
    return kErrorCodeUnknown;
  }

  // Get real pointer of ring.
  char *ring_pointer = (char *)this->queue_.BufferPointer();

  // Get the read size after read_pointer_.
  size_t after_read_pointer_size = 0;
  RING_QUEUE_READ_SIZE_AFTER_READ_POINTER(
      this->size_, this->read_pointer_,
      write_pointer, after_read_pointer_size);

  if(after_read_pointer_size >= sizeof(BoundQueueElement)) {
    BoundQueueElement *temp_data
      = (BoundQueueElement *)(&(ring_pointer[this->read_pointer_]));
    // Check size is enough.
    if(temp_data->data_size_ > size) {
      return kErrorCodeErrorSize;
    }
    size_t read_size = temp_data->data_size_ + sizeof(BoundQueueElement);
    // Check valid
    if(read_size > can_read_size/* || temp_data->data_size_ > (size_t)size*/) {
      CoreLog(ERROR, "%s:%d (%s) Error size, read_size(%d), can_read_size(%d), size(%d)",
          __FILE__, __LINE__, __FUNCTION__, read_size, can_read_size, size);
      return kErrorCodeUnknown;
    }
    // Read data.
    if(after_read_pointer_size > read_size) {
      // All this data data after read_pointer_.
      memcpy(data, temp_data->data_pointer_, temp_data->data_size_);
      this->read_pointer_ += read_size;
    } else if(after_read_pointer_size == read_size) {
      memcpy(data, temp_data->data_pointer_, temp_data->data_size_);
      if(write_pointer > this->read_pointer_) {
        this->read_pointer_ += read_size;
      } else {
        // If read to the end, set read_pointer_ 0 value.
        this->read_pointer_ = 0;
      }
    } else {
      // Copy the data from ring queue, when copy the first part of data, we should
      // filter the head size.
      size_t first_part_size = after_read_pointer_size - sizeof(BoundQueueElement);
      // first part.
      memcpy(data, temp_data->data_pointer_, first_part_size);
      size_t second_part_size = read_size - after_read_pointer_size;
      // second part.
      memcpy(&data[first_part_size], ring_pointer, second_part_size);
      this->read_pointer_ = second_part_size;
    }

    // Blocking pattern.
    if(this->blocking_pattern_ & BLOCKING_PRODUCER) {
      // AutoMutex auto_mutex(&this->mutex_not_full_);
      this->condition_not_full_.Signal();
    }

    return temp_data->data_size_;

  } else {
    // The read size after read_pointer is less than an head size.
    BoundQueueElement temp_data;
    char* head = (char *)(&temp_data);
    size_t second_part_size = sizeof(BoundQueueElement) - after_read_pointer_size;
    // Divide into 2 parts to copy a head.
    memcpy(head, &(ring_pointer[this->read_pointer_]), after_read_pointer_size);
    memcpy(&head[after_read_pointer_size], ring_pointer, second_part_size);
    // Check size is enough.
    if(temp_data.data_size_ > size) {
      return kErrorCodeErrorSize;
    }
    if(can_read_size < temp_data.data_size_ + sizeof(BoundQueueElement)) {
      CoreLog(ERROR, "%s:%d (%s) Error size, read size is too small, can_read_size(%d), need_size(%d)",
          __FILE__, __LINE__, __FUNCTION__, can_read_size, temp_data.data_size_ + sizeof(BoundQueueElement));
      return kErrorCodeUnknown;
    }
    // Copy the data directly.
    memcpy(data, &ring_pointer[second_part_size], temp_data.data_size_);
    this->read_pointer_ = temp_data.data_size_ + second_part_size;

    // Blocking pattern.
    if(this->blocking_pattern_ & BLOCKING_PRODUCER) {
      // AutoMutex auto_mutex(&this->mutex_not_full_);
      this->condition_not_full_.Signal();
    }

    return temp_data.data_size_;
  }
}

}  // namespace core

