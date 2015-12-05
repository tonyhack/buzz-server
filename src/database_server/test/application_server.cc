//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-21 17:51:16.
// File name: application_server.cc
//
// Description:
// Define class ApplicationServer.
//

#include "database_server/test/application_server.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"
#include "database_server/client/database_session.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "global/server_type_types.h"

namespace database {

namespace test {

ApplicationServer::ApplicationServer(const core::InetAddress &address,
    const std::string &name) : service_(address, name) {}
ApplicationServer::~ApplicationServer() {}

bool ApplicationServer::Initialize(const std::string &log,
    const global::protocol::ServerSignature &signature) {
  this->signature_ = signature;

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

  // Allocate and initialize schedule client.
  this->schedule_client_.reset(new (std::nothrow) schedule::client::ScheduleClient(
        core::InetAddress(server_info->ip_, server_info->port_), "ScheduleClient"));
  if(this->schedule_client_ == NULL || this->schedule_client_->Initialize(
        this->service_.AllocateLoop(), this->service_.GetLoop(), this->signature_,
        &this->packet_) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate/Initialize schedule client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // Set join/disjoin schedule server callback.
  this->schedule_client_->SetJoinScheduleCallback(
      boost::bind(&ApplicationServer::OnJoinSchedule, this, _1));
  this->schedule_client_->SetDisjoinScheduleCallback(
      boost::bind(&ApplicationServer::OnDisjoinSchedule, this, _1));

  // Initialize database client
  if(client::DatabaseClientSingleton::GetInstance().Initialize(signature) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize database client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // Set response callback.
  client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      protocol::StorageType::STORAGE_TEST_ECHO,
      boost::bind(&ApplicationServer::OnStorageTestEchoResponse,
        this, _1, _2, _3));
  return true;
}

void ApplicationServer::Finalize() {
  client::DatabaseClientSingleton::GetInstance().Finalize();
  this->schedule_client_.reset();
  this->service_.Finalize();
}

void ApplicationServer::Start() {
  this->service_.Start();
  this->schedule_client_->Start();
  client::DatabaseClientSingleton::GetInstance().Start();
  this->service_.AddTimer(TIMER_ID_ECHO, TIMER_MSEC_ECHO,
      -1, this, "ApplicationServer::Start");
}

void ApplicationServer::Stop() {
  client::DatabaseClientSingleton::GetInstance().Stop();
  this->schedule_client_->Stop();
  this->service_.Stop();
}

void ApplicationServer::OnTimer(core::uint32 id) {
  static core::uint32 argu = 0;
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%d", argu);
  protocol::StorageTestEchoRequest request;
  request.argu1_ = argu;
  request.argu2_ = buffer;
  size_t size = 0;
  const char *message = this->packet_.Serialize(request, size);
  if(message && this->Request(protocol::StorageType::STORAGE_TEST_ECHO,
        message, size, argu)) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Send request argu=%d", argu);
    ++argu;
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Serialize/Request failed.", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ApplicationServer::OnJoinSchedule(
    const global::protocol::ServerSignature &signature) {
  if(signature.type_ == global::protocol::ServerType::DATABASE_SERVER) {
    client::DatabaseClientSingleton::GetInstance().OnDatabaseServerJoin(
        this->service_.AllocateLoop(), this->service_.GetLoop(), signature);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_DEBUG,
        "Other application server[type=%d] login schedule server", signature.type_);
  }
}

void ApplicationServer::OnDisjoinSchedule(
    const global::protocol::ServerSignature &signature) {
  if(signature.type_ == global::protocol::ServerType::DATABASE_SERVER) {
    client::DatabaseClientSingleton::GetInstance().OnDatabaseServerDisjoin(signature);
  }
}

bool ApplicationServer::Request(protocol::StorageType::type type,
    const char *message, size_t size, core::uint64 channel) {
  return client::DatabaseClientSingleton::GetInstance().SendRequest(
      type, channel, message, size);
}

void ApplicationServer::OnStorageTestEchoResponse(const std::string &request,
    const std::string &response, protocol::ResponseType::type result) {
  protocol::StorageTestEchoRequest req_message;
  if(this->packet_.Deserialize(req_message, request)) {
    if(result == protocol::ResponseType::SUCCESS) {
      protocol::StorageTestEchoResponse res_message;
      if(this->packet_.Deserialize(res_message, response)) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
            "Echo success, request[%d,%s] response[%d,%s]", req_message.argu1_,
            req_message.argu2_.c_str(), res_message.retvalue1_,
            res_message.retvalue2_.c_str());
      } else {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "Echo failed, request[%d,%s] response[%d]", req_message.argu1_,
          req_message.argu2_.c_str(), result);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Description message failed.", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ApplicationServer::OnStorageTestSumResponse(const std::string &request,
    const std::string &response, protocol::ResponseType::type result) {
}

}  // namespace test

}  // namespace database

