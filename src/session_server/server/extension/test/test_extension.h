//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-25 11:03:16.
// File name: test_extension.h
//
// Description:
// Define class TestExtension.
//

#ifndef __SESSION__SERVER__TEST__TEST__EXTENSION__H
#define __SESSION__SERVER__TEST__TEST__EXTENSION__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension/test/event_handler.h"

namespace session {

namespace server {

namespace test {

class TestExtension : public core::Noncopyable {
 public:
  TestExtension();
  ~TestExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  EventHandler event_handler_;
};

}  // namespace test

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEST__TEST__EXTENSION__H

