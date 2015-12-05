//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-06 14:59:14.
// File name: actor_login_handler.cc
//
// Description:
// Define class ActorLoginHandler.
//

#include "login_server_informal/server/actor_login_handler.h"

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

ActorLoginHandler::ActorLoginHandler() {}
ActorLoginHandler::~ActorLoginHandler() {}

bool ActorLoginHandler::OnRequest(const core::TCPConnectionPtr &connection,
    const char *message, size_t size) {
  global::ThriftPacketPtr thrift =
    LoginServerSingleton::GetInstance().GetThreadPool()->GetPacket(connection->GetLoop());
  if(thrift.get() == NULL) {
    LOG_ERROR("Get thrift failed.");
    return false;
  }

  protocol::MessageActorLoginRequest request;
  if(thrift->Deserialize(request, message ,size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(request.open_id_.length() <= 0) {
    global::LogError("%s:%d (%s) Open id length < 0.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  LoginServer::MessageThreadPoolPtr thread_pool =
    LoginServerSingleton::GetInstance().GetThreadPool();
  if(thread_pool.get() == NULL) {
    global::LogError("%s:%d (%s) Get thread pool failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  const core::EventLoop *loop = connection->GetLoop();
  MysqlClientPtr mysql_client = thread_pool->GetMysql(loop);
  if(mysql_client.get() == NULL) {
    global::LogError("%s:%d (%s) Get mysql client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  MysqlSessionPtr session = mysql_client->GetCenterSession();
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get mysql session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // Check connection status.
  if(session->CheckConnectStatus() == false) {
    this->ResponseFailed(connection);
    return false;
  }

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get mysql query failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection);
    return false;
  }

  int open_id_hash = global::hash_string(request.open_id_.c_str());
  if(open_id_hash < 0) {
    open_id_hash = 0 - open_id_hash;
  }

  std::string game_session = "dreamfire_session_key";

  query->escape_string(&request.open_id_);

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_ACTOR_LOGIN)
    << "('" << request.open_id_ << "', " << request.zone_id_ << ", "
    << open_id_hash << ", '" << game_session << "', " << 2 << ")";

  core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_DEBUG,
      "Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if(result == NULL) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Store get result of null.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection);
    return false;
  }

  // Check any errors.
  if(query->errnum() > 0) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Procedure[%s] execute failed, error=[%s].", __FILE__, __LINE__,
        __FUNCTION__, query->str().c_str(), query->error());
    if(session->PingConnected() == true) {
      core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Ping mysql is ok, but error=[%s].",
          __FILE__, __LINE__, __FUNCTION__, query->error());
      this->ResponseFailed(connection);
      return false;
    } else {
      // Disconnect and reconnect.
      session->Disconnect();
      session->Reconnect();
      this->ResponseFailed(connection);
      return false;
    }
  }

  // Check result's valid.
  if(result->num_rows() != 1 || result->num_fields() != 4) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's row/fields error.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection);
    return false;
  }

  if(result->field_name(0) != "@ret_code" ||
      result->field_name(1) != "@ret_table_number" ||
      result->field_name(2) != "@ret_id" ||
      result->field_name(3) != "@ret_name") {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's field name error.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection);
    return false;
  }
  int ret_code = atoi((*result)[0][0].data());
  if(ret_code != 0) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Actor[%s] login failed.",
        __FILE__, __LINE__, __FUNCTION__, request.open_id_.c_str());
    this->ResponseFailed(connection);
    return false;
  }

  // Generate actor id;
  global::ActorID actor_id(request.zone_id_, atoi((*result)[0][1].data()),
      atoi((*result)[0][2].data()));
  char id[32];
  snprintf(id, sizeof(id), "%lu", actor_id.GetID());

  // Response.
  protocol::MessageActorLoginResponse response;
  response.__set_success_(true);
  response.__set_actor_id_(id);
  response.__set_game_session_(game_session);
  response.__set_actor_name_((*result)[0][3].data());
  response.__set_gateway_host_(ServerConfigure::GetInstance()->GetGatewayInfo()->ip_);
  response.__set_gateway_port_(ServerConfigure::GetInstance()->GetGatewayInfo()->port_);
  this->Response(connection, response);

  return true;
}

void ActorLoginHandler::ResponseFailed(const core::TCPConnectionPtr &connection) {
  protocol::MessageActorLoginResponse response;
  response.__set_success_(false);
  this->Response(connection, response);
}

void ActorLoginHandler::Response(const core::TCPConnectionPtr &connection,
    const protocol::MessageActorLoginResponse &res) {
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
      message2.__set_type_(protocol::MessageType::MESSAGE_ACTOR_LOGIN_RESPONSE);
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

