/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef game_task_request_TYPES_H
#define game_task_request_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "game_request_types.h"
#include "task_types.h"


namespace game { namespace server { namespace request {

struct RequestTaskOptType {
  enum type {
    ACCEPT = 0,
    FINISH = 1
  };
};

extern const std::map<int, const char*> _RequestTaskOptType_VALUES_TO_NAMES;


class RequestTaskAccept {
 public:

  static const char* ascii_fingerprint; // = "E2E24E9BC4DF93789792D14330D3E8BC";
  static const uint8_t binary_fingerprint[16]; // = {0xE2,0xE2,0x4E,0x9B,0xC4,0xDF,0x93,0x78,0x97,0x92,0xD1,0x43,0x30,0xD3,0xE8,0xBC};

  RequestTaskAccept() : task_id_(0), actor_id_(0), type_(( ::entity::TaskType::type)0) {
  }

  virtual ~RequestTaskAccept() throw() {}

  int32_t task_id_;
  int64_t actor_id_;
   ::entity::TaskType::type type_;

  void __set_task_id_(const int32_t val) {
    task_id_ = val;
  }

  void __set_actor_id_(const int64_t val) {
    actor_id_ = val;
  }

  void __set_type_(const  ::entity::TaskType::type val) {
    type_ = val;
  }

  bool operator == (const RequestTaskAccept & rhs) const
  {
    if (!(task_id_ == rhs.task_id_))
      return false;
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    if (!(type_ == rhs.type_))
      return false;
    return true;
  }
  bool operator != (const RequestTaskAccept &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RequestTaskAccept & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RequestTaskAccept &a, RequestTaskAccept &b);


class RequestTaskCheckGather {
 public:

  static const char* ascii_fingerprint; // = "9C2A05F173B50306037BDE9AE30E1B99";
  static const uint8_t binary_fingerprint[16]; // = {0x9C,0x2A,0x05,0xF1,0x73,0xB5,0x03,0x06,0x03,0x7B,0xDE,0x9A,0xE3,0x0E,0x1B,0x99};

  RequestTaskCheckGather() : npc_template_(0), actor_id_(0) {
  }

  virtual ~RequestTaskCheckGather() throw() {}

  int32_t npc_template_;
  int64_t actor_id_;

  void __set_npc_template_(const int32_t val) {
    npc_template_ = val;
  }

  void __set_actor_id_(const int64_t val) {
    actor_id_ = val;
  }

  bool operator == (const RequestTaskCheckGather & rhs) const
  {
    if (!(npc_template_ == rhs.npc_template_))
      return false;
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    return true;
  }
  bool operator != (const RequestTaskCheckGather &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RequestTaskCheckGather & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RequestTaskCheckGather &a, RequestTaskCheckGather &b);


class RequestTaskMaxLayer {
 public:

  static const char* ascii_fingerprint; // = "AFAFBCDB9822F9D1AA4E44188E720B47";
  static const uint8_t binary_fingerprint[16]; // = {0xAF,0xAF,0xBC,0xDB,0x98,0x22,0xF9,0xD1,0xAA,0x4E,0x44,0x18,0x8E,0x72,0x0B,0x47};

  RequestTaskMaxLayer() : actor_id_(0), task_layer_(0) {
  }

  virtual ~RequestTaskMaxLayer() throw() {}

  int64_t actor_id_;
  int32_t task_layer_;

  void __set_actor_id_(const int64_t val) {
    actor_id_ = val;
  }

  void __set_task_layer_(const int32_t val) {
    task_layer_ = val;
  }

  bool operator == (const RequestTaskMaxLayer & rhs) const
  {
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    if (!(task_layer_ == rhs.task_layer_))
      return false;
    return true;
  }
  bool operator != (const RequestTaskMaxLayer &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RequestTaskMaxLayer & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RequestTaskMaxLayer &a, RequestTaskMaxLayer &b);


class RequestTaskConditionCoordinate {
 public:

  static const char* ascii_fingerprint; // = "7F9433AE415529774108A73E2A9C2675";
  static const uint8_t binary_fingerprint[16]; // = {0x7F,0x94,0x33,0xAE,0x41,0x55,0x29,0x77,0x41,0x08,0xA7,0x3E,0x2A,0x9C,0x26,0x75};

