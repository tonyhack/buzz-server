/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef gateway_chat_protocol_TYPES_H
#define gateway_chat_protocol_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "chat_types.h"
#include "gateway_protocol_types.h"


namespace gateway { namespace protocol {

struct ChatErrorCode {
  enum type {
    SUCCESS = 0,
    TARGET_OFFLINE = 1,
    IN_TARGET_BLACKLIST = 2,
    ERROR_UNKNOWN = 3
  };
};

extern const std::map<int, const char*> _ChatErrorCode_VALUES_TO_NAMES;

typedef struct _MessageChatRequest__isset {
  _MessageChatRequest__isset() : target_name_(false) {}
  bool target_name_;
} _MessageChatRequest__isset;

class MessageChatRequest {
 public:

  static const char* ascii_fingerprint; // = "8A168E64138582CD39890B77E0F15FF4";
  static const uint8_t binary_fingerprint[16]; // = {0x8A,0x16,0x8E,0x64,0x13,0x85,0x82,0xCD,0x39,0x89,0x0B,0x77,0xE0,0xF1,0x5F,0xF4};

  MessageChatRequest() : channel_(( ::entity::ChatChannelType::type)0), message_(), target_name_() {
  }

  virtual ~MessageChatRequest() throw() {}

   ::entity::ChatChannelType::type channel_;
  std::string message_;
  std::string target_name_;

  _MessageChatRequest__isset __isset;

  void __set_channel_(const  ::entity::ChatChannelType::type val) {
    channel_ = val;
  }

  void __set_message_(const std::string& val) {
    message_ = val;
  }

  void __set_target_name_(const std::string& val) {
    target_name_ = val;
    __isset.target_name_ = true;
  }

  bool operator == (const MessageChatRequest & rhs) const
  {
    if (!(channel_ == rhs.channel_))
      return false;
    if (!(message_ == rhs.message_))
      return false;
    if (__isset.target_name_ != rhs.__isset.target_name_)
      return false;
    else if (__isset.target_name_ && !(target_name_ == rhs.target_name_))
      return false;
    return true;
  }
  bool operator != (const MessageChatRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageChatRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageChatRequest &a, MessageChatRequest &b);

typedef struct _MessageChatResponse__isset {
  _MessageChatResponse__isset() : target_id_(false), target_name_(false), message_(false) {}
  bool target_id_;
  bool target_name_;
  bool message_;
} _MessageChatResponse__isset;

class MessageChatResponse {
 public:

  static const char* ascii_fingerprint; // = "8095EC249A4927E7C686E5BB9D757A08";
  static const uint8_t binary_fingerprint[16]; // = {0x80,0x95,0xEC,0x24,0x9A,0x49,0x27,0xE7,0xC6,0x86,0xE5,0xBB,0x9D,0x75,0x7A,0x08};

  MessageChatResponse() : result_((ChatErrorCode::type)0), target_id_(), target_name_(), message_() {
  }

  virtual ~MessageChatResponse() throw() {}

  ChatErrorCode::type result_;
  std::string target_id_;
  std::string target_name_;
  std::string message_;

  _MessageChatResponse__isset __isset;

  void __set_result_(const ChatErrorCode::type val) {
    result_ = val;
  }

  void __set_target_id_(const std::string& val) {
    target_id_ = val;
    __isset.target_id_ = true;
  }

  void __set_target_name_(const std::string& val) {
    target_name_ = val;
    __isset.target_name_ = true;
  }

  void __set_message_(const std::string& val) {
    message_ = val;
    __isset.message_ = true;
  }

  bool operator == (const MessageChatResponse & rhs) const
  {
    if (!(result_ == rhs.result_))
      return false;
    if (__isset.target_id_ != rhs.__isset.target_id_)
      return false;
    else if (__isset.target_id_ && !(target_id_ == rhs.target_id_))
      return false;
    if (__isset.target_name_ != rhs.__isset.target_name_)
      return false;
    else if (__isset.target_name_ && !(target_name_ == rhs.target_name_))
      return false;
    if (__isset.message_ != rhs.__isset.message_)
      return false;
    else if (__isset.message_ && !(message_ == rhs.message_))
      return false;
    return true;
  }
  bool operator != (const MessageChatResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageChatResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageChatResponse &a, MessageChatResponse &b);


class MessageChatSynchronize {
 public:

  static const char* ascii_fingerprint; // = "EC8C8E8D6042A67E4AF0F04DA3F39A73";
  static const uint8_t binary_fingerprint[16]; // = {0xEC,0x8C,0x8E,0x8D,0x60,0x42,0xA6,0x7E,0x4A,0xF0,0xF0,0x4D,0xA3,0xF3,0x9A,0x73};

  MessageChatSynchronize() : channel_(( ::entity::ChatChannelType::type)0), message_(), actor_id_(), actor_name_() {
  }

  virtual ~MessageChatSynchronize() throw() {}

   ::entity::ChatChannelType::type channel_;
  std::string message_;
  std::string actor_id_;
  std::string actor_name_;

  void __set_channel_(const  ::entity::ChatChannelType::type val) {
    channel_ = val;
  }

  void __set_message_(const std::string& val) {
    message_ = val;
  }

  void __set_actor_id_(const std::string& val) {
    actor_id_ = val;
  }

  void __set_actor_name_(const std::string& val) {
    actor_name_ = val;
  }

  bool operator == (const MessageChatSynchronize & rhs) const
  {
    if (!(channel_ == rhs.channel_))
      return false;
    if (!(message_ == rhs.message_))
      return false;
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    if (!(actor_name_ == rhs.actor_name_))
      return false;
    return true;
  }
  bool operator != (const MessageChatSynchronize &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageChatSynchronize & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageChatSynchronize &a, MessageChatSynchronize &b);

}} // namespace

#endif
