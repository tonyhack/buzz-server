//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-07 10:13:50.
// File name: main.cc
//
// Description:
// Define main.
//

#include <libgen.h>
#include <signal.h>

#include <string>

#include <boost/bind.hpp>

#include "core/configure.h"
#include "core/event_loop.h"
#include "core/signal_set.h"
#include "core/base/logging.h"
#include "core/async_logging_singleton.h"
#include "coresh/packetizer.h"
#include "coresh/packetizer_table.h"
#include "coresh/reusable_buffers.h"
#include "coresh/variable_table.h"
#include "test/chat/pressure_client/configure.h"
#include "test/chat/pressure_client/pressure_client_set.h"

using namespace test::chat::pressure;

void OnTerminal(core::EventLoop *loop) {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO, "Pressure client terminal!");
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

  ArgsList::iterator iterator = args.find("-configure");
  if(iterator == args.end()) {
    std::cout << "Need argument -configure [configure file]." << std::endl;
  }
  std::string configure_file = iterator->second;

  iterator = args.find("-name");
  if(iterator == args.end()) {
    std::cout << "Need argument -name [name]" << std::endl;
    return 0;
  }
  std::string template_name = iterator->second;

  iterator = args.find("-client");
  if(iterator == args.end()) {
    std::cout << "Need argument -client [number]" << std::endl;
    return 0;
  }
  core::uint16 client_number = (core::uint16)atoi(iterator->second.c_str());

  if(ConfigureSingleton::GetInstance().LoadFile(configure_file.c_str()) == false) {
    std::cout << "Load configure error!!" << std::endl;
    return 0;
  }

  core::Configure::Initialize(ConfigureSingleton::GetInstance().core_configure_);
  if(core::Configure::CheckFinishLoad() == false) {
    std::cout << "load configure error!!" << std::endl;
    return 0;
  }

  char server_log_name[64];
  snprintf(server_log_name, sizeof(server_log_name), "%s", ::basename(argv[0]));

  std::string core_log_file = ConfigureSingleton::GetInstance().log_dir_ + "core-" +
    server_log_name + ".log";

  core::InitializeLogger(core_log_file, core_log_file, core_log_file);


  core::EventLoop loop;
  if(loop.Initialize(false) == false) {
    std::cout << "Initialize event loop failed." << std::endl;
    return 0;
  }

  if(daemon(1, 1) != 0) {
    std::cout << "failed to daemon." << std::endl;
    return 0;
  }

  core::AsyncLogging *logging = AsyncLoggingSingleton::GetAsyncLogging();
  if(logging && logging->Initialize(ConfigureSingleton::GetInstance().log_dir_ +
        server_log_name + "-" + template_name, core::kMaxLoggingSize)) {
    logging->Start();
  } else {
    std::cout << "Initialize asynchronous loggin failed." << std::endl;
    return 0;
  }

  PressureClientSet client(core::InetAddress(ConfigureSingleton::GetInstance().host_,
        ConfigureSingleton::GetInstance().port_), template_name);
  PressureClientSetSingleton::SetInstance(&client);
  if(client.Allocate() == false) {
    std::cout << "Allocate pressure client set failed." << std::endl;
    return 0;
  }
  if(client.Initialize(client_number) == false) {
    std::cout << "Initialize pressure client set failed." << std::endl;
    return 0;
  }

  client.Start();

  core::SignalSet::GetSingleton()->Insert(SIGTERM, boost::bind(OnTerminal, &loop));

  loop.Loop();

  client.Stop();
  client.Finalize();
  client.Deallocate();

  AsyncLoggingSingleton::GetAsyncLogging()->Stop();

  return 0;
}

