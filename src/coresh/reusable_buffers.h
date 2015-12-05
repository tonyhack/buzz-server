//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 12:26:26.
// File name: reusable_buffers.h
//
// Description:
// Define class ReusableBuffers.
//

#ifndef __CORESH__REUSABLE__BUFFER__H
#define __CORESH__REUSABLE__BUFFER__H

#include <map>

#include "core/event_loop.h"
#include "core/base/byte_buffer_dynamic.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace coresh {

class ReusableBuffers : public core::Noncopyable {
  typedef std::map<const core::EventLoop *, core::SimpleByteBufferDynamic> BufferMap;
 public:
  ReusableBuffers() {}
  ~ReusableBuffers() {}

  bool Allocate(const core::EventLoop *loop) {
    BufferMap::iterator iterator = this->buffers_.find(loop);
    if(iterator == this->buffers_.end()) {
      this->buffers_.insert(std::make_pair(loop, core::SimpleByteBufferDynamic()));
      return true;
    }
    return false;
  }

  bool Deallocate(const core::EventLoop *loop) {
    BufferMap::iterator iterator = this->buffers_.find(loop);
    if(iterator != this->buffers_.end()) {
      this->buffers_.erase(iterator);
      return true;
    }
    return false;
  }

  core::SimpleByteBufferDynamic *GetBuffer(const core::EventLoop *loop) {
    BufferMap::iterator iterator = this->buffers_.find(loop);
    if(iterator != this->buffers_.end()) {
      return &iterator->second;
    }
    return NULL;
  }

 private:
  BufferMap buffers_;
};

class ReusableBuffersSingleton : public core::Noncopyable {
 public:
  ReusableBuffersSingleton() {}
  ~ReusableBuffersSingleton() {}

  static inline ReusableBuffers &GetInstance() {
    return ReusableBuffersSingleton::buffers_;
  }

 private:
  static ReusableBuffers buffers_;
};

}  // namespace coresh

#endif  // __CORESH__REUSABLE__BUFFER__H

