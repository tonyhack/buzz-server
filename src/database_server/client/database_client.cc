//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-18 11:38:05.
// File name: database_client.cc
//
// Description:
// Define class DatabaseClient.
//

#include "database_server/client/database_client.h"

#include "core/async_logging_singleton.h"
#include "database_server/client/database_session.h"

namespace database {

namespace client {

DatabaseClient DatabaseClientSingleton::instance_;

DatabaseClient::DatabaseClient() {}
DatabaseClient::~DatabaseClient() {}

bool DatabaseClient::Initialize(const global::protocol::ServerSignature &signature,
    core::EventLoop *timer_loop) {
  if(timer_loop == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Params invalid.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->signature_ = signature;
  this->timer_loop_ = timer_loop;
  this->timer_loop_->AddTimer(1, DatabaseClient::kCacheSendInterval_, -1,
      this, "DatabaseClient::Initialize");
  return true;
}

void DatabaseClient::Finalize() {
  DatabaseSessionVector::iterator iterator = this->sessions_.begin();
  for(; iterator != this->sessions_.end(); ++iterator) {
    (*iterator)->Finalize();
  }
  this->sessions_.clear();
  this->timer_loop_->RemoveTimer(1, this);
}

void DatabaseClient::Start() {}

void DatabaseClient::Stop() {
/*
  DatabaseSessionVector::iterator iterator = this->sessions_.begin();
  for(; iterator != this->sessions_.end(); ++iterator) {
    (*iterator)->Stop();
  }
*/
}

void DatabaseClient::OnDatabaseServerJoin(core::EventLoop *network_loop,
    core::EventLoop *application_loop, const global::protocol::ServerSignature &signature) {
  if(signature.type_ == global::protocol::ServerType::DATABASE_SERVER) {
    client::DatabaseSessionPtr session(new (std::nothrow) client::DatabaseSession());
    if(session.get() == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate database session failed.", __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    if(session->Initialize(core::InetAddress(signature.host_, signature.port_), "DataProxyClientSession",
          network_loop, application_loop, this->signature_, &this->packet_) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Initialize database session failed.", __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    if(this->AddSession(session) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Add database session failed.", __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    session->Start();

    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Start database session client[%s:%d]", signature.host_.c_str(), signature.port_);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_DEBUG,
        "Other application server[type=%d] login schedule server", signature.type_);
  }
}

void DatabaseClient::OnDatabaseServerDisjoin(
    const global::protocol::ServerSignature &signature) {
  if(signature.type_ == global::protocol::ServerType::DATABASE_SERVER) {
    client::DatabaseClientSingleton::GetInstance().StopSession(signature.id_);
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Database session client[%s:%d] stopped.", signature.host_.c_str(), signature.port_);
  }
}

void DatabaseClient::OnDatabaseSessionLogined(
    const global::protocol::ServerSignature &signature) {
  if(this->database_session_logined_callback_) {
    this->database_session_logined_callback_(signature);
  }
}

bool DatabaseClient::AddSession(const DatabaseSessionPtr &session) {
  DatabaseSessionVector::iterator iterator = this->sessions_.begin();
  for(; iterator != this->sessions_.end(); ++iterator) {
    if((*iterator)->GetServerSignature().id_ ==
        session->GetServerSignature().id_) {
      return false;
    }
  }
  this->sessions_.push_back(session);
  return true;
}

bool DatabaseClient::StopSession(core::uint32 server_id) {
  DatabaseSessionVector::iterator iterator = this->sessions_.begin();
  for(; iterator != this->sessions_.end(); ++iterator) {
    if((*iterator)->GetServerSignature().id_ == (core::int32)server_id) {
      (*iterator)->Stop();
      return true;
    }
  }
  return false;
}

bool DatabaseClient::RemoveSession(core::uint32 server_id) {
  DatabaseSessionVector::iterator iterator = this->sessions_.begin();
  for(; iterator != this->sessions_.end(); ++iterator) {
    if((*iterator)->GetServerSignature().id_ == (core::int32)server_id) {
      this->sessions_.erase(iterator);
      return true;
    }
  }
  return false;
}

bool DatabaseClient::SendRequest(protocol::StorageType::type type,
    core::uint64 channel, const char *message, size_t size) {
  if(this->sessions_.empty()) {
    return this->CacheRequest(type, channel, message, size);
  }
  // Find one session.
  DatabaseSessionPtr session = this->sessions_[channel % this->sessions_.size()];
  if(session.get() == NULL) {
    return this->CacheRequest(type, channel, message, size);
  }
  // Check in logined status.
  if(session->CheckLoginStatues() == false) {
    return this->CacheRequest(type, channel, message, size);
  }

  if(this->request_cache_.empty()) {
    // Send request.
    protocol::MessageStorageRequest message2;
    message2.__set_type_(type);
    message2.__set_channel_(channel);
    message2.message_.assign(message, size);
    session->SendMessage(message2, protocol::MessageType::MESSAGE_STORAGE_REQUEST);
    return true;
  } else {
    return this->CacheRequest(type, channel, message, size);
  }
}

void DatabaseClient::ReceiveResponse(const protocol::MessageStorageResponse &response) {
  if(response.request_.type_ < protocol::StorageType::STORAGE_MAX &&
      this->response_handlers_[response.request_.type_]) {
    this->response_handlers_[response.request_.type_](response.request_.message_,
        response.response_, response.result_);
  }
}

bool DatabaseClient::CacheRequest(protocol::StorageType::type type, core::uint64 channel,
    const char *message, size_t size) {
  if(this->request_cache_.size() >= DatabaseClient::kMaxCacheSize_) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) request cache full.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  protocol::MessageStorageRequest message2;
  message2.__set_type_(type);
  message2.__set_channel_(channel);
  message2.message_.assign(message, size);
  this->request_cache_.push_back(message2);
  return true;
}

void DatabaseClient::SendRequestCache() {
  if(this->sessions_.empty()) {
    return ;
  }

  size_t count = 0;

  for(RequestCacheList::iterator iterator = this->request_cache_.begin();
      iterator != this->request_cache_.end() && ++count < DatabaseClient::kMaxRequestCount_;) {
    DatabaseSessionPtr session = this->sessions_[(*iterator).channel_ % this->sessions_.size()];
    if(session.get() != NULL && session->CheckLoginStatues()) {
      session->SendMessage(*iterator, protocol::MessageType::MESSAGE_STORAGE_REQUEST);
      this->request_cache_.erase(iterator++);
    }
  }
}

void DatabaseClient::OnTimer(core::uint32 id) {
  this->SendRequestCache();
}

}  // namespace client

}  // namespace database

