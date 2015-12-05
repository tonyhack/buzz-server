#include "database_server/server/storage_attr_actor_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_attr_actor_login_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageAttrActorLogin::StorageAttrActorLogin() {}
StorageAttrActorLogin::~StorageAttrActorLogin() {}

bool StorageAttrActorLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageAttrActorLogin falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ATTR_ACTOR_LOGIN,
      boost::bind(&StorageAttrActorLogin::Request, this, _1));

  return true;
}

void StorageAttrActorLogin::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageAttrActorLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageAttrActorLoginRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ATTR_ACTOR_LOGIN)
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
  protocol::StorageAttrActorLoginResponse response;
  response.__set_result_(protocol::AttrActorLoginResult::SUCCESS);

  if (result->num_rows() == 0) {
    // Default all zeros.
  } else if (result->num_rows() >= 1) {

    // Set values.
    for (size_t pos = 0; pos < protocol::AttrActorField::MAX; ++pos) {
      int index = result->field_num(protocol::g_storage_attr_actor_login_constants.kAttrActorFieldStr[pos]); 
      if (-1 == index) {
        LOG_ERROR("Get result [%s] failed.",
                  protocol::g_storage_attr_actor_login_constants.kAttrActorFieldStr[pos].c_str());
        this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
        return;
      }

      switch (pos) {
        case protocol::AttrActorField::NOBLE_LEVEL:
          response.field_.__set_noble_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_USED_ITEM_COUNT:
          response.field_.__set_noble_used_item_count_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_MAX_HP_ADD_VALUE:
          response.field_.__set_noble_max_hp_add_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_MAX_MP_ADD_VALUE:
          response.field_.__set_noble_max_mp_add_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_PHYSICS_ATTACK_ADD_VALUE:
          response.field_.__set_noble_physics_attack_add_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_PHYSICS_DEFENCE_ADD_VALUE:
          response.field_.__set_noble_physics_defence_add_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_MAGIC_ATTACK_ADD_VALUE:
          response.field_.__set_noble_magic_attack_add_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_MAGIC_DEFENCE_ADD_VALUE:
          response.field_.__set_noble_magic_defence_add_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_DODGE_ADD_VALUE:
          response.field_.__set_noble_dodge_add_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_CRIT_ADD_VALUE:
          response.field_.__set_noble_crit_add_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::NOBLE_IGNORE_DEFENCE_ADD_VALUE:
          response.field_.__set_noble_ignore_defence_add_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::SWORD_AVAIL_STEP:
          response.field_.__set_sword_avail_step_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::SWORD_CURRENT_STEP:
          response.field_.__set_sword_current_step_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::SWORD_LUCKY_VALUE:
          response.field_.__set_sword_lucky_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::SWORD_GAIN_TIME:
          response.field_.__set_sword_gain_time_(atol((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::ELEMENT_LEVEL:
          response.field_.__set_element_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::SPIRIT_LEVEL:
          response.field_.__set_spirit_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::SPIRIT_LUCKY_VALUE:
          response.field_.__set_spirit_lucky_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::SPIRIT_CURRENT_FACADE_TYPE:
          response.field_.__set_spirit_current_facade_type_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::SPIRIT_HORSE_FACADE_ID:
          response.field_.__set_spirit_horse_facade_id_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::SPIRIT_WING_FACADE_ID:
          response.field_.__set_spirit_wing_facade_id_(atoi((*result)[0][index].data()));
          break;
        case protocol::AttrActorField::RUNES:
          response.field_.__set_rune_slot_data_((*result)[0][index].data());
          break;
        case protocol::AttrActorField::LEGENDARY_WEAPON_PIECES:
          response.field_.__set_legendary_weapon_pieces_((*result)[0][index].data());
          break;
        default:
          break;
      }
    }
  }

  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageAttrActorLogin::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
    this->OnResponse(protocol::StorageAttrActorLoginResponse(), result, request);
}

void StorageAttrActorLogin::OnResponse(const protocol::StorageAttrActorLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
    this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

