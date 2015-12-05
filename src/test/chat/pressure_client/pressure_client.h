//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-06 14:24:34.
// File name: pressure_client.h
//
// Description:
// Define class PressureClient.
//

#ifndef __TEST__CHAT__PRESSURE__PRESSURE__CLIENT__H
#define __TEST__CHAT__PRESSURE__PRESSURE__CLIENT__H

#include <string>

#include <boost/shared_ptr.hpp>

#include "core/event_loop.h"
#include "core/inet_address.h"
#include "core/tcp_client.h"
#include "core/tcp_connection.h"
#include "core/timer_handler.h"
#include "core/base/noncopyable.h"
#include "global/thrift_packet.h"
#include "test/chat/protocol/protocol_types.h"

namespace test {

namespace chat {

namespace pressure {

class PressureClient : public core::Noncopyable, public core::TimerHandler {
  enum TimerType { TIMER_TYPE_CHAT = 0, };

 public:
  PressureClient(const core::InetAddress &server_address,
      const std::string &name);
  virtual ~PressureClient();

  bool Allocate();
  void Deallocate();

  bool Initialize(core::EventLoop *loop);
  void Finalize();

  void Start();
  void Stop();

  inline void SetName(const std::string &name) {
    this->name_ = name;
  }
  inline const std::string &GetName() const {
    return this->name_;
  }

  inline void SetLoginStatus(bool status) {
    this->login_status_ = status;
  }
  inline bool GetLoginStatus() const {
    return this->login_status_;
  }

  template <typename MessageType>
    void SendMessage(const MessageType &message,
        protocol::MessageType::type type);

  virtual void OnTimer(core::uint32 id);

 private:
  void OnConnection(const core::TCPConnectionPtr &connection);
  void OnMessage(const core::TCPConnectionPtr &connection,
      core::ByteBufferDynamic &buffer, const core::TimeTick &time_tick);
  void OnWriteComplete(const core::TCPConnectionPtr &connection);

  void SendMessage(const char *message, size_t size);
  void OnMessage(const char *message, size_t size);

  void OnMessageLoginResponse(const char *message, size_t size);

  void Chat();

  core::TCPClient client_;

  std::string name_;
  bool login_status_;

  global::ThriftPacket *thrift_;
};

typedef boost::shared_ptr<PressureClient> PressureClientPtr;

}  // namespace pressure

}  // namespace chat

}  // namespace test

#endif  // __TEST__CHAT__PRESSURE__PRESSURE__CLIENT__H

