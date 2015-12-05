//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-16 10:34:25.
// File name: database_terminal.cc
//
// Description:
// Define class DatabaseTerminal.
//

#include "database_server/server/database_terminal.h"

#include "core/base/auto_mutex.h"
#include "coresh/packetizer_table.h"
#include "coresh/protocol_format.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/server/database_server.h"

namespace database {

namespace server {

DatabaseTerminal::DatabaseTerminal()
  : packetizer_(NULL), login_status_(false) {}
DatabaseTerminal::~DatabaseTerminal() {}

bool DatabaseTerminal::Initialize(const core::TCPConnectionPtr &connection) {
  assert(connection);
  this->connection_ = connection;
  this->packetizer_ = coresh::PacketizerTableSingleton::GetInstance().GetPacketizer(
      connection->GetLoop());
  if(this->packetizer_ == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get packetizer failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void DatabaseTerminal::Finalize() {
  this->connection_.reset();
  this->packetizer_ = NULL;
}

void DatabaseTerminal::OnMessage(const char *message, size_t size) {
  protocol::Message message2;
  if(this->packet_.Deserialize(message2, message, size)) {
    switch(message2.type_) {
      case protocol::MessageType::MESSAGE_LOGIN_REQUEST:
        this->OnMessageLogin(message2.message_.data(),
            message2.message_.size());
        break;
      case protocol::MessageType::MESSAGE_LOGOUT_REQUEST:
        this->OnMessageLogout(message2.message_.data(),
            message2.message_.size());
        break;
      case protocol::MessageType::MESSAGE_STORAGE_REQUEST:
        this->OnMessageStorage(message2.message_.data(),
            message2.message_.size());
        break;
      default:
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Error message type[%d]", __FILE__, __LINE__,
            __FUNCTION__, message2.type_);
        break;
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void DatabaseTerminal::OnDisconnected() {
  if(this->GetLoginStatus()) {
    if(DatabaseServerSingleton::GetInstance().GetTerminals().OnTerminalLogout(
          shared_from_this()) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Logout failed", __FILE__, __LINE__, __FUNCTION__);
    }
  }
}

void DatabaseTerminal::SendMessage(const char *message, size_t size,
    coresh::Packetizer *packetizer) {
  assert(packetizer);
  core::TCPConnectionPtr connection = this->connection_.lock();
  if(connection) {
    coresh::ProtocolHead head = 0;
    size_t packet_size = 0;
    int retcode = 0;
    retcode = packetizer->Packet(message, size);
    if(retcode > 0) {
      const char *packet = packetizer->GetPacketMessage(packet_size);
      head = packet_size;
      coresh::SetProtocolEncode(head, packetizer->GetEncodeType());
      coresh::SetProtocolCompress(head, packetizer->GetCompressType());
      if(connection->GetLoop()->CheckInLoopThread()) {
        connection->SendMessage((const char *)&head, sizeof(head));
        connection->SendMessage(packet, packet_size);
      } else {
        core::AutoMutex auto_mutex(&this->mutex_);
        connection->SendMessage((const char *)&head, sizeof(head));
        connection->SendMessage(packet, packet_size);
      }
    } else if(retcode == 0) {
      head = size;
      coresh::SetProtocolEncode(head, 0);
      coresh::SetProtocolCompress(head, 0);
      if(connection->GetLoop()->CheckInLoopThread()) {
        connection->SendMessage((const char *)&head, sizeof(head));
        connection->SendMessage(message, size);
      } else {
        core::AutoMutex auto_mutex(&this->mutex_);
        connection->SendMessage((const char *)&head, sizeof(head));
        connection->SendMessage(message, size);
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Packet failed", __FILE__, __LINE__, __FUNCTION__);
    }
  }
}

void DatabaseTerminal::SendMessage(const protocol::Message &message) {
  size_t size = 0;
  const char *message1 = this->packet_.Serialize(message, size);
  if(message1) {
    this->SendMessage(message1, size, this->packetizer_);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Serialize message failed.", __FILE__, __LINE__, __FUNCTION__);
  }
}

void DatabaseTerminal::OnMessageLogin(const char *message, size_t size) {
  protocol::MessageLoginRequest message2;
  if(this->packet_.Deserialize(message2, message, size)) {
    this->SetSignature(message2.signature_);
    protocol::MessageLoginResponse response;
    if(DatabaseServerSingleton::GetInstance().GetTerminals().OnTerminalLogin(
          shared_from_this())) {
      this->SetLoginStatus(true);
      response.__set_logined_(true);
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
          "%s:%d (%s) Terminal [type=%d id=%d host=%s:%d] login in database server.",
          __FILE__, __LINE__, __FUNCTION__, this->GetSignature().type_,
          this->GetSignature().id_, this->GetSignature().host_.c_str(),
          this->GetSignature().port_);
    } else {
      response.__set_logined_(false);
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Terminal [type=%d id=%d host=%s:%d] login failed.",
          __FILE__, __LINE__, __FUNCTION__, this->GetSignature().type_,
          this->GetSignature().id_, this->GetSignature().host_.c_str(),
          this->GetSignature().port_);
    }
    this->SendMessage(response, protocol::MessageType::MESSAGE_LOGIN_RESPONSE);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
  }
}

void DatabaseTerminal::OnMessageLogout(const char *message, size_t size) {
  protocol::MessageLogoutRequest message2;
  if(this->packet_.Deserialize(message2, message, size)) {
    protocol::MessageLogoutResponse response;
    if(DatabaseServerSingleton::GetInstance().GetTerminals().OnTerminalLogout(
          shared_from_this())) {
      this->SetLoginStatus(false);
      response.__set_logouted_(true);
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
          "%s:%d (%s) Terminal [type=%d id=%d host=%s:%d] logout from database server.",
          __FILE__, __LINE__, __FUNCTION__, this->GetSignature().type_,
          this->GetSignature().id_, this->GetSignature().host_.c_str(),
          this->GetSignature().port_);
    } else {
      response.__set_logouted_(false);
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Terminal logout failed.", __FILE__, __LINE__, __FUNCTION__);
    }
    this->SendMessage(response, protocol::MessageType::MESSAGE_LOGOUT_RESPONSE);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
  }
}

void DatabaseTerminal::OnMessageStorage(const char *message, size_t size) {
  protocol::MessageStorageRequest message2;
  if(this->packet_.Deserialize(message2, message, size)) {
    protocol::AsyncRequestMessage request;
    request.__set_type_(message2.type_);
    request.__set_message_(message2.message_);
    request.__set_channel_(message2.channel_);
    request.terminal_key_.__set_type_(this->GetSignature().type_);
    request.terminal_key_.__set_id_(this->GetSignature().id_);
    size_t retsize = 0;
    const char *send = this->packet_.Serialize(request, retsize);
    if(send) {
      if(DatabaseServerSingleton::GetInstance().GetAsyncRequestService().Request(
            send, retsize, message2.channel_) == false) {
        this->ErrorResponse(protocol::ResponseType::ERROR_UNKNOWN, message2);
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Serialize message failed.", __FILE__, __LINE__, __FUNCTION__);
      }
    } else {
      this->ErrorResponse(protocol::ResponseType::ERROR_UNKNOWN, message2);
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Serialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    this->ErrorResponse(protocol::ResponseType::ERROR_MESSAGE, message2);
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
  }
}

void DatabaseTerminal::ErrorResponse(protocol::ResponseType::type type,
    const protocol::MessageStorageRequest &request) {
  protocol::MessageStorageResponse response;
  response.__set_request_(request);
  response.__set_result_(type);
  this->SendMessage(response, protocol::MessageType::MESSAGE_STORAGE_RESPONSE);
}

}  // namespace server

}  // namespace database

