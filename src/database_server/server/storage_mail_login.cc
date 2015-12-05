
#include "database_server/server/storage_mail_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_mail_login_constants.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageMailLogin::StorageMailLogin() {}

StorageMailLogin::~StorageMailLogin() {}

bool StorageMailLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  assert(async_request_loop);
  this->async_request_loop_ = async_request_loop;
  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_MAIL_LOGIN,
      boost::bind(&StorageMailLogin::Request, this, _1));
  return true;
}

void StorageMailLogin::Request(const protocol::AsyncRequestMessage  &message) {
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
  protocol::StorageMailLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_MAIL_LOGIN)
    << "(" << request.id_ << ")";

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
  protocol::StorageMailLoginResponse response;
  response.id_ = request.id_; 
  if(result->num_rows() > 0) {
    // Check field number.
    if(result->num_fields() != protocol::MailLoadField::MAX) {
      global::LogError("%s:%d (%s) Result's fields error.",
        __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    int index = 0;
    size_t pos = 0;
    for(; pos < result->num_rows(); ++pos) {
      protocol::MailHead field;
      for(size_t i = 0; i < protocol::MailLoadField::MAX; ++i) {
        index = result->field_num(protocol::g_storage_mail_login_constants.kMailLoadField[i]);
        if(index == -1) {
          global::LogError("%s:%d (%s) Get result [%s] failed.", __FILE__, __LINE__,
            __FUNCTION__, protocol::g_storage_mail_login_constants.kMailLoadField[i].c_str());
          this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
          return ;
        }
        switch(i) {
          case protocol::MailLoadField::ID :
            field.__set_id_(atol((*result)[pos][i].data()));
            break;
          case protocol::MailLoadField::TYPE :
            field.__set_type_((entity::MailType::type)atoi((*result)[pos][i].data()));
            break;
          case protocol::MailLoadField::AFFIX_STATE :
            field.__set_affix_type_((entity::MailAffixType::type)atoi((*result)[pos][i].data()));
            break;
          case protocol::MailLoadField::READ_STATE:
            field.__set_state_((entity::MailReadState::type)atoi((*result)[pos][i].data()));
            break;
          case protocol::MailLoadField::TITLE : 
            field.__set_title_((*result)[pos][i].data());
            break;
          case protocol::MailLoadField::CREATE_TIME:
            field.__set_time_(atol((*result)[pos][i].data()));
            break;
          case protocol::MailLoadField::ADDRESSER:
            field.__set_addresser_((*result)[pos][i].data());
            break;
          default:
            break;
        }
      }
      response.mail_list_.push_back(field);
    }
  }
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);

}

void StorageMailLogin::ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(protocol::StorageMailLoginResponse(),
      result, request);
}

void StorageMailLogin::OnResponse(const protocol::StorageMailLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

