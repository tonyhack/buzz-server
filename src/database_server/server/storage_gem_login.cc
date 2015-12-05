#include "database_server/server/storage_gem_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_gem_login_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageGemLogin::StorageGemLogin() : async_request_loop_(NULL) {}
StorageGemLogin::~StorageGemLogin() {}

bool StorageGemLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageGemLogin falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_GEM_LOGIN,
      boost::bind(&StorageGemLogin::Request, this, _1));

  return true;
}

void StorageGemLogin::Request(const protocol::AsyncRequestMessage &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetMysqlSession(message.channel_);
  if (session.get() == NULL) {
    LOG_ERROR("Get mysql session failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Check connection status.
  if (session->CheckConnectStatus() == false) {
    LOG_ERROR("Mysql session connection error.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if (NULL == query) {
    LOG_ERROR("Get mysql query failed");
    this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
    return;
  }

  // Deserialize.
  protocol::StorageGemLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageGemLoginRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_GEM_LOGIN)
    << "(" << request.actor_id_ << ")";

  LOG_DEBUG("Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if (NULL == result) {
    LOG_ERROR("Store query result failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Check any errors.
  if (query->errnum() > 0) {
    LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
              query->str().c_str(), query->error());
    if (session->PingConnected() == true) {
      LOG_ERROR("Ping mysql is ok, but error=[%s].", query->error());
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return;
    } else {
      // Disconnect and reconnect.
      session->Disconnect();
      DatabaseServerSingleton::GetInstance().GetAsyncRequestService().Reconnect(session);
      this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
      return;
    }
  }

  // Response.
  protocol::StorageGemLoginResponse response;
  response.__set_result_(protocol::GemLoginResult::SUCCESS);

  for (size_t row = 0; row < result->num_rows(); ++row) {
    protocol::StorageGemField field;

    for (size_t pos = 0; pos < protocol::GemField::MAX; ++pos) {
      int index = result->field_num(protocol::g_storage_gem_login_constants.kGemFieldStr[pos]); 
      if (-1 == index) {
        LOG_ERROR("Get result [%s] failed.",
                  protocol::g_storage_gem_login_constants.kGemFieldStr[pos].c_str());
        this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
        return;
      }

      switch (pos) {
        case protocol::GemField::LOCATION_TYPE:
          field.__set_location_type_(atoi((*result)[row][index].data()));
          break;
        case protocol::GemField::LOCATION_INDEX:
          field.__set_location_index_(atoi((*result)[row][index].data()));
          break;
        case protocol::GemField::GEM_ID:
          field.__set_gem_id_(atoi((*result)[row][index].data()));
          break;
        case protocol::GemField::GEM_LEVEL:
          field.__set_gem_level_(atoi((*result)[row][index].data()));
          break;
        case protocol::GemField::GEM_EXP:
          field.__set_gem_exp_(atoi((*result)[row][index].data()));
          break;
        case protocol::GemField::LOCKED:
          field.__set_locked_(atoi((*result)[row][index].data()));
          break;
        default:
          break;
      }
    }
    response.fields_.push_back(field);
  }

  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageGemLogin::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageGemLoginResponse(), result, request);
}

void StorageGemLogin::OnResponse(const protocol::StorageGemLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

