/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef storage_item_login_TYPES_H
#define storage_item_login_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace database { namespace protocol {

struct ItemField {
  enum type {
    LOCATION = 0,
    VOCATION = 1,
    TEMPLATE_ID = 2,
    NUMBER = 3,
    BIND = 4,
    INTENSIFY_LEVEL = 5,
    MAX_INTENSIFY_LEVEL = 6,
    RANDOM_ATTR1 = 7,
    RANDOM_ATTR_VALUE1 = 8,
    RANDOM_ATTR2 = 9,
    RANDOM_ATTR_VALUE2 = 10,
    RANDOM_ATTR3 = 11,
    RANDOM_ATTR_VALUE3 = 12,
    RANDOM_ATTR4 = 13,
    RANDOM_ATTR_VALUE4 = 14,
    RANDOM_ATTR5 = 15,
    RANDOM_ATTR_VALUE5 = 16,
    RANDOM_ATTR6 = 17,
    RANDOM_ATTR_VALUE6 = 18,
    FIRST_USE_TIME = 19,
    UPGRADE_LUCKY = 20,
    MAX = 21
  };
};

extern const std::map<int, const char*> _ItemField_VALUES_TO_NAMES;

struct ItemLoginResult {
  enum type {
    SUCCESS = 0,
    ERROR_UNKNOWN = 1
  };
};

extern const std::map<int, const char*> _ItemLoginResult_VALUES_TO_NAMES;


class StorageItemField {
 public:

  static const char* ascii_fingerprint; // = "4C382C23D352A6394F9EF66C9C675982";
  static const uint8_t binary_fingerprint[16]; // = {0x4C,0x38,0x2C,0x23,0xD3,0x52,0xA6,0x39,0x4F,0x9E,0xF6,0x6C,0x9C,0x67,0x59,0x82};

  StorageItemField() : location_(0), vocation_(0), template_id_(0), number_(0), bind_(0), intensify_level_(0), max_intensify_level_(0), random_attr1_(0), random_attr_value1_(0), random_attr2_(0), random_attr_value2_(0), random_attr3_(0), random_attr_value3_(0), random_attr4_(0), random_attr_value4_(0), random_attr5_(0), random_attr_value5_(0), random_attr6_(0), random_attr_value6_(0), first_use_time_(0), upgrade_lucky_(0) {
  }

  virtual ~StorageItemField() throw() {}

  int16_t location_;
  int32_t vocation_;
  int32_t template_id_;
  int32_t number_;
  int8_t bind_;
  int8_t intensify_level_;
  int8_t max_intensify_level_;
  int8_t random_attr1_;
  int32_t random_attr_value1_;
  int8_t random_attr2_;
  int32_t random_attr_value2_;
  int8_t random_attr3_;
  int32_t random_attr_value3_;
  int8_t random_attr4_;
  int32_t random_attr_value4_;
  int8_t random_attr5_;
  int32_t random_attr_value5_;
  int8_t random_attr6_;
  int32_t random_attr_value6_;
  int64_t first_use_time_;
  int32_t upgrade_lucky_;

  void __set_location_(const int16_t val) {
    location_ = val;
  }

  void __set_vocation_(const int32_t val) {
    vocation_ = val;
  }

  void __set_template_id_(const int32_t val) {
    template_id_ = val;
  }

  void __set_number_(const int32_t val) {
    number_ = val;
  }

  void __set_bind_(const int8_t val) {
    bind_ = val;
  }

  void __set_intensify_level_(const int8_t val) {
    intensify_level_ = val;
  }

  void __set_max_intensify_level_(const int8_t val) {
    max_intensify_level_ = val;
  }

  void __set_random_attr1_(const int8_t val) {
    random_attr1_ = val;
  }

  void __set_random_attr_value1_(const int32_t val) {
    random_attr_value1_ = val;
  }

  void __set_random_attr2_(const int8_t val) {
    random_attr2_ = val;
  }

  void __set_random_attr_value2_(const int32_t val) {
    random_attr_value2_ = val;
  }

  void __set_random_attr3_(const int8_t val) {
    random_attr3_ = val;
  }

  void __set_random_attr_value3_(const int32_t val) {
    random_attr_value3_ = val;
  }

  void __set_random_attr4_(const int8_t val) {
    random_attr4_ = val;
  }

  void __set_random_attr_value4_(const int32_t val) {
    random_attr_value4_ = val;
  }

  void __set_random_attr5_(const int8_t val) {
    random_attr5_ = val;
  }

  void __set_random_attr_value5_(const int32_t val) {
    random_attr_value5_ = val;
  }

  void __set_random_attr6_(const int8_t val) {
    random_attr6_ = val;
  }

  void __set_random_attr_value6_(const int32_t val) {
    random_attr_value6_ = val;
  }

