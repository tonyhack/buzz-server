//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 18:14:17.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __TEST__CHAT__CLIENT__CONFIGURE__H
#define __TEST__CHAT__CLIENT__CONFIGURE__H

#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace test {

namespace chat {

namespace client {

class Configure : public core::Noncopyable {
 public:
  Configure();
  ~Configure();

  bool Load(const char *file);

  std::string host_;
  core::uint16 port_;

  core::uint8 thread_number_;

  std::string core_configure_;

  std::string log_dir_;
};

}  // namespace client

}  // namespace chat

}  // namespace test

#endif  // __TEST__CHAT__CLIENT__CONFIGURE__H

