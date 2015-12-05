//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-19 16:48:35.
// File name: storage_actor_login.cc
//
// Description:
//

#include "database_server/server/storage_actor_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_actor_login_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageActorLogin::StorageActorLogin() : async_request_loop_(NULL) {}
StorageActorLogin::~StorageActorLogin() {}

bool StorageActorLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  assert(async_request_loop);
  this->async_request_loop_ = async_request_loop;
  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ACTOR_LOGIN,
      boost::bind(&StorageActorLogin::Request, this, _1));
  return true;
}

void StorageActorLogin::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageActorLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ACTOR_LOGIN)
    << "(" << request.id_ << ")";

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
  if(result->num_rows() != 1 || result->num_fields() < 1) {
    global::LogError("%s:%d (%s) Result's row/fields error.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check ret code.
  int index = result->field_num("@ret_code");
  if(index == -1) {
    global::LogError("%s:%d (%s) Get ret code failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Response.
  protocol::StorageActorLoginResponse response;
  protocol::StorageActorField field;
  if(atoi((*result)[0][index].data()) == -1) {
    response.__set_result_(protocol::ActorLoginResult::ERROR_INEXIST);
  } else {
    // Check fields number.
    if(result->num_fields() != protocol::ActorField::MAX + 1) {
      global::LogError("%s:%d (%s) Result's fields error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    if(protocol::g_storage_actor_login_constants.kActorFieldStr.size() <
        protocol::ActorField::MAX) {
      global::LogError("%s:%d (%s) Actor field string's size is error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    // Set actor field.
    for(size_t pos = 0; pos < protocol::ActorField::MAX; ++pos) {
      index = result->field_num(protocol::g_storage_actor_login_constants.kActorFieldStr[pos]);
      if(index == -1) {
        global::LogError("%s:%d (%s) Get result [%s] failed.", __FILE__, __LINE__,
            __FUNCTION__, protocol::g_storage_actor_login_constants.kActorFieldStr[pos].c_str());
        this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
        return ;
      }
      switch(pos) {
        case protocol::ActorField::NAME:
          field.__set_name_((*result)[0][index].data());
          break;
        case protocol::ActorField::LEVEL:
          field.__set_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::VOCATION:
          field.__set_vocation_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::GENDER:
          field.__set_gender_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::HEAD:
          field.__set_head_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::HAIR:
          field.__set_hair_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::CURRENT_HP:
          field.__set_current_hp_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::CURRENT_MP:
          field.__set_current_mp_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::CURRENT_EXP:
          field.__set_current_exp_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::NIMBUS:
          field.__set_nimbus_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::GOLD:
          field.__set_gold_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::FREEDOM_DOLLARS:
          field.__set_freedom_dollars_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::RESTRICT_DOLLARS:
          field.__set_restrict_dollars_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::HONOUR:
          field.__set_honour_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::WAR_SOUL:
          field.__set_war_soul_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::GEM_SHARD:
          field.__set_gem_shard_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::JUSTICE:
          field.__set_justice_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::MAP:
          field.__set_map_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::X:
          field.__set_x_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::Y:
          field.__set_y_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::PRE_MAP:
          field.__set_pre_map_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::PRE_X:
          field.__set_pre_x_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::PRE_Y:
          field.__set_pre_y_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::BROWN_NAME:
          field.__set_brown_name_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::PK_MODE:
          field.__set_pk_mode_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::FUNCTIONALITY_STATE:
          field.__set_functionality_state_((*result)[0][index].data());
          break;
        case protocol::ActorField::PACKET_CAPACITY:
          field.__set_packet_capacity_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::STORAGE_CAPACITY:
          field.__set_storage_capacity_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::AXE_FIGHT_SCORE:
          field.__set_axe_fight_score_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::SHOOT_FIGHT_SCORE:
          field.__set_shoot_fight_score_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::MAGIC_FIGHT_SCORE:
          field.__set_magic_fight_score_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::LAST_LOGOUT_TIME:
          field.__set_last_logout_time_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::VIP_REMAINDER_TIME:
          field.__set_vip_remainder_time_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::VIP_TYPE:
          field.__set_vip_type_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::WORLD_JUMP_VIP_NUM:
          field.__set_world_jump_vip_num_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::FCM_TYPE:
          field.__set_fcm_type_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::FCM_ONLINE_TIME:
          field.__set_fcm_online_time_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::CREATE_TIME:
          field.__set_create_time_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::HP_TASLIMAN:
          field.__set_hp_tasliman_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::MP_TASLIMAN:
          field.__set_mp_tasliman_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::MIRACLE_INTEGRAL:
          field.__set_miracle_integral_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::LEGEND_INTEGRAL:
          field.__set_legend_integral_(atoi((*result)[0][index].data()));
          break;
        case protocol::ActorField::ARENA_INTEGRAL:
          field.__set_arena_integral_(atoi((*result)[0][index].data()));
          break;
        default:
          break;
      }
    }
  }
  response.__set_field_(field);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageActorLogin::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageActorLoginResponse(), result, request);
}

void StorageActorLogin::OnResponse(const protocol::StorageActorLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

