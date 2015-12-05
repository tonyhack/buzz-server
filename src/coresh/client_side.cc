//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-31 15:46:40.
// File name: client_side.cc
//
// Description:
// Define class ClientSide.
//

#include "coresh/client_side.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"
#include "coresh/connection_pool.h"

namespace coresh {

ClientSide::ClientSide()
  : network_loop_(NULL), application_loop_(NULL) {}

ClientSide::~ClientSide() {}

bool ClientSide::Initialize(const core::InetAddress &server_address, const std::string &name,
    core::EventLoop *network_loop, core::EventLoop *application_loop,
    const ConnectReadyQueue::ConnectCallback &connect_ready_callback, bool retry) {
  if(network_loop == NULL || application_loop == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Invalid params", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->network_loop_ = network_loop;
  this->application_loop_ = application_loop;

  if(this->client_.Initialize(server_address, name, this->network_loop_, retry) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize tcp client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->client_.SetConnectionCallback(boost::bind(&ClientSide::OnConnection, this, _1));
  this->client_.SetMessageCallback(boost::bind(&ClientSide::OnMessage, this, _1, _2, _3));
  this->client_.SetWriteCallback(boost::bind(&ClientSide::OnWriteComplete, this, _1));

  if(this->connect_ready_queue_.Initialize(this->application_loop_) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize ConnectReadyQueue failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->connect_ready_queue_.SetConnectCallback(connect_ready_callback);

  return true;
}

void ClientSide::Finalize() {
  this->connect_ready_queue_.ResetConnectionCallback();
  this->connect_ready_queue_.Finalize();

  this->client_.Finalize();
}

void ClientSide::Start() {
  this->client_.Start();
}

void ClientSide::Stop() {
  this->client_.Stop();
}

void ClientSide::OnConnection(const core::TCPConnectionPtr &connection) {
  if(connection->CheckConnected()) {
    ConnectionPtr conn = ConnectionPoolSingleton::GetInstance().Allocate();
    if(conn && conn->Initialize(connection, this->application_loop_)) {
      connection->GetContext() = conn;
      this->connect_ready_queue_.OnReceive(conn);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate/Initialize connection failed.",
          __FILE__, __LINE__, __FUNCTION__);
      ConnectionPoolSingleton::GetInstance().Deallocate(conn);
    }
  } else {
    boost::any &context = connection->GetContext();
    if(context.empty() == false) {
      ConnectionPtr conn = boost::any_cast<ConnectionPtr>(context);
      assert(conn);
      conn->NetworkDisconnect();
      connection->ClearContext();
    }
  } 
}

void ClientSide::OnMessage(const core::TCPConnectionPtr &connection,
    core::ByteBufferDynamic &buffer, const core::TimeTick &time_tick) {
  ConnectionPtr conn = boost::any_cast<ConnectionPtr>(connection->GetContext());
  assert(conn);
  conn->ReceiveMessage(connection, buffer);
}

void ClientSide::OnWriteComplete(const core::TCPConnectionPtr &connection) {}

}  // namespace coresh

