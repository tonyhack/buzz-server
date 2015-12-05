#include "database_server/server/storage_item_actor_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_item_actor_save_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageItemActorSave::StorageItemActorSave() : async_request_loop_(NULL) {}
StorageItemActorSave::~StorageItemActorSave() {}

bool StorageItemActorSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageItemActorSave falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ITEM_ACTOR_SAVE,
      boost::bind(&StorageItemActorSave::Request, this, _1));

  return true;
}

void StorageItemActorSave::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageItemActorSaveRequest request;
  if (this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageItemActorSaveRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ITEM_ACTOR_SAVE)
         << "(" 
         << request.actor_id_ << ", "
         << request.field_.gem_gathering_point_ << ", "
         << request.field_.shortcut1 << ", "
         << request.field_.shortcut2 << ", "
         << request.field_.shortcut3 << ", "
         << request.field_.shortcut4 << ", "
         << request.field_.gifts_pick_num_ << ", "
         << " '" << request.field_.item_buy_num_.c_str() << "' "
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

  protocol::StorageItemActorSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageItemActorSave::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageItemActorSaveResponse(), result, request);
}

void StorageItemActorSave::OnResponse(const protocol::StorageItemActorSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

