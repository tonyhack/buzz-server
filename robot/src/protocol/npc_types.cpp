/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "npc_types.h"

#include <algorithm>

namespace entity {

int _kNpcKindValues[] = {
  NpcKind::MIN,
  NpcKind::FUNCTION,
  NpcKind::MONSTER,
  NpcKind::TELEPORTER,
  NpcKind::TASK,
  NpcKind::TASK_GATHER,
  NpcKind::GATHER,
  NpcKind::TRANSPORT,
  NpcKind::MAX
};
const char* _kNpcKindNames[] = {
  "MIN",
  "FUNCTION",
  "MONSTER",
  "TELEPORTER",
  "TASK",
  "TASK_GATHER",
  "GATHER",
  "TRANSPORT",
  "MAX"
};
const std::map<int, const char*> _NpcKind_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(9, _kNpcKindValues, _kNpcKindNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kNpcOwnerTypeValues[] = {
  NpcOwnerType::MIN,
  NpcOwnerType::ALL,
  NpcOwnerType::KILL,
  NpcOwnerType::DAMAGE,
  NpcOwnerType::CAMP_COMPETE,
  NpcOwnerType::CAMP_EACH,
  NpcOwnerType::MAX
};
const char* _kNpcOwnerTypeNames[] = {
  "MIN",
  "ALL",
  "KILL",
  "DAMAGE",
  "CAMP_COMPETE",
  "CAMP_EACH",
  "MAX"
};
const std::map<int, const char*> _NpcOwnerType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(7, _kNpcOwnerTypeValues, _kNpcOwnerTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kNpcAoiFieldsValues[] = {
  NpcAoiFields::AOI_BEGIN,
  NpcAoiFields::AOI_END
};
const char* _kNpcAoiFieldsNames[] = {
  "AOI_BEGIN",
  "AOI_END"
};
const std::map<int, const char*> _NpcAoiFields_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(2, _kNpcAoiFieldsValues, _kNpcAoiFieldsNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kNpcClientFieldsValues[] = {
  NpcClientFields::CLIENT_BEGIN,
  NpcClientFields::CLIENT_END
};
const char* _kNpcClientFieldsNames[] = {
  "CLIENT_BEGIN",
  "CLIENT_END"
};
const std::map<int, const char*> _NpcClientFields_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(2, _kNpcClientFieldsValues, _kNpcClientFieldsNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kNpcMobTypeValues[] = {
  NpcMobType::GENERAL_NPC,
  NpcMobType::CREAM_NPC,
  NpcMobType::BOSS_NPC
};
const char* _kNpcMobTypeNames[] = {
  "GENERAL_NPC",
  "CREAM_NPC",
  "BOSS_NPC"
};
const std::map<int, const char*> _NpcMobType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _kNpcMobTypeValues, _kNpcMobTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

const char* NpcAoiField::ascii_fingerprint = "46A703A33337BED2F62F386FC66B2A5F";
const uint8_t NpcAoiField::binary_fingerprint[16] = {0x46,0xA7,0x03,0xA3,0x33,0x37,0xBE,0xD2,0xF6,0x2F,0x38,0x6F,0xC6,0x6B,0x2A,0x5F};

uint32_t NpcAoiField::read(::apache::thrift::protocol::TProtocol* iprot) {

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
          this->field_ = (NpcAoiFields::type)ecast0;
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

uint32_t NpcAoiField::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("NpcAoiField");

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

void swap(NpcAoiField &a, NpcAoiField &b) {
  using ::std::swap;
  swap(a.field_, b.field_);
  swap(a.value_, b.value_);
}

} // namespace