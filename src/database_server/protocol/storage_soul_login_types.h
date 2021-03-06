/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef storage_soul_login_TYPES_H
#define storage_soul_login_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace database { namespace protocol {

struct SoulField {
  enum type {
    ID = 0,
    LEVEL = 1,
    STEP = 2,
    COMMON_SKILL_LEVEL = 3,
    APPEAR_SKILL_LEVEL = 4,
    SOUL_SKILL_LEVEL = 5,
    MAX = 6
  };
};

extern const std::map<int, const char*> _SoulField_VALUES_TO_NAMES;

struct SoulLoginResult {
  enum type {
    SUCCESS = 0,
    ERROR_UNKNOWN = 1
  };
};

extern const std::map<int, const char*> _SoulLoginResult_VALUES_TO_NAMES;


class StorageSoulField {
 public:

  static const char* ascii_fingerprint; // = "62CBF95059CB084430B0BABE2E5A68C7";
  static const uint8_t binary_fingerprint[16]; // = {0x62,0xCB,0xF9,0x50,0x59,0xCB,0x08,0x44,0x30,0xB0,0xBA,0xBE,0x2E,0x5A,0x68,0xC7};

  StorageSoulField() : id_(0), level_(0), step_(0), common_skill_level_(0), appear_skill_level_(0), soul_skill_level_(0) {
  }

  virtual ~StorageSoulField() throw() {}

  int32_t id_;
  int32_t level_;
  int32_t step_;
  int32_t common_skill_level_;
  int32_t appear_skill_level_;
  int32_t soul_skill_level_;

  void __set_id_(const int32_t val) {
    id_ = val;
  }

  void __set_level_(const int32_t val) {
    level_ = val;
  }

  void __set_step_(const int32_t val) {
    step_ = val;
  }

  void __set_common_skill_level_(const int32_t val) {
    common_skill_level_ = val;
  }

  void __set_appear_skill_level_(const int32_t val) {
    appear_skill_level_ = val;
  }

  void __set_soul_skill_level_(const int32_t val) {
    soul_skill_level_ = val;
  }

  bool operator == (const StorageSoulField & rhs) const
  {
    if (!(id_ == rhs.id_))
      return false;
    if (!(level_ == rhs.level_))
      return false;
    if (!(step_ == rhs.step_))
      return false;
    if (!(common_skill_level_ == rhs.common_skill_level_))
      return false;
    if (!(appear_skill_level_ == rhs.appear_skill_level_))
      return false;
    if (!(soul_skill_level_ == rhs.soul_skill_level_))
      return false;
    return true;
  }
  bool operator != (const StorageSoulField &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StorageSoulField & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StorageSoulField &a, StorageSoulField &b);


class StorageSoulLoginRequest {
 public:

  static const char* ascii_fingerprint; // = "56A59CE7FFAF82BCA8A19FAACDE4FB75";
  static const uint8_t binary_fingerprint[16]; // = {0x56,0xA5,0x9C,0xE7,0xFF,0xAF,0x82,0xBC,0xA8,0xA1,0x9F,0xAA,0xCD,0xE4,0xFB,0x75};

  StorageSoulLoginRequest() : actor_id_(0) {
  }

  virtual ~StorageSoulLoginRequest() throw() {}

  int64_t actor_id_;

  void __set_actor_id_(const int64_t val) {
    actor_id_ = val;
  }

  bool operator == (const StorageSoulLoginRequest & rhs) const
  {
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    return true;
  }
  bool operator != (const StorageSoulLoginRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StorageSoulLoginRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StorageSoulLoginRequest &a, StorageSoulLoginRequest &b);


class StorageSoulLoginResponse {
 public:

  static const char* ascii_fingerprint; // = "2B08EB97564FBB4AF730341C4A2653B3";
  static const uint8_t binary_fingerprint[16]; // = {0x2B,0x08,0xEB,0x97,0x56,0x4F,0xBB,0x4A,0xF7,0x30,0x34,0x1C,0x4A,0x26,0x53,0xB3};

  StorageSoulLoginResponse() : result_((SoulLoginResult::type)0) {
  }

  virtual ~StorageSoulLoginResponse() throw() {}

  SoulLoginResult::type result_;
  std::vector<StorageSoulField>  fields_;

  void __set_result_(const SoulLoginResult::type val) {
    result_ = val;
  }

  void __set_fields_(const std::vector<StorageSoulField> & val) {
    fields_ = val;
  }

  bool operator == (const StorageSoulLoginResponse & rhs) const
  {
    if (!(result_ == rhs.result_))
      return false;
    if (!(fields_ == rhs.fields_))
      return false;
    return true;
  }
  bool operator != (const StorageSoulLoginResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StorageSoulLoginResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StorageSoulLoginResponse &a, StorageSoulLoginResponse &b);

}} // namespace

#endif
