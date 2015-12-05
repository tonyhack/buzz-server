//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 11:58:54.
// File name: storage_item_load.cc
//
// Description:
// Define class StorageItemLoad.
//

#include "database_server/server/storage_item_load.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_item_login_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageItemLoad::StorageItemLoad() : async_request_loop_(NULL) {}
StorageItemLoad::~StorageItemLoad() {}

bool StorageItemLoad::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->async_request_loop_ = async_request_loop;
  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ITEM_LOGIN,
      boost::bind(&StorageItemLoad::Request, this, _1));
  return true;
}

void StorageItemLoad::Request(const protocol::AsyncRequestMessage &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetMysqlSession(message.channel_);
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get mysql session failed.", __FILE__, __LINE__,
        __FUNCTION__);
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
    global::LogError("%s:%d (%s) Get mysql query failed.", __FILE__, __LINE__,
        __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
    return ;
  }

  // Deserialize.
  protocol::StorageItemLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ITEM_LOGIN)
    << "(" << request.actor_id_ << ", " << (int)request.contain_type_ << ")";

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
      global::LogError("%s:%d (%s) Ping mysql is ok, but error=[%s]",
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

  // Response.
  protocol::StorageItemLoginResponse response;

  if(result->num_rows() > 0) {
    // Check field number.
    if(result->num_fields() != protocol::ItemField::MAX) {
      global::LogError("%s:%d (%s) Result's fields error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    int index = 0;
    size_t pos = 0;
    // TODO: big message should be divided into several small messages.
    for(; pos < result->num_rows(); ++pos) {
      protocol::StorageItemField field;
      for(size_t i = 0; i < protocol::ItemField::MAX; ++i) {
        index = result->field_num(protocol::g_storage_item_login_constants.kItemFieldStr[i]);
        if(index == -1) {
          global::LogError("%s:%d (%s) Get result [%s] failed.", __FILE__, __LINE__, __FUNCTION__,
              protocol::g_storage_item_login_constants.kItemFieldStr[i].c_str());
          this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
          return ;
        }
        switch(i) {
          case protocol::ItemField::LOCATION:
            field.__set_location_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::VOCATION:
            field.__set_vocation_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::TEMPLATE_ID:
            field.__set_template_id_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::NUMBER:
            field.__set_number_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::BIND:
            field.__set_bind_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::INTENSIFY_LEVEL:
            field.__set_intensify_level_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::MAX_INTENSIFY_LEVEL:
            field.__set_max_intensify_level_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR1:
            field.__set_random_attr1_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR_VALUE1:
            field.__set_random_attr_value1_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR2:
            field.__set_random_attr2_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR_VALUE2:
            field.__set_random_attr_value2_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR3:
            field.__set_random_attr3_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR_VALUE3:
            field.__set_random_attr_value3_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR4:
            field.__set_random_attr4_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR_VALUE4:
            field.__set_random_attr_value4_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR5:
            field.__set_random_attr5_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR_VALUE5:
            field.__set_random_attr_value5_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR6:
            field.__set_random_attr6_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::RANDOM_ATTR_VALUE6:
            field.__set_random_attr_value6_(atoi((*result)[pos][i].data()));
            break;
          case protocol::ItemField::FIRST_USE_TIME:
            field.__set_first_use_time_(atol((*result)[pos][i].data()));
            break;
          case protocol::ItemField::UPGRADE_LUCKY:
            field.__set_upgrade_lucky_(atoi((*result)[pos][i].data()));
            break;
          default:
            break;
        }
      }
      response.fields_.push_back(field);
      response.__isset.fields_ = true;
      response.__set_result_(protocol::ItemLoginResult::SUCCESS);
    }
  }
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageItemLoad::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageItemLoginResponse(), result, request);
}

void StorageItemLoad::OnResponse(const protocol::StorageItemLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