  void __set_first_use_time_(const int64_t val) {
    first_use_time_ = val;
  }

  void __set_upgrade_lucky_(const int32_t val) {
    upgrade_lucky_ = val;
  }

  bool operator == (const StorageItemField & rhs) const
  {
    if (!(location_ == rhs.location_))
      return false;
    if (!(vocation_ == rhs.vocation_))
      return false;
    if (!(template_id_ == rhs.template_id_))
      return false;
    if (!(number_ == rhs.number_))
      return false;
    if (!(bind_ == rhs.bind_))
      return false;
    if (!(intensify_level_ == rhs.intensify_level_))
      return false;
    if (!(max_intensify_level_ == rhs.max_intensify_level_))
      return false;
    if (!(random_attr1_ == rhs.random_attr1_))
      return false;
    if (!(random_attr_value1_ == rhs.random_attr_value1_))
      return false;
    if (!(random_attr2_ == rhs.random_attr2_))
      return false;
    if (!(random_attr_value2_ == rhs.random_attr_value2_))
      return false;
    if (!(random_attr3_ == rhs.random_attr3_))
      return false;
    if (!(random_attr_value3_ == rhs.random_attr_value3_))
      return false;
    if (!(random_attr4_ == rhs.random_attr4_))
      return false;
    if (!(random_attr_value4_ == rhs.random_attr_value4_))
      return false;
    if (!(random_attr5_ == rhs.random_attr5_))
      return false;
    if (!(random_attr_value5_ == rhs.random_attr_value5_))
      return false;
    if (!(random_attr6_ == rhs.random_attr6_))
      return false;
    if (!(random_attr_value6_ == rhs.random_attr_value6_))
      return false;
    if (!(first_use_time_ == rhs.first_use_time_))
      return false;
    if (!(upgrade_lucky_ == rhs.upgrade_lucky_))
      return false;
    return true;
  }
  bool operator != (const StorageItemField &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StorageItemField & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StorageItemField &a, StorageItemField &b);


class StorageItemLoginRequest {
 public:

  static const char* ascii_fingerprint; // = "DDB540D232DEE4BB76F25D4DAA0FC261";
  static const uint8_t binary_fingerprint[16]; // = {0xDD,0xB5,0x40,0xD2,0x32,0xDE,0xE4,0xBB,0x76,0xF2,0x5D,0x4D,0xAA,0x0F,0xC2,0x61};

  StorageItemLoginRequest() : actor_id_(0), contain_type_(0) {
  }

  virtual ~StorageItemLoginRequest() throw() {}

  int64_t actor_id_;
  int8_t contain_type_;

  void __set_actor_id_(const int64_t val) {
    actor_id_ = val;
  }

  void __set_contain_type_(const int8_t val) {
    contain_type_ = val;
  }

  bool operator == (const StorageItemLoginRequest & rhs) const
  {
    if (!(actor_id_ == rhs.actor_id_))
      return false;
    if (!(contain_type_ == rhs.contain_type_))
      return false;
    return true;
  }
  bool operator != (const StorageItemLoginRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StorageItemLoginRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StorageItemLoginRequest &a, StorageItemLoginRequest &b);

typedef struct _StorageItemLoginResponse__isset {
  _StorageItemLoginResponse__isset() : fields_(false) {}
  bool fields_;
} _StorageItemLoginResponse__isset;

class StorageItemLoginResponse {
 public:

  static const char* ascii_fingerprint; // = "BB9FCA77782560E13445F26D69E29AEF";
  static const uint8_t binary_fingerprint[16]; // = {0xBB,0x9F,0xCA,0x77,0x78,0x25,0x60,0xE1,0x34,0x45,0xF2,0x6D,0x69,0xE2,0x9A,0xEF};

  StorageItemLoginResponse() : result_((ItemLoginResult::type)0) {
  }

  virtual ~StorageItemLoginResponse() throw() {}

  ItemLoginResult::type result_;
  std::vector<StorageItemField>  fields_;

  _StorageItemLoginResponse__isset __isset;

  void __set_result_(const ItemLoginResult::type val) {
    result_ = val;
  }

  void __set_fields_(const std::vector<StorageItemField> & val) {
    fields_ = val;
    __isset.fields_ = true;
  }

  bool operator == (const StorageItemLoginResponse & rhs) const
  {
    if (!(result_ == rhs.result_))
      return false;
    if (__isset.fields_ != rhs.__isset.fields_)
      return false;
    else if (__isset.fields_ && !(fields_ == rhs.fields_))
      return false;
    return true;
  }
  bool operator != (const StorageItemLoginResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StorageItemLoginResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StorageItemLoginResponse &a, StorageItemLoginResponse &b);

}} // namespace

#endif
