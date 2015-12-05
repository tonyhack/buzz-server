//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-10 11:21:37.
// File name: test_extension.h
//
// Description:
// Define class TestExtension.
//

#ifndef __GAME__SERVER__TEST__TEST__EXTENSION__H
#define __GAME__SERVER__TEST__TEST__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/test/event_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace test {

class TestExtension : public global::SingletonFactory<TestExtension> {
 friend class global::SingletonFactory<TestExtension>;
 public:
  TestExtension();
  ~TestExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  EventHandler event_handler_;
};

}  // namespace test

}  // namespace server

}  // namespace game

#endif // __GAME__SERVER__TEST__TEST__EXTENSION__H

