//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:35:32.
// File name: connection.cc
//
// Description: 
// Define class Connection.
//

#include "coresh/connection.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"
#include "coresh/connection_pool.h"
#include "coresh/reusable_buffers.h"
#include "coresh/packetizer.h"
#include "coresh/packetizer_table.h"
#include "coresh/protocol_format.h"
#include "coresh/variable_table.h"

namespace coresh {

Connection::Connection()
  : read_queue_(VariableTableSingleton::GetInstance().GetMessageQueueInitialSize(),
      VariableTableSingleton::GetInstance().GetMaxMessageSize()),
  write_queue_(VariableTableSingleton::GetInstance().GetMessageQueueInitialSize(),
      VariableTableSingleton::GetInstance().GetMaxMessageSize()),
  connect_status_(false) {}

Connection::~Connection() {}

bool Connection::Allocate() {
  core::CreateEventPipe(this->disconnect_fd_);
  if(this->write_queue_.Allocate() == false || this->read_queue_.Allocate() == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate write/read queue failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void Connection::Deallocate() {
  core::CloseEventPipe(this->disconnect_fd_);
  this->write_queue_.Deallocate();
  this->read_queue_.Deallocate();
}

bool Connection::Initialize(const core::TCPConnectionPtr &connection,
    core::EventLoop *application_loop) {
  assert(application_loop);
  this->connection_ = connection;

  this->read_queue_.SetReadableCallback(boost::bind(&Connection::OnReadMessageQueue, this->shared_from_this()));
  if(this->read_queue_.Initialize(application_loop) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize read message queue failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->write_queue_.SetReadableCallback(boost::bind(&Connection::OnWriteMessageQueue, this->shared_from_this()));
  if(this->write_queue_.Initialize(connection->GetLoop()) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize write message queue failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->disconnect_channel_.Initialize(application_loop,
        core::EventPipeReadfd(this->disconnect_fd_)) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize Channel failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->disconnect_channel_.SetEventLoop(application_loop);
  this->disconnect_channel_.SetReadCallback(
      boost::bind(&Connection::OnApplicationDisconnect, this->shared_from_this(), _1));
  this->disconnect_channel_.EnableRead();

  this->SetConnectStatus(true);

  return true;
}

void Connection::Finalize() {
  this->disconnect_channel_.GetEventLoop()->RemoveTimer(TIMER_READ_MESSAGE_ID, this);
  this->disconnect_channel_.DisableRead();
  this->disconnect_channel_.ResetReadCallback();
  this->disconnect_channel_.Finalize();
  this->read_queue_.Finalize();
  this->write_queue_.Finalize();
  this->connection_.reset();
  this->SetConnectStatus(false);
}

void Connection::NetworkDisconnect() {
  this->SetConnectStatus(false);
  core::TCPConnectionPtr connection = this->connection_.lock();
  if(connection) {
    core::uint64 data = 1;
    ssize_t size = ::write(core::EventPipeWritefd(this->disconnect_fd_),
        &data, sizeof(data));
    if(size != sizeof(data)) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) write to disconnect fd [%lld] bytes data.",
          __FILE__, __LINE__, __FUNCTION__, size);
    }
  }
}

void Connection::ApplicationDisconnect() {
  assert(this->disconnect_channel_.GetEventLoop()->CheckInLoopThread());
  this->write_queue_.Terminal();
}

void Connection::OnApplicationDisconnect(const core::TimeTick &time_tick) {
  if(this->disconnect_callback_) {
    this->disconnect_callback_();
  }
  core::uint64 data = 0;
  ssize_t size = ::read(core::EventPipeReadfd(this->disconnect_fd_), &data, sizeof(data));
  if(size != sizeof(data)) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) read from disconnect fd [%lld] bytes data.",
        __FILE__, __LINE__, __FUNCTION__, size);
  }
  ConnectionPoolSingleton::GetInstance().Deallocate(this->shared_from_this());
}

void Connection::ReceiveMessage(const core::TCPConnectionPtr &connection,
    core::ByteBufferDynamic &buffer) {
  assert(connection->GetLoop()->CheckInLoopThread());
  Packetizer *packetizer = PacketizerTableSingleton::GetInstance().GetPacketizer(
      connection->GetLoop());
  if(packetizer) {
    size_t read_size = 0, message_size = 0, unpacket_size = 0;
    int retcode = 0;
    const ProtocolHead *protocol_head = NULL;
    while(true) {
      read_size = buffer.ReadSize();
      // Can read a message head.
      if(read_size < kProtocolHeadSize) {
        break;
      }
      message_size = GetProtocolSize(buffer.ReadPointer());
      // Can read a whole message.
      if(message_size > VariableTableSingleton::GetInstance().GetMaxProtocolSize()) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Error message size[%llu] in protocol head.",
            __FILE__, __LINE__, __FUNCTION__, message_size);
        connection->Shutdown();
        break;
      } else if(read_size < message_size) {
        break;
      }
      protocol_head = ProtocolHeadCast(buffer.ReadPointer());
      if(GetProtocolEncode(*protocol_head) != (core::uint32)packetizer->GetEncodeType() ||
          GetProtocolCompress(*protocol_head) != (core::uint32)packetizer->GetCompressType()) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Error encode/compress type in protocol head.",
            __FILE__, __LINE__, __FUNCTION__);
        connection->Shutdown();
        break;
      }
      retcode = packetizer->Unpacket(GetProtocolData(buffer.ReadPointer()), message_size);
      if(retcode > 0) {
        const char *message = packetizer->GetUnpacketMessage(unpacket_size);
        retcode = this->read_queue_.Write(message, unpacket_size);
        if((size_t)retcode != unpacket_size) {
          AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
              "%s:%d (%s) Write message to read queue failed [%ld].",
              __FILE__, __LINE__, __FUNCTION__, retcode);
        }
      } else if(retcode == 0) {
        message_size -= kProtocolHeadSize;
        retcode = this->read_queue_.Write(GetProtocolData(buffer.ReadPointer()), message_size);
        if((size_t)retcode != message_size) {
          AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
              "%s:%d (%s) Write message to read queue failed [%ld].",
              __FILE__, __LINE__, __FUNCTION__, retcode);
        }
      } else {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Unpacket message failed and will be ignored",
            __FILE__, __LINE__, __FUNCTION__);
      }
      buffer.ReadShift(message_size + kProtocolHeadSize);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get packetizer failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void Connection::OnTimer(core::uint32 id) {
  if(id == TIMER_READ_MESSAGE_ID) {
    this->OnReadMessageQueue();
  }
}

