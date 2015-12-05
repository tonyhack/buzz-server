#ifndef __SESSION__SERVER__ADMIN__ADMIN__SERVICE__H
#define __SESSION__SERVER__ADMIN__ADMIN__SERVICE__H

#include <cstddef>
#include <string>
#include <boost/function.hpp>

#include "coresh/connection.h"
#include "global/singleton_factory.h"
#include "session_server/protocol/session_admin_protocol_types.h"
#include "session_server/server/admin/admin_message_handler.h"

namespace core {
class InetAddress;
}

namespace coresh {
class ApplicationService;
class NetworkService;
}

namespace session {

namespace server {

namespace admin {

class AdminSession;

class AdminService : public global::SingletonFactory<AdminService> {
  friend class global::SingletonFactory<AdminService>;

 public:
  typedef boost::function<void (AdminSession *, const char *, size_t)> MessageHandler;

  bool Initialize(coresh::ApplicationService *application_service,
      const core::InetAddress &address, const std::string &name);
  void Finalize();

  void Start();
  void Stop();

  void SetMessageHandler(session::protocol::AdminMessageType::type type,
      const MessageHandler &handler) {
    this->message_handlers_[type] = handler;
  }
  void ResetMessageHandler(session::protocol::AdminMessageType::type type) {
    this->message_handlers_[type].clear();
  }
  void OnMessage(AdminSession *session, const char *message, size_t size);

 private:
  AdminService();
  ~AdminService();
  void OnConnection(const coresh::ConnectionPtr &connection);

  core::EventLoop *application_loop_;
  coresh::NetworkService *network_service_;
  MessageHandler message_handlers_[session::protocol::AdminMessageType::MESSAGE_MAX];
  AdminMessageHandler admin_message_handler_;
};

}  // namespace admin

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__ADMIN__ADMIN__SERVICE__H

