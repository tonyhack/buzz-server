#include "session_server/server/admin/admin_service.h"

#include <boost/bind.hpp>

#include "coresh/application_service.h"
#include "coresh/network_service.h"
#include "coresh/variable_table.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "session_server/server/session_server.h"
#include "session_server/server/admin/admin_session_manager.h"
#include "session_server/server/admin/admin_session_pool.h"

namespace session {

namespace server {

namespace admin {

AdminService::AdminService() :
  application_loop_(NULL), network_service_(NULL) {}

AdminService::~AdminService() {}

bool AdminService::Initialize(coresh::ApplicationService *application_service,
    const core::InetAddress &address, const std::string &name) {
  if (this->network_service_ != NULL) {
    LOG_ERROR("Can not initialize twice.");
    return false;
  }

  this->network_service_ = new (std::nothrow) coresh::NetworkService(address, name);
  if (NULL == this->network_service_) {
    LOG_ERROR("Allocate network service failed.");
    return false;
  }

  if (this->network_service_->Initialize(
        coresh::VariableTableSingleton::GetInstance().GetNetworkThreadNumber(),
        application_service->GetLoop(),
        boost::bind(&AdminService::OnConnection, this, _1)) == false) {
    LOG_ERROR("Init network service failed.");
    return false;
  }
  this->application_loop_ = application_service->GetLoop();

  if (AdminSessionPool::GetInstance() == NULL) {
    LOG_ERROR("Init AdminSessionPool singleton failed.");
    return false;
  }
  if (AdminSessionManager::GetInstance() == NULL) {
    LOG_ERROR("Init AdminSessionManager singleton failed.");
    return false;
  }

  if (AdminSessionPool::GetInstance()->Initialize(100, 10) == false) {
    LOG_ERROR("Init AdminSessionPool failed.");
    return false;
  }
  if (AdminSessionManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Init AdminSessionManager failed.");
    return false;
  }
  if (this->admin_message_handler_.Initialize() == false) {
    LOG_ERROR("Init AdminMessageHandler failed.");
    return false;
  }

  return true;
}

void AdminService::Finalize() {
  this->admin_message_handler_.Finalize();
  AdminSessionManager::GetInstance()->Finalize();
  AdminSessionPool::GetInstance()->Finalize();
  AdminSessionManager::ReleaseInstance();
  AdminSessionPool::ReleaseInstance();

  if (this->network_service_ != NULL) {
    this->network_service_->Finalize();
    delete this->network_service_;
  }
}

void AdminService::Start() {
  if (this->network_service_ != NULL) {
    this->network_service_->Start();
  }
}

void AdminService::Stop() {
  if (this->network_service_ != NULL) {
    this->network_service_->Stop();
  }
}

void AdminService::OnConnection(const coresh::ConnectionPtr &connection) {
  AdminSession *session =
    AdminSessionManager::GetInstance()->CreateSession(connection);
  if (NULL == session) {
    LOG_ERROR("Create AdminSession failed.");
    connection->ApplicationDisconnect();
    return;
  }
}

void AdminService::OnMessage(AdminSession *session, const char *message, size_t size) {
  if (NULL == session) {
    LOG_ERROR("session is null.");
    return;
  }
  global::ThriftPacket *thrift = SessionServerSingleton::GetInstance().GetThrift();
  if (NULL == thrift) {
    session->Close();
    LOG_ERROR("thrift is null.");
    return;
  }

  protocol::MessageAdmin message2;
  if (thrift->Deserialize(message2, message, size) == false) {
    session->Close();
    LOG_ERROR("Deserialize message failed.");
    return;
  }

  if (message2.type_ < protocol::AdminMessageType::MESSAGE_MIN ||
      message2.type_ >= protocol::AdminMessageType::MESSAGE_MAX ||
      this->message_handlers_[message2.type_].empty()) {
    session->Close();
    LOG_ERROR("Error admin message type[%d]", message2.type_);
    return;
  }

  this->message_handlers_[message2.type_](session,
      message2.message_.c_str(), message2.message_.size());
}

}  // namespace admin

}  // namespace server

}  // namespace session

