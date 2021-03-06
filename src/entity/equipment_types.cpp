/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "equipment_types.h"

#include <algorithm>

namespace entity {

int _kEquipAttributeTypeValues[] = {
  EquipAttributeType::MIN,
  EquipAttributeType::PHYSIQUE,
  EquipAttributeType::STRENGTH,
  EquipAttributeType::INTELLIGENCE,
  EquipAttributeType::AGILE,
  EquipAttributeType::SPIRIT,
  EquipAttributeType::MAX_HP,
  EquipAttributeType::MAX_MP,
  EquipAttributeType::PHYSICS_ATTACK,
  EquipAttributeType::PHYSICS_DEFENCE,
  EquipAttributeType::MAGIC_ATTACK,
  EquipAttributeType::MAGIC_DEFENCE,
  EquipAttributeType::DODGE,
  EquipAttributeType::CRIT,
  EquipAttributeType::IGNORE_DEFENCE,
  EquipAttributeType::MOVE_SPEED,
  EquipAttributeType::MAX
};
const char* _kEquipAttributeTypeNames[] = {
  "MIN",
  "PHYSIQUE",
  "STRENGTH",
  "INTELLIGENCE",
  "AGILE",
  "SPIRIT",
  "MAX_HP",
  "MAX_MP",
  "PHYSICS_ATTACK",
  "PHYSICS_DEFENCE",
  "MAGIC_ATTACK",
  "MAGIC_DEFENCE",
  "DODGE",
  "CRIT",
  "IGNORE_DEFENCE",
  "MOVE_SPEED",
  "MAX"
};
const std::map<int, const char*> _EquipAttributeType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(17, _kEquipAttributeTypeValues, _kEquipAttributeTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kEquipClientFieldsValues[] = {
  EquipClientFields::CLIENT_BEGIN,
  EquipClientFields::INTENSIFY,
  EquipClientFields::MAX_INTENSIFY,
  EquipClientFields::RANDOM_ATTR_1,
  EquipClientFields::RANDOM_VALUE_1,
  EquipClientFields::RANDOM_ATTR_2,
  EquipClientFields::RANDOM_VALUE_2,
  EquipClientFields::RANDOM_ATTR_3,
  EquipClientFields::RANDOM_VALUE_3,
  EquipClientFields::RANDOM_ATTR_4,
  EquipClientFields::RANDOM_VALUE_4,
  EquipClientFields::RANDOM_ATTR_5,
  EquipClientFields::RANDOM_VALUE_5,
  EquipClientFields::RANDOM_ATTR_6,
  EquipClientFields::RANDOM_VALUE_6,
  EquipClientFields::UPGRADE_LUCKY,
  EquipClientFields::CLIENT_END
};
const char* _kEquipClientFieldsNames[] = {
  "CLIENT_BEGIN",
  "INTENSIFY",
  "MAX_INTENSIFY",
  "RANDOM_ATTR_1",
  "RANDOM_VALUE_1",
  "RANDOM_ATTR_2",
  "RANDOM_VALUE_2",
  "RANDOM_ATTR_3",
  "RANDOM_VALUE_3",
  "RANDOM_ATTR_4",
  "RANDOM_VALUE_4",
  "RANDOM_ATTR_5",
  "RANDOM_VALUE_5",
  "RANDOM_ATTR_6",
  "RANDOM_VALUE_6",
  "UPGRADE_LUCKY",
  "CLIENT_END"
};
const std::map<int, const char*> _EquipClientFields_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(17, _kEquipClientFieldsValues, _kEquipClientFieldsNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kEquipServerFieldsValues[] = {
  EquipServerFields::SERVER_BEGIN,
  EquipServerFields::SERVER_END
};
const char* _kEquipServerFieldsNames[] = {
  "SERVER_BEGIN",
  "SERVER_END"
};
const std::map<int, const char*> _EquipServerFields_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(2, _kEquipServerFieldsValues, _kEquipServerFieldsNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

const char* EquipClientField::ascii_fingerprint = "46A703A33337BED2F62F386FC66B2A5F";
const uint8_t EquipClientField::binary_fingerprint[16] = {0x46,0xA7,0x03,0xA3,0x33,0x37,0xBE,0xD2,0xF6,0x2F,0x38,0x6F,0xC6,0x6B,0x2A,0x5F};

uint32_t EquipClientField::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_field_ = false;
  bool isset_value_ = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast0;
          xfer += iprot->readI32(ecast0);
          this->field_ = (EquipClientFields::type)ecast0;
          isset_field_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->value_);
          isset_value_ = true;
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

  if (!isset_field_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_value_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t EquipClientField::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("EquipClientField");

  xfer += oprot->writeFieldBegin("field_", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->field_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value_", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->value_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(EquipClientField &a, EquipClientField &b) {
  using ::std::swap;
  swap(a.field_, b.field_);
  swap(a.value_, b.value_);
}

const char* EquipServerField::ascii_fingerprint = "46A703A33337BED2F62F386FC66B2A5F";
const uint8_t EquipServerField::binary_fingerprint[16] = {0x46,0xA7,0x03,0xA3,0x33,0x37,0xBE,0xD2,0xF6,0x2F,0x38,0x6F,0xC6,0x6B,0x2A,0x5F};

uint32_t EquipServerField::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_field_ = false;
  bool isset_value_ = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast1;
          xfer += iprot->readI32(ecast1);
          this->field_ = (EquipServerFields::type)ecast1;
          isset_field_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->value_);
          isset_value_ = true;
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

  if (!isset_field_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_value_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t EquipServerField::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("EquipServerField");

  xfer += oprot->writeFieldBegin("field_", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->field_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value_", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->value_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(EquipServerField &a, EquipServerField &b) {
  using ::std::swap;
  swap(a.field_, b.field_);
  swap(a.value_, b.value_);
}

const char* EquipFacadeField::ascii_fingerprint = "7E45131B6619DD5D7A5FB9DC011E869F";
const uint8_t EquipFacadeField::binary_fingerprint[16] = {0x7E,0x45,0x13,0x1B,0x66,0x19,0xDD,0x5D,0x7A,0x5F,0xB9,0xDC,0x01,0x1E,0x86,0x9F};

uint32_t EquipFacadeField::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_location_ = false;
  bool isset_template_id_ = false;
  bool isset_intensify_ = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast2;
          xfer += iprot->readI32(ecast2);
          this->location_ = ( ::entity::EquipmentType::type)ecast2;
          isset_location_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->template_id_);
          isset_template_id_ = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_BYTE) {
          xfer += iprot->readByte(this->intensify_);
          isset_intensify_ = true;
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

  if (!isset_location_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_template_id_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_intensify_)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t EquipFacadeField::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("EquipFacadeField");

  xfer += oprot->writeFieldBegin("location_", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->location_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("template_id_", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->template_id_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("intensify_", ::apache::thrift::protocol::T_BYTE, 3);
  xfer += oprot->writeByte(this->intensify_);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(EquipFacadeField &a, EquipFacadeField &b) {
  using ::std::swap;
  swap(a.location_, b.location_);
  swap(a.template_id_, b.template_id_);
  swap(a.intensify_, b.intensify_);
}

} // namespace
