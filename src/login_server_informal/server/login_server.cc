//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 14:26:48.
// File name: login_server.cc
//
// Description:
// Define class LoginServer.
//

#include "login_server_informal/server/login_server.h"

#include <boost/bind.hpp>

#include "core/event_loop.h"
#include "core/base/byte_buffer_dynamic.h"
#include "core/base/logging.h"
#include "global/logging.h"
#include "global/thrift_packet.h"
#include "login_server_informal/server/http_context.h"
#include "login_server_informal/server/http_response.h"
#include "login_server_informal/server/server_configure.h"

namespace login {

namespace server {

const std::string kFlashPolicyStr = "<?xml version=\"1.0\"?><!DOCTYPE cross-domain-policy SYSTEM \"/xml/dtds/cross-domain-policy.dtd\"><cross-domain-policy>    <allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>";

LoginServer *LoginServerSingleton::instance_ = NULL;

LoginServer::LoginServer(const core::InetAddress &address,
    const std::string &name) : server_(address, name) {}
LoginServer::~LoginServer() {}

bool LoginServer::Initialize() {
  LoginServerSingleton::SetInstance(this);

  this->thread_pool_.reset(new (std::nothrow) MessageThreadPool());

  if(this->thread_pool_.get() == NULL) {
    global::LogError("%s:%d (%s) Allocate thread pool error.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->thread_pool_->Initialize(2, "LoginServerThreadPool") == false) {
    global::LogError("%s:%d (%s) Initialize thread pool error.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->server_.Initialize(this->thread_pool_->GetNextLoop(),
        this->thread_pool_->GetEventLoopThreadGroup()) == false) {
    global::LogError("%s:%d (%s) Initialize tcp server error.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->http_service_handler_.SetHttpCallback(boost::bind(
        &LoginServer::OnHttpRequest, this, _1));

  this->server_.SetConnectionCallback(boost::bind(&HttpServiceHandler::OnConnection,
        &this->http_service_handler_, _1));
  this->server_.SetMessageCallback(boost::bind(&HttpServiceHandler::OnMessage,
        &this->http_service_handler_, _1, _2, _3));
  this->server_.SetWriteCallback(boost::bind(&HttpServiceHandler::OnWriteComplete,
        &this->http_service_handler_, _1));

  if(this->word_filter_.Initialize(ServerConfigure::GetInstance()->GetDirtyWordFile()) == false) {
    global::LogError("%s:%d (%s) Initialize word filter error.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  global::LogInfo("Login server initialize complete.");

  return true;
}
void LoginServer::Finalize() {}

void LoginServer::Start() {
  this->thread_pool_->Start();
  this->server_.Start();
  global::LogInfo("Login server start complete.");
}

void LoginServer::Stop() {
  this->server_.Stop();
  this->thread_pool_->Stop();
  global::LogInfo("Login server stopped.");
  core::AsyncLoggingSingleton::GetAsyncLogging()->Stop();
}

bool LoginServer::OnHttpRequest(const core::TCPConnectionPtr &connection) {
  global::LogInfo("Receive request from http client.");
  HttpContext *context = &boost::any_cast<HttpContext &>(connection->GetContext());
  if(context == NULL) {
    global::LogError("%s:%d (%s) Get http context failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // Flash http policyd.
  const HttpRequest &request = context->GetRequest();
  if(request.GetMethod() == HttpRequest::GET) {
    if(request.GetPath() == "/crossdomain.xml") {
      HttpResponse response;

      response.SetStatusCode(HttpResponse::STATUS_200_OK);
      response.SetStatusMessage("OK");
      response.SetCloseConnection(false);
      response.SetContentType("text/xml");
      response.AddHeader("Server", "Buzz-LoginServer");
      response.SetBody(kFlashPolicyStr);

      core::ByteBufferDynamic buffer(5120);
      response.ToBuffer(&buffer);
      connection->SendMessage(buffer.ReadPointer(), buffer.ReadSize());

      return true;
    }
  }
  const core::EventLoop *loop = connection->GetLoop();
  global::ThriftPacketPtr thrift = this->thread_pool_->GetPacket(loop);
  if(thrift.get() == NULL) {
    global::LogError("%s:%d (%s) Get thrift packet failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  protocol::Message message;
  if(thrift->Deserialize(message, context->GetRequest().GetContent()) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  switch(message.type_) {
    case protocol::MessageType::MESSAGE_CREATE_ACTOR_REQUEST:
      return this->create_actor_handler_.OnRequest(connection,
          message.message_.data(), message.message_.size());
      break;
    case protocol::MessageType::MESSAGE_ACTOR_LOGIN_REQUEST:
      return this->actor_login_handler_.OnRequest(connection,
          message.message_.data(), message.message_.size());
      break;
    case protocol::MessageType::MESSAGE_CHECK_ACTOR_EXIST_REQUEST:
      return this->check_actor_exist_handler_.OnRequest(connection,
          message.message_.data(), message.message_.size());
      break;
    default:
      global::LogError("%s:%d (%s) Bad request type[%d].",
          __FILE__, __LINE__, __FUNCTION__, message.type_);
      break;
  }

  return false;
}

}  // namespace server

}  // namespace login

