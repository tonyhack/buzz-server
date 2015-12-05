
#include "database_server/server/storage_mail_del.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_mail_del_types.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageMailDel::StorageMailDel() {}

StorageMailDel::~StorageMailDel() {}

bool StorageMailDel::Initialize(AsyncRequestLoop *async_request_loop) {
  assert(async_request_loop);
  this->async_request_loop_ = async_request_loop;
  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_MAIL_DEL,
      boost::bind(&StorageMailDel::Request, this, _1));
  return true;
}

void StorageMailDel::Request(const protocol::AsyncRequestMessage  &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetMysqlSession(message.channel_);
  if(session.get() == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check connection status.
  if(session->CheckConnectStatus() == false) {
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get mysql query failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
    return ;
  }

  // Deserialize.
  protocol::StorageDelMailRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_MAIL_DEL)
    << "(" << request.id_list_.size() << ", '";

  for(size_t i = 0; i < request.id_list_.size(); ++i) {
    *query << request.id_list_[i] << ",";
  }
  *query << "', " << request.id_ << ")";
  global::LogDebug("Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if(result == NULL) {
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check any errors.
  if(query->errnum() > 0) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Procedure[%s] execute failed, error=[%s].", __FILE__, __LINE__,
        __FUNCTION__, query->str().c_str(), query->error());
    if(session->PingConnected() == true) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Ping mysql is ok, but error=[%s].",
          __FILE__, __LINE__, __FUNCTION__, query->error());
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    } else {
      // Disconnect and reconnect.
      session->Disconnect();
      DatabaseServerSingleton::GetInstance().GetAsyncRequestService().Reconnect(session);
      this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
      return ;
    }
  }

  // Response
  protocol::StorageDelMailResponse response;
  response.id_ = request.id_; 
  response.__set_id_list_(request.id_list_); 
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageMailDel::ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageDelMailResponse(),
      result, request);
}

void StorageMailDel::OnResponse(const protocol::StorageDelMailResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

