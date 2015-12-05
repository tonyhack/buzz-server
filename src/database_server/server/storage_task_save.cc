//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-15 15:12:05.
// File name: storage_task_save.cc
//
// Description:
// Define class StorageTaskSave.
//

#include "database_server/server/storage_task_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_task_save_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageTaskSave::StorageTaskSave() : async_request_loop_(NULL) {}
StorageTaskSave::~StorageTaskSave() {}

bool StorageTaskSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_TASK_SAVE,
      boost::bind(&StorageTaskSave::Request, this, _1));

  return true;
}

void StorageTaskSave::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageTaskSaveRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_TASK_SAVE)
    << "(" << request.id_ << "," << request.actives_.size() << ",";

  // Active task.
  if(request.actives_.empty() == false) {
    *query << "'" << request.actives_[0].task_ << ":" << (int)request.actives_[0].cond_type1_
      << ":" << request.actives_[0].var1_ << ":" << request.actives_[0].value1_
      << ":" << (int)request.actives_[0].cond_type2_ << ":" << request.actives_[0].var2_
      << ":" << request.actives_[0].value2_ << ":" << (int)request.actives_[0].cond_type3_
      << ":" << request.actives_[0].var3_ << ":" << request.actives_[0].value3_ << ":" << (core::int32)request.actives_[0].task_type_;
    for(size_t pos = 1; pos < request.actives_.size(); ++pos) {
      *query << "," << request.actives_[pos].task_ << ":" << (int)request.actives_[pos].cond_type1_
        << ":" << request.actives_[pos].var1_ << ":" << request.actives_[pos].value1_
        << ":" << (int)request.actives_[pos].cond_type2_ << ":" << request.actives_[pos].var2_
        << ":" << request.actives_[pos].value2_ << ":" << (int)request.actives_[pos].cond_type3_
        << ":" << request.actives_[pos].var3_ << ":" << request.actives_[pos].value3_ << ":" << (core::int32)request.actives_[pos].task_type_;
    }
    *query << "',";
  } else {
    *query << "'',";
  }

  // Finished task.
  *query << request.finishes_.size() << ",";

  if(request.finishes_.empty() == false) {
    *query << "'" << request.finishes_[0].task_id_ << ":" <<
    request.finishes_[0].type_;
    for(size_t pos = 1; pos < request.finishes_.size(); ++pos) {
      *query << "," << request.finishes_[pos].task_id_ << ":" <<
      request.finishes_[pos].type_;
    }
    *query << "')";
  } else {
    *query << "'')";
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
  protocol::StorageTaskSaveResponse response;
  response.__set_result_(true);
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageTaskSave::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageTaskSaveResponse(), result, request);
}

void StorageTaskSave::OnResponse(const protocol::StorageTaskSaveResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

