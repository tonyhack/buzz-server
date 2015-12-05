/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef game_ai_event_TYPES_H
#define game_ai_event_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "entity_types.h"
#include "game_event_types.h"


namespace game { namespace server { namespace event {


class EventAiThreatStart {
 public:

  static const char* ascii_fingerprint; // = "1AB3A1CF9912FDEF32C4878AE56EE1B0";
  static const uint8_t binary_fingerprint[16]; // = {0x1A,0xB3,0xA1,0xCF,0x99,0x12,0xFD,0xEF,0x32,0xC4,0x87,0x8A,0xE5,0x6E,0xE1,0xB0};

  EventAiThreatStart() : threat_role_type_(( ::entity::EntityType::type)0), threat_role_id_(0), npc_(0) {
  }

  virtual ~EventAiThreatStart() throw() {}

   ::entity::EntityType::type threat_role_type_;
  int64_t threat_role_id_;
  int64_t npc_;

  void __set_threat_role_type_(const  ::entity::EntityType::type val) {
    threat_role_type_ = val;
  }

  void __set_threat_role_id_(const int64_t val) {
    threat_role_id_ = val;
  }

  void __set_npc_(const int64_t val) {
    npc_ = val;
  }

  bool operator == (const EventAiThreatStart & rhs) const
  {
    if (!(threat_role_type_ == rhs.threat_role_type_))
      return false;
    if (!(threat_role_id_ == rhs.threat_role_id_))
      return false;
    if (!(npc_ == rhs.npc_))
      return false;
    return true;
  }
  bool operator != (const EventAiThreatStart &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EventAiThreatStart & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(EventAiThreatStart &a, EventAiThreatStart &b);


class EventAiThreatEnd {
 public:

  static const char* ascii_fingerprint; // = "1AB3A1CF9912FDEF32C4878AE56EE1B0";
  static const uint8_t binary_fingerprint[16]; // = {0x1A,0xB3,0xA1,0xCF,0x99,0x12,0xFD,0xEF,0x32,0xC4,0x87,0x8A,0xE5,0x6E,0xE1,0xB0};

  EventAiThreatEnd() : threat_role_type_(( ::entity::EntityType::type)0), threat_role_id_(0), npc_(0) {
  }

  virtual ~EventAiThreatEnd() throw() {}

   ::entity::EntityType::type threat_role_type_;
  int64_t threat_role_id_;
  int64_t npc_;

  void __set_threat_role_type_(const  ::entity::EntityType::type val) {
    threat_role_type_ = val;
  }

  void __set_threat_role_id_(const int64_t val) {
    threat_role_id_ = val;
  }

  void __set_npc_(const int64_t val) {
    npc_ = val;
  }

  bool operator == (const EventAiThreatEnd & rhs) const
  {
    if (!(threat_role_type_ == rhs.threat_role_type_))
      return false;
    if (!(threat_role_id_ == rhs.threat_role_id_))
      return false;
    if (!(npc_ == rhs.npc_))
      return false;
    return true;
  }
  bool operator != (const EventAiThreatEnd &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EventAiThreatEnd & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(EventAiThreatEnd &a, EventAiThreatEnd &b);

}}} // namespace

#endif