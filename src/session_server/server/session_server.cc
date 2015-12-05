//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-04 11:29:23.
// File name: session_server.cc
//
// Description:
// Define class SessionServer.
//

#include "session_server/server/session_server.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_actor_logout_center_types.h"
#include "global/global_packet.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "global/configure/configure.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/game_message_handler.h"
#include "session_server/server/gateway_message_handler.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/session_actor_pool.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/sync_database_operation_manager.h"
#include "session_server/server/world_data_manager.h"
#include "session_server/server/admin/admin_service.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_system_event_types.h"

namespace session {

namespace server {

SessionServer *SessionServerSingleton::instance_ = NULL;

SessionServer::SessionServer(const core::InetAddress &address, const std::string &name) :
  service_(address, name) {}

SessionServer::~SessionServer() {}

bool SessionServer::Initialize(const global::protocol::ServerSignature &signature,
    const core::InetAddress &admin_address) {
  this->SetSignature(signature);

  // Set singleton instance.
  SessionServerSingleton::SetInstance(this);

  global::GlobalPacket::SetPacket(&this->open_thrift_);

  // Initialize singletons.
  if(SyncDatabaseOperationManager::GetInstance() == NULL) {
    LOG_ERROR("Initialize SyncDatabaseOperationManager singleton failed.");
    return false;
  }
  if(WorldDataManager::GetInstance() == NULL) {
    LOG_ERROR("Initialize WorldDataManager singleton failed.");
    return false;
  }
  if(SessionActorPool::GetInstance() == NULL) {
    LOG_ERROR("Initialize SessionActorPool singleton failed.");
    return false;
  }
  if(SessionActorManager::GetInstance() == NULL) {
    LOG_ERROR("Initialize SessionActorManager singleton failed.");
    return false;
  }
  if(GatewayMessageHandler::GetInstance() == NULL) {
    LOG_ERROR("Initialize GatewayMessageHandler singleton failed.");
    return false;
  }
  if(GameMessageHandler::GetInstance() == NULL) {
    LOG_ERROR("Initialize GameMessageHandler singleton failed.");
    return false;
  }
  if(ExtensionManager::GetInstance() == NULL) {
    LOG_ERROR("Initialize ExtensionManager singleton failed.");
    return false;
  }
  if (admin::AdminService::GetInstance() == NULL) {
    LOG_ERROR("Initialize AdminService singleton failed.");
    return false;
  }

  // Initialize application service.
  if(this->service_.Initialize() == false) {
    LOG_ERROR("Initialize application service failed.");
    return false;
  }

  // Connection callback.
  this->service_.SetConnectionCallback(
      boost::bind(&SessionServer::OnConnection, this, _1));

  // Initialize admin service.
  if (admin::AdminService::GetInstance()->Initialize(&this->service_,
          admin_address, "AdminService") == false) {
    LOG_ERROR("Initialize admin service failed.");
    return false;
  }

  // Get session server configure.
  const global::SessionServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetSessionServer();
  if(information == NULL) {
    LOG_ERROR("Get session server configure failed.");
    return false;
  }

  // Get schedule server configure.
  const global::ServerInformation *schedule_server_info =
    global::ServerConfigureSingleton::GetInstance().GetScheduleServer();
  if(schedule_server_info == NULL) {
    LOG_ERROR("Get schedule server information failed.");
    return false;
  }
  this->schedule_client_.reset(new (std::nothrow) schedule::client::ScheduleClient());
  if(this->schedule_client_.get() == NULL || this->schedule_client_->Initialize(
        core::InetAddress(schedule_server_info->ip_, schedule_server_info->port_),
        "ScheduleClient", this->service_.AllocateLoop(), this->service_.GetLoop(),
        signature, &this->thrift_) == false) {
    LOG_ERROR("Allocate/Initialize schedule client failed.");
    return false;
  }

  this->schedule_client_->SetJoinScheduleCallback(
      boost::bind(&SessionServer::OnServerJoinSchedule, this, _1));
  this->schedule_client_->SetDisjoinScheduleCallback(
      boost::bind(&SessionServer::OnServerDisjoinSchedule, this, _1));

  // Initialize database client.
  if(database::client::DatabaseClientSingleton::GetInstance().Initialize(
        this->signature_, this->service_.GetLoop()) == false) {
    LOG_ERROR("Initialize database client failed.");
    return false;
  }
  database::client::DatabaseClientSingleton::GetInstance().SetDatabaseSessionLoginedCallback(
      boost::bind(&SessionServer::OnDatabaseSessionLogined, this, _1));

  // Load world data
  if(WorldDataManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Initialize WorldDataManager failed.");
    return false;
  }

  if(SessionActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    LOG_ERROR("Initialize session actor pool failed.");
    return false;
  }
  if(SessionActorManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Initialize SessionActorManager failed.");
    return false;
  }
  if(ExtensionManager::GetInstance()->Initialize() == false) {
    LOG_ERROR("Initialize ExtensionManager failed.");
    return false;
  }

  // Load from database.
  if (SyncDatabaseOperationManager::GetInstance()->LoadFromDatabase() == false) {
    LOG_ERROR("Load data from database failed.");
    return false;
  }

  if(GatewayMessageHandler::GetInstance()->Initialize() == false) {
    LOG_ERROR("Initialize GatewayMessageHandler failed.");
    return false;
  }
  if(GameMessageHandler::GetInstance()->Initialize() == false) {
    LOG_ERROR("Initialize GameMessageHandler failed.");
    return false;
  }
  if(this->storage_response_.Initialize() == false) {
    LOG_ERROR("Initialize StorageResponse failed.");
    return false;
  }
  if(this->word_filter_.Initialize(
         global::configure::ConfigureSingleton::GetInstance().GetDirtyWordFile()) == false) {
    LOG_ERROR("Initialize WordFilter failed.");
    return false;
  }
  if(this->day_pass_.Initialize() == false) {
    LOG_ERROR("Initialize DayPass failed.");
    return false;
  }

  return true;
}

void SessionServer::Finalize() {
  this->day_pass_.Finalize();
  this->storage_response_.Finalize();
  GameMessageHandler::GetInstance()->Finalize();
  GatewayMessageHandler::GetInstance()->Finalize();

  // Save to database
  if (SyncDatabaseOperationManager::GetInstance()->SaveToDatabase() == false) {
    LOG_ERROR("Save data to database failed.");
  }

  ExtensionManager::GetInstance()->Finalize();
  SessionActorManager::GetInstance()->Finalize();
  SessionActorPool::GetInstance()->Finalize();

  database::client::DatabaseClientSingleton::GetInstance().Finalize();
  this->schedule_client_->Finalize();
  this->schedule_client_.reset();
  admin::AdminService::GetInstance()->Finalize();
  this->service_.Finalize();

  admin::AdminService::ReleaseInstance();
  ExtensionManager::ReleaseInstance();
  GameMessageHandler::ReleaseInstance();
  GatewayMessageHandler::ReleaseInstance();
  SessionActorManager::ReleaseInstance();
  SessionActorPool::ReleaseInstance();
  SyncDatabaseOperationManager::ReleaseInstance();

  TerminalSet::iterator iterator = this->connections_.begin();
  for(; iterator != this->connections_.end(); ++iterator) {
    if(*iterator) {
      (*iterator)->Finalize();
      delete *iterator;
    }
  }
  this->connections_.clear();

  TerminalMap::iterator iterator2 = this->terminals_.begin();
  for(; iterator2 != this->terminals_.end(); ++iterator2) {
    if(iterator2->second) {
      iterator2->second->Finalize();
      delete iterator2->second;
    }
  }
  this->terminals_.clear();
}

void SessionServer::Start() {
  this->service_.Start();
  admin::AdminService::GetInstance()->Start();
  this->schedule_client_->Start();
  database::client::DatabaseClientSingleton::GetInstance().Start();
  ExtensionManager::GetInstance()->Start();
}

void SessionServer::Stop() {
  ExtensionManager::GetInstance()->Stop();
  database::client::DatabaseClientSingleton::GetInstance().Stop();
  this->schedule_client_->Stop();
  admin::AdminService::GetInstance()->Stop();
  this->service_.Stop();
}

bool SessionServer::RequestLogin(SessionTerminal *terminal) {
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) Terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  TerminalSet::iterator iterator = this->connections_.find(terminal);
  if(iterator != this->connections_.end()) {
    this->connections_.erase(iterator);
  }

