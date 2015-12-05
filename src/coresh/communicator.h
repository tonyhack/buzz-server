//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-08 17:13:26.
// File name: communicator.h
//
// Description:
// Define class Communicator.
//

#ifndef __CORESH__COMMUNICATOR__H
#define __CORESH__COMMUNICATOR__H

#include <ext/hash_map>

#include <boost/function.hpp>
#include <boost/signal.hpp>
#include <boost/shared_ptr.hpp>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace coresh {

namespace communicator {

class CommunicatorKey {
 public:
  CommunicatorKey() : message_(0), channel_(0), channel_type_(0) {}
  CommunicatorKey(core::uint32 message, core::uint64 channel, core::uint8 channel_type)
    : message_(message), channel_(channel), channel_type_(channel_type) {}

  bool operator==(const CommunicatorKey &key) const {
    return this->message_ == key.message_ &&
      this->channel_ == key.channel_ &&
      this->channel_type_ == key.channel_type_;
  }
  bool operator<(const CommunicatorKey &key) const {
    if(this->message_ != key.message_) {
      return this->message_ < key.message_;
    } else if(this->channel_ != key.channel_) {
      return this->channel_ < key.channel_;
    } else {
      return this->channel_type_ < key.channel_type_;
    }
  }

  core::uint32 message_;
  core::uint64 channel_;
  core::uint8 channel_type_;
};

class HashFunction {
 public:
  size_t operator()(const CommunicatorKey &key) const {
    return key.message_ + key.channel_;
  }
};

}  // namespace communicator

class Communicator : public core::Noncopyable {
 typedef boost::signal<void (core::uint64, core::uint8, const void *, size_t)> Signal;
 typedef boost::shared_ptr<Signal> SignalPtr;
 typedef __gnu_cxx::hash_map<communicator::CommunicatorKey, SignalPtr,
         communicator::HashFunction> CommunicatorHashmap;

 public:
  typedef boost::signals::connection Connection;
  typedef Signal::slot_function_type SubscriberCallback;

  Communicator();
  ~Communicator();

  // Follow/Unfollow message.
  Connection Follow(core::uint32 message_type, core::uint64 channel,
      core::uint8 channel_type, const SubscriberCallback &callback);
  void Unfollow(const Connection &connection);

  // Broadcast message.
  void Broadcast(core::uint32 message_type, core::uint64 channel,
      core::uint8 channel_type, const void *message, size_t size);

  static core::uint64 GetChannelAll() {
    return Communicator::kChannelAll_;
  }

 private:
  static core::uint64 kChannelAll_;

  CommunicatorHashmap communicators_;
};

class CommunicatorSingleton : public core::Noncopyable {
 public:
  CommunicatorSingleton() {}
  ~CommunicatorSingleton() {}

  static Communicator &GetInstance() {
    return CommunicatorSingleton::instance_;
  }

 private:
  static Communicator instance_;
};

}  // namespace coresh

#endif  // __CORESH__COMMUNICATOR__H

