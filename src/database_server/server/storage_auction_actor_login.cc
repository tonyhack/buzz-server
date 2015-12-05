#include "database_server/server/storage_auction_actor_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_auction_actor_login_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageAuctionActorLogin::StorageAuctionActorLogin() {}
StorageAuctionActorLogin::~StorageAuctionActorLogin() {}

bool StorageAuctionActorLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageAuctionActorLogin falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_AUCTION_ACTOR_LOGIN,
      boost::bind(&StorageAuctionActorLogin::Request, this, _1));

  return true;
}

void StorageAuctionActorLogin::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageAuctionActorLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageAuctionActorLoginRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_AUCTION_ACTOR_LOGIN)
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
  protocol::StorageAuctionActorLoginResponse response;
  response.__set_result_(protocol::AuctionActorLoginResult::SUCCESS);

  if (result->num_rows() == 0) {
    // Default all zeros.
  } else if (result->num_rows() >= 1) {

    // Set values.
    for (size_t pos = 0; pos < protocol::AuctionActorField::MAX; ++pos) {
      int index = result->field_num(protocol::g_storage_auction_actor_login_constants.kAuctionActorFieldStr[pos]); 
      if (-1 == index) {
        LOG_ERROR("Get result [%s] failed.",
                  protocol::g_storage_auction_actor_login_constants.kAuctionActorFieldStr[pos].c_str());
        this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
        return;
      }

      switch (pos) {
        case protocol::AuctionActorField::SELL_ITEMS:
          response.field_.__set_sell_items_((*result)[0][index].data());
          break;
        case protocol::AuctionActorField::BUY_ITEMS:
          response.field_.__set_buy_items_((*result)[0][index].data());
          break;
        case protocol::AuctionActorField::HISTORY:
          response.field_.__set_history_((*result)[0][index].data());
          break;
        default:
          break;
      }
    }
  }

  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageAuctionActorLogin::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
    this->OnResponse(protocol::StorageAuctionActorLoginResponse(), result, request);
}

void StorageAuctionActorLogin::OnResponse(const protocol::StorageAuctionActorLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
    this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

