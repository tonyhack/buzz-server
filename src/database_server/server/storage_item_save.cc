//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-24 14:03:23.
// File name: storage_item_save.cc
//
// Description:
// Define class StorageItemSave.
//

#include "database_server/server/storage_item_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_item_save_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageItemSave::StorageItemSave() : async_request_loop_(NULL) {}
StorageItemSave::~StorageItemSave() {}

bool StorageItemSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ITEM_SAVE,
      boost::bind(&StorageItemSave::Request, this, _1));

  return true;
}

void StorageItemSave::Request(const protocol::AsyncRequestMessage &message) {
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

  // Deserialize.
  protocol::StorageItemSaveRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ITEM_SAVE)
    << "(" << request.id_ << "," << (int)request.contain_type_ << "," << request.fields_.size() << ",";

  if(request.__isset.fields_ == false || request.fields_.empty()) {
    *query << "'')";
  } else {
    *query << "'" << request.fields_[0].location_ << ":" << request.fields_[0].vocation_ << ":" << request.fields_[0].template_id_
      << ":" << request.fields_[0].number_ << ":" << (int)request.fields_[0].bind_
      << ":" << (int)request.fields_[0].intensify_level_ << ":" << (int)request.fields_[0].max_intensify_level_
      << ":" << (int)request.fields_[0].random_attr1_ << ":" << request.fields_[0].random_attr_value1_
      << ":" << (int)request.fields_[0].random_attr2_ << ":" << request.fields_[0].random_attr_value2_
      << ":" << (int)request.fields_[0].random_attr3_ << ":" << request.fields_[0].random_attr_value3_
      << ":" << (int)request.fields_[0].random_attr4_ << ":" << request.fields_[0].random_attr_value4_
      << ":" << (int)request.fields_[0].random_attr5_ << ":" << request.fields_[0].random_attr_value5_
      << ":" << (int)request.fields_[0].random_attr6_ << ":" << request.fields_[0].random_attr_value6_
      << ":" << request.fields_[0].first_use_time_ << ":" << request.fields_[0].upgrade_lucky_;

    for(size_t pos = 1; pos < request.fields_.size(); ++pos) {
      *query << "," << request.fields_[pos].location_ << ":" << request.fields_[pos].vocation_ << ":" << request.fields_[pos].template_id_
        << ":" << request.fields_[pos].number_ << ":" << (int)request.fields_[pos].bind_
        << ":" << (int)request.fields_[pos].intensify_level_ << ":" << (int)request.fields_[pos].max_intensify_level_
        << ":" << (int)request.fields_[pos].random_attr1_ << ":" << request.fields_[pos].random_attr_value1_
        << ":" << (int)request.fields_[pos].random_attr2_ << ":" << request.fields_[pos].random_attr_value2_
        << ":" << (int)request.fields_[pos].random_attr3_ << ":" << request.fields_[pos].random_attr_value3_
        << ":" << (int)request.fields_[pos].random_attr4_ << ":" << request.fields_[pos].random_attr_value4_
        << ":" << (int)request.fields_[pos].random_attr5_ << ":" << request.fields_[pos].random_attr_value5_
        << ":" << (int)request.fields_[pos].random_attr6_ << ":" << request.fields_[pos].random_attr_value6_
        << ":" << request.fields_[pos].first_use_time_ << ":" << request.fields_[0].upgrade_lucky_;
    }

    *query << "')";
  }

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
  protocol::StorageItemSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);

}

void StorageItemSave::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageItemSaveResponse(), result, request);
}

void StorageItemSave::OnResponse(const protocol::StorageItemSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

