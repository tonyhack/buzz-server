//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-06 17:51:00.
// File name: pressure_client_set.cc
//
// Description:
// Define class PressureClientSet.
//

#include "test/chat/pressure_client/pressure_client_set.h"

#include "core/async_logging_singleton.h"
#include "coresh/packetizer.h"
#include "coresh/packetizer_table.h"
#include "coresh/variable_table.h"
#include "global/thrift_packet.h"
#include "test/chat/pressure_client/configure.h"
#include "test/chat/pressure_client/multiplex_thrift.h"
#include "test/chat/pressure_client/pressure_client.h"

namespace test {

namespace chat {

namespace pressure {

PressureClientSet *PressureClientSetSingleton::instance_ = NULL;

PressureClientSet::PressureClientSet(const core::InetAddress &server_address,
    const std::string &template_name) : template_name_(template_name),
  server_address_(server_address), thread_group_(NULL) {}

PressureClientSet::~PressureClientSet() {}

bool PressureClientSet::Allocate() {
  this->thread_group_ = new core::EventLoopThreadGroup();
  if(this->thread_group_ == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate thread group failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->thread_group_->Initialize(ConfigureSingleton::GetInstance().thread_number_,
        "PressureClientSet", true) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize thread group failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  const core::EventLoopThreadGroup::LoopGroup *group = this->thread_group_->GetLoopGroup();
  assert(group);
  core::EventLoopThreadGroup::LoopGroup::const_iterator iterator = group->begin();
  for(; iterator != group->end(); ++iterator) {
    global::ThriftPacket *thrift = new global::ThriftPacket();
    if(thrift == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate thrift packet failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(MultiplexThriftSinglton::GetInstance().Insert(*iterator, thrift) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Insert multiplex thrift failed.",
          __FILE__, __LINE__, __FUNCTION__);
      delete thrift;
      return false;
    }
    coresh::Packetizer *packetizer = new (std::nothrow) coresh::Packetizer();
    if(packetizer == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate packetizer failed.",
          __FILE__, __LINE__, __FUNCTION__);
    } else {
      packetizer->SetCompressor(coresh::VariableTableSingleton::GetInstance().GetCompressType());
      packetizer->SetEncoder(coresh::VariableTableSingleton::GetInstance().GetEncodeType());
      if(coresh::PacketizerTableSingleton::GetInstance().Insert(*iterator, packetizer) == false) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Insert packetizer table failed.",
            __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
    }
  }

  return true;
}

void PressureClientSet::Deallocate() {
  const core::EventLoopThreadGroup::LoopGroup *group = this->thread_group_->GetLoopGroup();
  assert(group);
  core::EventLoopThreadGroup::LoopGroup::const_iterator iterator = group->begin();
  for(; iterator != group->end(); ++iterator) {
    global::ThriftPacket *thrift = MultiplexThriftSinglton::GetInstance().Remove(*iterator);
    if(thrift == NULL) {
      delete thrift;
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Remove multiplex thrift failed.",
          __FILE__, __LINE__, __FUNCTION__);
    }
    coresh::Packetizer *packetizer = coresh::PacketizerTableSingleton::GetInstance().Remove(*iterator);
    if(packetizer == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deallocate packetizer failed.",
          __FILE__, __LINE__, __FUNCTION__);
    } else {
      delete packetizer;
    }
  }

  delete this->thread_group_;
  this->thread_group_ = NULL;
}

bool PressureClientSet::Initialize(size_t client_number) {
  char name[64];
  size_t pos = 0;
  for(; pos < client_number; ++pos) {
    snprintf(name, sizeof(name), "%s_%zd", this->template_name_.c_str(), pos);
    PressureClientPtr client(new PressureClient(this->server_address_, name));
    if(client == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate pressure client failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(client->Allocate() == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate2 pressure client failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(client->Initialize(this->thread_group_->GetNextLoop()) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Initialize pressure client failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->clients_.insert(std::make_pair(std::string(name), client));
  }

  return true;
}

void PressureClientSet::Finalize() {
  this->clients_.clear();
}

void PressureClientSet::Start() {
  this->thread_group_->Start();
  ClientMap::iterator iterator = this->clients_.begin();
  for(; iterator != this->clients_.end(); ++iterator) {
    iterator->second->Start();
  }
}

void PressureClientSet::Stop() {
  ClientMap::iterator iterator = this->clients_.begin();
  for(; iterator != this->clients_.end(); ++iterator) {
    iterator->second->Stop();
  }
  this->thread_group_->Stop();
}

}  // namespace pressure

}  // namespace chat

}  // namespace test

