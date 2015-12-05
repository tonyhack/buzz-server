//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-07-08 10:41:04.
// File name: check_actor_exist_handler.cc
//
// Description:
// Define class CheckActorExistHandler.
//

#include "login_server_informal/server/check_actor_exist_handler.h"

#include <mysql++/mysql++.h>

#include "core/event_loop.h"
#include "core/async_logging_singleton.h"
#include "core/base/byte_buffer_dynamic.h"
#include "global/actor_id.h"
#include "global/hash_string.h"
#include "global/logging.h"
#include "global/thrift_packet.h"
#include "login_server_informal/server/http_context.h"
#include "login_server_informal/server/http_response.h"
#include "login_server_informal/server/login_server.h"
#include "login_server_informal/server/mysql_client.h"

namespace login {

namespace server {

CheckActorExistHandler::CheckActorExistHandler() {}
CheckActorExistHandler::~CheckActorExistHandler() {}

bool CheckActorExistHandler::OnRequest(const core::TCPConnectionPtr &connection,
    const char *message, size_t size) {
  global::ThriftPacketPtr thrift =
    LoginServerSingleton::GetInstance().GetThreadPool()->GetPacket(connection->GetLoop());
  if(thrift.get() == NULL) {
    LOG_ERROR("Get thrift failed.");
    return false;
  }

  protocol::MessageCheckActorExistRequest request;
  if (thrift->Deserialize(request, message, size) == false) {
    LOG_ERROR("Deserialize message failed.");
    return false;
  }

  if (request.open_id_.length() <= 0) {
    LOG_ERROR("opend id length <= 0.");
    return false;
  }

  LoginServer::MessageThreadPoolPtr thread_pool =
    LoginServerSingleton::GetInstance().GetThreadPool();
  if (thread_pool.get() == NULL) {
    LOG_ERROR("Get thread pool failed.");
    return false;
  }

  const core::EventLoop *loop = connection->GetLoop();
  MysqlClientPtr mysql_client = thread_pool->GetMysql(loop);
  if (mysql_client.get() == NULL) {
    LOG_ERROR("Get mysql client failed.");
    return false;
  }
  MysqlSessionPtr session = mysql_client->GetCenterSession();
  if (session.get() == NULL) {
    LOG_ERROR("Get mysql session failed.");
    return false;
  }
  if(session->CheckConnectStatus() == false) {
    this->ResponseFailed(connection);
    return false;
  }
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    LOG_ERROR("Get mysql query failed.");
    this->ResponseFailed(connection);
    return false;
  }

  int open_id_hash = global::hash_string(request.open_id_.c_str());
  if(open_id_hash < 0) {
    open_id_hash = 0 - open_id_hash;
  }

  query->escape_string(&request.open_id_);

  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORATE_CHECK_ACTOR_EXIST)
    << "('" << request.open_id_ << "'," << request.zone_id_ << ","
    << open_id_hash << ")";

  LOG_INFO("Call protocol=[%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if (result == NULL) {
    LOG_ERROR("Store get result of null.");
    this->ResponseFailed(connection);
    return false;
  }

  if (query->errnum() > 0) {
    LOG_ERROR("Procedure[%s] execute failed, error=[%s].", query->str().c_str(), query->error());
    if (session->PingConnected() == true) {
      LOG_ERROR("Ping mysql success, error=[%s].", query->error());
      this->ResponseFailed(connection);
      return false;
    } else {
      session->Disconnect();
      session->Reconnect();
      this->ResponseFailed(connection);
      return false;
    }
  }

  if (result->num_rows() != 1 || result->num_fields() != 1) {
    LOG_ERROR("Result's row/fields error.");
    this->ResponseFailed(connection);
    return false;
  }

  if (result->field_name(0) != "@ret_code") {
    LOG_ERROR("Result's field name error.");
    this->ResponseFailed(connection);
  }
  int ret_code = atoi((*result)[0][0].data());

  protocol::MessageCheckActorExistResponse response;
  response.__set_exist_(ret_code != 0);
  this->Response(connection, response);

  return true;
}

void CheckActorExistHandler::ResponseFailed(const core::TCPConnectionPtr &connection) {
  protocol::MessageCheckActorExistResponse response;
  response.__set_exist_(false);
  this->Response(connection, response);
}

void CheckActorExistHandler::Response(const core::TCPConnectionPtr &connection,
    const protocol::MessageCheckActorExistResponse &res) {
  assert(connection);

  global::ThriftPacketPtr thrift =
    LoginServerSingleton::GetInstance().GetThreadPool()->GetPacket(connection->GetLoop());
  if(thrift.get() == NULL) {
    LOG_ERROR("Get thrift failed.");
    return;
  }

  HttpResponse response;
  HttpContext *context = &boost::any_cast<HttpContext &>(connection->GetContext());
  if(context == NULL) {
    global::LogError("%s:%d (%s) Get http context failed.",
        __FILE__, __LINE__, __FUNCTION__);
    response.SetStatusCode(HttpResponse::STATUS_400_BAD_REQUEST);
    response.SetStatusMessage("Unknown error");
    response.SetCloseConnection(true);
  } else {
    size_t size = 0;
    const char *message = thrift->Serialize(res, size);
    if(message == NULL) {
      global::LogError("%s:%d (%s) Serialize message failed.",
          __FILE__, __LINE__, __FUNCTION__);
      response.SetStatusCode(HttpResponse::STATUS_400_BAD_REQUEST);
      response.SetStatusMessage("Unknown error");
      response.SetCloseConnection(true);
    } else {
      protocol::Message message2;
      message2.__set_type_(protocol::MessageType::MESSAGE_CHECK_ACTOR_EXIST_RESPONSE);
      message2.message_.assign(message, size);
      message = thrift->Serialize(message2, size);
      if(message == NULL) {
        global::LogError("%s:%d (%s) Serialize message failed.",
            __FILE__, __LINE__, __FUNCTION__);
        response.SetStatusCode(HttpResponse::STATUS_400_BAD_REQUEST);
        response.SetStatusMessage("Unknown error");
        response.SetCloseConnection(true);
      } else {
        response.SetStatusCode(HttpResponse::STATUS_200_OK);
        response.SetStatusMessage("OK");
        response.SetCloseConnection(false);
        response.SetContentType("thrift-0.9");
        response.AddHeader("Server", "Buzz-LoginServer");
        response.SetBody(message, size);
      }
    }
  }

  core::ByteBufferDynamic buffer(128);
  response.ToBuffer(&buffer);
  connection->SendMessage(buffer.ReadPointer(), buffer.ReadSize());
}

}  // namespace server

}  // namespace login

