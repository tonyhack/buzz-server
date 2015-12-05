/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "session_playing_request_types.h"

#include <algorithm>

namespace session { namespace server { namespace request {

const char* RequestPlayingActorConnectInfo::ascii_fingerprint = "37E83EB5F8F9ABE2473651E6C51F0AB9";
const uint8_t RequestPlayingActorConnectInfo::binary_fingerprint[16] = {0x37,0xE8,0x3E,0xB5,0xF8,0xF9,0xAB,0xE2,0x47,0x36,0x51,0xE6,0xC5,0x1F,0x0A,0xB9};

uint32_t RequestPlayingActorConnectInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_req_actor_ = false;
  bool isset_ret_playing_ = false;
  bool isset_ret_playing_template_ = false;
  bool isset_ret_scene_ = false;
  bool isset_ret_online_back_to_ = false;

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
          xfer += iprot->readI64(this->req_actor_);
          isset_req_actor_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->ret_playing_);
          isset_ret_playing_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->ret_playing_template_);
          isset_ret_playing_template_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->ret_scene_);
          isset_ret_scene_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->ret_online_back_to_);
          isset_ret_online_back_to_ = true;
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

  if (!isset_req_actor_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_ret_playing_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_ret_playing_template_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_ret_scene_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_ret_online_back_to_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t RequestPlayingActorConnectInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("RequestPlayingActorConnectInfo");

  xfer += oprot->writeFieldBegin("req_actor_", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->req_actor_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ret_playing_", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->ret_playing_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ret_playing_template_", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->ret_playing_template_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ret_scene_", ::apache::thrift::protocol::T_I64, 4);
  xfer += oprot->writeI64(this->ret_scene_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ret_online_back_to_", ::apache::thrift::protocol::T_BOOL, 5);
  xfer += oprot->writeBool(this->ret_online_back_to_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(RequestPlayingActorConnectInfo &a, RequestPlayingActorConnectInfo &b) {
  using ::std::swap;
  swap(a.req_actor_, b.req_actor_);
  swap(a.ret_playing_, b.ret_playing_);
  swap(a.ret_playing_template_, b.ret_playing_template_);
  swap(a.ret_scene_, b.ret_scene_);
  swap(a.ret_online_back_to_, b.ret_online_back_to_);
}

}}} // namespace