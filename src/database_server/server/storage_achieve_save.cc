
#include "database_server/server/storage_achieve_save.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_achieve_login_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageAchieveSave::StorageAchieveSave() : async_request_loop_(NULL) {}
StorageAchieveSave::~StorageAchieveSave() {}

bool StorageAchieveSave::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ACHIEVE_SAVE,
      boost::bind(&StorageAchieveSave::Request, this, _1));

  return true;
}

void StorageAchieveSave::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageAchieveSave request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ACHIEVE_SAVE)
    << "(" << request.actor_ << "," << request.achieves_.size() << ",";

  // Active achieve.
  if(request.achieves_.empty() == false) {
    *query << "'" << request.achieves_[0].achieve_id_ << ":" << (int)request.achieves_[0].condition1_param1_
      << ":" << request.achieves_[0].condition1_param2_ << ":" << request.achieves_[0].condition1_value_
      << ":" << (int)request.achieves_[0].condition2_param1_ << ":" << request.achieves_[0].condition2_param2_
      << ":" << request.achieves_[0].condition2_value_ << ":" << request.achieves_[0].finish_time_;
    for(size_t pos = 1; pos < request.achieves_.size(); ++pos) {
      *query << "," << request.achieves_[pos].achieve_id_ << ":" << (int)request.achieves_[pos].condition1_param1_
        << ":" << request.achieves_[pos].condition1_param2_ << ":" << request.achieves_[pos].condition1_value_
        << ":" << (int)request.achieves_[pos].condition2_param1_ << ":" << request.achieves_[pos].condition2_param2_
        << ":" << request.achieves_[pos].condition2_value_ << ":" << request.achieves_[pos].finish_time_;
    }
    *query << "',";
  } else {
    *query << "'',";
  }

  // Finished achieve.
  *query << request.finish_achieves_.size() << ",";

  if(request.finish_achieves_.empty() == false) {
    *query << "'" << request.finish_achieves_[0].achieve_id_ << ":" << request.finish_achieves_[0].finish_time_;
    for(size_t pos = 1; pos < request.finish_achieves_.size(); ++pos) {
      *query << "," << request.finish_achieves_[pos].achieve_id_ << ":" << request.finish_achieves_[0].finish_time_;
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
}

void StorageAchieveSave::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageAchieveSave(), result, request);
}

void StorageAchieveSave::OnResponse(const protocol::StorageAchieveSave &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

