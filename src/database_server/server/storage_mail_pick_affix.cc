
#include "database_server/server/storage_mail_pick_affix.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>
#include "database_server/protocol/database_storage_types.h"
#include "database_server/server/async_request_loop.h"
#include "database_server/server/database_server.h"
#include "entity/mail_types.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageMailAffix::StorageMailAffix() {}

StorageMailAffix::~StorageMailAffix() {}

bool StorageMailAffix::Initialize(AsyncRequestLoop *async_request_loop) {
  assert(async_request_loop);
  this->async_request_loop_ = async_request_loop;
  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_MAIL_PICK_AFFIX,
      boost::bind(&StorageMailAffix::Request, this, _1));
  return true;
}

void StorageMailAffix::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StoragePickAffixRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_MAIL_PICK_AFFIX)
  << "(" << entity::MailAffixType::PICKED << "," << request.mail_id_ << "," <<
  request.id_<< ")";

  global::LogDebug("Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if(result == NULL) {
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check result's valid.
  if(result->num_rows() != 1 || result->num_fields() != protocol::MailAffixField::MAX) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's row/fields error.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  protocol::StoragePickAffixResponse response;
  response.id_ = request.id_;
  response.mail_id_ = request.mail_id_;
  response.affix_.__set_item1_(atoi((*result)[0][0].data()));
  response.affix_.__set_item1_num_(atoi((*result)[0][1].data()));
  response.affix_.__set_item2_(atoi((*result)[0][2].data()));
  response.affix_.__set_item2_num_(atoi((*result)[0][3].data()));
  response.affix_.__set_item3_(atoi((*result)[0][4].data()));
  response.affix_.__set_item3_num_(atoi((*result)[0][5].data()));
  response.affix_.__set_item4_(atoi((*result)[0][6].data()));
  response.affix_.__set_item4_num_(atoi((*result)[0][7].data()));
  response.affix_.__set_item5_(atoi((*result)[0][8].data()));
  response.affix_.__set_item5_num_(atoi((*result)[0][9].data()));
  response.affix_.__set_gold_(atoi((*result)[0][10].data()));
  response.affix_.__set_restrict_dollars_(atoi((*result)[0][11].data()));
  
  this->async_request_loop_->SendResponse(response,protocol::ResponseType::SUCCESS, message);
}

void StorageMailAffix::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(protocol::StoragePickAffixResponse(),
      result, request);
}

void StorageMailAffix::OnResponse(const protocol::StoragePickAffixResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

