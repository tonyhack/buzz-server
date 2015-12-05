//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 17:01:48.
// File name: create_actor_handler.cc
//
// Description:
// Define class CreateActorHandler.
//

#include "login_server_informal/server/create_actor_handler.h"

#include <cstdio>
#include <mysql++/mysql++.h>

#include "core/event_loop.h"
#include "core/async_logging_singleton.h"
#include "core/base/byte_buffer_dynamic.h"
#include "global/actor_id.h"
#include "global/common_functions.h"
#include "global/hash_string.h"
#include "global/logging.h"
#include "global/thrift_packet.h"
#include "login_server_informal/server/http_context.h"
#include "login_server_informal/server/http_response.h"
#include "login_server_informal/server/login_server.h"

namespace login {

namespace server {

CreateActorHandler::CreateActorHandler() {}
CreateActorHandler::~CreateActorHandler() {}

bool CreateActorHandler::OnRequest(const core::TCPConnectionPtr &connection,
    const char *message, size_t size) {
  global::ThriftPacketPtr thrift =
    LoginServerSingleton::GetInstance().GetThreadPool()->GetPacket(connection->GetLoop());
  if(thrift.get() == NULL) {
    LOG_ERROR("Get thrift failed.");
    return false;
  }

  protocol::MessageCreateActorRequest request;
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
    this->ResponseFailed(connection, protocol::CreateActorType::FAILED_UNKNOWN);
    return false;
  }

  ssize_t visual_length = global::GetStrVisualLengthUTF8(request.actor_name_.c_str());
  if(visual_length < 4 || visual_length > 14) {
    global::LogError("%s:%d (%s) Actor name length is invalid.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection, protocol::CreateActorType::FAILED_NAME_LENGTH);
    return false;
  }

  // Word filter.
  if(LoginServerSingleton::GetInstance().GetWordFilter().Search(request.actor_name_) == true) {
    global::LogError("%s:%d (%s) Actor name is invalid.",
        __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection, protocol::CreateActorType::FAILED_NAME_INVALID);
    return false;
  }

  // Get real actor name.
  char buffer[256] = "";
  snprintf(buffer, sizeof(buffer), "[%d区]%s", request.zone_id_, request.actor_name_.c_str());
  request.actor_name_ = buffer;

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get mysql query failed.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection, protocol::CreateActorType::FAILED_UNKNOWN);
    return false;
  }

  int open_id_hash = global::hash_string(request.open_id_.c_str());
  if(open_id_hash < 0) {
    open_id_hash = 0 - open_id_hash;
  }  

  std::string game_session = "dreamfire_session_key";

  query->escape_string(&request.actor_name_);
  query->escape_string(&request.open_id_);

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_CREATE_ACTOR)
    << "('" << request.open_id_ << "', " << request.zone_id_ << ", "
    << open_id_hash << ", '" << request.actor_name_ << "', '" << game_session << "')";

  core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_DEBUG,
      "Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if(result == NULL) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Store get result of null.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection, protocol::CreateActorType::FAILED_UNKNOWN);
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
      this->ResponseFailed(connection, protocol::CreateActorType::FAILED_UNKNOWN);
      return false;
    } else {
      // Disconnect and reconnect.
      session->Disconnect();
      session->Reconnect();
      this->ResponseFailed(connection, protocol::CreateActorType::FAILED_UNKNOWN);
      return false;
    }
  }

  // Check result's valid.
  if(result->num_rows() != 1 || result->num_fields() != 3) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's row/fields error.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection, protocol::CreateActorType::FAILED_UNKNOWN);
    return false;
  }
  if(result->field_name(0) != "@ret_code" ||
      result->field_name(1) != "@ret_table_id" ||
      result->field_name(2) != "@ret_id") {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's field name error.", __FILE__, __LINE__, __FUNCTION__);
    this->ResponseFailed(connection, protocol::CreateActorType::FAILED_UNKNOWN);
    return false;
  }

  int ret_code = atoi((*result)[0][0].data());
  if(ret_code != 0) {
    if(ret_code == -1) {
      this->ResponseFailed(connection, protocol::CreateActorType::FAILED_NAME_EXIST);
      return false;
    } else if(ret_code == -2) {
      this->ResponseFailed(connection, protocol::CreateActorType::FAILED_ACTOR_EXIST);
      return false;
    } else {
      this->ResponseFailed(connection, protocol::CreateActorType::FAILED_UNKNOWN);
      return false;
    }
  }

  // Generate actor id;
  global::ActorID actor_id(request.zone_id_, atoi((*result)[0][1].data()),
      atoi((*result)[0][2].data()));

  // Create actor to game database.
  if(this->CreateGameActor(mysql_client, actor_id.GetID(), request.actor_name_,
        request.hair_, request.male_, request.vocation_) == false) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Create game actor[open_id=%s, zone_id=%d, actor_name=%s] failed.",
        __FILE__, __LINE__, __FUNCTION__, request.open_id_.c_str(),
        request.zone_id_, request.actor_name_.c_str());
    this->ResponseFailed(connection, protocol::CreateActorType::FAILED_UNKNOWN);
  }

  protocol::MessageCreateActorResponse response;
  response.__set_type_(protocol::CreateActorType::SUCCESS);
  this->Response(connection, response);
  return true;

}

