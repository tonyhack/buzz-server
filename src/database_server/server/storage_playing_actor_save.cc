//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-25 13:08:25.
// File name: storage_playing_actor_save.cc
//
// Description:
// Define class StoragePlayingActorSave.
//

#include "database_server/server/storage_playing_actor_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_playing_actor_save_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StoragePlayingActorSave::StoragePlayingActorSave() {}
StoragePlayingActorSave::~StoragePlayingActorSave() {}

bool StoragePlayingActorSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    LOG_ERROR("async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_PLAYING_ACTOR_SAVE,
      boost::bind(&StoragePlayingActorSave::Request, this, _1));

  return true;
}

void StoragePlayingActorSave::Request(const protocol::AsyncRequestMessage &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetMysqlSession(message.channel_);
  if(session.get() == NULL) {
    LOG_ERROR("Get mysql session failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Check connection status.
  if(session->CheckConnectStatus() == false) {
    LOG_ERROR("Mysql session connection error.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    LOG_ERROR("Get mysql query failed");
    this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
    return;
  }

  // Deserialize.
  protocol::StoragePlayingActorSaveRequest request;
  if (this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StoragePlayingActorSaveRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_PLAYING_ACTOR_SAVE)
         << "(" 
         << request.actor_id_ << ","
         << "'" << request.field_.records_ << "',"
         << "'" << request.field_.results_ << "',"
         << request.field_.auto_playing_ << ","
         << request.field_.auto_playing_group_ << ","
         << request.field_.auto_finish_time_ << ","
         << request.field_.maze_ap_ << ","
         << request.field_.maze_last_ap_recover_time_ << ","
         << request.field_.maze_floor_ << ","
         << "'" << request.field_.maze_explored_area_ << "',"
         << "'" << request.field_.maze_recent_position_ << "',"
         << request.field_.maze_gift_number_
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

  protocol::StoragePlayingActorSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StoragePlayingActorSave::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StoragePlayingActorSaveResponse(), result, request);
}

void StoragePlayingActorSave::OnResponse(const protocol::StoragePlayingActorSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

