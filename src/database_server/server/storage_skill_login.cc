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

#include "database_server/server/storage_skill_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_skill_login_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageSkillLogin::StorageSkillLogin() : async_request_loop_(NULL) {}
StorageSkillLogin::~StorageSkillLogin() {}

bool StorageSkillLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->async_request_loop_ = async_request_loop;
  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_SKILL_LOGIN,
      boost::bind(&StorageSkillLogin::Request, this, _1));
  return true;
}

void StorageSkillLogin::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageSkillLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_SKILL_LOGIN)
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
  protocol::StorageSkillLoginResponse response;

  if(result->num_rows() > 0) {
    // Check field number.
    if(result->num_fields() != protocol::SkillField::MAX) {
      global::LogError("%s:%d (%s) Result's fields error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    int index = 0;
    // Set cooling fields.
    size_t pos = 0;
    for(; pos < result->num_rows(); ++pos) {
      protocol::StorageSkillField field;
      for(size_t i = 0; i < protocol::SkillField::MAX; ++i) {
        index = result->field_num(protocol::g_storage_skill_login_constants.kSkillFieldStr[i]);
        if(index == -1) {
          global::LogError("%s:%d (%s) Get result [%s] failed.", __FILE__, __LINE__, __FUNCTION__,
              protocol::g_storage_skill_login_constants.kSkillFieldStr[i].c_str());
          this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
          return ;
        }
        switch(i) {
          case protocol::SkillField::SKILL_ID:
            field.__set_skill_id_(atol((*result)[pos][i].data()));
            break;
          case protocol::SkillField::CURRENT_LEVEL:
            field.__set_skill_current_level_(atol((*result)[pos][i].data()));
            break;
          default:
            break;
        }
      }
      response.fields_.push_back(field);
    }
    if(response.fields_.empty() == false) {
      response.__isset.fields_ = true;
    }
  }
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageSkillLogin::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageSkillLoginResponse(), result, request);
}

void StorageSkillLogin::OnResponse(const protocol::StorageSkillLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