bool CreateActorHandler::CreateGameActor(const MysqlClientPtr &mysql_client,
    core::uint64 id, const std::string name, core::int32 hair, bool male, core::int32 vocation) {
  // Get session.
  MysqlSessionPtr session = mysql_client->GetSession(id);
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Get mysql session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Check connection status.
  if(session->CheckConnectStatus() == false) {
    global::LogError("%s:%d (%s) Mysql session connection error.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  } 

  // Reset and get query.
  session->ResetQuery();
  mysqlpp::Query *query = session->GetQuery();
  if(query == NULL) {
    global::LogError("%s:%d (%s) Get mysql query failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Execute procedure.
  *query << "CALL " << session->GetProcedure(protocol::StorageType::STORAGE_GAME_ACTOR_CREATE)
    << "(" << id << ", '" << name << "', " << hair << ", "
    << (male ? 1 : 2) << ", " << vocation << ")";

  global::LogDebug("Call procedure = [%s]", query->str().c_str());

  const mysqlpp::StoreQueryResult *result = session->Store();
  if(result == NULL) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Check any errors. 
  if(query->errnum() > 0) {
    global::LogError("%s:%d (%s) Procedure[%s] execute failed, error=[%s].",
        __FILE__, __LINE__, __FUNCTION__, query->str().c_str(), query->error());
    if(session->PingConnected() == true) {
      global::LogError("%s:%d (%s) Ping mysql is ok, but error=[%s].",
          __FILE__, __LINE__, __FUNCTION__, query->error());
      return false;
    } else {
      // Disconnect and reconnect.
      session->Disconnect();
      session->Reconnect();
      return false;
    }
  }

  // Check result's valid.
  if(result->num_rows() != 1 || result->num_fields() != 1) {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's row/fields error.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(result->field_name(0) != "@ret_code") {
    core::AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Result's field name error.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return atoi((*result)[0][0].data()) == 0;
}

void CreateActorHandler::ResponseFailed(const core::TCPConnectionPtr &connection,
    protocol::CreateActorType::type type) {
  protocol::MessageCreateActorResponse response;
  response.__set_type_(type);
  this->Response(connection, response);
}

void CreateActorHandler::Response(const core::TCPConnectionPtr &connection,
      const protocol::MessageCreateActorResponse &res) {
  assert(connection);

  global::ThriftPacketPtr thrift =
    LoginServerSingleton::GetInstance().GetThreadPool()->GetPacket(connection->GetLoop());
  if(thrift.get() == NULL) {
    LOG_ERROR("Get thrift failed.");
    return ;
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
      message2.__set_type_(protocol::MessageType::MESSAGE_CREATE_ACTOR_RESPONSE);
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

