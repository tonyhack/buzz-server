#include "database_server/server/storage_social_actor_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_social_actor_login_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageSocialActorLogin::StorageSocialActorLogin() {}
StorageSocialActorLogin::~StorageSocialActorLogin() {}

bool StorageSocialActorLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageSocialActorLogin falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_SOCIAL_ACTOR_LOGIN,
      boost::bind(&StorageSocialActorLogin::Request, this, _1));

  return true;
}

void StorageSocialActorLogin::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageSocialActorLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageSocialActorLoginRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_SOCIAL_ACTOR_LOGIN)
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
  protocol::StorageSocialActorLoginResponse response;
  response.__set_result_(protocol::SocialActorLoginResult::SUCCESS);

  if (result->num_rows() == 0) {
    // Default all zeros.
  } else if (result->num_rows() >= 1) {

    // Set values.
    for (size_t pos = 0; pos < protocol::SocialActorField::MAX; ++pos) {
      int index = result->field_num(protocol::g_storage_social_actor_login_constants.kSocialActorFieldStr[pos]); 
      if (-1 == index) {
        LOG_ERROR("Get result [%s] failed.",
                  protocol::g_storage_social_actor_login_constants.kSocialActorFieldStr[pos].c_str());
        this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
        return;
      }

      switch (pos) {
        case protocol::SocialActorField::SIGNATURE:
          response.field_.__set_signature_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::FRIENDS:
          response.field_.__set_friends_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::BLACKLIST:
          response.field_.__set_blacklist_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::ENEMIES:
          response.field_.__set_enemies_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::HOMELAND_UPGRADE_START_TIME:
          response.field_.__set_homeland_upgrade_start_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_UPGRADE_FINISH_TIME:
          response.field_.__set_homeland_upgrade_finish_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TEMPLE_LEVEL:
          response.field_.__set_homeland_temple_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TEMPLE_HARVEST_TIMES:
          response.field_.__set_homeland_temple_harvest_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TEMPLE_FORCE_HARVEST_TIMES:
          response.field_.__set_homeland_temple_force_harvest_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TEMPLE_NEXT_HARVEST_TIME:
          response.field_.__set_homeland_temple_next_harvest_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_GOLDMINE_LEVEL:
          response.field_.__set_homeland_goldmine_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_GOLDMINE_ROBBED_TIMES:
          response.field_.__set_homeland_goldmine_robbed_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_GOLDMINE_LOSS_RATE:
          response.field_.__set_homeland_goldmine_loss_rate_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_GOLDMINE_NEXT_HARVEST_TIME:
          response.field_.__set_homeland_goldmine_next_harvest_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_GOLDMINE_ROB_TIMES:
          response.field_.__set_homeland_goldmine_rob_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_GOLDMINE_NEXT_ROB_TIME:
          response.field_.__set_homeland_goldmine_next_rob_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_POOL_LEVEL:
          response.field_.__set_homeland_pool_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_POOL_LAST_HARVEST_TIME:
          response.field_.__set_homeland_pool_last_harvest_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_CHURCH_FREE_PRAY_TIMES:
          response.field_.__set_homeland_church_free_pray_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_CHURCH_PAID_PRAY_TIMES:
          response.field_.__set_homeland_church_paid_pray_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_CHURCH_FREE_REFRESH_TIMES:
          response.field_.__set_homeland_church_free_refresh_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_CHURCH_CHEST_TYPE:
          response.field_.__set_homeland_church_chest_type_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TREE_LEVEL:
          response.field_.__set_homeland_tree_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TREE_HARVEST_TIMES:
          response.field_.__set_homeland_tree_harvest_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TREE_STAGE:
          response.field_.__set_homeland_tree_stage_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TREE_WATERED_TIMES:
          response.field_.__set_homeland_tree_watered_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TREE_NEXT_WATER_TIME:
          response.field_.__set_homeland_tree_next_water_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TREE_GROWING_FINISH_TIME:
          response.field_.__set_homeland_tree_growing_finish_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TREE_ASSIST_WATER_TIMES:
          response.field_.__set_homeland_tree_assist_water_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TREE_WATERED_ACTORS:
          response.field_.__set_homeland_tree_watered_actors_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::HOMELAND_TREE_STOLEN_ACTORS:
          response.field_.__set_homeland_tree_stolen_actors_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::HOMELAND_TOWER1_LEVEL:
          response.field_.__set_homeland_tower1_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_TOWER2_LEVEL:
          response.field_.__set_homeland_tower2_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_STRONGHOLD_LEVEL:
          response.field_.__set_homeland_stronghold_level_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_STRONGHOLD_NEXT_CALL_SUPPORT_TIME:
          response.field_.__set_homeland_stronghold_next_call_support_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_STRONGHOLD_SUPPORTED_SOULS:
          response.field_.__set_homeland_stronghold_supported_souls_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::HOMELAND_STRONGHOLD_SUPPORT_SOULS:
          response.field_.__set_homeland_stronghold_support_souls_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::HOMELAND_STRONGHOLD_SUPPORT_TIMES:
          response.field_.__set_homeland_stronghold_support_times_(atoi((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::HOMELAND_INVADERS:
          response.field_.__set_homeland_invaders_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::HOMELAND_EVENTS:
          response.field_.__set_homeland_events_((*result)[0][index].data());
          break;
        case protocol::SocialActorField::LAST_SAVE_TIME:
          response.field_.__set_last_save_time_(atol((*result)[0][index].data()));
          break;
        case protocol::SocialActorField::SOUL_DATA:
          response.field_.__set_soul_data_((*result)[0][index].data());
          break;
        default:
          break;
      }
    }
  }

  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageSocialActorLogin::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
    this->OnResponse(protocol::StorageSocialActorLoginResponse(), result, request);
}

void StorageSocialActorLogin::OnResponse(const protocol::StorageSocialActorLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
    this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

