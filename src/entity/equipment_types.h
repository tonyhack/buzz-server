/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef equipment_TYPES_H
#define equipment_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "item_types.h"


namespace entity {

struct EquipAttributeType {
  enum type {
    MIN = 0,
    PHYSIQUE = 1,
    STRENGTH = 2,
    INTELLIGENCE = 3,
    AGILE = 4,
    SPIRIT = 5,
    MAX_HP = 6,
    MAX_MP = 7,
    PHYSICS_ATTACK = 8,
    PHYSICS_DEFENCE = 9,
    MAGIC_ATTACK = 10,
    MAGIC_DEFENCE = 11,
    DODGE = 12,
    CRIT = 13,
    IGNORE_DEFENCE = 14,
    MOVE_SPEED = 15,
    MAX = 16
  };
};

extern const std::map<int, const char*> _EquipAttributeType_VALUES_TO_NAMES;

struct EquipClientFields {
  enum type {
    CLIENT_BEGIN = 0,
    INTENSIFY = 0,
    MAX_INTENSIFY = 1,
    RANDOM_ATTR_1 = 2,
    RANDOM_VALUE_1 = 3,
    RANDOM_ATTR_2 = 4,
    RANDOM_VALUE_2 = 5,
    RANDOM_ATTR_3 = 6,
    RANDOM_VALUE_3 = 7,
    RANDOM_ATTR_4 = 8,
    RANDOM_VALUE_4 = 9,
    RANDOM_ATTR_5 = 10,
    RANDOM_VALUE_5 = 11,
    RANDOM_ATTR_6 = 12,
    RANDOM_VALUE_6 = 13,
    UPGRADE_LUCKY = 14,
    CLIENT_END = 15
  };
};

extern const std::map<int, const char*> _EquipClientFields_VALUES_TO_NAMES;

struct EquipServerFields {
  enum type {
    SERVER_BEGIN = 0,
    SERVER_END = 1
  };
};

extern const std::map<int, const char*> _EquipServerFields_VALUES_TO_NAMES;


class EquipClientField {
 public:

  static const char* ascii_fingerprint; // = "46A703A33337BED2F62F386FC66B2A5F";
  static const uint8_t binary_fingerprint[16]; // = {0x46,0xA7,0x03,0xA3,0x33,0x37,0xBE,0xD2,0xF6,0x2F,0x38,0x6F,0xC6,0x6B,0x2A,0x5F};

  EquipClientField() : field_((EquipClientFields::type)0), value_(0) {
  }

  virtual ~EquipClientField() throw() {}

  EquipClientFields::type field_;
  int32_t value_;

  void __set_field_(const EquipClientFields::type val) {
    field_ = val;
  }

  void __set_value_(const int32_t val) {
    value_ = val;
  }

  bool operator == (const EquipClientField & rhs) const
  {
    if (!(field_ == rhs.field_))
      return false;
    if (!(value_ == rhs.value_))
      return false;
    return true;
  }
  bool operator != (const EquipClientField &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EquipClientField & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(EquipClientField &a, EquipClientField &b);


class EquipServerField {
 public:

  static const char* ascii_fingerprint; // = "46A703A33337BED2F62F386FC66B2A5F";
  static const uint8_t binary_fingerprint[16]; // = {0x46,0xA7,0x03,0xA3,0x33,0x37,0xBE,0xD2,0xF6,0x2F,0x38,0x6F,0xC6,0x6B,0x2A,0x5F};

  EquipServerField() : field_((EquipServerFields::type)0), value_(0) {
  }

  virtual ~EquipServerField() throw() {}

  EquipServerFields::type field_;
  int32_t value_;

  void __set_field_(const EquipServerFields::type val) {
    field_ = val;
  }

  void __set_value_(const int32_t val) {
    value_ = val;
  }

  bool operator == (const EquipServerField & rhs) const
  {
    if (!(field_ == rhs.field_))
      return false;
    if (!(value_ == rhs.value_))
      return false;
    return true;
  }
  bool operator != (const EquipServerField &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EquipServerField & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(EquipServerField &a, EquipServerField &b);


class EquipFacadeField {
 public:

  static const char* ascii_fingerprint; // = "7E45131B6619DD5D7A5FB9DC011E869F";
  static const uint8_t binary_fingerprint[16]; // = {0x7E,0x45,0x13,0x1B,0x66,0x19,0xDD,0x5D,0x7A,0x5F,0xB9,0xDC,0x01,0x1E,0x86,0x9F};

  EquipFacadeField() : location_(( ::entity::EquipmentType::type)0), template_id_(0), intensify_(0) {
  }

  virtual ~EquipFacadeField() throw() {}

   ::entity::EquipmentType::type location_;
  int32_t template_id_;
  int8_t intensify_;

  void __set_location_(const  ::entity::EquipmentType::type val) {
    location_ = val;
  }

  void __set_template_id_(const int32_t val) {
    template_id_ = val;
  }

  void __set_intensify_(const int8_t val) {
    intensify_ = val;
  }

  bool operator == (const EquipFacadeField & rhs) const
  {
    if (!(location_ == rhs.location_))
      return false;
    if (!(template_id_ == rhs.template_id_))
      return false;
    if (!(intensify_ == rhs.intensify_))
      return false;
    return true;
  }
  bool operator != (const EquipFacadeField &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EquipFacadeField & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(EquipFacadeField &a, EquipFacadeField &b);

} // namespace

#endif
