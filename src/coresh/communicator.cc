//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-08 17:36:00.
// File name: communicator.cc
//
// Description:
// Define calss Communicator.
//

#include "coresh/communicator.h"

namespace coresh {

core::uint64 Communicator::kChannelAll_ = 0;

Communicator CommunicatorSingleton::instance_;

Communicator::Communicator() {}
Communicator::~Communicator() {}

Communicator::Connection Communicator::Follow(core::uint32 message_type,
    core::uint64 channel, core::uint8 channel_type, const SubscriberCallback &callback) {
  communicator::CommunicatorKey key(message_type, channel, channel_type);
  CommunicatorHashmap::iterator iterator = this->communicators_.find(key);
  if(iterator == this->communicators_.end()) {
    SignalPtr communicator(new Signal);
    assert(communicator);
    Communicator::Connection connection = communicator->connect(callback);
    this->communicators_.insert(std::make_pair(key, communicator));
    return connection;
  } else {
    return iterator->second->connect(callback);
  }
}

void Communicator::Unfollow(const Connection &connection) {
  connection.disconnect();
}

void Communicator::Broadcast(core::uint32 message_type,
    core::uint64 channel, core::uint8 channel_type, const void *message, size_t size) {
  if(channel != Communicator::GetChannelAll()) {
    communicator::CommunicatorKey key(message_type, channel, channel_type);
    CommunicatorHashmap::iterator iterator = this->communicators_.find(key);
    if(iterator != this->communicators_.end()) {
      (*iterator->second)(channel, channel_type, message, size);
    }
  }
  communicator::CommunicatorKey key2(message_type,
      Communicator::GetChannelAll(), channel_type);
  CommunicatorHashmap::iterator iterator = this->communicators_.find(key2);
  if(iterator != this->communicators_.end()) {
    (*iterator->second)(channel, channel_type, message, size);
  }
}

}  // namespace coresh

