
#include "database_server/server/storage_achieve_login.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_achieve_login_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageAchieveLogin::StorageAchieveLogin() {}
StorageAchieveLogin::~StorageAchieveLogin() {}

bool StorageAchieveLogin::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_ACHIEVE_LOGIN,
      boost::bind(&StorageAchieveLogin::Request, this, _1));

  return true;
}

void StorageAchieveLogin::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageAchieveLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ACHIEVE_LOGIN)
    << "(" << request.actor_ << ")";

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
  protocol::StorageAchieveLoginResponse response;

  if(result->num_rows() > 0) {
    // Check field number.
    if(result->num_fields() != protocol::AchieveField::MAX) {
      global::LogError("%s:%d (%s) Result's fields error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    int index = 0;
    size_t pos = 0;
    for(; pos < result->num_rows(); ++pos) {
      protocol::StorageAchieveField field;
      protocol::StorageAchieveFinishField finish_field;
      core::int32 flag = 0;
      for(size_t i = 0; i < protocol::AchieveField::MAX; ++i) {
        index = result->field_num(protocol::g_storage_achieve_login_constants.kAchieveFieldStr[i]);
        if(index == -1) {
          global::LogError("%s:%d (%s) Get result [%s] failed.", __FILE__, __LINE__,
              __FUNCTION__, protocol::g_storage_achieve_login_constants.kAchieveFieldStr[i].c_str());
          this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
          return ;
        }
        switch(i) {
          case protocol::AchieveField::ACHIEVE_ID:
            field.__set_achieve_id_(atoi((*result)[pos][i].data()));
            break;
          case protocol::AchieveField::FLAG:
            flag = atoi((*result)[pos][i].data());
            break;
          case protocol::AchieveField::CONDITION1_PARAM1:
            field.__set_condition1_param1_(atoi((*result)[pos][i].data()));
            break;
          case protocol::AchieveField::CONDITION1_PARAM2:
            field.__set_condition1_param2_(atoi((*result)[pos][i].data()));
            break;
          case protocol::AchieveField::CONDITION1_VALUE:
            field.__set_condition1_value_(atoi((*result)[pos][i].data()));
            break;
          case protocol::AchieveField::CONDITION2_PARAM1:
            field.__set_condition2_param1_(atoi((*result)[pos][i].data()));
            break;
          case protocol::AchieveField::CONDITION2_PARAM2:
            field.__set_condition2_param2_(atoi((*result)[pos][i].data()));
            break;
          case protocol::AchieveField::CONDITION2_VALUE:
            field.__set_condition2_value_(atoi((*result)[pos][i].data()));
            break;
          case protocol::AchieveField::FINISH_TIME:
            field.__set_finish_time_(atoi((*result)[pos][i].data()));
            break;
          default:
            break;
        }
      }
      if(flag == protocol::AchieveFlag::ACTIVE) {
        response.achieves_.push_back(field);
        response.__set_result_(protocol::AchieveLoginResult::SUCCESS);
      } else if(flag == protocol::TaskFlag::FINISHED) {
        finish_field.__set_finish_time_(field.finish_time_);
        finish_field.__set_achieve_id_(field.achieve_id_);
        response.finish_achieves_.push_back(finish_field);
      }
    }
  }
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageAchieveLogin::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageAchieveLoginResponse(), result, request);
}

void StorageAchieveLogin::OnResponse(const protocol::StorageAchieveLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

