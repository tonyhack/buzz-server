//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-29 01:59:23.
// File name: storage_actor_save.cc
//
// Description:
// Define class StorageActorSave.
//

#include "database_server/server/storage_actor_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageActorSave::StorageActorSave() : async_request_loop_(NULL) {}
StorageActorSave::~StorageActorSave() {}

bool StorageActorSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Argument async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->async_request_loop_ = async_request_loop;
  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ACTOR_SAVE,
      boost::bind(&StorageActorSave::Request, this, _1));
  return true;
}

void StorageActorSave::Request(const protocol::AsyncRequestMessage &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetMysqlSession(message.channel_);
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get mysql session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check connection status.
  if(session->CheckConnectStatus() == false) {
    global::LogError("%s:%d (%s) Mysql session connection error.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    global::LogError("%s:%d (%s) Get mysql query failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
    return ;
  }

  //Deserialize.
  protocol::StorageActorSaveRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ACTOR_SAVE)
    << "(" << request.id_ << ", " << request.fields_.level_ << ", "
    << request.fields_.vocation_ << ", " << (int)request.fields_.gender_ << ", "
    << (int)request.fields_.head_ << ", " << (int)request.fields_.hair_ << ", "
    << request.fields_.current_hp_ << ", " << request.fields_.current_mp_ << ", "
    << request.fields_.current_exp_ << ", " << request.fields_.nimbus_ << ", "
    << request.fields_.gold_ << ", " << request.fields_.freedom_dollars_ << ", "
    << request.fields_.restrict_dollars_ << ", " << request.fields_.honour_ << ", "
    << request.fields_.war_soul_ << ", "  << request.fields_.gem_shard_ << ", "
    << request.fields_.justice_ << ", " << request.fields_.miracle_integral_ << ", "
    << request.fields_.legend_integral_ << ", "
    << request.fields_.arena_integral_ << ", "
    << request.fields_.map_ << ", " << request.fields_.x_ << ", " << request.fields_.y_ << ", "
    << request.fields_.pre_map_ << ", " << request.fields_.pre_x_ << ", " << request.fields_.pre_y_ << ", "
    << request.fields_.brown_name_ << ", " << request.fields_.pk_mode_ << ", "
    << "'" << request.fields_.functionality_state_ << "', "
    << request.fields_.packet_capacity_ << ", " << request.fields_.storage_capacity_ << ", "
    << request.fields_.axe_fight_score_ << ", "
    << request.fields_.shoot_fight_score_ << ", "
    << request.fields_.magic_fight_score_ << ", "
    << request.fields_.last_logout_time_ << ", "
    << (int)request.fields_.vip_type_ << ", "
    << request.fields_.vip_remainder_time_ << ", "
    << request.fields_.world_jump_vip_num_ << ", "
    << request.fields_.fcm_type_ << ", "
    << request.fields_.fcm_online_time_ << ", "
    << request.fields_.create_time_ << ", "
    << request.fields_.hp_tasliman_ << ", "
    << request.fields_.mp_tasliman_ << ")";

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
      global::LogError("%s:%d (%s) Ping mysql is ok, but error=[%s].",
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

  // Check result's valid.
  if(result->num_rows() != 1 || result->num_fields() != 1) {
    global::LogError("%s:%d (%s) Result's row/fields error.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check ret code.
  int ret = result->field_num("@ret_code");
  if(ret == -1) {
    global::LogError("%s:%d (%s) Get ret code failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Response.
  protocol::StorageActorSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageActorSave::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageActorSaveResponse(), result, request);
}

void StorageActorSave::OnResponse(const protocol::StorageActorSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

