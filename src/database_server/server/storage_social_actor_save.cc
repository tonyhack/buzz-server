#include "database_server/server/storage_social_actor_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_social_actor_save_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageSocialActorSave::StorageSocialActorSave() : async_request_loop_(NULL) {}
StorageSocialActorSave::~StorageSocialActorSave() {}

bool StorageSocialActorSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageSocialActorSave falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_SOCIAL_ACTOR_SAVE,
      boost::bind(&StorageSocialActorSave::Request, this, _1));

  return true;
}

void StorageSocialActorSave::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageSocialActorSaveRequest request;
  if (this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageSocialActorSaveRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Escape string.
  query->escape_string(&request.field_.signature_);

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_SOCIAL_ACTOR_SAVE)
         << "(" 
         << request.actor_id_ << ", "
         << "'" << request.field_.signature_ << "', "
         << "'" << request.field_.friends_ << "', "
         << "'" << request.field_.blacklist_ << "', "
         << "'" << request.field_.enemies_ << "', "
         << request.field_.homeland_upgrade_start_time_ << ", "
         << request.field_.homeland_upgrade_finish_time_ << ", "
         << request.field_.homeland_temple_level_ << ", "
         << request.field_.homeland_temple_harvest_times_ << ", "
         << request.field_.homeland_temple_force_harvest_times_ << ", "
         << request.field_.homeland_temple_next_harvest_time_ << ", "
         << request.field_.homeland_goldmine_level_ << ", "
         << request.field_.homeland_goldmine_robbed_times_ << ", "
         << request.field_.homeland_goldmine_loss_rate_ << ", "
         << request.field_.homeland_goldmine_next_harvest_time_ << ", "
         << request.field_.homeland_goldmine_rob_times_ << ", "
         << request.field_.homeland_goldmine_next_rob_time_ << ", "
         << request.field_.homeland_pool_level_ << ", "
         << request.field_.homeland_pool_last_harvest_time_ << ", "
         << request.field_.homeland_church_free_pray_times_ << ", "
         << request.field_.homeland_church_paid_pray_times_ << ", "
         << request.field_.homeland_church_free_refresh_times_ << ", "
         << request.field_.homeland_church_chest_type_ << ", "
         << request.field_.homeland_tree_level_ << ", "
         << request.field_.homeland_tree_harvest_times_ << ", "
         << request.field_.homeland_tree_stage_ << ", "
         << request.field_.homeland_tree_watered_times_ << ", "
         << request.field_.homeland_tree_next_water_time_ << ", "
         << request.field_.homeland_tree_growing_finish_time_ << ", "
         << request.field_.homeland_tree_assist_water_times_ << ", "
         << "'" << request.field_.homeland_tree_watered_actors_ << "', "
         << "'" << request.field_.homeland_tree_stolen_actors_ << "', "
         << request.field_.homeland_tower1_level_ << ", "
         << request.field_.homeland_tower2_level_ << ", "
         << request.field_.homeland_stronghold_level_ << ", "
         << request.field_.homeland_stronghold_next_call_support_time_ << ", "
         << "'" << request.field_.homeland_stronghold_supported_souls_ << "', "
         << "'" << request.field_.homeland_stronghold_support_souls_ << "', "
         << request.field_.homeland_stronghold_support_times_ << ", "
         << "'" << request.field_.homeland_invaders_ << "', "
         << "'" << request.field_.homeland_events_ << "', "
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

  protocol::StorageSocialActorSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageSocialActorSave::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageSocialActorSaveResponse(), result, request);
}

void StorageSocialActorSave::OnResponse(const protocol::StorageSocialActorSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

