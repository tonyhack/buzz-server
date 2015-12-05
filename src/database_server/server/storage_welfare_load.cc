
#include "database_server/server/storage_welfare_load.h"

#include <boost/bind.hpp>
#include <mysql++/mysql++.h>

#include "database_server/protocol/storage_welfare_load_constants.h"
#include "database_server/server/async_request_loop.h"
#include "global/logging.h"

namespace database {

namespace server {

StorageWelfareLoad::StorageWelfareLoad() {}
StorageWelfareLoad::~StorageWelfareLoad() {}

bool StorageWelfareLoad::Initialize(AsyncRequestLoop *async_request_loop) {
  if(async_request_loop == NULL) {
    global::LogError("%s:%d (%s) Parameter async_request_loop is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->async_request_loop_ = async_request_loop;

  // Set request handler.
  this->async_request_loop_->SetMessageHandler(protocol::StorageType::STORAGE_WELFARE_LOAD,
      boost::bind(&StorageWelfareLoad::Request, this, _1));

  return true;
}

void StorageWelfareLoad::Request(const protocol::AsyncRequestMessage &message) {
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
  protocol::StorageWelfareLoadRequest request;
  if(this->async_request_loop_->GetPacket()->Deserialize(request,
        message.message_.data(), message.message_.size()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
    return ;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_WELFARE_LOAD)
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
  protocol::StorageWelfareLoadResponse response;

  if(result->num_rows() > 0) {
    // Check field number.
    if(result->num_fields() != protocol::WelfareField::MAX) {
      global::LogError("%s:%d (%s) Result's fields error.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
      return ;
    }
    int index = 0;
    size_t pos = 0;
    for(; pos < result->num_rows(); ++pos) {
      protocol::StorageWelfareField field;
      core::int32 flag = 0;
      for(size_t i = 0; i < protocol::WelfareField::MAX; ++i) {
        index = result->field_num(protocol::g_storage_welfare_load_constants.kWelfareFieldStr[i]);
        if(index == -1) {
          global::LogError("%s:%d (%s) Get result [%s] failed.", __FILE__, __LINE__,
              __FUNCTION__, protocol::g_storage_welfare_load_constants.kWelfareFieldStr[i].c_str());
          this->ResponseFailed(protocol::ResponseType::ERROR_UNKNOWN, message);
          return ;
        }
        switch(i) {
          case protocol::WelfareField::TITLE_ID:
            response.field_.__set_title_id_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::DAILY_CHECKINS:
            response.field_.__set_daily_checkins_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::DAILY_AWARD:
            response.field_.__set_daily_award_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::GENERAL_CHECKINS:
            response.field_.__set_general_checkins_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::VIP_CHECKINS:
            response.field_.__set_vip_checkins_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::LAST_MONTH_RESOURCE:
            response.field_.__set_last_month_resource_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::CURRENT_MONTH_RESOURCE:
            response.field_.__set_current_month_resource_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::LAST_MONTH_AWARD_RESOURCE:
            response.field_.__set_last_month_award_resource_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::DAILY_TIME:
            response.field_.__set_daily_time_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::TOTAL_TIME:
            response.field_.__set_total_time_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::REPLENISH_CHECKINS_NUM:
            response.field_.__set_replenish_checkins_num_(atoi((*result)[pos][i].data()));
            break;
          case protocol::WelfareField::ONLINE_AWARD_ITEMS:
            response.field_.online_award_items_ = (*result)[pos][i].data();
            break;
          case protocol::WelfareField::RESPENISH_DAYS:
            response.field_.respenish_days_ = (*result)[pos][i].data();
            break;
          default:
            break;
        }
      }
    }
  }
  this->OnResponse(response, protocol::ResponseType::SUCCESS, message);
}

void StorageWelfareLoad::ResponseFailed(protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  this->OnResponse(protocol::StorageWelfareLoadResponse(), result, request);
}

void StorageWelfareLoad::OnResponse(const protocol::StorageWelfareLoadResponse &response,
    protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request) {
  this->async_request_loop_->SendResponse(response, result, request);
}

}  // namespace server

}  // namespace database

