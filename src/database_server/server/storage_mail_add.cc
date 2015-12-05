#include "database_server/server/storage_mail_add.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>
#include "database_server/protocol/database_storage_types.h"
#include "database_server/server/async_request_loop.h"
#include "database_server/server/database_server.h"
#include "entity/mail_types.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageMailAdd::StorageMailAdd() {}

StorageMailAdd::~StorageMailAdd() {}

bool StorageMailAdd::Initialize(AsyncRequestLoop *async_request_loop) {
  assert(async_request_loop);
  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_MAIL_ADD,
      boost::bind(&StorageMailAdd::Request, this, _1));
  return true;
}

void StorageMailAdd::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageAddMailRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  bool is_affix = false;
  if((entity::MailType::type)request.type_ != entity::MailType::GENERAL) {
    if(request.affix_.item1_ != 0 || request.affix_.item2_ != 0 || request.affix_.item3_ != 0
        || request.affix_.item4_ != 0 || request.affix_.item5_ != 0 || request.affix_.gold_ != 0
        || request.affix_.restrict_dollars_ != 0) {
      is_affix = true;
    }
  }
  entity::MailAffixType::type affix_type =  entity::MailAffixType::NONE;
  if(is_affix == true) {
    affix_type = entity::MailAffixType::EXIST ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_MAIL_ADD)
    << "(" << request.addressee_id_ << "," <<request.addresser_id_ << ",'" << request.addresser_
    << "', '" << request.title_  << "', " << request.type_ << " ,"<< affix_type 
    << ", '" << request.context_ <<"', " << request.affix_.item1_<< ", " << request.affix_.item1_num_ 
    << " ," << request.affix_.item2_  << " ," << request.affix_.item2_num_ 
    << ", "<<request.affix_.item3_ << " ,"<<request.affix_.item3_num_<< " ,"<< request.affix_.item4_ 
    << ", "<<request.affix_.item4_num_ << " ,"<<request.affix_.item5_ << " ,"<<request.affix_.item5_num_
    << ", "<< request.affix_.gold_ << " ,"<<request.affix_.restrict_dollars_<< ") ";

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

  // Check result's valid.
  if(result->num_rows() != 1 || result->num_fields() != protocol::MailAddField::MAX) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's row/fields error.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Response
  protocol::StorageAddMailResponse response;
  if(result->num_rows() > 0) {
    // Check field number.
    if(result->num_fields() != protocol::MailAddField::MAX) {
      global::LogError("%s:%d (%s) Result's fields error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    response.mail_head_.id_ = atoi((*result)[0][0].data());
    response.addressee_id_ = atol((*result)[0][1].data());
    response.addresser_id_ = request.addresser_id_;
    response.mail_head_.addresser_ = request.addresser_;
    response.mail_head_.type_ = request.type_;
    response.mail_head_.affix_type_ = affix_type;
    response.mail_head_.time_ = atol((*result)[0][2].data());
    response.mail_head_.state_ = entity::MailReadState::UNREAD;
    response.mail_head_.title_ = request.title_; 
  }
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);

}

void StorageMailAdd::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(protocol::StorageAddMailResponse(),
      result, request);
}

void StorageMailAdd::OnResponse(const protocol::StorageAddMailResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

