//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-05 14:37:50.
// File name: game_server.cc
//
// Description:
// Define class GameServer.
//

#include "game_server/server/game_server.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "game_server/server/configure.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_actor_pool.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/game_npc_pool.h"
#include "game_server/server/game_terminal.h"
#include "game_server/server/event/game_system_event_types.h"
#include "global/direction_calculator.h"
#include "global/distance_calculator.h"
#include "global/global_packet.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

GameServer *GameServerSingleton::instance_ = NULL;

GameServer::GameServer(const core::InetAddress &address,
    const std::string &name) : service_(address, name) {}
GameServer::~GameServer() {}

bool GameServer::Initialize(const global::protocol::ServerSignature &signature) {
  this->SetSignature(signature);

  // Set singleton instance.
  GameServerSingleton::SetInstance(this);

  // Set global packet.
  global::GlobalPacket::SetPacket(&this->open_thrift_);

  // Initialize application service.
  if(this->service_.Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize application service failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Get configure failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(global::DistanceCalculator::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Get distance calculator failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(global::DirectionCalculator::GetInstance() == NULL) {
    LOG_ERROR("Get DirectionCalculator failed.");
    return false;
  }

  if(GameNpcManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) get game npc manager failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(GameActorManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Get game actor manager failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(ExtensionManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Get extension manager failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(Configure::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize configure failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(global::DistanceCalculator::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize distance calculator failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(global::DirectionCalculator::GetInstance()->Initialize() == false) {
    LOG_ERROR("Initialize DirectionCalculator failed.");
    return false;
  }

  if(ExtensionManager::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize extension manager failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Connection callback.
  this->service_.SetConnectionCallback(
      boost::bind(&GameServer::OnConnection, this, _1));

  // Get game server configure.
  const global::GameServerInformation *information =
      global::ServerConfigureSingleton::GetInstance().GetGameServers(
      this->GetSignature().id_);
  if(information == NULL) {
    global::LogError("%s:%d (%s) Get game server configure failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Initialize actor pool.
  if(GameActorPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Get game actor pool failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(GameActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_initial_) == false) {
    global::LogError("%s:%d (%s) Initialize game actor pool failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Initialize npc pool.
  if(GameNpcPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Get game npc pool failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(GameNpcPool::GetInstance()->Initialize(information->npc_pool_initial_,
        information->npc_pool_initial_) == false) {
    global::LogError("%s:%d (%s) Initialize game npc pool failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Get schedule server configure.
  const global::ServerInformation *schedule_server_info =
    global::ServerConfigureSingleton::GetInstance().GetScheduleServer();
  if(schedule_server_info == NULL) {
    global::LogError("%s:%d (%s) Get schedule server information failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->schedule_client_.reset(new (std::nothrow) schedule::client::ScheduleClient());
  if(this->schedule_client_.get() == NULL || this->schedule_client_->Initialize(
        core::InetAddress(schedule_server_info->ip_, schedule_server_info->port_),
        "ScheduleClient", this->service_.AllocateLoop(), this->service_.GetLoop(),
        this->GetSignature(), &this->thrift_) == false) {
    global::LogError("%s:%d (%s) Allocate/Initialize schedule client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->schedule_client_->SetJoinScheduleCallback(
      boost::bind(&GameServer::OnServerJoinSchedule, this, _1));
  this->schedule_client_->SetDisjoinScheduleCallback(
      boost::bind(&GameServer::OnServerDisjoinSchedule, this, _1));

  // Initialize database client.
  if(database::client::DatabaseClientSingleton::GetInstance().Initialize(
        signature, this->service_.GetLoop()) == false) {
    global::LogError("%s:%d (%s) Initialize database client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Initialize storage response handler.
  if(this->storage_response_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize storage response handler failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->session_client_.reset(new session::client::SessionClient());
  if(this->session_client_.get() == NULL) {
    global::LogError("%s:%d (%s) Allocate session client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Initialize word filter.
  if(this->word_filter_.Initialize(
      global::configure::ConfigureSingleton::GetInstance().GetDirtyWordFile()) == false) {
    LOG_ERROR("Initialize word filter failed.");
    return false;
  }

  // Initialize day pass.
  if(this->day_pass_.Initialize() == false) {
    LOG_ERROR("Initialize day pass failed.");
    return false;
  }

  return true;
}

void GameServer::Finalize() {
  this->day_pass_.Finalize();
  this->storage_response_handler_.Finalize();
  database::client::DatabaseClientSingleton::GetInstance().Finalize();
  this->schedule_client_->Finalize();
  this->schedule_client_.reset();
  GameNpcPool::GetInstance()->Finalize();
  GameNpcPool::ReleaseInstance();
  GameActorPool::GetInstance()->Finalize();
  GameActorPool::ReleaseInstance();
  ExtensionManager::GetInstance()->Finalize();
  Configure::GetInstance()->Finalize();
  global::DirectionCalculator::GetInstance()->Finalize();
  global::DistanceCalculator::GetInstance()->Finalize();
  ExtensionManager::ReleaseInstance();
  GameNpcManager::ReleaseInstance();
  GameActorManager::ReleaseInstance();
  Configure::ReleaseInstance();
  global::DirectionCalculator::ReleaseInstance();
  global::DistanceCalculator::ReleaseInstance();
  this->service_.Finalize();
  global::GlobalPacket::SetPacket(NULL);

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

void GameServer::Start() {
  ExtensionManager::GetInstance()->Start();
  this->service_.Start();
  this->schedule_client_->Start();
  database::client::DatabaseClientSingleton::GetInstance().Start();
}

void GameServer::Stop() {
  database::client::DatabaseClientSingleton::GetInstance().Stop();
  this->schedule_client_->Stop();
  this->service_.Stop();
  ExtensionManager::GetInstance()->Stop();
}

bool GameServer::OnTerminalLogin(GameTerminal *terminal) {
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
    this->terminals_.insert(std::make_pair(key, terminal));
    response.__set_logined_(true);
  } else {
    response.__set_logined_(false);
  }

  terminal->SendMessage(response, protocol::MessageType::MESSAGE_LOGIN_RESPONSE);

  return true;
}

bool GameServer::OnTerminalLogout(GameTerminal *terminal) {
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) Terminal is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  protocol::MessageLogoutResponse response;
  TerminalKey key(terminal->GetSignature().type_, terminal->GetSignature().id_);

  TerminalMap::iterator iterator = this->terminals_.find(key);
  if(iterator != this->terminals_.end()) {
    this->terminals_.erase(iterator);
    response.__set_logouted_(true);
  } else {
    response.__set_logouted_(false);
  }

  terminal->SendMessage(response, protocol::MessageType::MESSAGE_LOGOUT_RESPONSE);

  return response.logouted_;
}

void GameServer::OnDisconnect(GameTerminal *terminal) {
  this->OnTerminalLogout(terminal);
  this->connections_.erase(terminal);
  terminal->Finalize();
  delete terminal;
}

GameTerminal *GameServer::GetGatewayTerminal(core::uint id) {
  TerminalKey key(global::protocol::ServerType::GATEWAY_SERVER, id);
  TerminalMap::iterator iterator = this->terminals_.find(key);
  if(iterator != this->terminals_.end()) {
    return iterator->second;
  }
  return NULL;
}

void GameServer::OnConnection(const coresh::ConnectionPtr &connection) {
  if(connection.get() == NULL) {
    global::LogError("%s:%d (%s) connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  GameTerminal *terminal = new (std::nothrow) GameTerminal();
  if(terminal == NULL) {
    connection->ApplicationDisconnect();
    global::LogError("%s:%d (%s) Allocate game terminal failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(terminal->Initialize(connection, &this->open_thrift_) == false) {
    delete terminal;
    connection->ApplicationDisconnect();
    global::LogError("%s:%d (%s) Initialize game terminal failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connections_.insert(terminal);
}

void GameServer::OnServerJoinSchedule(const global::protocol::ServerSignature &signature) {
  global::LogDebug("Application server[type=%d, id=%d, host=%s:%d] join schedule server",
      signature.type_, signature.id_, signature.host_.c_str(), signature.port_);
  switch(signature.type_) {
    case global::protocol::ServerType::DATABASE_SERVER:
      this->OnDatabaseJoin(signature);
      break;
    case global::protocol::ServerType::SESSION_SERVER:
      this->OnSessionJoin(signature);
      break;
    case global::protocol::ServerType::GATEWAY_SERVER:
      this->OnGatewayJoin(signature);
      break;
    default:
      break;
  }
}

void GameServer::OnServerDisjoinSchedule(const global::protocol::ServerSignature &signature) {
  global::LogDebug("Application server[type=%d, id=%d, host=%s:%d] disjoin schedule server",
      signature.type_, signature.id_, signature.host_.c_str(), signature.port_);
  switch(signature.type_) {
    case global::protocol::ServerType::DATABASE_SERVER:
      this->OnDatabaseDisjoin(signature);
      break;
    case global::protocol::ServerType::SESSION_SERVER:
      this->OnSessionDisjoin(signature);
      break;
    case global::protocol::ServerType::GATEWAY_SERVER:
      this->OnGatewayDisjoin(signature);
      break;
    default:
      break;
  }
}

void GameServer::OnDatabaseJoin(const global::protocol::ServerSignature &signature) {
  database::client::DatabaseClientSingleton::GetInstance().OnDatabaseServerJoin(
      this->service_.AllocateLoop(), this->service_.GetLoop(), signature);
}

void GameServer::OnDatabaseDisjoin(const global::protocol::ServerSignature &signature) {
  database::client::DatabaseClientSingleton::GetInstance().OnDatabaseServerDisjoin(signature);
}

void GameServer::OnSessionJoin(const global::protocol::ServerSignature &signature) {
  if(this->session_client_.get() == NULL) {
    global::LogError("%s:%d (%s) Session client is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(this->session_client_->Initialize(core::InetAddress(signature.host_, signature.port_),
        "SessionClient", this->service_.AllocateLoop(), this->service_.GetLoop(),
        this->GetSignature(), &this->thrift_) == false) {
    global::LogError("%s:%d (%s) Initialize session client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(this->session_message_handler_.Initialize(this->GetThrift()) == false) {
    global::LogError("%s:%d (%s) Initialize session message handler failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // Set login session callback.
  this->session_client_->SetLoginCallback(boost::bind(&GameServer::OnLoginSession, this));
  // Start session client.
  this->session_client_->Start();
}

void GameServer::OnSessionDisjoin(const global::protocol::ServerSignature &signature) {
  if(this->session_client_.get() == NULL) {
    global::LogError("%s:%d (%s) Session client is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->session_client_->ResetLoginCallback();
  this->session_client_->Stop();
  this->session_message_handler_.Finalize();
  this->session_client_->Finalize();
}

void GameServer::OnGatewayJoin(const global::protocol::ServerSignature &signature) {
}

void GameServer::OnGatewayDisjoin(const global::protocol::ServerSignature &signature) {
  GameActorManager::GetInstance()->KickActors();
}

void GameServer::OnLoginSession() {
  // Broadcast event.
  event::EventSystemLoginSession event;
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SYSTEM_LOGIN_SESSION, 0,
      event::ChannelType::CHANNEL_SYSTEM, &event, sizeof(event));
}

}  // namespace server

}  // namespace name

