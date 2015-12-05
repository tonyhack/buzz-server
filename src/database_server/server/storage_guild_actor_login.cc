#include "database_server/server/storage_guild_actor_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_guild_actor_login_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageGuildActorLogin::StorageGuildActorLogin() {}
StorageGuildActorLogin::~StorageGuildActorLogin() {}

bool StorageGuildActorLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageGuildActorLogin falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_GUILD_ACTOR_LOGIN,
      boost::bind(&StorageGuildActorLogin::Request, this, _1));

  return true;
}

void StorageGuildActorLogin::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageGuildActorLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageGuildActorLoginRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_GUILD_ACTOR_LOGIN)
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
  protocol::StorageGuildActorLoginResponse response;
  response.__set_result_(protocol::GuildActorLoginResult::SUCCESS);

  if (result->num_rows() == 0) {
    // Default all zeros, set not found.
    response.__set_result_(protocol::GuildActorLoginResult::NOT_FOUND);
  } else if (result->num_rows() >= 1) {

    // Set values.
    for (size_t pos = 0; pos < protocol::GuildActorField::MAX; ++pos) {
      int index = result->field_num(protocol::g_storage_guild_actor_login_constants.kGuildActorFieldStr[pos]); 
      if (-1 == index) {
        LOG_ERROR("Get result [%s] failed.",
                  protocol::g_storage_guild_actor_login_constants.kGuildActorFieldStr[pos].c_str());
        this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
        return;
      }

      switch (pos) {
        case protocol::GuildActorField::GUILD_ID:
          response.field_.__set_guild_id_(atol((*result)[0][index].data()));
          break;
        case protocol::GuildActorField::GUILD_POSITION:
          response.field_.__set_guild_position_(atoi((*result)[0][index].data()));
          break;
        case protocol::GuildActorField::CURRENT_CONTRIBUTION_VALUE:
          response.field_.__set_current_contribution_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::GuildActorField::TOTAL_CONTRIBUTION_VALUE:
          response.field_.__set_total_contribution_value_(atoi((*result)[0][index].data()));
          break;
        case protocol::GuildActorField::GOLD_CONTRIBUTION:
          response.field_.__set_gold_contribution_(atoi((*result)[0][index].data()));
          break;
        case protocol::GuildActorField::FREEDOM_DOLLARS_CONTRIBUTION:
          response.field_.__set_freedom_dollars_contribution_(atoi((*result)[0][index].data()));
          break;
        case protocol::GuildActorField::LIGHT_CRYSTAL_CONTRIBUTION:
          response.field_.__set_light_crystal_contribution_(atoi((*result)[0][index].data()));
          break;
        case protocol::GuildActorField::DARK_CRYSTAL_CONTRIBUTION:
          response.field_.__set_dark_crystal_contribution_(atoi((*result)[0][index].data()));
          break;
        case protocol::GuildActorField::HOLY_CRYSTAL_CONTRIBUTION:
          response.field_.__set_holy_crystal_contribution_(atoi((*result)[0][index].data()));
          break;
        case protocol::GuildActorField::GUILD_SKILLS:
          response.field_.__set_guild_skills_((*result)[0][index].data());
          break;
        case protocol::GuildActorField::GUILD_BUFFS:
          response.field_.__set_guild_buffs_((*result)[0][index].data());
          break;
        case protocol::GuildActorField::AWARDED_GUILD_PLAYINGS:
          response.field_.__set_awarded_guild_playings_((*result)[0][index].data());
          break;
        default:
          break;
      }
    }
  }

  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageGuildActorLogin::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
    this->OnResponse(protocol::StorageGuildActorLoginResponse(), result, request);
}

void StorageGuildActorLogin::OnResponse(const protocol::StorageGuildActorLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
    this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

