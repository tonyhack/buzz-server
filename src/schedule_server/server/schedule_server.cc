//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-10 10:07:05.
// File name: schedule_server.cc
//
// Description:
// Define class ScheduleServer.
//

#include "schedule_server/server/schedule_server.h"

#include <boost/bind.hpp>

#include "schedule_server/server/schedule_terminal.h"

namespace schedule {

namespace server {

ScheduleServer *ScheduleServerSingleton::instance_ = NULL;

ScheduleServer::ScheduleServer(const core::InetAddress &address,
    const std::string &name) : service_(address, name) {}

ScheduleServer::~ScheduleServer() {}

bool ScheduleServer::Initialize() {
  ScheduleServerSingleton::SetInstance(this);
  this->service_.SetConnectionCallback(
      boost::bind(&ScheduleServer::OnConnection, this, _1));
  return this->service_.Initialize();
}

void ScheduleServer::Finalize() {
  TerminalSet::iterator iterator = this->connections_.begin();
  for(; iterator != this->connections_.end(); ++iterator) {
    (*iterator)->Finalize();
    delete *iterator;
  }
  this->connections_.clear();
  TerminalMap::iterator iterator2 = this->terminals_.begin();
  for(; iterator2 != this->terminals_.end(); ++iterator2) {
    iterator2->second->Finalize();
    delete iterator2->second;
  }
  this->terminals_.clear();
  this->service_.Finalize();
}

void ScheduleServer::Start() {
  this->service_.Start();
}

void ScheduleServer::Stop() {
  this->service_.Stop();
}

bool ScheduleServer::RequestLogin(ScheduleTerminal *terminal) {
  assert(terminal);
  TerminalSet::iterator iterator = this->connections_.find(terminal);
  if(iterator != this->connections_.end()) {
    this->connections_.erase(iterator);
  }

  bool login = false;

  protocol::MessageLoginResponse response;
  TerminalKey key(terminal->GetSignature().type_, terminal->GetSignature().id_);
  if(this->terminals_.find(key) == this->terminals_.end()) {
    // Broadcast login.
    protocol::MessageLoginBroadcast message;
    message.signature_ = terminal->GetSignature();
    this->BroadcastMessage(message, protocol::MessageType::MESSAGE_LOGIN_BROADCAST);

    // Response logined servers' signatures.
    TerminalMap::iterator iterator2 = this->terminals_.begin();
    for(; iterator2 != this->terminals_.end(); ++iterator2) {
      response.servers_.push_back(iterator2->second->GetSignature());
    }

    // Insert into terminal list.
    this->terminals_.insert(std::make_pair(key, terminal));

    login = true;
  }

  response.__set_logined_(login);
  terminal->SendMessage(response, protocol::MessageType::MESSAGE_LOGIN_RESPONSE);

  return true;
}

bool ScheduleServer::RequestLogout(ScheduleTerminal *terminal) {
  assert(terminal);

  bool logout = false;
  protocol::MessageLogoutResponse response;
  TerminalKey key(terminal->GetSignature().type_, terminal->GetSignature().id_);

  TerminalMap::iterator iterator = this->terminals_.find(key);
  if(iterator != this->terminals_.end()) {
    this->terminals_.erase(iterator);

    // Broadcast logout.
    protocol::MessageLogoutBroadcast message;
    message.__set_type_(terminal->GetSignature().type_);
    message.__set_id_(terminal->GetSignature().id_);
    this->BroadcastMessage(message, protocol::MessageType::MESSAGE_LOGOUT_BROADCAST);

    logout = true;
  }

  // Response logout.
  response.__set_logouted_(logout);
  terminal->SendMessage(response, protocol::MessageType::MESSAGE_LOGOUT_RESPONSE);

  return logout;
}

void ScheduleServer::OnDisconnect(ScheduleTerminal *terminal) {
  this->RequestLogout(terminal);
  this->connections_.erase(terminal);
  terminal->Finalize();
  delete terminal;
}

void ScheduleServer::BroadcastMessage(const char *message, size_t size) {
  TerminalMap::const_iterator iterator = this->terminals_.begin();
  for(; iterator != this->terminals_.end(); ++iterator) {
    iterator->second->SendMessage(message, size);
  }
}

void ScheduleServer::BroadcastMessage(const protocol::Message &message) {
  size_t size = 0;
  const char *message1 = this->packet_.Serialize(message, size);
  if(message1) {
    this->BroadcastMessage(message1, size);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Serialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleServer::OnConnection(const coresh::ConnectionPtr &connection) {
  assert(connection);
  ScheduleTerminal *terminal = new (std::nothrow) ScheduleTerminal();
  if(terminal) {
    if(terminal->Initialize(connection, &this->open_packet_) == true) {
      this->connections_.insert(terminal);
    } else {
      delete terminal;
      connection->ApplicationDisconnect();
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Initialize schedule terminal failed.",
          __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    connection->ApplicationDisconnect();
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate schedule terminal failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

}  // namespace server

}  // namespace schedule

