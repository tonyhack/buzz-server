//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 11:26:20.
// File name: login_server.h
//
// Description:
// Define class LoginServer.
//

#ifndef __LOGIN__SERVER__LOGIN__SERVER__H
#define __LOGIN__SERVER__LOGIN__SERVER__H

#include <boost/shared_ptr.hpp>

#include "core/base/noncopyable.h"
#include "core/tcp_server.h"
#include "global/word_filter.h"
#include "login_server_informal/server/actor_login_handler.h"
#include "login_server_informal/server/check_actor_exist_handler.h"
#include "login_server_informal/server/create_actor_handler.h"
#include "login_server_informal/server/message_thread_pool.h"
#include "login_server_informal/server/http_service_handler.h"

namespace login {

namespace server {

class LoginServer : public core::Noncopyable {
 public:
  typedef boost::shared_ptr<MessageThreadPool> MessageThreadPoolPtr;

  LoginServer(const core::InetAddress &address, const std::string &name);
  ~LoginServer();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

  inline const MessageThreadPoolPtr &GetThreadPool() {
    return this->thread_pool_;
  }

  inline global::WordFilter &GetWordFilter() {
    return this->word_filter_;
  }

 private:
  bool OnHttpRequest(const core::TCPConnectionPtr &connection);

  MessageThreadPoolPtr thread_pool_;
  core::TCPServer server_;

  HttpServiceHandler http_service_handler_;

  CreateActorHandler create_actor_handler_;
  ActorLoginHandler actor_login_handler_;
  CheckActorExistHandler check_actor_exist_handler_;

  global::WordFilter word_filter_;
};

class LoginServerSingleton : public core::Noncopyable {
 public:
  LoginServerSingleton() {}
  ~LoginServerSingleton() {}

  static inline void SetInstance(LoginServer *instance) {
    LoginServerSingleton::instance_ = instance;
  }
  static inline LoginServer &GetInstance() {
    return *LoginServerSingleton::instance_;
  }

 private:
  static LoginServer *instance_;
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__LOGIN__SERVER__H

