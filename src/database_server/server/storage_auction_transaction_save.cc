#include "database_server/server/storage_auction_transaction_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageAuctionTransactionSave::StorageAuctionTransactionSave() : async_request_loop_(NULL) {}
StorageAuctionTransactionSave::~StorageAuctionTransactionSave() {}

bool StorageAuctionTransactionSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageAuctionTransactionSave failed, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_AUCTION_TRANSACTION_SAVE,
      boost::bind(&StorageAuctionTransactionSave::Request, this, _1));

  return true;
}

void StorageAuctionTransactionSave::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageAuctionTransactionSaveRequest request;
  if (this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageAuctionTransactionSaveRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_AUCTION_TRANSACTION_SAVE)
         << "("
         << "'" << request.field_.id_ << "', "
         << request.field_.item_template_id_ << ", "
         << request.field_.item_number_ << ", "
         << "'" << request.field_.item_extra_info_ << "', "
         << request.field_.auction_time_id_ << ", "
         << request.field_.start_bid_time_ << ", "
         << request.field_.end_bid_time_ << ", "
         << request.field_.owner_ << ", "
         << request.field_.buyer_ << ", "
         << request.field_.start_bid_price_ << ", "
         << request.field_.last_bid_price_ << ", "
         << request.field_.buyout_price_ << ", "
         << request.field_.finished_ << ", "
         << request.field_.money_withdrawn_ << ", "
         << request.field_.item_withdrawn_
         << ")";

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

  protocol::StorageAuctionTransactionSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageAuctionTransactionSave::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageAuctionTransactionSaveResponse(), result, request);
}

void StorageAuctionTransactionSave::OnResponse(const protocol::StorageAuctionTransactionSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

