#include "database_server/server/storage_item_actor_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "global/logging.h"
#include "database_server/protocol/storage_item_actor_login_constants.h"
#include "database_server/server/async_request_loop.h"

namespace database {

namespace server {

StorageItemActorLogin::StorageItemActorLogin() {}
StorageItemActorLogin::~StorageItemActorLogin() {}

bool StorageItemActorLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if (NULL == async_request_loop) {
    LOG_ERROR("Init StorageItemActorLogin falied, async_request_loop is null.");
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ITEM_ACTOR_LOGIN,
      boost::bind(&StorageItemActorLogin::Request, this, _1));

  return true;
}

void StorageItemActorLogin::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageItemActorLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    LOG_ERROR("Deserialize message StorageItemActorLoginRequest failed.");
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ITEM_ACTOR_LOGIN)
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
  protocol::StorageItemActorLoginResponse response;
  response.__set_result_(protocol::ItemActorLoginResult::SUCCESS);

  if (result->num_rows() == 0) {
    // default all zeros
  } else if (result->num_rows() >= 1) {

    // set values
    for (size_t pos = 0; pos < protocol::ItemActorField::MAX; ++pos) {
      int index = result->field_num(protocol::g_storage_item_actor_login_constants.kItemActorFieldStr[pos]); 
      if (-1 == index) {
        LOG_ERROR("Get result [%s] failed.",
                  protocol::g_storage_item_actor_login_constants.kItemActorFieldStr[pos].c_str());
        this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
        return;
      }

      switch (pos) {
        case protocol::ItemActorField::GEM_GATHERING_POINT:
          response.field_.__set_gem_gathering_point_(atoi((*result)[0][pos].data()));
          break;
        case protocol::ItemActorField::SHORTCUT1:
          response.field_.__set_shortcut1(atoi((*result)[0][pos].data()));
          break;
        case protocol::ItemActorField::SHORTCUT2:
          response.field_.__set_shortcut2(atoi((*result)[0][pos].data()));
          break;
        case protocol::ItemActorField::SHORTCUT3:
          response.field_.__set_shortcut3(atoi((*result)[0][pos].data()));
          break;
        case protocol::ItemActorField::SHORTCUT4:
          response.field_.__set_shortcut4(atoi((*result)[0][pos].data()));
          break;
        case protocol::ItemActorField::GIFTS_PICK_NUM:
          response.field_.__set_gifts_pick_num_(atoi((*result)[0][pos].data()));
          break;
        case protocol::ItemActorField::ITEM_BUY_NUM:
          response.field_.__set_item_buy_num_((*result)[0][pos].data());
          break;
        default:
          break;
      }
    }
  }

  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageItemActorLogin::ResponseFailed(protocol::ResponseType::type result, 
    const protocol::AsyncRequestMessage &request) {
    this->OnResponse(protocol::StorageItemActorLoginResponse(), result, request);
}

void StorageItemActorLogin::OnResponse(const protocol::StorageItemActorLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
    this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

