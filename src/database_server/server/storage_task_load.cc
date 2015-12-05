//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-15 14:39:20.
// File name: storage_task_load.cc
//
// Description:
// Define class StorageTaskLoad.
//

#include "database_server/server/storage_task_load.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_task_login_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageTaskLoad::StorageTaskLoad() {}
StorageTaskLoad::~StorageTaskLoad() {}

bool StorageTaskLoad::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_TASK_LOGIN,
      boost::bind(&StorageTaskLoad::Request, this, _1));

  return true;
}

void StorageTaskLoad::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageTaskLoginRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_TASK_LOGIN)
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
  protocol::StorageTaskLoginResponse response;

  if(result->num_rows() > 0) {
    // Check field number.
    if(result->num_fields() != protocol::TaskField::MAX) {
      global::LogError("%s:%d (%s) Result's fields error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    int index = 0;
    size_t pos = 0;
    for(; pos < result->num_rows(); ++pos) {
      protocol::StorageTaskField field;
      for(size_t i = 0; i < protocol::TaskField::MAX; ++i) {
        index = result->field_num(protocol::g_storage_task_login_constants.kTaskFieldStr[i]);
        if(index == -1) {
          global::LogError("%s:%d (%s) Get result [%s] failed.", __FILE__, __LINE__,
              __FUNCTION__, protocol::g_storage_task_login_constants.kTaskFieldStr[i].c_str());
          this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
          return ;
        }
        switch(i) {
          case protocol::TaskField::TASK:
            field.__set_task_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::FLAG:
            field.__set_flag_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::COND_TYPE1:
            field.__set_cond_type1_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::VAR1:
            field.__set_var1_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::VALUE1:
            field.__set_value1_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::COND_TYPE2:
            field.__set_cond_type2_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::VAR2:
            field.__set_var2_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::VALUE2:
            field.__set_value2_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::COND_TYPE3:
            field.__set_cond_type3_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::VAR3:
            field.__set_var3_(atoi((*result)[pos][i].data()));
            break;
          case protocol::TaskField::VALUE3:
            field.__set_value3_(atoi((*result)[pos][i].data()));
          case protocol::TaskField::TASK_TYPE:
            field.__set_task_type_(atoi((*result)[pos][i].data()));
            break;
          default:
            break;
        }
      }
      if(field.flag_ == protocol::TaskFlag::ACTIVE) {
        response.actives_.push_back(field);
        response.__set_result_(protocol::TaskLoginResult::SUCCESS);
      } else if(field.flag_ == protocol::TaskFlag::FINISHED) {
        protocol::StorageTaskFinish finish_task;
        finish_task.__set_type_(field.task_type_);
        finish_task.__set_task_id_(field.task_);
        response.finishes_.push_back(finish_task);
      }
    }
  }
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageTaskLoad::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageTaskLoginResponse(), result, request);
}

void StorageTaskLoad::OnResponse(const protocol::StorageTaskLoginResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

