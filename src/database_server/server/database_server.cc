//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-14 15:40:40.
// File name: database_server.cc
//
// Description:
// Define class DatabaseServer.
//

#include "database_server/server/database_server.h"

#include <boost/bind.hpp>

#include "core/async_logging.h"
#include "core/async_logging_singleton.h"
#include "coresh/packetizer_table.h"
#include "coresh/protocol_format.h"
#include "coresh/variable_table.h"
#include "global/server_configure.h"

namespace database {

namespace server {

DatabaseServer *DatabaseServerSingleton::instance_ = NULL;

DatabaseServer::DatabaseServer(const core::InetAddress &address,
    const std::string &name) : server_id_(0), server_(address, name) {}

DatabaseServer::~DatabaseServer() {}

bool DatabaseServer::Initialize(core::uint32 server_id) {
  DatabaseServerSingleton::SetInstance(this);

  this->server_id_ = server_id;

  // Get server configure.
  const global::DatabaseServerInformation *configure =
    global::ServerConfigureSingleton::GetInstance().GetDatabaseServers(server_id);
  if(configure == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get database server[%d] configure failed.",
        __FILE__, __LINE__, __FUNCTION__, server_id);
    return false;
  }

  // Load coresh configure.
  if(global::ServerConfigureSingleton::GetInstance().LoadCoreshFile(
        configure->coresh_.c_str()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Load coresh configure file [%s] failed.",
        __FILE__, __LINE__, __FUNCTION__, configure->coresh_.c_str());
    return false;
  }

