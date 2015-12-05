#include "database_server/server/storage_attr_actor_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_attr_actor_save_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageAttrActorSave::StorageAttrActorSave() : async_request_loop_(NULL) {}
StorageAttrActorSave::~StorageAttrActorSave() {}

bool StorageAttrActorSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageAttrActorSave falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ATTR_ACTOR_SAVE,
      boost::bind(&StorageAttrActorSave::Request, this, _1));

  return true;
}

void StorageAttrActorSave::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageAttrActorSaveRequest request;
  if (this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageAttrActorSaveRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ATTR_ACTOR_SAVE)
         << "(" 
         << request.actor_id_ << ", "
         << request.field_.noble_level_ << ", "
         << request.field_.noble_used_item_count_ << ", "
         << request.field_.noble_max_hp_add_value_ << ", "
         << request.field_.noble_max_mp_add_value_ << ", "
         << request.field_.noble_physics_attack_add_value_ << ", "
         << request.field_.noble_physics_defence_add_value_ << ", "
         << request.field_.noble_magic_attack_add_value_ << ", "
         << request.field_.noble_magic_defence_add_value_ << ", "
         << request.field_.noble_dodge_add_value_ << ", "
         << request.field_.noble_crit_add_value_ << ", "
         << request.field_.noble_ignore_defence_add_value_ << ", "
         << request.field_.sword_avail_step_ << ", "
         << request.field_.sword_current_step_ << ", "
         << request.field_.sword_lucky_value_ << ", "
         << request.field_.sword_gain_time_ << ", "
         << request.field_.element_level_ << ", "
         << request.field_.spirit_level_ << ", "
         << request.field_.spirit_lucky_value_ << ", "
         << request.field_.spirit_current_facade_type_ << ", "
         << request.field_.spirit_horse_facade_id_ << ", "
         << request.field_.spirit_wing_facade_id_ << ", "
         << "'" << request.field_.rune_slot_data_.c_str() << "', "
         << "'" << request.field_.legendary_weapon_pieces_ << "'"
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

  protocol::StorageAttrActorSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageAttrActorSave::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageAttrActorSaveResponse(), result, request);
}

void StorageAttrActorSave::OnResponse(const protocol::StorageAttrActorSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

