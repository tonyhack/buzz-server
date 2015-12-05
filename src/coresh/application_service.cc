//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-28 16:23:09.
// File name: application_service.cc
//
// Description:
// Define class ApplicationService.
//

#include "coresh/application_service.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"
#include "coresh/connection_pool.h"
#include "coresh/packetizer.h"
#include "coresh/packetizer_table.h"
#include "coresh/reusable_buffers.h"
#include "coresh/variable_table.h"

namespace coresh {

ApplicationService::ApplicationService(const core::InetAddress &address, const std::string &name)
  : network_service_(address, name), thread_(&loop_, "ApplicationServiceThread") {}

ApplicationService::~ApplicationService() {}

bool ApplicationService::Initialize() {
  if(this->loop_.Initialize(true) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize application event loop failed",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Initialize connection pool.
  if(ConnectionPoolSingleton::GetInstance().Initialize(
        VariableTableSingleton::GetInstance().GetConnectionPoolInitialNumber(),
        VariableTableSingleton::GetInstance().GetConnectionPoolExtendNumber()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize connection pool failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Allocate reusable buffers.
  if(ReusableBuffersSingleton::GetInstance().Allocate(&this->loop_) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate reusable buffers failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Allocate packetizer.
  Packetizer *packetizer = new (std::nothrow) Packetizer();
  if(packetizer == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate packetizer failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  packetizer->SetCompressor(VariableTableSingleton::GetInstance().GetCompressType());
  packetizer->SetEncoder(VariableTableSingleton::GetInstance().GetEncodeType());
  if(PacketizerTableSingleton::GetInstance().Insert(&this->loop_, packetizer) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Insert packetizer table failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->network_service_.Initialize(VariableTableSingleton::GetInstance().GetNetworkThreadNumber(),
        &this->loop_, boost::bind(&ApplicationService::OnConnection, this, _1)) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize network service failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void ApplicationService::Finalize() {
  this->network_service_.Finalize();

  // Deallocate packetizer.
  Packetizer *packetizer = PacketizerTableSingleton::GetInstance().Remove(&this->loop_);
  if(packetizer == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deallocate packetizer failed.",
        __FILE__, __LINE__, __FUNCTION__);
  } else {
    delete packetizer;
  }
  // Deallocate reusable buffers.
  ReusableBuffersSingleton::GetInstance().Deallocate(&this->loop_);

  // Finalize connection pool.
  ConnectionPoolSingleton::GetInstance().Finalize();

  this->loop_.Finalize();
}

void ApplicationService::Start() {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Start application service.");
  this->network_service_.Start();
  this->thread_.StartLoop();
}

void ApplicationService::Stop() {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Stop application service.");
  this->thread_.StopLoop();
  this->network_service_.Stop();
}

void ApplicationService::OnConnection(const ConnectionPtr &connection) {
  this->connection_callback_(connection);
}

}  // namespace coresh