void Connection::SendMessage(const char *message, size_t size) {
  // assert(this->disconnect_channel_.GetEventLoop()->CheckInLoopThread());
  if(this->GetConnectStatus()) {
    int retcode = this->write_queue_.Write(message, size);
    if((size_t)retcode != size) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Send message failed [%ld].",
          __FILE__, __LINE__, __FUNCTION__, retcode);
    }
  }
}

bool Connection::OnReadMessageQueue() {
  if(this->message_callback_) {
    assert(this->disconnect_channel_.GetEventLoop()->CheckInLoopThread());
    core::SimpleByteBufferDynamic *buffer =
      ReusableBuffersSingleton::GetInstance().GetBuffer(
          this->disconnect_channel_.GetEventLoop());
    if(buffer) {
      size_t max_message_size = VariableTableSingleton::GetInstance().GetMaxMessageSize();
      buffer->PreWrite(max_message_size);
      int retcode = 0;
      while((retcode = this->read_queue_.Read(buffer->HeadPointer(), max_message_size)) > 0) {
        this->message_callback_(buffer->HeadPointer(), retcode);
      }
      if(retcode != MessageQueueError::ERROR_EMPTY) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Read message from read_queue_ failed [%d], shutdown the connection.",
            __FILE__, __LINE__, __FUNCTION__, retcode);
        this->ApplicationDisconnect();
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Get resusable buffer failed, shutdown the connection.",
          __FILE__, __LINE__, __FUNCTION__);
      this->ApplicationDisconnect();
    }
    return true;
  } else {
    this->disconnect_channel_.GetEventLoop()->AddTimer(TIMER_READ_MESSAGE_ID,
        TIMER_READ_MESSAGE_MSEC, 1, this, "Connection::OnReadMessageQueue");
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_WARNING,
        "%s:%d (%s) message_callback_ is null.", __FILE__, __LINE__, __FUNCTION__);
    return true;
  }
}

bool Connection::OnWriteMessageQueue() {
  core::TCPConnectionPtr connection = this->connection_.lock();
  if(connection) {
    assert(connection->GetLoop()->CheckInLoopThread());
    core::SimpleByteBufferDynamic *buffer =
      ReusableBuffersSingleton::GetInstance().GetBuffer(connection->GetLoop());
    Packetizer *packetizer =
      PacketizerTableSingleton::GetInstance().GetPacketizer(connection->GetLoop());
    if(buffer && packetizer) {
      ProtocolHead head = 0;
      size_t packet_size = 0, max_message_size = VariableTableSingleton::GetInstance().GetMaxMessageSize();
      buffer->PreWrite(max_message_size);
      int message_size = 0, retcode = 0;
      while((message_size = this->write_queue_.Read(buffer->HeadPointer(), max_message_size)) > 0) {
        retcode = packetizer->Packet(buffer->HeadPointer(), message_size);
        if(retcode > 0) {
          const char *message = packetizer->GetPacketMessage(packet_size);
          head = packet_size;
          SetProtocolEncode(head, packetizer->GetEncodeType());
          SetProtocolCompress(head, packetizer->GetCompressType());
          connection->SendMessage((const char *)&head, sizeof(head));
          connection->SendMessage(message, packet_size);
        } else if(retcode == 0) {
          head = message_size;
          SetProtocolEncode(head, 0);
          SetProtocolCompress(head, 0);
          connection->SendMessage((const char *)&head, sizeof(head));
          connection->SendMessage(buffer->HeadPointer(), (size_t)message_size);
        } else {
          AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
              "%s:%d (%s) Packet failed", __FILE__, __LINE__, __FUNCTION__);
        }
      }
      if(message_size == MessageQueueError::ERROR_COMPLETE) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
            "%s:%d (%s) Application disconnected.",
            __FILE__, __LINE__, __FUNCTION__, message_size);
        connection->Shutdown();
      } else if(message_size != MessageQueueError::ERROR_EMPTY) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Read message from write_queue_ failed [%d], shutdown the connection.",
            __FILE__, __LINE__, __FUNCTION__, message_size);
        connection->Shutdown();
      }
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Get resusable buffer or Packetizer failed.",
          __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) connection is not available.", __FILE__, __LINE__, __FUNCTION__);
  }
  return true;
}

}  // namespace coresh

