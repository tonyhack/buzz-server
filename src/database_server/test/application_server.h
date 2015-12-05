//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-21 17:47:18.
// File name: application_server.h
//
// Description:
// Define class ApplicationServer.
//

#ifndef __DATABASE__TEST__APPLICATION__SERVER__H
#define __DATABASE__TEST__APPLICATION__SERVER__H

#include <string>

#include <boost/shared_ptr.hpp>

#include "core/inet_address.h"
#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/application_service.h"
#include "database_server/client/database_client.h"
#include "global/thrift_packet.h"
#include "global/server_type_types.h"
#include "schedule_server/client/schedule_client.h"

namespace database {

namespace test {

class ApplicationServer : public core::Noncopyable, public core::TimerHandler {
  enum { TIMER_ID_ECHO = 0, };
  enum { TIMER_MSEC_ECHO = 100, };

 public:
  ApplicationServer(const core::InetAddress &address, const std::string &name);
  virtual ~ApplicationServer();

  bool Initialize(const std::string &log,
      const global::protocol::ServerSignature &signature);
  void Finalize();

  void Start();
  void Stop();

  bool Request(protocol::StorageType::type type, const char *message,
      size_t size, core::uint64 channel);

  virtual void OnTimer(core::uint32 id);

 private:
  void OnConnection(const coresh::ConnectionPtr &connection) {}

  void OnJoinSchedule(const global::protocol::ServerSignature &signature);
  void OnDisjoinSchedule(const global::protocol::ServerSignature &signature);

  void OnStorageTestEchoResponse(const std::string &request, const std::string &response,
      protocol::ResponseType::type result);
  void OnStorageTestSumResponse(const std::string &request, const std::string &response,
      protocol::ResponseType::type result);

  coresh::ApplicationService service_;

  schedule::client::ScheduleClientPtr schedule_client_;

  global::ThriftPacket packet_;

  global::protocol::ServerSignature signature_;
};

}  // namespace test

}  // namespace database

#endif  // __DATABASE__TEST__APPLICATION__SERVER__H

