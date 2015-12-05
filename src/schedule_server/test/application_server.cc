//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-11 13:58:43.
// File name: application_server.cc
//
// Description:
// Define class ApplicationServer.
//

#include "schedule_server/test/application_server.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"
#include "global/server_configure.h"
#include "global/server_type_types.h"

namespace schedule {

namespace test {

ApplicationServer::ApplicationServer(const core::InetAddress &address,
    const std::string &name) : service_(address, name) {}

ApplicationServer::~ApplicationServer() {}

bool ApplicationServer::Initialize(const std::string &log,
    const global::protocol::ServerSignature &signature) {
  this->service_.SetConnectionCallback(
      boost::bind(&ApplicationServer::OnConnection, this, _1));
  if(this->service_.Initialize(log) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize appcliation service failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  const global::ServerInformation *server_info =
    global::ServerConfigureSingleton::GetInstance().GetScheduleServer();
  if(server_info == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get schedule server information failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->schedule_.reset(new (std::nothrow) client::ScheduleClient(
        core::InetAddress(server_info->ip_, server_info->port_), "ScheduleClient"));
  if(this->schedule_ == NULL || this->schedule_->Initialize(this->service_.AllocateLoop(),
        this->service_.GetLoop(), signature, &this->packet_) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate/Initialize schedule client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->schedule_->SetJoinScheduleCallback(
      boost::bind(&ApplicationServer::OnJoinSchedule, this, _1));
  this->schedule_->SetDisjoinScheduleCallback(
      boost::bind(&ApplicationServer::OnDisjoinSchedule, this, _1));

  return true;
}

void ApplicationServer::Finalize() {
  this->service_.Finalize();
  this->schedule_.reset();
}

void ApplicationServer::Start() {
  this->service_.Start();
  this->schedule_->Start();
}

void ApplicationServer::Stop() {
  this->schedule_->Stop();
  this->service_.Stop();
}

void ApplicationServer::OnJoinSchedule(const global::protocol::ServerSignature &signature) {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Application server [type=%d id=%d host=%s:%d] join schedule.",
      signature.type_, signature.id_, signature.host_.c_str(), signature.port_);
}

void ApplicationServer::OnDisjoinSchedule(const global::protocol::ServerSignature &signature) {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Application server [type=%d id=%d host=%s:%d] disjoin schedule.",
      signature.type_, signature.id_, signature.host_.c_str(), signature.port_);
}

}  // namespace test

}  // namespace schedule

