//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 17:42:02.
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
#include "coresh/connection_pool.h"
#include "coresh/packetizer.h"
#include "coresh/packetizer_table.h"
#include "coresh/reusable_buffers.h"
#include "coresh/variable_table.h"
#include "test/chat/client/chat_client.h"
#include "test/chat/client/configure.h"

using namespace test::chat::client;

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
    return 0;
  }
  std::string configure_file = iterator->second;

  iterator = args.find("-name");
  if(iterator == args.end()) {
    std::cout << "Need argument -name [name]." << std::endl;
    return 0;
  }

  std::string user_name = iterator->second;

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

  char server_log_name[64];
  snprintf(server_log_name, sizeof(server_log_name), "%s", ::basename(argv[0]));


  std::string core_log_file = configure.log_dir_ + "core-" + server_log_name + ".log";
  core::InitializeLogger(core_log_file, core_log_file, core_log_file);


  ChatClient client(core::InetAddress(configure.host_, configure.port_), user_name);








  core::AsyncLogging *logging = AsyncLoggingSingleton::GetAsyncLogging();
  if(logging && logging->Initialize(configure.log_dir_ + server_log_name + "-" + user_name,
        core::kMaxLoggingSize)) {
    logging->Start();
  } else {
    std::cout << "Initialize asynchronous loggin failed." << std::endl;
    return 0;
  }

  // Initialize connection pool.
  if(coresh::ConnectionPoolSingleton::GetInstance().Initialize(5, 1) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize connection pool failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return 0;
  }

  // Allocate reusable buffers.
  if(coresh::ReusableBuffersSingleton::GetInstance().Allocate(client.GetEventLoop()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate reusable buffers failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return 0;
  }

  // Allocate packetizer.
  coresh::Packetizer *packetizer = new (std::nothrow) coresh::Packetizer();
  if(packetizer == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate packetizer failed.",
        __FILE__, __LINE__, __FUNCTION__);
  } else {
    packetizer->SetCompressor(coresh::VariableTableSingleton::GetInstance().GetCompressType());
    packetizer->SetEncoder(coresh::VariableTableSingleton::GetInstance().GetEncodeType());
    if(coresh::PacketizerTableSingleton::GetInstance().Insert(client.GetEventLoop(), packetizer) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Insert packetizer table failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
  }







  if(client.Initialize() == false) {
    std::cout << "Initialize chat client failed." << std::endl;
    return 0;
  }

  client.Start();

  std::string command;

  while(true) {
    std::cin >> command;
    if(command == "quit") {
      break;
    } else {
      client.Input(command);
    }
  }

  client.Stop();

  delete packetizer;


  return 0;
}

