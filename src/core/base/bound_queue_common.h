//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:45:04.
// File name: bound_queue_common.h
//
// Description:
// Defines of the class BoundQueue.
//

#ifndef __CORE__BOUND__QUEUE__COMMON__H
#define __CORE__BOUND__QUEUE__COMMON__H

#include <stddef.h>

namespace core {

struct BoundQueueElement {
  size_t data_size_;
  char data_pointer_[0];
};

// Blocking pattern.
enum BlockingPattern {
  NONBLOCKING = 0x0,
  BLOCKING_CONSUMER = 0x1,
  BLOCKING_PRODUCER = 0x2, 
}; 

// Blocking pattern.
const static int kNonBlockingPattern = NONBLOCKING;
const static int kBlockingConsumerPattern = BLOCKING_CONSUMER;
const static int kBlockingProducerPattern = BLOCKING_PRODUCER;
const static int kBlockingAllPattern = BLOCKING_CONSUMER | BLOCKING_PRODUCER;

// Sucessful.
const static int kErrorCodeSuccess = 0;

// Only in non-blocking pattern.
const static int kErrorCodeFull = -1;
const static int kErrorCodeEmpty = -1;
const static int kErrorCodeAllocate = -2;

// Either blocking/non-blocking pattern.
const static int kErrorCodeComplete = -2;
const static int kErrorCodeErrorSize = -3;
const static int kErrorCodeUnknown = -4;

}  // namespace core

// check is empty.
#define RING_QUEUE_EMPTY(read_pointer, write_pointer) \
  ((read_pointer) == (write_pointer))
  
// check is full.
#define RING_QUEUE_FULL(max_size, read_pointer, write_pointer) \
  (((write_pointer + 1) % (max_size + 1)) == (read_pointer))
  
// get free size.
#define RING_QUEUE_FREE_SIZE(max_size, read_pointer, write_pointer, free_size) \
  do { \
    if(RING_QUEUE_EMPTY(read_pointer, write_pointer)) { \
      free_size = max_size; \
    } else if(read_pointer > write_pointer) { \
      free_size = read_pointer - write_pointer - 1; \
    } else { \
      free_size = max_size - (write_pointer - read_pointer); \
    } \
  } while(false)
  
// The size after write_pointer.
#define RING_QUEUE_FREE_SIZE_AFTER_WRITE_POINTER(max_size, read_pointer, write_pointer, free_size_after_write_pointer) \
  do { \
    if(read_pointer == write_pointer) \
      free_size_after_write_pointer = max_size - write_pointer + 1;\
    else if(read_pointer > write_pointer) \
      free_size_after_write_pointer = read_pointer - write_pointer - 1; \
    else \
      if(read_pointer == 0) \
        free_size_after_write_pointer = max_size - write_pointer; \
      else \
        free_size_after_write_pointer = max_size - write_pointer + 1; \
  } while(false)
  
// The size you can read from ring queue.
#define RING_QUEUE_READ_SIZE(max_size, read_pointer, write_pointer, read_size) \
  do { \
    RING_QUEUE_FREE_SIZE(max_size, read_pointer, write_pointer, read_size); \
    read_size = max_size - read_size; \
  } while(false)
  
// The read size after read_pointer.
#define RING_QUEUE_READ_SIZE_AFTER_READ_POINTER(max_size, read_pointer, write_pointer, read_size_after_read_pointer) \
  do { \
    if(write_pointer == read_pointer) \
      read_size_after_read_pointer = 0; \
    else if(write_pointer > read_pointer) \
      read_size_after_read_pointer = write_pointer - read_pointer; \
    else \
      read_size_after_read_pointer = max_size - read_pointer + 1; \
  } while(false)

#endif  // __CORE__BOUND__QUEUE__COMMON__H

