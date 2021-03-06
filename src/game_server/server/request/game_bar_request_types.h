/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef game_bar_request_TYPES_H
#define game_bar_request_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "bar_types.h"
#include "game_request_types.h"


namespace game { namespace server { namespace request {


class RequestBarStart {
 public:

  static const char* ascii_fingerprint; // = "1436097DE98403ECF290FF7F19A88437";
  static const uint8_t binary_fingerprint[16]; // = {0x14,0x36,0x09,0x7D,0xE9,0x84,0x03,0xEC,0xF2,0x90,0xFF,0x7F,0x19,0xA8,0x84,0x37};

  RequestBarStart() : actor_id_(0), bar_id_(0), type_(( ::entity::BarModuleType::type)0) {
  }

  virtual ~RequestBarStart() throw() {}

  int64_t actor_id_;
  int32_t bar_id_;
   ::entity::BarModuleType::type type_;

  void __set_actor_id_(const int64_t val) {
    actor_id_ = val;
  }

  void __set_bar_id_(const int32_t val) {
    bar_id_ = val;
  }

  void __set_type_(const  ::entity::BarModuleType::type val) {
    type_ = val;
  }

  bool operator == (const RequestBarStart & rhs) const
  {
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    if (!(bar_id_ == rhs.bar_id_))
      return false;
    if (!(type_ == rhs.type_))
      return false;
    return true;
  }
  bool operator != (const RequestBarStart &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RequestBarStart & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RequestBarStart &a, RequestBarStart &b);


class RequestBarCheck {
 public:

  static const char* ascii_fingerprint; // = "9C2A05F173B50306037BDE9AE30E1B99";
  static const uint8_t binary_fingerprint[16]; // = {0x9C,0x2A,0x05,0xF1,0x73,0xB5,0x03,0x06,0x03,0x7B,0xDE,0x9A,0xE3,0x0E,0x1B,0x99};

  RequestBarCheck() : bar_id_(0), actor_id_(0) {
  }

  virtual ~RequestBarCheck() throw() {}

  int32_t bar_id_;
  int64_t actor_id_;

  void __set_bar_id_(const int32_t val) {
    bar_id_ = val;
  }

  void __set_actor_id_(const int64_t val) {
    actor_id_ = val;
  }

  bool operator == (const RequestBarCheck & rhs) const
  {
    if (!(bar_id_ == rhs.bar_id_))
      return false;
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    return true;
  }
  bool operator != (const RequestBarCheck &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RequestBarCheck & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RequestBarCheck &a, RequestBarCheck &b);

}}} // namespace

#endif
