#include "database_server/server/storage_spirit_facade_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_spirit_facade_login_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageSpiritFacadeLogin::StorageSpiritFacadeLogin() : async_request_loop_(NULL) {}
StorageSpiritFacadeLogin::~StorageSpiritFacadeLogin() {}

bool StorageSpiritFacadeLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageSpiritFacadeLogin falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_SPIRIT_FACADE_LOGIN,
      boost::bind(&StorageSpiritFacadeLogin::Request, this, _1));

  return true;
}

void StorageSpiritFacadeLogin::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageSpiritFacadeLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageSpiritFacadeLoginRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_SPIRIT_FACADE_LOGIN)
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
  protocol::StorageSpiritFacadeLoginResponse response;
  response.__set_result_(protocol::SpiritFacadeLoginResult::SUCCESS);

  for (size_t row = 0; row < result->num_rows(); ++row) {
    protocol::StorageSpiritFacadeField field;

    for (size_t pos = 0; pos < protocol::SpiritFacadeField::MAX; ++pos) {
      int index = result->field_num(protocol::g_storage_spirit_facade_login_constants.kSpiritFacadeFieldStr[pos]); 
      if (-1 == index) {
        LOG_ERROR("Get result [%s] failed.",
                  protocol::g_storage_spirit_facade_login_constants.kSpiritFacadeFieldStr[pos].c_str());
        this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
        return;
      }

      switch (pos) {
        case protocol::SpiritFacadeField::SPIRIT_FACADE_ID:
          field.__set_spirit_facade_id_(atoi((*result)[row][index].data()));
          break;
        case protocol::SpiritFacadeField::EXPIRE_TIME:
          field.__set_expire_time_(atoi((*result)[row][index].data()));
          break;
        default:
          break;
      }
    }
    response.fields_.push_back(field);
  }

  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageSpiritFacadeLogin::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageSpiritFacadeLoginResponse(), result, request);
}

void StorageSpiritFacadeLogin::OnResponse(const protocol::StorageSpiritFacadeLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

