//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 12:27:00.
// File name: main.cc
//
// Description:
// Define main.
//

#include <libgen.h>
#include <signal.h>

#include <boost/bind.hpp>

#include "core/configure.h"
#include "core/event_loop.h"
#include "core/signal_set.h"
#include "core/base/logging.h"
#include "core/async_logging_singleton.h"
#include "test/chat/server/configure.h"
#include "test/chat/server/chat_server.h"

using namespace test::chat::server;

void OnTerminal(core::EventLoop *loop) {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO, "Chat server terminal!");
  loop->Quit();
}

int main(int argc, char *argv[]) {
  typedef std::map<std::string, std::string> ArgsList;
  ArgsList args;
  int pos = 1;
  while(pos + 1 < argc) {
    args.insert(std::make_pair(argv[pos], argv[pos + 1]));
    ++pos;
  }

  bool daemon_flag = true;


  ArgsList::iterator iterator = args.find("-daemon");
  if(iterator != args.end()) {
    if(iterator->second == "true") {
      daemon_flag = true;
    }
  }

  iterator = args.find("-configure");
  if(iterator == args.end()) {
    std::cout << "Need argument -configure [configure file]." << std::endl;
    return 0;
  }

  std::string configure_file = iterator->second;

  char server_log_name[64];
  snprintf(server_log_name, sizeof(server_log_name), "%s", ::basename(argv[0]));

  Configure configure;
  if(configure.Load(configure_file.c_str()) == false) {
    std::cout << "Load " << configure_file << " failed." << std::endl;
    return 0;
  }


  core::Configure::Initialize(configure.core_configure_);
  if(core::Configure::CheckFinishLoad() == false) {
    std::cout << "load configure error!!" << std::endl;
    return 0;
  }

  core::EventLoop loop;
  if(loop.Initialize(false) == false) {
    std::cout << "Initialize event loop failed." << std::endl;
    return 0;
  }

  if(daemon_flag == true) {
    if(daemon(1, 1) != 0) {
      std::cout << "failed to daemon." << std::endl;
      return 0;
    }
  }


  std::string core_log_file = configure.log_dir_ + "core-" + server_log_name + ".log";
  core::InitializeLogger(core_log_file, core_log_file, core_log_file);

  ChatServer server(core::InetAddress(configure.host_, configure.port_), "ChatServer");
  if(server.Initialize(configure.log_dir_ + server_log_name) == false) {
    std::cout << "Initialize chat server failed." << std::endl;
    return 0;
  }

  server.Start();

  core::SignalSet::GetSingleton()->Insert(SIGTERM, boost::bind(OnTerminal, &loop));

  loop.Loop();

  server.Stop();


  return 0;
}

