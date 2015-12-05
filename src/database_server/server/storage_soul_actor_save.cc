//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-30 10:29:05.
// File name: storage_soul_actor_save.cc
//
// Description:
// Define class StorageSoulActorSave.
//

#include "database_server/server/storage_soul_actor_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_soul_actor_save_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageSoulActorSave::StorageSoulActorSave() : async_request_loop_(NULL) {}
StorageSoulActorSave::~StorageSoulActorSave() {}

bool StorageSoulActorSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    LOG_ERROR("Parameter async_request_loop is null.");
    return false;
  }
  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_SOUL_ACTOR_SAVE,
      boost::bind(&StorageSoulActorSave::Request, this, _1));

  return true;
}

void StorageSoulActorSave::Request(const protocol::AsyncRequestMessage &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetMysqlSession(message.channel_);
  if(session.get() == NULL) {
    LOG_ERROR("Get mysql session failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check connection status.
  if(session->CheckConnectStatus() == false) {
    LOG_ERROR("Mysql session connection error.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    LOG_ERROR("Get mysql query failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
    return ;
  }

  // Deserialize.
  protocol::StorageSoulActorSaveRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "Call " << session->GetProcedure(protocol::StorageType::STORAGE_SOUL_ACTOR_SAVE)
    << "(" << request.actor_id_ << ","
    << request.field_.axe_setting_ << ","
    << request.field_.shoot_setting_ << ","
    << request.field_.magic_setting_ << ", "
    << "'" << request.field_.settings_ << "', "
    << request.field_.altar_summon_paid_ << ", "
    << request.field_.altar_free_summon_times_ << ", "
    << "'" << request.field_.altar_cards_ << "', "
    << request.field_.altar_card_bind_ << ","
    << request.field_.current_energy_ << ")";

  global::LogDebug("Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if(result == NULL) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check any errors.
  if(query->errnum() > 0) {
    global::LogError("%s:%d (%s) Procedure[%s] execute failed, error=[%s].",
        __FILE__, __LINE__, __FUNCTION__, query->str().c_str(), query->error());
    if(session->PingConnected() == true) {
      global::LogError("%s:%d (%s) Ping mysql is ok, but error=[%s]",
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

  // Response.
  protocol::StorageSoulActorSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageSoulActorSave::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageSoulActorSaveResponse(), result, request);
}

void StorageSoulActorSave::OnResponse(const protocol::StorageSoulActorSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database


