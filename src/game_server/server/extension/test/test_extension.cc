//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-10 11:24:02.
// File name: test_extension.cc
//
// Description:
// Define class TestExtension.
//

#include "game_server/server/extension/test/test_extension.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace test {

TestExtension::TestExtension() {}
TestExtension::~TestExtension() {}

bool TestExtension::Initialize() {
  if(this->event_handler_.Initialize() == false) {
    LOG_ERROR("初始化事件回调失败");
    return false;
  }
  return true;
}

void TestExtension::Finalize() {
  this->event_handler_.Finalize();
}

void TestExtension::Start() {}

void TestExtension::Stop() {}

}  // namespace test

}  // namespace server

}  // namespace game

