#include "database_server/server/storage_guild_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageGuildSave::StorageGuildSave() : async_request_loop_(NULL) {}
StorageGuildSave::~StorageGuildSave() {}

bool StorageGuildSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageGuildSave failed, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_GUILD_SAVE,
      boost::bind(&StorageGuildSave::Request, this, _1));

  return true;
}

void StorageGuildSave::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageGuildSaveRequest request;
  if (this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageGuildSaveRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Escape string.
  query->escape_string(&request.field_.name_);
  query->escape_string(&request.field_.announcement_);

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_GUILD_SAVE)
         << "("
         << request.field_.id_ << ", "
         << "'" << request.field_.name_ << "', "
         << request.field_.logo_ << ", "
         << request.field_.level_ << ", "
         << request.field_.suppress_join_ << ", "
         << "'" << request.field_.pending_members_ << "', "
         << "'" << request.field_.members_ << "', "
         << "'" << request.field_.announcement_ << "', "
         << "'" << request.field_.events_ << "', "
         << request.field_.gold_ << ", "
         << request.field_.light_crystal_ << ", "
         << request.field_.dark_crystal_ << ", "
         << request.field_.holy_crystal_ << ", "
         << request.field_.light_attribute_ << ", "
         << request.field_.dark_attribute_ << ", "
         << request.field_.holy_attribute_ << ", "
         << request.field_.shop_level_ << ", "
         << request.field_.college_level_ << ", "
         << request.field_.barrack_level_ << ", "
         << "'" << request.field_.opened_playing_groups_ << "', "
         << "'" << request.field_.finished_playings_ << "', "
         << request.field_.last_save_time_
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

  protocol::StorageGuildSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageGuildSave::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageGuildSaveResponse(), result, request);
}

void StorageGuildSave::OnResponse(const protocol::StorageGuildSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

