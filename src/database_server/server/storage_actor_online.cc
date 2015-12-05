//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-04 14:54:00.
// File name: storage_actor_online.cc
//
// Description:
//

#include "database_server/server/storage_actor_online.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/server/async_request_loop.h"
#include "global/actor_id.h"

namespace database {

namespace server {

StorageActorOnline::StorageActorOnline() : async_request_loop_(NULL) {}
StorageActorOnline::~StorageActorOnline() {}

bool StorageActorOnline::Initialize(AsyncRequestLoop *async_request_loop) {
  assert(async_request_loop);
  this->async_request_loop_ = async_request_loop;
  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ACTOR_ONLINE,
      boost::bind(&StorageActorOnline::Request, this, _1));
  return true;
}

void StorageActorOnline::Request(const protocol::AsyncRequestMessage &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetCenterMysqlSession();
  if(session.get() == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check connection status.
  if(session->CheckConnectStatus() == false) {
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get mysql query failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_DISCONNECTED, message);
    return ;
  }

  // Deserialize.
  protocol::StorageActorOnlineRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
      message.message_.data(), message.message_.size()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  global::ActorID id(request.id_);

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ACTOR_ONLINE)
    << "(" << id.table_ << ", " << id.offset_ << ")";

  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_DEBUG,
      "Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if(result == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Store get result of null.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Check any errors.
  if(query->errnum() > 0) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Procedure[%s] execute failed, error=[%s].", __FILE__, __LINE__,
        __FUNCTION__, query->str().c_str(), query->error());
    if(session->PingConnected() == true) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Ping mysql is ok, but error=[%s].",
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
  if(result->num_rows() != 1 || result->num_fields() != 1) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's row/fields error.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }
  if(result->field_name(0) != "@ret_online_status") {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's field name error.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Get and response the result.
  protocol::StorageActorOnlineResponse response;
  response.__set_online_(atoi((*result)[0][0].data()));
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageActorOnline::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageActorOnlineResponse(), result, request);
}

void StorageActorOnline::OnResponse(const protocol::StorageActorOnlineResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