  RequestTaskConditionCoordinate() : actor_id_(0), type_(( ::entity::TaskType::type)0), task_id_(0), condition_pos_(0), map_id_(0), x_(0), y_(0) {
  }

  virtual ~RequestTaskConditionCoordinate() throw() {}

  int64_t actor_id_;
   ::entity::TaskType::type type_;
  int32_t task_id_;
  int32_t condition_pos_;
  int32_t map_id_;
  int32_t x_;
  int32_t y_;

  void __set_actor_id_(const int64_t val) {
    actor_id_ = val;
  }

  void __set_type_(const  ::entity::TaskType::type val) {
    type_ = val;
  }

  void __set_task_id_(const int32_t val) {
    task_id_ = val;
  }

  void __set_condition_pos_(const int32_t val) {
    condition_pos_ = val;
  }

  void __set_map_id_(const int32_t val) {
    map_id_ = val;
  }

  void __set_x_(const int32_t val) {
    x_ = val;
  }

  void __set_y_(const int32_t val) {
    y_ = val;
  }

  bool operator == (const RequestTaskConditionCoordinate & rhs) const
  {
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    if (!(type_ == rhs.type_))
      return false;
    if (!(task_id_ == rhs.task_id_))
      return false;
    if (!(condition_pos_ == rhs.condition_pos_))
      return false;
    if (!(map_id_ == rhs.map_id_))
      return false;
    if (!(x_ == rhs.x_))
      return false;
    if (!(y_ == rhs.y_))
      return false;
    return true;
  }
  bool operator != (const RequestTaskConditionCoordinate &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RequestTaskConditionCoordinate & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RequestTaskConditionCoordinate &a, RequestTaskConditionCoordinate &b);


class RequestTaskAcceptAndFinishCoordinate {
 public:

  static const char* ascii_fingerprint; // = "17717B3620341B2C0CC9164DE787961F";
  static const uint8_t binary_fingerprint[16]; // = {0x17,0x71,0x7B,0x36,0x20,0x34,0x1B,0x2C,0x0C,0xC9,0x16,0x4D,0xE7,0x87,0x96,0x1F};

  RequestTaskAcceptAndFinishCoordinate() : actor_id_(0), type_(( ::entity::TaskType::type)0), task_id_(0), opt_type_((RequestTaskOptType::type)0), map_id_(0), x_(0), y_(0) {
  }

  virtual ~RequestTaskAcceptAndFinishCoordinate() throw() {}

  int64_t actor_id_;
   ::entity::TaskType::type type_;
  int32_t task_id_;
  RequestTaskOptType::type opt_type_;
  int32_t map_id_;
  int32_t x_;
  int32_t y_;

  void __set_actor_id_(const int64_t val) {
    actor_id_ = val;
  }

  void __set_type_(const  ::entity::TaskType::type val) {
    type_ = val;
  }

  void __set_task_id_(const int32_t val) {
    task_id_ = val;
  }

  void __set_opt_type_(const RequestTaskOptType::type val) {
    opt_type_ = val;
  }

  void __set_map_id_(const int32_t val) {
    map_id_ = val;
  }

  void __set_x_(const int32_t val) {
    x_ = val;
  }

  void __set_y_(const int32_t val) {
    y_ = val;
  }

  bool operator == (const RequestTaskAcceptAndFinishCoordinate & rhs) const
  {
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    if (!(type_ == rhs.type_))
      return false;
    if (!(task_id_ == rhs.task_id_))
      return false;
    if (!(opt_type_ == rhs.opt_type_))
      return false;
    if (!(map_id_ == rhs.map_id_))
      return false;
    if (!(x_ == rhs.x_))
      return false;
    if (!(y_ == rhs.y_))
      return false;
    return true;
  }
  bool operator != (const RequestTaskAcceptAndFinishCoordinate &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RequestTaskAcceptAndFinishCoordinate & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RequestTaskAcceptAndFinishCoordinate &a, RequestTaskAcceptAndFinishCoordinate &b);

}}} // namespace

#endif
