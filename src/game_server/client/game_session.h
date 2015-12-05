//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 15:59:48.
// File name: game_session.h
//
// Description:
// Define class GameSession.
//

#ifndef __GAME__CLIENT__GAME__SESSION__H
#define __GAME__CLIENT__GAME__SESSION__H

#include <boost/enable_shared_from_this.hpp> 
#include <boost/shared_ptr.hpp>

#include "core/inet_address.h"
#include "core/base/noncopyable.h"
#include "coresh/client_side.h"
#include "game_server/protocol/game_protocol_types.h"
#include "global/logging.h"
#include "global/server_type_types.h"
#include "global/thrift_packet.h"

namespace game {

namespace client {

class GameSession : public core::Noncopyable,
  public boost::enable_shared_from_this<GameSession> {
  enum StatusLogin { STATUS_NOT_LOGIN = 0, STATUS_LOGINED, STATUS_STOP, STATUS_FINAL, STATUS_MAX, };

 public:
  GameSession();
  ~GameSession();

  bool Initialize(const core::InetAddress &server_address, const std::string &name,
      core::EventLoop *network_loop, core::EventLoop *application_loop,
      const global::protocol::ServerSignature &signature, global::ThriftPacket *packet);
  void Finalize();

  void Start();
  void Stop();

  template <typename T>
  void SendMessage(const T &message, protocol::MessageType::type type);

  void SendMessage(const protocol::Message &message);
  void SendMessage(const char *message, size_t size);

  inline void SetServerSignature(const global::protocol::ServerSignature &signature) {
    this->signature_ = signature;
  }
  inline const global::protocol::ServerSignature &GetServerSignature() const {
    return this->signature_;
  }

  inline void SetStatus(StatusLogin status) {
    this->status_ = status;
  }
  inline StatusLogin GetStatus() const {
    return this->status_;
  }
  inline bool CheckLoginStatues() const {
    return this->GetStatus() == STATUS_LOGINED;
  }

  inline void SetActorNumber(core::uint32 number) {
    this->actor_number_ = number;
  }
  inline core::uint32 GetActorNumber() const {
    return this->actor_number_;
  }

 private:
  void OnConnection(const coresh::ConnectionPtr &connection);
  void OnMessage(const char *message, size_t size);
  void OnDisconnect();

  // Message handler.
  void OnMessageLoginResponse(const char *message, size_t size);
  void OnMessageLogoutResponse(const char *message, size_t size);
  void OnMessageCallback(const char *message, size_t size);

  coresh::ConnectionPtr connection_;
  coresh::ClientSide client_;

  global::ThriftPacket *thrift_;

  global::protocol::ServerSignature signature_;
  StatusLogin status_;

  core::uint32 actor_number_;
};

typedef boost::shared_ptr<GameSession> GameSessionPtr;

template <typename T>
void GameSession::SendMessage(const T &message, protocol::MessageType::type type) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  size_t size = 0;
  const char *message1 = this->thrift_->Serialize(message, size);
  if(message1) {
    protocol::Message message2;
    message2.__set_type_(type);
    message2.message_.assign(message1, size);
    this->SendMessage(message2);
  } else {
    global::LogError("%s:%d (%s) Serialize message[%d] failed.",
        __FILE__, __LINE__, __FUNCTION__, type);
  }
}

}  // namespace client

}  // namespace game

#endif  // __GAME__CLIENT__GAME__SESSION__H

