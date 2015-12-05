//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-18 11:08:32.
// File name: database_client.h
//
// Description:
// Define class DatabaseClient.
//

#ifndef __DATABASE__CLIENT__DATABASE__CLIENT__H
#define __DATABASE__CLIENT__DATABASE__CLIENT__H

#include <vector>

#include <ext/hash_map>
#include <boost/function.hpp>

#include "core/event_loop.h"
#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/database_storage_types.h"
#include "global/logging.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"

namespace database {

namespace client {

class DatabaseSession;

typedef boost::shared_ptr<DatabaseSession> DatabaseSessionPtr;

class DatabaseClient : public core::Noncopyable,
  public core::TimerHandler {
  typedef std::vector<DatabaseSessionPtr> DatabaseSessionVector;

  typedef std::list<protocol::MessageStorageRequest> RequestCacheList;

  typedef boost::function<void (const std::string &, const std::string &,
          protocol::ResponseType::type)> ResponseCallback;

  typedef boost::function<void (const global::protocol::ServerSignature &)>
          DatabaseSessionLoginedCallback;

 public:
  DatabaseClient();
  ~DatabaseClient();

  bool Initialize(const global::protocol::ServerSignature &signature,
      core::EventLoop *timer_loop);
  void Finalize();

  void Start();
  void Stop();

  void OnDatabaseServerJoin(core::EventLoop *network_loop,
      core::EventLoop *application_loop,
      const global::protocol::ServerSignature &signature);
  void OnDatabaseServerDisjoin(const global::protocol::ServerSignature &signature);
  void OnDatabaseSessionLogined(const global::protocol::ServerSignature &signature);

  // Login/Logout.
  bool AddSession(const DatabaseSessionPtr &session);
  bool StopSession(core::uint32 server_id);
  bool RemoveSession(core::uint32 server_id);

  // Send/Receive request.
  bool SendRequest(protocol::StorageType::type type, core::uint64 channel,
      const char *message, size_t size);
  void ReceiveResponse(const protocol::MessageStorageResponse &response);

  template <typename T>
  bool SendRequest(const T &message, protocol::StorageType::type type,
      core::uint64 channel);

  // Response handler.
  inline void SetResponseCallback(protocol::StorageType::type type,
      const ResponseCallback &callback) {
    this->response_handlers_[type] = callback;
  }
  inline void ResetResponseCallback(protocol::StorageType::type type) {
    this->response_handlers_[type].clear();
  }

  // Database session logined callback.
  inline void SetDatabaseSessionLoginedCallback(const DatabaseSessionLoginedCallback &callback) {
    this->database_session_logined_callback_ = callback;
  }

  virtual void OnTimer(core::uint32 id);

 private:
  bool CacheRequest(protocol::StorageType::type type, core::uint64 channel,
      const char *message, size_t size);
  void SendRequestCache();

  static const size_t kMaxCacheSize_ = 10240;
  static const size_t kMaxRequestCount_ = 256;
  static const core::uint32 kCacheSendInterval_ = 200;

  global::protocol::ServerSignature signature_;
  DatabaseSessionVector sessions_;

  RequestCacheList request_cache_;

  ResponseCallback response_handlers_[protocol::StorageType::STORAGE_MAX];

  DatabaseSessionLoginedCallback database_session_logined_callback_;

  global::ThriftPacket packet_;

  core::EventLoop *timer_loop_;
};

template <typename T>
bool DatabaseClient::SendRequest(const T &message, protocol::StorageType::type type,
    core::uint64 channel) {
  size_t size = 0;
  const char *request = this->packet_.Serialize(message, size);
  if(request == NULL) {
    global::LogError("%s:%d (%s) Serialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return this->SendRequest(type, channel, request, size);
}

class DatabaseClientSingleton : public core::Noncopyable {
 public:
  DatabaseClientSingleton() {}
  ~DatabaseClientSingleton() {}

  static inline DatabaseClient &GetInstance() {
    return DatabaseClientSingleton::instance_;
  }

 private:
  static DatabaseClient instance_;
};

}  // namespace client

}  // namespace database

#endif  // __DATABASE__CLIENT__DATABASE__CLIENT__H

