//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-06 14:52:55.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __TEST__CHAT__PRESSURE__CONFIGURE__H
#define __TEST__CHAT__PRESSURE__CONFIGURE__H

#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace test {

namespace chat {

namespace pressure {

class Configure : public core::Noncopyable {
  typedef std::vector<std::string> ChatVector;

 public:
  Configure();
  ~Configure();

  bool LoadFile(const char *file);

  const std::string &GetChat(core::uint32 pos) {
    pos %= this->chats_.size();
    return this->chats_[pos];
  }

  std::string host_;
  core::uint16 port_;

  size_t thread_number_;
  size_t chat_frequency_;

  std::string core_configure_;
  std::string log_dir_;

  ChatVector chats_;
};

class ConfigureSingleton : public core::Noncopyable {
 public:
  ConfigureSingleton() {}
  ~ConfigureSingleton() {}

  static Configure &GetInstance() {
    return ConfigureSingleton::instance_;
  }

 private:
  static Configure instance_;
};

}  // pressure

}  // namespace chat

}  // namespace test

#endif  // __TEST__CHAT__PRESSURE__CONFIGURE__H

