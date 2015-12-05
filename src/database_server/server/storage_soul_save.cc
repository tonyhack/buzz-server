//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-18 11:48:15.
// File name: storage_soul_save.cc
//
// Description:
// Define class StorageSoulSave.
//

#include "database_server/server/storage_soul_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_soul_save_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageSoulSave::StorageSoulSave() : async_request_loop_(NULL) {}
StorageSoulSave::~StorageSoulSave() {}

bool StorageSoulSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_SOUL_SAVE,
      boost::bind(&StorageSoulSave::Request, this, _1));

  return true;
}

void StorageSoulSave::Request(const protocol::AsyncRequestMessage &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetMysqlSession(message.channel_);
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get mysql session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check connection status.
  if(session->CheckConnectStatus() == false) {
    global::LogError("%s:%d (%s) Mysql session connection error.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    global::LogError("%s:%d (%s) Get mysql query failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
    return ;
  }

  // Deserialize.
  protocol::StorageSoulSaveRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  if(request.fields_.empty()) {
    global::LogError("%s:%d (%s) fields is empty", __FILE__,
        __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_SOUL_SAVE)
    << "(" << request.id_ << "," << request.fields_.size() << ", '";

  for(size_t pos = 0; pos < request.fields_.size(); ++pos) {
    *query << request.fields_[pos].id_ << ":"
      << request.fields_[pos].level_ << ":"
      << request.fields_[pos].step_ << ":"
      << request.fields_[pos].common_skill_level_ << ":"
      << request.fields_[pos].appear_skill_level_ << ":"
      << request.fields_[pos].soul_skill_level_ << ",";
  }

  *query << "')";

  global::LogDebug("Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if(result == NULL) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check any errors.
  if(query->errnum() > 0) {
    global::LogError("%s:%d (%s) Procedure[%s] execute failed, error=[%s].",
        __FILE__, __LINE__, __FUNCTION__, query->str().c_str(), query->error());
    if(session->PingConnected() == true) {
      global::LogError("%s:%d (%s) Ping mysql is ok, but error=[%s]",
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

  // Response.
  protocol::StorageSoulSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageSoulSave::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageSoulSaveResponse(), result, request);
}

void StorageSoulSave::OnResponse(const protocol::StorageSoulSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

