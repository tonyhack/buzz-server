//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-14 12:19:41.
// File name: database_server.h
//
// Description:
// Define class DatabaseServer.
//

#ifndef __DATEBASE__SERVER__DATABASE__SERVER__H
#define __DATEBASE__SERVER__DATABASE__SERVER__H

#include <boost/shared_ptr.hpp>

#include "core/base/noncopyable.h"
#include "core/event_loop_thread_group.h"
#include "core/tcp_connection.h"
#include "core/tcp_server.h"
#include "database_server/server/async_request_service.h"
#include "database_server/server/database_terminal_set.h"
#include "global/multiplex_thrift.h"
#include "global/server_type_types.h"
#include "schedule_server/client/schedule_client2.h"

namespace database {

namespace server {

class DatabaseServer : public core::Noncopyable {
  typedef boost::shared_ptr<core::EventLoopThreadGroup> EventLoopThreadGroupPtr;
  typedef boost::shared_ptr<schedule::client::ScheduleClient2> ScheduleClientPtr;

 public:
  DatabaseServer(const core::InetAddress &address, const std::string &name);
  ~DatabaseServer();

  bool Initialize(core::uint32 server_id);
  void Finalize();

  bool Start();
  void Stop();

  inline DatabaseTerminalSet &GetTerminals() {
    return this->terminals_;
  }
  inline AsyncRequestService &GetAsyncRequestService() {
    return this->async_request_service_;
  }

 private:
  void OnConnection(const core::TCPConnectionPtr &connection);
  void OnMessage(const core::TCPConnectionPtr &connection,
    core::ByteBufferDynamic &buffer, const core::TimeTick &time_tick);
  void OnWriteComplete(const core::TCPConnectionPtr &connection);

  void OnServerJoinSchedule(const global::protocol::ServerSignature &signature);
  void OnServerDisjoinSchedule(const global::protocol::ServerSignature &signature);

  core::uint32 server_id_;

  EventLoopThreadGroupPtr thread_pool_;
  core::TCPServer server_;

  DatabaseTerminalSet terminals_;
  AsyncRequestService async_request_service_;

  global::MultiplexThrift packets_;

  ScheduleClientPtr schedule_client_;
};

class DatabaseServerSingleton : public core::Noncopyable {
 public:
  DatabaseServerSingleton() {}
  ~DatabaseServerSingleton() {}

  static inline DatabaseServer &GetInstance() {
    return *DatabaseServerSingleton::instance_;
  }
  static inline void SetInstance(DatabaseServer *instance) {
    DatabaseServerSingleton::instance_ = instance;
  }

 private:
  static DatabaseServer *instance_;
};

}  // namespace server

}  // namespace database

#endif  // __DATEBASE__SERVER__DATABASE__SERVER__H