  protocol::MessageLoginResponse response;
  TerminalKey key(terminal->GetSignature().type_, terminal->GetSignature().id_);
  if(this->terminals_.find(key) == this->terminals_.end()) {
    if(terminal->GetSignature().type_ == global::protocol::ServerType::GAME_SERVER) {
      if(this->game_server_index_.Add(terminal->GetSignature().id_)) {
        this->terminals_.insert(std::make_pair(key, terminal));
        response.__set_logined_(true);
      } else {
        response.__set_logined_(false);
      }
    } else {
      this->terminals_.insert(std::make_pair(key, terminal));
      response.__set_logined_(true);
    }
  } else {
    response.__set_logined_(false);
  }

  terminal->SendMessage(response, protocol::MessageType::MESSAGE_LOGIN_RESPONSE);

  return true;
}

bool SessionServer::RequestLogout(SessionTerminal *terminal) {
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) Terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  protocol::MessageLogoutResponse response;
  TerminalKey key(terminal->GetSignature().type_, terminal->GetSignature().id_);

  TerminalMap::iterator iterator = this->terminals_.find(key);
  if(iterator != this->terminals_.end()) {
    if(terminal->GetSignature().type_ == global::protocol::ServerType::GAME_SERVER) {
      this->game_server_index_.Remove(terminal->GetSignature().id_);
    }
    this->terminals_.erase(iterator);
    response.__set_logouted_(true);
  } else {
    response.__set_logouted_(false);
  }