  const global::ServerInformation *schedule_configure = 
    global::ServerConfigureSingleton::GetInstance().GetScheduleServer();
  if(schedule_configure == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get schedule server configure failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Allocate and initialize thread pool.
  this->thread_pool_.reset(new (std::nothrow) core::EventLoopThreadGroup());
  if(this->thread_pool_ == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate EventLoopThreadGroup failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->thread_pool_->Initialize(coresh::VariableTableSingleton::GetInstance().GetNetworkThreadNumber(),
        "DatabaseServerThread", true) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize EventLoopThreadGroup failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Allocate thrift packet.
  const core::EventLoopThreadGroup::LoopGroup *loops = this->thread_pool_->GetLoopGroup();
  core::EventLoopThreadGroup::LoopGroup::const_iterator iterator = loops->begin();
  for(; iterator != loops->end(); ++iterator) {
    global::ThriftPacket *packet = new (std::nothrow) global::ThriftPacket();
    if(packet == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate thrift packet failed.", __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(this->packets_.Insert(*iterator, packet) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Insert into multiplex thrift packets failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
  }

  // Allocate Packetizer.
  iterator = loops->begin();
  for(; iterator != loops->end(); ++iterator) {
    coresh::Packetizer *packetizer = new (std::nothrow) coresh::Packetizer();
    if(packetizer == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate packetizer failed.", __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    packetizer->SetCompressor(coresh::VariableTableSingleton::GetInstance().GetCompressType());
    packetizer->SetEncoder(coresh::VariableTableSingleton::GetInstance().GetEncodeType());
    if(coresh::PacketizerTableSingleton::GetInstance().Insert(*iterator, packetizer) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Insert packetizer table failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
  }

  // Get schedule loop and thrift packet.
  core::EventLoop *schedule_loop = this->thread_pool_->GetNextLoop();
  if(schedule_loop == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get event loop failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  global::ThriftPacket *packet = this->packets_.GetThriftPacket(schedule_loop);
  if(packet == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get thrift packet failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  global::protocol::ServerSignature signature;
  this->server_.GetAddress().GetAddressInfo(signature.host_, (core::uint16 &)signature.port_);
  signature.__set_type_(global::protocol::ServerType::DATABASE_SERVER);
  signature.__set_id_(this->server_id_);

  // Allocate and initialize schedule client.
  this->schedule_client_.reset(new (std::nothrow) schedule::client::ScheduleClient2());
  if(this->schedule_client_.get() == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate ScheduleClient2 failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;

  }
  if(this->schedule_client_->Initialize(core::InetAddress(schedule_configure->ip_,
          schedule_configure->port_), "ScheduleClient2", schedule_loop,
        signature, packet) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize ScheduleClient2 failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->schedule_client_->SetJoinScheduleCallback(
      boost::bind(&DatabaseServer::OnServerJoinSchedule, this, _1));
  this->schedule_client_->SetDisjoinScheduleCallback(
      boost::bind(&DatabaseServer::OnServerDisjoinSchedule, this, _1));

  // Initialize server.
  if(this->server_.Initialize(this->thread_pool_->GetNextLoop(),
        this->thread_pool_.get()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize server failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->server_.SetConnectionCallback(boost::bind(&DatabaseServer::OnConnection, this, _1));
  this->server_.SetMessageCallback(boost::bind(&DatabaseServer::OnMessage, this, _1, _2, _3));
  this->server_.SetWriteCallback(boost::bind(&DatabaseServer::OnWriteComplete, this, _1));

  if(this->async_request_service_.Initialize(configure->request_queue_size_,
        configure->request_queue_reduce_waterline_, configure->db_thread_number_,
        coresh::VariableTableSingleton::GetInstance().GetCompressType(),
        coresh::VariableTableSingleton::GetInstance().GetEncodeType()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize async request service failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }


  return true;
}

void DatabaseServer::Finalize() {
  this->schedule_client_->Finalize();
  this->schedule_client_.reset();

  const core::EventLoopThreadGroup::LoopGroup *loops = this->thread_pool_->GetLoopGroup();

  // Deallocate Packetizer.
  core::EventLoopThreadGroup::LoopGroup::const_iterator iterator = loops->begin();
  for(; iterator != loops->end(); ++iterator) {
    coresh::Packetizer *packetizer =
      coresh::PacketizerTableSingleton::GetInstance().Remove(*iterator);
    if(packetizer == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deallocate packetizer failed.", __FILE__, __LINE__, __FUNCTION__);
    } else {
      delete packetizer;
    }
  }

  // Deallocate thrift packet.
  iterator = loops->begin();
  for(; iterator != loops->end(); ++iterator) {
    global::ThriftPacket *packet = this->packets_.Remove(*iterator);
    if(packet == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deallocate thrift packet failed.", __FILE__, __LINE__, __FUNCTION__);
    } else {
      delete packet;
    }
  }

  this->thread_pool_->Finalize();
  this->thread_pool_.reset();
  AsyncLoggingSingleton::GetAsyncLogging()->Stop();
}

bool DatabaseServer::Start() {
  this->thread_pool_->Start();
  this->server_.Start();
  if(this->async_request_service_.Start() == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "Async request service start failed.");
    return false;
  }
  this->schedule_client_->Start();
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Database server start complete.");

  return true;
}

void DatabaseServer::Stop() {
  this->schedule_client_->Stop();
  this->async_request_service_.Stop();
  this->server_.Stop();
  this->thread_pool_->Stop();
}

void DatabaseServer::OnConnection(const core::TCPConnectionPtr &connection) {
  if(connection->CheckConnected()) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Connection established.");
    DatabaseTerminalPtr terminal(new (std::nothrow) DatabaseTerminal());
    if(terminal && terminal->Initialize(connection)) {
      connection->GetContext() = terminal;
    } else {
      connection->Shutdown();
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate/Initialize database terminal failed.",
          __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Connection disconnected.");
    DatabaseTerminalPtr terminal = boost::any_cast<DatabaseTerminalPtr>(connection->GetContext());
    if(terminal) {
      terminal->OnDisconnected();
      connection->ClearContext();
    }
  }
}

void DatabaseServer::OnMessage(const core::TCPConnectionPtr &connection,
    core::ByteBufferDynamic &buffer, const core::TimeTick &time_tick) {
  boost::any &context = connection->GetContext();
  if(context.empty() == false){
    DatabaseTerminalPtr terminal = boost::any_cast<DatabaseTerminalPtr>(context);
    assert(terminal);
    coresh::Packetizer *packetizer = coresh::PacketizerTableSingleton::GetInstance().GetPacketizer(
        connection->GetLoop());
    if(packetizer) {
      size_t read_size = 0, message_size = 0, unpacket_size = 0;
      int retcode = 0;
      const coresh::ProtocolHead *protocol_head = NULL;
      while(true) {
        read_size = buffer.ReadSize();
        // Can read a message head.
        if(read_size < coresh::kProtocolHeadSize) {
          break;
        }
        message_size = coresh::GetProtocolSize(buffer.ReadPointer());
        // Can read a whole message.
        if(message_size > coresh::VariableTableSingleton::GetInstance().GetMaxProtocolSize()) {
          AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
              "%s:%d (%s) Error message size[%llu] in protocol head.",
              __FILE__, __LINE__, __FUNCTION__, message_size);
          connection->Shutdown();
          break;
        } else if(read_size < message_size) {
          break;
        }
        protocol_head = coresh::ProtocolHeadCast(buffer.ReadPointer());
        if(coresh::GetProtocolEncode(*protocol_head) != (core::uint32)packetizer->GetEncodeType() ||
            coresh::GetProtocolCompress(*protocol_head) != (core::uint32)packetizer->GetCompressType()) {
          AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
              "%s:%d (%s) Error encode/compress type in protocol head.",
              __FILE__, __LINE__, __FUNCTION__);
          connection->Shutdown();
          break;
        }
        retcode = packetizer->Unpacket(coresh::GetProtocolData(buffer.ReadPointer()), message_size);
        if(retcode > 0) {
          const char *message = packetizer->GetUnpacketMessage(unpacket_size);
          terminal->OnMessage(message, unpacket_size);
        } else if(retcode == 0) {
          terminal->OnMessage(coresh::GetProtocolData(buffer.ReadPointer()),
              message_size - coresh::kProtocolHeadSize);
        } else {
          AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
              "%s:%d (%s) Unpacket message failed and will be ignored",
              __FILE__, __LINE__, __FUNCTION__);
        }
        buffer.ReadShift(message_size);
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Get packetizer failed.", __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Connection get a empty context.",  __FILE__, __LINE__, __FUNCTION__);
  }
}

void DatabaseServer::OnWriteComplete(const core::TCPConnectionPtr &connection) {}

void DatabaseServer::OnServerJoinSchedule(const global::protocol::ServerSignature &signature) {}

void DatabaseServer::OnServerDisjoinSchedule(const global::protocol::ServerSignature &signature) {}

}  // namespace server

}  // namespace database

