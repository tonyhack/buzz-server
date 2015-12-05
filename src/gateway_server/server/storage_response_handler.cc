//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-31 17:03:33.
// File name: storage_response_handler.cc
//
// Description:
// Define class StorageResponseHandler.
//

#include "gateway_server/server/storage_response_handler.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_actor_online_types.h"
#include "database_server/protocol/storage_check_session_types.h"
#include "gateway_server/server/gateway_user.h"
#include "global/logging.h"

namespace gateway {

namespace server {

StorageResponseHandler::StorageResponseHandler() {}
StorageResponseHandler::~StorageResponseHandler() {}

bool StorageResponseHandler::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_CHECK_SESSION,
      boost::bind(&StorageResponseHandler::OnCheckSessionResponse, this, _1, _2, _3));
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_ACTOR_ONLINE,
      boost::bind(&StorageResponseHandler::OnActorOnlineResponse, this, _1, _2, _3));
  return true;
}

void StorageResponseHandler::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_CHECK_SESSION);
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_ACTOR_ONLINE);
}

void StorageResponseHandler::OnCheckSessionResponse(const std::string &request,
    const std::string &response, database::protocol::ResponseType::type result) {
  database::protocol::StorageCheckSessionRequest req;
  // 消息解析
  if(this->thrift_.Deserialize(req, request) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 得到玩家对象，并从session检测队列删除
  GatewayUser *user = CheckSessionQueue::GetInstance()->Remove(req.sequence_);
  if(user == NULL) {
    global::LogError("%s:%d (%s) Get user failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 事件通知玩家检测结果
  StatusEventCheckSession event;
  if(result != database::protocol::ResponseType::SUCCESS) {
    event.result_ = false;
  } else {
    database::protocol::StorageCheckSessionResponse res;
    if(this->thrift_.Deserialize(res, response) == false) {
      global::LogError("%s:%d (%s) Deserialize message failed.",
          __FILE__, __LINE__, __FUNCTION__);
      event.result_ = false;
    }
    event.online_ = res.online_;
    event.result_ = res.pass_;
  }
  user->OnEvent(StatusEventType::EVENT_CHECK_SESSION, (const char *)&event, sizeof(event));
}

void StorageResponseHandler::OnActorOnlineResponse(const std::string &request,
    const std::string &response, database::protocol::ResponseType::type result) {
  database::protocol::StorageActorOnlineRequest req;
  // 消息解析
  if(this->thrift_.Deserialize(req, request) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 得到玩家对象，并从session检测队列删除
  GatewayUser *user = CheckSessionQueue::GetInstance()->Remove(req.sequence_);
  if(user == NULL) {
    global::LogError("%s:%d (%s) Get user failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 事件通知玩家检测结果
  StatusEventUserOnline event;
  if(result != database::protocol::ResponseType::SUCCESS) {
    event.online_ = -1;
  } else {
    database::protocol::StorageActorOnlineResponse res;
    if(this->thrift_.Deserialize(res, response) == false) {
      global::LogError("%s:%d (%s) Deserialize message failed.",
          __FILE__, __LINE__, __FUNCTION__);
      event.online_ = -1;
    }
    event.online_ = res.online_;
  }
  user->OnEvent(StatusEventType::EVENT_USER_ONLINE, (const char *)&event, sizeof(event));
}

}  // namespace server

}  // namespace gateway

