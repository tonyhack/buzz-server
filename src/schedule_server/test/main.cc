//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-11 14:25:22.
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
#include "global/server_configure.h"
#include "global/server_type_types.h"
#include "schedule_server/test/application_server.h"

void OnTerminal(core::EventLoop *loop) {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "schedule test terminal!");
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
    std::cout << "Need argument -conf [configure file]." << std::endl;
    return 0;
  }
  std::string configure_file = iterator->second;

  // ID argument.
  iterator = args.find("-id");
  if(iterator == args.end()) {
    std::cout << "Need argument -id [number]." << std::endl;
    return 0;
  }
  core::uint32 id = (core::uint32)atoi(iterator->second.c_str());

  // Server log file.
  char server_log_name[64];
  snprintf(server_log_name, sizeof(server_log_name), "%s", ::basename(argv[0]));

  // Server configure.
  if(global::ServerConfigureSingleton::GetInstance().LoadFile(configure_file.c_str()) == false) {
    std::cout << "Load " << configure_file << " failed." << std::endl;
    return 0;
  }
  const global::ServerInformation *server_info =
    global::ServerConfigureSingleton::GetInstance().GetGatewayServers(id);
  if(server_info == NULL) {
    std::cout << "Get server information failed." << std::endl;
    return 0;
  }

  if(global::ServerConfigureSingleton::GetInstance().LoadCoreshFile(
        server_info->coresh_.c_str()) == false) {
    std::cout << "Load coresh configure file ("
      << global::ServerConfigureSingleton::GetInstance().LoadCoreshFile(
          server_info->coresh_.c_str())
      << ") failed." << std::endl;
    return 0;
  }

  // Core configure.
  core::Configure::Initialize(server_info->core_);
  if(core::Configure::CheckFinishLoad() == false) {
    std::cout << "Load core configure failed." << std::endl;
    return 0;
  }

  // Initialize main event loop.
  core::EventLoop loop;
  if(loop.Initialize(false) == false) {
    std::cout << "Initialize main event loop failed." << std::endl;
    return 0;
  }

  // Daemon.
  if(daemon_flag == true) {
    if(daemon(1, 1) != 0) {
      std::cout << "failed to daemon." << std::endl;
      return 0;
    }
  }

  // Core log.
  std::string core_log_file = server_info->log_ + "core-" + server_log_name + ".log";
  core::InitializeLogger(core_log_file, core_log_file, core_log_file);

  global::protocol::ServerSignature signature;
  signature.__set_type_(global::protocol::ServerType::GATEWAY_SERVER);
  signature.__set_id_(id);
  signature.__set_host_(server_info->ip_);
  signature.__set_port_(server_info->port_);

  // Server initialize.
  schedule::test::ApplicationServer test(core::InetAddress(server_info->ip_, server_info->port_),
      "TestServer");
  if(test.Initialize(server_info->log_ + server_log_name + "-" + iterator->second, signature) == false) {
    std::cout << "Initialize test server failed." << std::endl;
    return 0;
  }

  // Terminal signal.
  core::SignalSet::GetSingleton()->Insert(SIGTERM, boost::bind(OnTerminal, &loop));

  test.Start();

  loop.Loop();
  test.Stop();

  return 0;
}

