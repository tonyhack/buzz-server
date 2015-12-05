//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-26 12:08:46.
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
#include "global/server_configure.h"
#include "global/configure/configure.h"
#include "session_server/server/session_server.h"

void OnTerminal(core::EventLoop *loop) {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "session server terminal!");
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
    std::cout << "[session_server] Need argument -conf [configure file]." << std::endl;
    return 0;
  }
  std::string configure_file = iterator->second;

  // Server log file.
  char server_log_name[64];
  snprintf(server_log_name, sizeof(server_log_name), "%s", ::basename(argv[0]));

  // Server configure.
  global::ServerConfigure configure;
  if(global::ServerConfigureSingleton::GetInstance().LoadFile(configure_file.c_str()) == false) {
    std::cout << "[session_server] Load " << configure_file << " failed." << std::endl;
    return 0;
  }
  const global::SessionServerInformation *server_info = 
    global::ServerConfigureSingleton::GetInstance().GetSessionServer();
  if(server_info == NULL) {
    std::cout << "[session_server] Get server information failed." << std::endl;
    return 0;
  }

  // coresh configure.
  if(global::ServerConfigureSingleton::GetInstance().LoadCoreshFile(
        server_info->coresh_.c_str()) == false) {
    std::cout << "[session_server] Load coresh configure file ("
      << server_info->coresh_ << ") failed." << std::endl;
  }

  // Core configure.
  core::Configure::Initialize(server_info->core_);
  if(core::Configure::CheckFinishLoad() == false) {
    std::cout << "[session_server] Load core configure failed." << std::endl;
    return 0;
  }

  // Initialize main event loop.
  core::EventLoop loop;
  if(loop.Initialize(false) == false) {
    std::cout << "[session_server] Initialize main event loop failed." << std::endl;
    return 0;
  }

  // Daemon.
  if(daemon_flag == true) {
    if(daemon(1, 1) != 0) {
      std::cout << "[session_server] failed to daemon." << std::endl;
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
    std::cout << "[session_server] failed to open async log." << std::endl;
    return 0;
  }
  logging->Start();

  // Load global configure.
  if (global::configure::ConfigureSingleton::GetInstance().LoadConfigure(
          configure_file.c_str()) == false) {
    std::cout << "[session_server] failed to load global configure." << std::endl;
    logging->Stop();
    return 0;
  }

  global::protocol::ServerSignature signature;
  signature.__set_type_(global::protocol::ServerType::SESSION_SERVER);
  signature.__set_id_(0);
  signature.__set_host_(server_info->ip_);
  signature.__set_port_(server_info->port_);

  // Server initialize.
  session::server::SessionServer server(
      core::InetAddress(server_info->ip_, server_info->port_), "SessionServer");
  if(server.Initialize(signature,
         core::InetAddress(server_info->admin_ip_, server_info->admin_port_)) == false) {
    std::cout << "[session_server] Initialize test server failed." << std::endl;
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
        std::cout << "[session_server] Write pid file failed." << std::endl;
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

  loop.Finalize();

  return 0;
}

