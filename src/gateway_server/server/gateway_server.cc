//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-31 16:00:37.
// File name: gateway_server.cc
//
// Description:
// Define class GatewayServer.
//

#include "gateway_server/server/gateway_server.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_actor_logout_center_types.h"
#include "game_server/client/game_client.h"
#include "gateway_server/server/check_session_queue.h"
#include "gateway_server/server/gateway_user.h"
#include "gateway_server/server/gateway_user_pool.h"
#include "gateway_server/server/session_channel.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"
#include "session_server/protocol/session_gateway_protocol_types.h"

namespace gateway {

namespace server {

GatewayServer *GatewayServerSingleton::instance_ = NULL;

GatewayServer::GatewayServer(const core::InetAddress &address,
    const std::string &name) : service_(address, name) {}
GatewayServer::~GatewayServer() {}

bool GatewayServer::Initialize(const global::protocol::ServerSignature &signature) {
  this->SetSignature(signature);

  //Set singleton instance.
  GatewayServerSingleton::SetInstance(this);

  // Initialize application service.
  if(this->service_.Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize application service failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Connection callback.
  this->service_.SetConnectionCallback(
      boost::bind(&GatewayServer::OnConnection, this, _1));

  if(CheckSessionQueue::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) Get check session queue failed.",
            __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Get gateway server configure.
  const global::GatewayServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGatewayServers(this->signature_.id_);
  if(information == NULL) {
    global::LogError("%s:%d (%s) Get gateway server[id=%d] configure failed.",
        __FILE__, __LINE__, __FUNCTION__, this->signature_.id_);
    return false;
  }

  // Initialize gateway user pool.
  if(GatewayUserPoolSingleton::GetInstance().Initialize(information->user_pool_initial_,
        information->user_pool_extend_) == false) {
    global::LogError("%s:%d (%s) Initialize gateway user pool failed.",
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
  if(this->schedule_client_.get() == NULL  ||
      this->schedule_client_->Initialize(core::InetAddress(schedule_server_info->ip_,
          schedule_server_info->port_), "ScheduleClient", this->service_.AllocateLoop(),
        this->service_.GetLoop(), signature, &this->thrift_) == false) {
    global::LogError("%s:%d (%s) Allocate/Initialize schedule client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->schedule_client_->SetJoinScheduleCallback(
      boost::bind(&GatewayServer::OnServerJoinSchedule, this, _1));
  this->schedule_client_->SetDisjoinScheduleCallback(
      boost::bind(&GatewayServer::OnServerDisjoinSchedule, this, _1));

  // Initialize database client.
  if(database::client::DatabaseClientSingleton::GetInstance().Initialize(
        this->signature_, this->service_.GetLoop()) == false) {
    global::LogError("%s:%d (%s) Initialize database client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Initialize storage response handler.
  if(this->storage_response_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize storage response handler.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(game::client::GameClientSingleton::GetInstance().Initialize(this->GetSignature()) == false) {
    global::LogError("%s:%d (%s) Initialize game client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;                    
  }
  if(this->game_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize game message handler failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->session_client_.reset(new session::client::SessionClient());
  if(this->session_client_.get() == NULL) {
    global::LogError("%s:%d (%s) Allocate session client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

void GatewayServer::Finalize() {
  this->game_message_handler_.Finalize();
  game::client::GameClientSingleton::GetInstance().Finalize();
  database::client::DatabaseClientSingleton::GetInstance().Finalize();
  this->schedule_client_->Finalize();
  this->schedule_client_.reset();
  GatewayUserPoolSingleton::GetInstance().Finalize();
  CheckSessionQueue::ReleaseInstance();
  this->service_.Finalize();
}

void GatewayServer::Start() {
  this->service_.Start();
  this->schedule_client_->Start();
  game::client::GameClientSingleton::GetInstance().Start();
}

void GatewayServer::Stop() {
  game::client::GameClientSingleton::GetInstance().Stop();
  this->schedule_client_->Stop();
  this->service_.Stop();
}

void GatewayServer::OnDisconnect(GatewayUser *user) {
  if(user) {
    this->connect_users_.erase(user);
    user->Finalize();
    GatewayUserPoolSingleton::GetInstance().Deallocate(user);
  }
}

bool GatewayServer::Login(GatewayUser *user) {
  if(user == NULL) {
    return false;
  }

  ConnectUserSet::iterator const_iterator = this->connect_users_.find(user);
  if(const_iterator == this->connect_users_.end()) {
    return false;
  }
  this->connect_users_.erase(user);

  core::uint64 id = user->GetActorID().GetID();
  LoginUserMap::iterator iterator = this->login_users_.find(id);
  if(iterator != this->login_users_.end()) {
    return false;
  }
  this->login_users_.insert(std::make_pair(id, user));
  user->SetLogined(true);

  global::LogDebug("login_users_.size() = %d", this->login_users_.size());

  return true;
}

bool GatewayServer::Logout(GatewayUser *user) {
  if(user->GetLogined() == false) {
    return true;
  }
  core::uint64 id = user->GetActorID().GetID();
  LoginUserMap::iterator iterator = this->login_users_.find(id);
  if(iterator == this->login_users_.end()) {
    return false;
  }
  this->login_users_.erase(iterator);

  global::LogDebug("login_users_.size() = %d", this->login_users_.size());

  database::protocol::StorageActorLogoutCenterRequest request;
  request.__set_id_(user->GetActorID().GetID());
  size_t size = 0;
  const char *message = this->thrift_.Serialize(request, size);
  if(message) {
    if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          database::protocol::StorageType::STORAGE_ACTOR_LOGOUT_CENTER,
          request.id_, message, size) == false) {
      global::LogError("%s:%d (%s) Send request failed",
          __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    global::LogError("%s:%d (%s) Serialize failed",
        __FILE__, __LINE__, __FUNCTION__);
  }

  return true;
}

void GatewayServer::KickOut(core::uint64 id,
    global::protocol::KickoutActorType::type reason) {
  GatewayUser *user = GetUser(id);
  if(user) {
    // 玩家登陆在本服务器中，则直接下线
    protocol::MessageKickoutActor message;
    message.__set_type_(reason);
    user->SendMessage(message, protocol::MessageType::MESSAGE_KICKOUT_ACTOR);
    user->Exit();
  } else {
    // 玩家登陆在非本服务器中，发送给被迫下线到会话服务器
    session::protocol::GateMessageKickoutUser message;
    message.__set_actor_id_(id);
    message.__set_type_(reason);
    SessionChannel::SendMessage(message,
        session::protocol::GatewayMessageType::MESSAGE_KICKOUT_USER);
  }
}

GatewayUser *GatewayServer::GetUser(core::uint64 id) {
  LoginUserMap::iterator iterator = this->login_users_.find(id);
  if(iterator != this->login_users_.end()) {
    return iterator->second;
  }
  return NULL;
}

void GatewayServer::OnConnection(const coresh::ConnectionPtr &connection) {
  assert(connection);
  GatewayUser *user = GatewayUserPoolSingleton::GetInstance().Allocate();
  if(user == NULL) {
    connection->ApplicationDisconnect();
    global::LogError("%s:%d (%s) Allocate gateway user failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(user->Initialize(connection) == false) {
    GatewayUserPoolSingleton::GetInstance().Deallocate(user);
    global::LogError("%s:%d (%s) Initialize gateway user failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connect_users_.insert(user);
}

void GatewayServer::OnServerJoinSchedule(const global::protocol::ServerSignature &signature) {
  global::LogDebug("Application server[type=%d, id=%d, host=%s:%d] join schedule server",
      signature.type_, signature.id_, signature.host_.c_str(), signature.port_);
  switch(signature.type_) {
    case global::protocol::ServerType::DATABASE_SERVER:
      this->OnDatabaseJoin(signature);
      break;
    case global::protocol::ServerType::SESSION_SERVER:
      this->OnSessionJoin(signature);
      break;
    case global::protocol::ServerType::GAME_SERVER:
      this->OnGameJoin(signature);
      break;
    default:
      break;
  }
}

void GatewayServer::OnServerDisjoinSchedule(const global::protocol::ServerSignature &signature) {
  global::LogDebug("Application server[type=%d, id=%d, host=%s:%d] disjoin schedule server",
      signature.type_, signature.id_, signature.host_.c_str(), signature.port_);
  switch(signature.type_) {
    case global::protocol::ServerType::DATABASE_SERVER:
      this->OnDatabaseDisjoin(signature);
      break;
    case global::protocol::ServerType::SESSION_SERVER:
      this->OnSessionDisjoin(signature);
      break;
    case global::protocol::ServerType::GAME_SERVER:
      this->OnGameDisjoin(signature);
      break;
    default:
      break;
  }
}

void GatewayServer::OnGameJoin(const global::protocol::ServerSignature &signature) {
  game::client::GameClientSingleton::GetInstance().OnGameServerJoin(this->service_.AllocateLoop(),
          this->service_.GetLoop(), signature);
}

void GatewayServer::OnGameDisjoin(const global::protocol::ServerSignature &signature) {
  game::client::GameClientSingleton::GetInstance().OnGameServerDisjoin(signature);
}

void GatewayServer::OnDatabaseJoin(const global::protocol::ServerSignature &signature) {
  database::client::DatabaseClientSingleton::GetInstance().OnDatabaseServerJoin(
      this->service_.AllocateLoop(), this->service_.GetLoop(), signature);
}

void GatewayServer::OnDatabaseDisjoin(const global::protocol::ServerSignature &signature) {
  database::client::DatabaseClientSingleton::GetInstance().OnDatabaseServerDisjoin(signature);
}

void GatewayServer::OnSessionJoin(const global::protocol::ServerSignature &signature) {
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
  if(this->session_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize session message handler failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->session_client_->Start();
}

void GatewayServer::OnSessionDisjoin(const global::protocol::ServerSignature &signature) {
  if(this->session_client_.get() == NULL) {
    global::LogError("%s:%d (%s) Session client is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->session_client_->Stop();
  this->session_message_handler_.Finalize();
  this->session_client_->Finalize();
}

}  // namespace server

}  // namespace gateway

