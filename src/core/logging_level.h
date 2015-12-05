//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:40:09.
// File name: logging_level.h
//
// Description:
// Define misc of logging.
//

#ifndef __CORE__LOGGING__LEVEL__H
#define __CORE__LOGGING__LEVEL__H

namespace core {

enum LoggingLevel {
  LOGGING_LEVEL_DEBUG = 0,
  LOGGING_LEVEL_WARNING,
  LOGGING_LEVEL_INFO,
  LOGGING_LEVEL_ERROR,
  LOGGING_LEVEL_FATAL,
  LOGGING_LEVEL_MAX,
};

}  // namespace core

#endif  // __CORE__LOGGING__LEVEL__H

