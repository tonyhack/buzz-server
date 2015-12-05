//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-28 14:15:59.
// File name: network_service.cc
//
// Description:
// Define class NetworkService.
//

#include "coresh/network_service.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"
#include "coresh/connection.h"
#include "coresh/connection_pool.h"
#include "coresh/packetizer.h"
#include "coresh/packetizer_table.h"
#include "coresh/reusable_buffers.h"
#include "coresh/variable_table.h"

namespace coresh {

NetworkService::NetworkService(const core::InetAddress &address,
    const std::string &name) : server_(address, name),
  application_loop_(NULL) {}

NetworkService::~NetworkService() {}

bool NetworkService::Initialize(size_t thread_number, core::EventLoop *application_loop,
    const ConnectReadyQueue::ConnectCallback &connect_ready_callback) {
  assert(connect_ready_callback);

  this->thread_pool_.reset(new (std::nothrow) core::EventLoopThreadGroup());
  if(this->thread_pool_ == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate EventLoopThreadGroup failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->thread_pool_->Initialize(thread_number, "NetworkServiceThread",
        true) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize EventLoopThreadGroup failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  core::EventLoop *loop = this->thread_pool_->GetNextLoop();
  if(this->server_.Initialize(loop, this->thread_pool_.get()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize TCPServer failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->server_.SetConnectionCallback(boost::bind(&NetworkService::OnConnection,
        this, _1));
  this->server_.SetMessageCallback(boost::bind(&NetworkService::OnMessage,
        this, _1, _2, _3));
  this->server_.SetWriteCallback(boost::bind(&NetworkService::OnWriteComplete,
        this, _1));

  this->application_loop_ = application_loop;
  if(this->connect_ready_queue_.Initialize(this->application_loop_) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize ConnectReadyQueue failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->connect_ready_queue_.SetConnectCallback(connect_ready_callback);

  // Allocate reusable buffers.
  const core::EventLoopThreadGroup::LoopGroup *loops = this->thread_pool_->GetLoopGroup();
  core::EventLoopThreadGroup::LoopGroup::const_iterator iterator = loops->begin();
  for(; iterator != loops->end(); ++iterator) {
    if(ReusableBuffersSingleton::GetInstance().Allocate(*iterator) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate reusable buffers failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
  }

  // Allocate packetizer.
  iterator = loops->begin();
  for(; iterator != loops->end(); ++iterator) {
    Packetizer *packetizer = new (std::nothrow) Packetizer();
    if(packetizer == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate packetizer failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    packetizer->SetCompressor(VariableTableSingleton::GetInstance().GetCompressType());
    packetizer->SetEncoder(VariableTableSingleton::GetInstance().GetEncodeType());
    if(PacketizerTableSingleton::GetInstance().Insert(*iterator, packetizer) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Insert packetizer table failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
  }

  return true;
}

void NetworkService::Finalize() {
  // Deallocate packetizer.
  const core::EventLoopThreadGroup::LoopGroup *loops = this->thread_pool_->GetLoopGroup();
  core::EventLoopThreadGroup::LoopGroup::const_iterator iterator = loops->begin();
  for(; iterator != loops->end(); ++iterator) {
    Packetizer *packetizer = PacketizerTableSingleton::GetInstance().Remove(*iterator);
    if(packetizer == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deallocate packetizer failed.",
          __FILE__, __LINE__, __FUNCTION__);
    } else {
      delete packetizer;
    }
  }

  // Deallocate reusable buffers.
  iterator = loops->begin();
  for(; iterator != loops->end(); ++iterator) {
    ReusableBuffersSingleton::GetInstance().Deallocate(*iterator);
  }

  this->thread_pool_->Finalize();
  this->connect_ready_queue_.Finalize();
}

void NetworkService::Start() {
  this->thread_pool_->Start();
  this->server_.Start();
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Network service started.");
}

void NetworkService::Stop() {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Stop network service.");
  this->server_.Stop();
  this->thread_pool_->Stop();
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Network service stopped.");
}

void NetworkService::OnConnection(const core::TCPConnectionPtr &connection) {
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
      connection->Shutdown();
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

void NetworkService::OnMessage(const core::TCPConnectionPtr &connection,
    core::ByteBufferDynamic &buffer, const core::TimeTick &time_tick) {
  ConnectionPtr conn = boost::any_cast<ConnectionPtr>(connection->GetContext());
  assert(conn);
  conn->ReceiveMessage(connection, buffer);
}

void NetworkService::OnWriteComplete(const core::TCPConnectionPtr &connection) {
}

}  // namespace coresh

