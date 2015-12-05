//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 20:24:55.
// File name: storage_cooling_login.cc
//
// Description:
//

#include "database_server/server/storage_cooling_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_cooling_login_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageCoolingLogin::StorageCoolingLogin() : async_request_loop_(NULL) {}
StorageCoolingLogin::~StorageCoolingLogin() {}

bool StorageCoolingLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->async_request_loop_ = async_request_loop;
  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_COOLING_LOGIN,
      boost::bind(&StorageCoolingLogin::Request, this, _1));
  return true;
}

void StorageCoolingLogin::Request(const protocol::AsyncRequestMessage &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetMysqlSession(message.channel_);
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get mysql session failed.", __FILE__, __LINE__,
        __FUNCTION__);
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
    global::LogError("%s:%d (%s) Get mysql query failed.", __FILE__, __LINE__,
        __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
    return ;
  }

  // Deserialize.
  protocol::StorageCoolingLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_COOLING_LOGIN)
    << "(" << request.id_ << ")";

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
  protocol::StorageCoolingLoginResponse response;

  if(result->num_rows() > 0) {
    // Check field number.
    if(result->num_fields() != protocol::CoolingField::MAX) {
      global::LogError("%s:%d (%s) Result's fields error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    int index = 0;
    // Set cooling fields.
    size_t pos = 0;
    // TODO: big message should be divided into several small messages.
    for(; pos < result->num_rows(); ++pos) {
      protocol::StorageCoolingField field;
      for(size_t i = 0; i < protocol::CoolingField::MAX; ++i) {
        index = result->field_num(protocol::g_storage_cooling_login_constants.kCoolingFieldStr[i]);
        if(index == -1) {
          global::LogError("%s:%d (%s) Get result [%s] failed.", __FILE__, __LINE__, __FUNCTION__,
              protocol::g_storage_cooling_login_constants.kCoolingFieldStr[i].c_str());
          this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
          return ;
        }
        switch(i) {
          case protocol::CoolingField::COOLING_ID:
            field.__set_cooling_id_(atol((*result)[pos][i].data()));
            break;
          case protocol::CoolingField::COOLING_TIME:
            field.__set_cooling_time_(atol((*result)[pos][i].data()));
            break;
          default:
            break;
        }
      }
      response.fields_.push_back(field);
      response.__isset.fields_ = true;
    }
  }
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageCoolingLogin::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageCoolingLoginResponse(), result, request);
}

void StorageCoolingLogin::OnResponse(const protocol::StorageCoolingLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

