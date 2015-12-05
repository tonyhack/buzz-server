//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-08 14:34:58.
// File name: schedule_server.h
//
// Description:
// Define class ScheduleServer.
//

#ifndef __SCHEDULE__SERVER__SCHEDULE__SERVER__H
#define __SCHEDULE__SERVER__SCHEDULE__SERVER__H

#include <map>
#include <set>

#include "core/async_logging_singleton.h"
#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/application_service.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"
#include "schedule_server/protocol/schedule_protocol_types.h"

namespace schedule {

namespace server {

class ScheduleTerminal;

class ScheduleServer : public core::Noncopyable {
  typedef std::pair<global::protocol::ServerType::type, core::uint32> TerminalKey;
  typedef std::map<TerminalKey, ScheduleTerminal *> TerminalMap;
  typedef std::set<ScheduleTerminal *> TerminalSet;

 public:
  ScheduleServer(const core::InetAddress &address, const std::string &name);
  ~ScheduleServer();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

  bool RequestLogin(ScheduleTerminal *terminal);
  bool RequestLogout(ScheduleTerminal *terminal);

  void OnDisconnect(ScheduleTerminal *terminal);

  template <typename T>
  void BroadcastMessage(const T &message, protocol::MessageType::type type);

  void BroadcastMessage(const char *message, size_t size);
  void BroadcastMessage(const protocol::Message &message);

  inline global::ThriftPacket *GetOpenPacket() {
    return &this->open_packet_;
  }

 private:
  void OnConnection(const coresh::ConnectionPtr &connection);

  coresh::ApplicationService service_;

  TerminalMap terminals_;
  TerminalSet connections_;

  global::ThriftPacket packet_;
  global::ThriftPacket open_packet_;
};

template <typename T>
void ScheduleServer::BroadcastMessage(const T &message,
    protocol::MessageType::type type) {
  size_t size = 0;
  const char *message1 = this->packet_.Serialize(message, size);
  if(message1) {
    protocol::Message message2;
    message2.__set_type_(type);
    message2.message_.assign(message1, size);
    this->BroadcastMessage(message2);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Serialize message[%d] failed.",
        __FILE__, __LINE__, __FUNCTION__, type);
  }
}

class ScheduleServerSingleton : public core::Noncopyable {
 public:
  ScheduleServerSingleton() {}
  ~ScheduleServerSingleton() {}

  static inline void SetInstance(ScheduleServer *instance) {
    ScheduleServerSingleton::instance_ = instance;
  }
  static inline ScheduleServer &GetInstance() {
    return *ScheduleServerSingleton::instance_;
  }

 private:
  static ScheduleServer *instance_;
};

}  // namespace server

}  // namespace schedule

#endif  // __SCHEDULE__SERVER__SCHEDULE__SERVER__H

