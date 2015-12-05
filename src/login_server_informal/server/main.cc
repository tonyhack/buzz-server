//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 18:02:59.
// File name: main.cc
//
// Description:
// Define main.
//

#include <libgen.h>
#include <signal.h>

#include <map>
#include <string>

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"
#include "core/configure.h"
#include "core/event_loop.h"
#include "core/signal_set.h"
#include "core/base/logging.h"
#include "coresh/system_functions.h"
#include "login_server_informal/server/login_server.h"
#include "login_server_informal/server/server_configure.h"

void OnTerminal(core::EventLoop *loop) {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "login server terminal!");
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

  // Daemon argument.
  bool daemon_flag = true;
  ArgsList::iterator iterator = args.find("-daemon");
  if(iterator != args.end()) {
    if(iterator->second == "false") {
      daemon_flag = false;
    }
  }

  // Configure argument.
  iterator = args.find("-conf");
  if(iterator == args.end()) {
    std::cout << "[login server] Need argument -conf [configure file]." << std::endl;
    return 0;
  }
  std::string configure_file = iterator->second;

  // Server log file.
  char server_log_name[64];
  snprintf(server_log_name, sizeof(server_log_name), "%s", ::basename(argv[0]));

  // Server configure.
  login::server::ServerConfigure configure;
  if(login::server::ServerConfigure::GetInstance()->LoadFile(configure_file.c_str()) == false) {
    std::cout << "[login server] Load " << configure_file << " failed." << std::endl;
    return 0;
  }
  const login::server::ServerInformation *server_info = 
    login::server::ServerConfigure::GetInstance()->GetLocalServer();
  if(server_info == NULL) {
    std::cout << "[login server] Get server information failed." << std::endl;
    return 0;
  }

  // Core configure.
  core::Configure::Initialize(login::server::ServerConfigure::GetInstance()->GetCore());
  if(core::Configure::CheckFinishLoad() == false) {
    std::cout << "[login server] Load core configure failed." << std::endl;
    return 0;
  }

  // Initialize main event loop.
  core::EventLoop loop;
  if(loop.Initialize(false) == false) {
    std::cout << "[login server] Initialize main event loop failed." << std::endl;
    return 0;
  }

  // Daemon.
  if(daemon_flag == true) {
    if(daemon(1, 1) != 0) {
      std::cout << "[login server] failed to daemon." << std::endl;
      return 0;
    }
  }

  // Core log.
  std::string core_log_file = login::server::ServerConfigure::GetInstance()->GetLog()
    + "core-" + server_log_name + ".log";
  core::InitializeLogger(core_log_file, core_log_file, core_log_file);

  // Async log.
  std::string async_log_file = login::server::ServerConfigure::GetInstance()->GetLog() + server_log_name;
  core::AsyncLogging *logging = AsyncLoggingSingleton::GetAsyncLogging();
  if(NULL == logging || logging->Initialize(async_log_file, core::kMaxLoggingSize) == false) {
    std::cout << "[login_server] failed to open async log." << std::endl;
    return 0;
  }
  logging->Start();

  // Server initialize.
  login::server::LoginServer server(core::InetAddress(server_info->ip_, server_info->port_),
      "SessionServer");
  if(server.Initialize() == false) {
    std::cout << "[login server] Initialize login server failed." << std::endl;
    logging->Stop();
    return 0;
  }

  // Terminal signal.
  core::SignalSet::GetSingleton()->Insert(SIGTERM, boost::bind(OnTerminal, &loop));

  // Write pid file.
  iterator = args.find("-pidfile");
  if(iterator != args.end()) {
    if(!iterator->second.empty()) {
      if(coresh::CreatePidFile(iterator->second.c_str()) == false) {
        std::cout << "[login server] Write pid file failed." << std::endl;
        logging->Stop();
        return 0;
      }
    }
  }

  server.Start();

  loop.Loop();
  server.Stop();
  server.Finalize();
  logging->Stop();

  return 0;
}

