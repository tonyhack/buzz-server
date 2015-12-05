//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-05 10:53:34.
// File name: message_queue_test.cc
//
// Description:
// Define class MessageQueueTest.
//

#include "coresh/test/message_queue_test.h"

#include <libgen.h>
#include <signal.h>

#include <string>

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"

namespace coresh {

namespace test {

bool MessageQueueTest::Initialize() {
  if(this->loop_.Initialize(false) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "Initialize message queue test failed.");
    return false;
  }
  if(this->queue_.Allocate() == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "Allocate message queue failed.");
    return false;
  }
  if(this->queue_.Initialize(&this->loop_) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "Initialize message queue failed.");
    return false;
  }
  this->queue_.SetReadableCallback(boost::bind(&MessageQueueTest::ReceiveMessage, this));

  return true;
}

void MessageQueueTest::Start() {
  this->thread_.StartLoop();
}

void MessageQueueTest::SendMessage(const char *message, size_t size) {
  std::cout << "SendMessage: " << message << std::endl;
  this->queue_.Write(message, size);
}

bool MessageQueueTest::ReceiveMessage() {
  char message[64];
  size_t size = 64;
  int retcode = 0;
  while((retcode = this->queue_.Read(message, size)) > 0) {
    std::cout << "ReceiveMessage: " << message << std::endl;
  }
  return true;
}

}  // namespace test

}  // namespace coresh

int main(int argc, char *argv[]) {
  char log_name[64];
  snprintf(log_name, sizeof(log_name), "%s", ::dirname(argv[0]));

  std::cout << log_name << std::endl;

  AsyncLoggingSingleton::GetAsyncLogging()->Initialize(std::string(log_name) + "-.log", 1024);
  AsyncLoggingSingleton::GetAsyncLogging()->Start();

  coresh::test::MessageQueueTest test;

  if(test.Initialize() == true) {
    test.Start();

    sleep(1);

    int pos = 100;
    while(pos--) {
      char str[32] = {0};
      snprintf(str, sizeof(str), "%d-message", pos);
      std::string message = str;
      test.SendMessage(message.data(), message.size() + 1);
      int i = 100000;
      while(i--);
      // usleep(1);
    }
  } else {
    std::cout << "Initialize test failed." << std::endl;
  }

  return 0;
}

