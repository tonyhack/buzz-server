//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:46:43.
// File name: logging.cc
//
// Description:
// Define class logger.
//

#include "core/base/logging.h"

#include <stdlib.h>
#include <time.h>

#include "core/base/auto_mutex.h"
#include "core/base/mutex.h"
#include "core/base/variable_parameter.h"

namespace core {


std::ofstream Logger::info_log_file_;
std::ofstream Logger::warn_log_file_;
std::ofstream Logger::erro_log_file_;

void InitializeLogger(const std::string& info_log_filename,
                      const std::string& warn_log_filename,
                      const std::string& erro_log_filename) {
  Logger::info_log_file_.open(info_log_filename.c_str(), std::ios::app);
  Logger::warn_log_file_.open(warn_log_filename.c_str(), std::ios::app);
  Logger::erro_log_file_.open(erro_log_filename.c_str(), std::ios::app);
}

/*static*/
std::ostream& Logger::GetStream(LogSeverity severity) {
  return (severity == INFO) ?
      (info_log_file_.is_open() ? info_log_file_ : std::cout) :
      (severity == WARNING ?
       (warn_log_file_.is_open() ? warn_log_file_ : std::cerr) :
       (erro_log_file_.is_open() ? erro_log_file_ : std::cerr));
}

/*static*/
std::ostream& Logger::Start(LogSeverity severity) {
  static const std::string kServrityString[MAX] = { "INFO", "WARNING", "ERROR", "FATAL" };
  time_t tm;
  time(&tm);
  char time_string[128];
  ctime_r(&tm, time_string);
/*
  return GetStream(severity) //<< time_string
                             << " [" << kServrityString[severity] << "] " << file << ":" << line
                             << " (" << function << ") " << std::flush;
*/
  return GetStream(severity) << time_string << " [" << kServrityString[severity] << "] " << std::flush;
}

Logger::~Logger() {
  GetStream(severity_) << "\n" << std::flush;

  if (severity_ == FATAL) {
    info_log_file_.close();
    warn_log_file_.close();
    erro_log_file_.close();
    abort();
  }
}

void CoreLog(LogSeverity level, const char *content, ...) {
  static Mutex mutex;
  static const uint32 kMaxLogLength = 10240;

  char log[kMaxLogLength + 1] = {0};
  memset(log, 0, sizeof(log));
  GET_VARIABLE_PARAMETERS(log, sizeof(log), content);

  AutoMutex auto_mutex(&mutex);
  LOG(level) << log;
}


}  // namespace core

