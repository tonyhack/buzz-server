//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-16 23:32:34.
// File name: storage_task_actor_load.cc
//
// Description:
//
#include "database_server/server/storage_task_actor_load.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_task_actor_login_constants.h"
#include "database_server/protocol/storage_task_actor_login_types.h"
#include "database_server/protocol/storage_task_actor_save_types.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageTaskActorLoad::StorageTaskActorLoad() {}
StorageTaskActorLoad::~StorageTaskActorLoad() {}

bool StorageTaskActorLoad::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_TASK_ACTOR_LOGIN,
      boost::bind(&StorageTaskActorLoad::Request, this, _1));

  return true;
}

void StorageTaskActorLoad::Request(const protocol::AsyncRequestMessage &message) {
  // Get session.
  MysqlSessionPtr session = this->async_request_loop_->GetMysqlSession(message.channel_);
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get mysql session failed.", __FILE__, __LINE__,
        __FILE__);
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
  protocol::StorageTaskActorLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_TASK_ACTOR_LOGIN)
    << "(" << request.actor_id_ << ")";

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
  // Response
  protocol::StorageTaskActorLoginResponse response;

  if(result->num_rows()  == 1) {
    // Check field number.
    if(result->num_fields() != protocol::TaskActorField::MAX) {
      global::LogError("%s:%d (%s) Result's fields error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    response.__set_result_(database::protocol::TaskActorResult::SUCCESS);
    database::protocol::StorageTaskActorField field;
    field.__set_flag_(atoi((*result)[0][0].data()));
    field.__set_round_num_(atoi((*result)[0][1].data()));
    field.__set_total_num_(atoi((*result)[0][2].data()));
    field.__set_circle_num_(atoi((*result)[0][3].data()));
    field.__set_lv_stage_(atoi((*result)[0][4].data()));
    field.__set_condition_id_(atoi((*result)[0][5].data()));
    field.__set_trip_num_(atoi((*result)[0][6].data()));
    field.__set_exploit_award_(atoi((*result)[0][7].data()));
    field.__set_daily_exploit_(atoi((*result)[0][8].data()));
    field.__set_transport_finished_count_(atoi((*result)[0][9].data()));
    field.__set_transport_free_refreshed_count_(atoi((*result)[0][10].data()));
    field.__set_transport_robed_count_(atoi((*result)[0][11].data()));
    field.__set_transport_current_transport_(atoi((*result)[0][12].data()));
    field.__set_transport_status_(atoi((*result)[0][13].data()));
    response.__set_data_(field);
  }
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageTaskActorLoad::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageTaskActorLoginResponse(), result, request);
}

void StorageTaskActorLoad::OnResponse(const protocol::StorageTaskActorLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

