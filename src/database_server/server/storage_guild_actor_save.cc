#include "database_server/server/storage_guild_actor_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_guild_actor_save_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageGuildActorSave::StorageGuildActorSave() : async_request_loop_(NULL) {}
StorageGuildActorSave::~StorageGuildActorSave() {}

bool StorageGuildActorSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageGuildActorSave falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_GUILD_ACTOR_SAVE,
      boost::bind(&StorageGuildActorSave::Request, this, _1));

  return true;
}

void StorageGuildActorSave::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageGuildActorSaveRequest request;
  if (this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageGuildActorSaveRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_GUILD_ACTOR_SAVE)
         << "(" 
         << request.actor_id_ << ", "
         << request.field_.guild_id_ << ", "
         << request.field_.guild_position_ << ", "
         << request.field_.current_contribution_value_ << ", "
         << request.field_.total_contribution_value_ << ", "
         << request.field_.gold_contribution_ << ", "
         << request.field_.freedom_dollars_contribution_ << ", "
         << request.field_.light_crystal_contribution_ << ", "
         << request.field_.dark_crystal_contribution_ << ", "
         << request.field_.holy_crystal_contribution_ << ", "
         << "'" << request.field_.guild_skills_ << "', "
         << "'" << request.field_.guild_buffs_ << "', "
         << "'" << request.field_.awarded_guild_playings_ << "'"
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

  protocol::StorageGuildActorSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageGuildActorSave::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageGuildActorSaveResponse(), result, request);
}

void StorageGuildActorSave::OnResponse(const protocol::StorageGuildActorSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

