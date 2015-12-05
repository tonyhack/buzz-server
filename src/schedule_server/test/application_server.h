//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-11 13:51:35.
// File name: application_server.h
//
// Description:
// Define class ApplicationServer.
//

#ifndef __SCHEDULE__TEST__APPLICATION__SERVER__H
#define __SCHEDULE__TEST__APPLICATION__SERVER__H

#include <string>

#include <boost/shared_ptr.hpp>

#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/application_service.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"
#include "schedule_server/client/schedule_client.h"

namespace schedule {

namespace test {

class ApplicationServer : public core::Noncopyable {
  typedef boost::shared_ptr<client::ScheduleClient> ScheduleClientPtr;

 public:
  ApplicationServer(const core::InetAddress &address, const std::string &name);
  ~ApplicationServer();

  bool Initialize(const std::string &log, const global::protocol::ServerSignature &signature);
  void Finalize();

  void Start();
  void Stop();

 private:
  void OnConnection(const coresh::ConnectionPtr &connection) {}

  void OnJoinSchedule(const global::protocol::ServerSignature &signature);
  void OnDisjoinSchedule(const global::protocol::ServerSignature &signature);

  coresh::ApplicationService service_;
  ScheduleClientPtr schedule_;

  global::ThriftPacket packet_;
};

}  // namespace test

}  // namespace schedule

#endif  // __SCHEDULE__TEST__APPLICATION__SERVER__H

