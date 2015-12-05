//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-17 11:36:43.
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
#include "database_server/server/database_server.h"
#include "global/server_configure.h"

void OnTerminal(core::EventLoop *loop) {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "database server terminal!");
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
    std::cout << "[database_server] Need argument -conf [configure file]." << std::endl;
    return 0;
  }
  std::string configure_file = iterator->second;

  // ID argument.
  iterator = args.find("-id");
  if(iterator == args.end()) {
    std::cout << "[database_server] Need argument -id [number]." << std::endl;
    return 0;
  }
  core::uint32 server_id = (core::uint32)atoi(iterator->second.c_str());

  // Server log file.
  char server_log_name[64];
  snprintf(server_log_name, sizeof(server_log_name), "%s-%u", ::basename(argv[0]), server_id);

  // Server configure.
  global::ServerConfigure configure;
  if(global::ServerConfigureSingleton::GetInstance().LoadFile(configure_file.c_str()) == false) {
    std::cout << "[database_server] Load " << configure_file << " failed." << std::endl;
    return 0;
  }
  const global::DatabaseServerInformation *server_info = 
    global::ServerConfigureSingleton::GetInstance().GetDatabaseServers(server_id);
  if(server_info == NULL) {
    std::cout << "[database_server] Get server information failed." << std::endl;
    return 0;
  }

  // coresh configure.
  if(global::ServerConfigureSingleton::GetInstance().LoadCoreshFile(
          server_info->coresh_.c_str()) == false) {
    std::cout << "[database_server] Load coresh configure file ("
      << server_info->coresh_ << ") failed." << std::endl;
    return 0;
  }

  // Core configure.
  core::Configure::Initialize(server_info->core_);
  if(core::Configure::CheckFinishLoad() == false) {
    std::cout << "[database_server] Load core configure failed." << std::endl;
    return 0;
  }

  // Initialize main event loop.
  core::EventLoop loop;
  if(loop.Initialize(false) == false) {
    std::cout << "[database_server] Initialize main event loop failed." << std::endl;
    return 0;
  }

  // Daemon.
  if(daemon_flag == true) {
    if(daemon(1, 1) != 0) {
      std::cout << "[database_server] failed to daemon." << std::endl;
      return 0;
    }
  }

  // Core log.
  std::string core_log_file = server_info->log_ + "core-" + server_log_name + ".log";
  core::InitializeLogger(core_log_file, core_log_file, core_log_file);

  // Async log.
  std::string async_log_file = server_info->log_ + server_log_name;
  core::AsyncLogging *logging = AsyncLoggingSingleton::GetAsyncLogging();
  if(NULL == logging || logging->Initialize(async_log_file, core::kMaxLoggingSize) == false) {
    std::cout << "[database_server] failed to open async log." << std::endl;
    return 0;
  }
  logging->Start();

  // Server initialize
  database::server::DatabaseServer server(core::InetAddress(server_info->ip_, server_info->port_),
      "DatabaseServer");
  if(server.Initialize(server_id) == false) {
    std::cout << "[database_server] Initialize database server failed." << std::endl;
    ::usleep(100);
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
        std::cout << "[database_server] Write pid file failed." << std::endl;
        logging->Stop();
        return 0;
      }
    }
  }

  if(server.Start() == false) {
    ::usleep(100);
    std::cout << "[database_server] Start database server failed." << std::endl;
    logging->Stop();
    return 0;
  }

  loop.Loop();
  server.Stop();
  server.Finalize();
  logging->Stop();

  loop.Finalize();

  return 0;
}