  terminal->SendMessage(response, protocol::MessageType::MESSAGE_LOGOUT_RESPONSE);

  return response.logouted_;
}

void SessionServer::OnDisconnect(SessionTerminal *terminal) {
  this->RequestLogout(terminal);
  this->connections_.erase(terminal);
  terminal->Finalize();
  delete terminal;
}

SessionTerminal *SessionServer::AllocateGameServer() {
  core::uint32 id = this->game_server_index_.GetServer();
  return this->GetTerminal(global::protocol::ServerType::GAME_SERVER, id);
}

SessionTerminal *SessionServer::GetTerminal(global::protocol::ServerType::type type,
    core::uint32 id) {
  TerminalKey key(type, id);
  TerminalMap::iterator iterator = this->terminals_.find(key);
  if(iterator != this->terminals_.end()) {
    return iterator->second;
  }

  return NULL;
}

void SessionServer::ActorLogoutCenter(core::uint64 id) {
  database::protocol::StorageActorLogoutCenterRequest request;
  request.__set_id_(id);
  size_t size = 0;
  const char *message = this->thrift_.Serialize(request, size);
  if(message == NULL) {
    global::LogError("%s:%d (%s) Serialize failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        database::protocol::StorageType::STORAGE_ACTOR_LOGOUT_CENTER,
        request.id_, message, size) == false) {
    global::LogError("%s:%d (%s) Send actor logout center request failed",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

void SessionServer::OnConnection(const coresh::ConnectionPtr &connection) {
  if(connection.get() == NULL) {
    global::LogError("%s:%d (%s) connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  SessionTerminal *terminal = new (std::nothrow) SessionTerminal();
  if(terminal == NULL) {
    connection->ApplicationDisconnect();
    global::LogError("%s:%d (%s) Allocate session terminal failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(terminal->Initialize(connection, &this->open_thrift_) == false) {
    delete terminal;
    connection->ApplicationDisconnect();
    global::LogError("%s:%d (%s) Initialize session terminal failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connections_.insert(terminal);
}

void SessionServer::OnServerJoinSchedule(
    const global::protocol::ServerSignature &signature) {
  global::LogDebug("Application server[type=%d, id=%d, host=%s:%d] join schedule server",
      signature.type_, signature.id_, signature.host_.c_str(), signature.port_);
  if(signature.type_ == global::protocol::ServerType::DATABASE_SERVER) {
    database::client::DatabaseClientSingleton::GetInstance().OnDatabaseServerJoin(
        this->service_.AllocateLoop(), this->service_.GetLoop(), signature);
  }
}

void SessionServer::OnServerDisjoinSchedule(
    const global::protocol::ServerSignature &signature) {
  global::LogDebug("Application server[type=%d, id=%d, host=%s:%d] disjoin schedule server",
      signature.type_, signature.id_, signature.host_.c_str(), signature.port_);
  if(signature.type_ == global::protocol::ServerType::DATABASE_SERVER) {
    database::client::DatabaseClientSingleton::GetInstance().OnDatabaseServerDisjoin(signature);
  } else if (signature.type_ == global::protocol::ServerType::GATEWAY_SERVER) {
    SessionActorManager::GetInstance()->KickActorsInGateway(signature.id_);    
  }
}

void SessionServer::OnDatabaseSessionLogined(
    const global::protocol::ServerSignature &signature) {
  // 发送数据库连接成功事件
  event::EventSystemDatabaseServerLogined event;
  event.__set_id_(signature.id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SYSTEM_DATABASE_SERVER_LOGINED, 0,
      event::ChannelType::CHANNEL_SYSTEM, &event, sizeof(event));
}

}  // namespace server

}  // namespace session

