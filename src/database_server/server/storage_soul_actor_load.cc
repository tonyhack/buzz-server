//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-30 10:11:23.
// File name: storage_soul_actor_load.cc
//
// Description:
// Define class StorageSoulActorLoad.
//

#include "database_server/server/storage_soul_actor_load.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_soul_actor_login_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageSoulActorLoad::StorageSoulActorLoad() {}
StorageSoulActorLoad::~StorageSoulActorLoad() {}

bool StorageSoulActorLoad::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    LOG_ERROR("Parameter async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_SOUL_ACTOR_LOGIN,
      boost::bind(&StorageSoulActorLoad::Request, this, _1));

  return true;
}

void StorageSoulActorLoad::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageSoulActorLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_SOUL_ACTOR_LOGIN)
    << "(" << request.actor_id_ << ")";

  LOG_DEBUG("Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if(result == NULL) {
    LOG_ERROR("Deserialize message failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check any errors.
  if(query->errnum() > 0) {
    LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
        query->str().c_str(), query->error());
    if(session->PingConnected() == true) {
      LOG_ERROR("Ping mysql is ok, but error=[%s]", query->error());
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
  protocol::StorageSoulActorLoginResponse response;

  if(result->num_rows() == 1) {

    // Check field number.
    if(result->num_fields() != protocol::SoulActorField::MAX) {
      LOG_ERROR("Result's fields error.");
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    int index = 0;
    size_t pos = 0;
    for(size_t i = 0; i < protocol::SoulActorField::MAX; ++i) {
      index = result->field_num(protocol::g_storage_soul_actor_login_constants.kSoulActorFieldStr[i]);
      if(index == -1) {
        LOG_ERROR("Get result [%s] failed.",
            protocol::g_storage_soul_actor_login_constants.kSoulActorFieldStr[i].c_str());
        this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
        return ;
      }
      switch(i) {
        case protocol::SoulActorField::AXE_SETTING:
          response.field_.__set_axe_setting_(atoi((*result)[0][i].data()));
          break;
        case protocol::SoulActorField::SHOOT_SETTING:
          response.field_.__set_shoot_setting_(atoi((*result)[0][i].data()));
          break;
        case protocol::SoulActorField::MAGIC_SETTING:
          response.field_.__set_magic_setting_(atoi((*result)[0][i].data()));
          break;
        case protocol::SoulActorField::SETTINGS:
          response.field_.__set_settings_((*result)[0][i].data());
          break;
        case protocol::SoulActorField::ALTAR_SUMMON_PAID:
          response.field_.__set_altar_summon_paid_(atoi((*result)[0][i].data()));
          break;
        case protocol::SoulActorField::ALTAR_FREE_SUMMON_TIMES:
          response.field_.__set_altar_free_summon_times_(atoi((*result)[0][i].data()));
          break;
        case protocol::SoulActorField::ALTAR_CARDS:
          response.field_.__set_altar_cards_((*result)[0][i].data());
          break;
        case protocol::SoulActorField::ALTAR_CARD_BIND:
          response.field_.__set_altar_card_bind_(atoi((*result)[0][i].data()));
          break;
        case protocol::SoulActorField::CURRENT_ENERGY:
          response.field_.__set_current_energy_(atoi((*result)[0][i].data()));
          break;
        default:
          break;
      }
    }
  }

  response.__set_result_(protocol::SoulActorLoginResult::SUCCESS);

  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageSoulActorLoad::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageSoulActorLoginResponse(), result, request);
}

void StorageSoulActorLoad::OnResponse(
    const protocol::StorageSoulActorLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

