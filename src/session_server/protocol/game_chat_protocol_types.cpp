/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "game_chat_protocol_types.h"

#include <algorithm>

namespace session { namespace protocol {

const char* MessageChatRequest::ascii_fingerprint = "A78EA985455C10F579EE2F62E554F11A";
const uint8_t MessageChatRequest::binary_fingerprint[16] = {0xA7,0x8E,0xA9,0x85,0x45,0x5C,0x10,0xF5,0x79,0xEE,0x2F,0x62,0xE5,0x54,0xF1,0x1A};

uint32_t MessageChatRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_actor_id_ = false;
  bool isset_channel_ = false;
  bool isset_message_ = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->actor_id_);
          isset_actor_id_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast0;
          xfer += iprot->readI32(ecast0);
          this->channel_ = ( ::entity::ChatChannelType::type)ecast0;
          isset_channel_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->message_);
          isset_message_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->target_name_);
          this->__isset.target_name_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_actor_id_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_channel_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_message_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t MessageChatRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("MessageChatRequest");

  xfer += oprot->writeFieldBegin("actor_id_", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->actor_id_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("channel_", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32((int32_t)this->channel_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("message_", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->message_);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.target_name_) {
    xfer += oprot->writeFieldBegin("target_name_", ::apache::thrift::protocol::T_STRING, 4);
    xfer += oprot->writeString(this->target_name_);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MessageChatRequest &a, MessageChatRequest &b) {
  using ::std::swap;
  swap(a.actor_id_, b.actor_id_);
  swap(a.channel_, b.channel_);
  swap(a.message_, b.message_);
  swap(a.target_name_, b.target_name_);
  swap(a.__isset, b.__isset);
}

}} // namespace