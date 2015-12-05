//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:33:54.
// File name: async_logging_loop.cc
//
// Description:
// Define class AsyncLoggingLoop.
//

#include "core/async_logging_loop.h"

#include <fstream>

#include "core/base/byte_buffer_dynamic.h"
#include "core/base/auto_mutex.h"
#include "core/logging_level.h"

namespace core {

AsyncLoggingLoop::AsyncLoggingLoop ()
  : file_suffix_(".log"), flush_interval_msecs_(5) {}

AsyncLoggingLoop::~AsyncLoggingLoop() {}

bool AsyncLoggingLoop::Initialize(const std::string file, int max_logging_size, int flush_interval_msecs) {
  this->file_ = file;
  this->flush_interval_msecs_ = flush_interval_msecs;
  this->logging_buffer_.reset(new (std::nothrow) core::ByteBufferDynamic(max_logging_size));
  if(this->logging_buffer_ == NULL) {
    return false;
  } else {
    return true;
  }
}

void AsyncLoggingLoop::Loop() {
  this->looping_ = true;
  int read_size = 0;

  while(this->quit_ == false) {
    core::AutoMutex auto_mutex(&this->mutex_);
    while((read_size = this->logging_buffer_->ReadSize()) <= 0 && this->quit_ == false) {
      this->condition_.Wait(&this->mutex_, this->flush_interval_msecs_);
    }

    if(this->quit_ == true) {
      break;
    }

    time_t now;
    now = time(NULL);
    tm *tm_now = localtime(&now);

    char file_time[32];
    strftime(file_time, sizeof(file_time), "-%Y-%m-%d", tm_now);

    this->temp_file_ = "";
    this->temp_file_ += this->file_ + file_time + this->file_suffix_;

    std::ofstream out;
    out.open(this->temp_file_.c_str(), std::ios::app);
    out.write(this->logging_buffer_->ReadPointer(), read_size);

    this->logging_buffer_->ReadShift(read_size);
  }

  this->looping_ = false;
}

void AsyncLoggingLoop::Wakeup() {
  core::AutoMutex auto_mutex(&this->mutex_);
  this->condition_.Signal();
}

void AsyncLoggingLoop::Logging(LoggingLevel level, const char *log) {
  static const std::string kLoggingLevelString[LOGGING_LEVEL_MAX] = {
    "DEBUG", "WARN", "INFO", "ERROR", "FATAL"
  };

  time_t now;
  now = time(NULL);
  tm *tm_now = localtime(&now);
  char current_time[32] = {0};
  strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", tm_now);

  core::AutoMutex auto_mutex(&this->mutex_);

  snprintf(this->buffer_, sizeof(this->buffer_), "[%s] [%s] %s\n",
      current_time, kLoggingLevelString[level].c_str(), log);

  int size = strlen(this->buffer_);
  if(this->logging_buffer_->PreWrite(size)) {
    memcpy(this->logging_buffer_->WritePointer(), this->buffer_, size);
    this->logging_buffer_->WriteShift(size);
  }

  this->condition_.Signal();
}

}  // namespace core

