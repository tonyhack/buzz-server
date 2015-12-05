/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef gateway_mail_protocol_TYPES_H
#define gateway_mail_protocol_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "gateway_protocol_types.h"
#include "mail_types.h"


namespace gateway { namespace protocol {


class MailContext {
 public:

  static const char* ascii_fingerprint; // = "FD11762B3A3F8327BC5B1EFA7C2DBC14";
  static const uint8_t binary_fingerprint[16]; // = {0xFD,0x11,0x76,0x2B,0x3A,0x3F,0x83,0x27,0xBC,0x5B,0x1E,0xFA,0x7C,0x2D,0xBC,0x14};

  MailContext() : title_(), addresser_(), state_(( ::entity::MailReadState::type)0), affix_type_(( ::entity::MailAffixType::type)0), type_(( ::entity::MailType::type)0), time_(), id_(0) {
  }

  virtual ~MailContext() throw() {}

  std::string title_;
  std::string addresser_;
   ::entity::MailReadState::type state_;
   ::entity::MailAffixType::type affix_type_;
   ::entity::MailType::type type_;
  std::string time_;
  int32_t id_;

  void __set_title_(const std::string& val) {
    title_ = val;
  }

  void __set_addresser_(const std::string& val) {
    addresser_ = val;
  }

  void __set_state_(const  ::entity::MailReadState::type val) {
    state_ = val;
  }

  void __set_affix_type_(const  ::entity::MailAffixType::type val) {
    affix_type_ = val;
  }

  void __set_type_(const  ::entity::MailType::type val) {
    type_ = val;
  }

  void __set_time_(const std::string& val) {
    time_ = val;
  }

  void __set_id_(const int32_t val) {
    id_ = val;
  }

  bool operator == (const MailContext & rhs) const
  {
    if (!(title_ == rhs.title_))
      return false;
    if (!(addresser_ == rhs.addresser_))
      return false;
    if (!(state_ == rhs.state_))
      return false;
    if (!(affix_type_ == rhs.affix_type_))
      return false;
    if (!(type_ == rhs.type_))
      return false;
    if (!(time_ == rhs.time_))
      return false;
    if (!(id_ == rhs.id_))
      return false;
    return true;
  }
  bool operator != (const MailContext &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MailContext & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MailContext &a, MailContext &b);


class MessageMailInitialize {
 public:

  static const char* ascii_fingerprint; // = "E17E47AF4E833D82823160A2A91DEAD1";
  static const uint8_t binary_fingerprint[16]; // = {0xE1,0x7E,0x47,0xAF,0x4E,0x83,0x3D,0x82,0x82,0x31,0x60,0xA2,0xA9,0x1D,0xEA,0xD1};

  MessageMailInitialize() {
  }

  virtual ~MessageMailInitialize() throw() {}

  std::vector<MailContext>  mail_list_;

  void __set_mail_list_(const std::vector<MailContext> & val) {
    mail_list_ = val;
  }

  bool operator == (const MessageMailInitialize & rhs) const
  {
    if (!(mail_list_ == rhs.mail_list_))
      return false;
    return true;
  }
  bool operator != (const MessageMailInitialize &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailInitialize & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailInitialize &a, MessageMailInitialize &b);


class MessageMailAdd {
 public:

  static const char* ascii_fingerprint; // = "9FEA553781DD6D54785551DBC4AB87A0";
  static const uint8_t binary_fingerprint[16]; // = {0x9F,0xEA,0x55,0x37,0x81,0xDD,0x6D,0x54,0x78,0x55,0x51,0xDB,0xC4,0xAB,0x87,0xA0};

  MessageMailAdd() {
  }

  virtual ~MessageMailAdd() throw() {}

  MailContext mail_;

  void __set_mail_(const MailContext& val) {
    mail_ = val;
  }

  bool operator == (const MessageMailAdd & rhs) const
  {
    if (!(mail_ == rhs.mail_))
      return false;
    return true;
  }
  bool operator != (const MessageMailAdd &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailAdd & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailAdd &a, MessageMailAdd &b);


class MessageMailSendRequest {
 public:

  static const char* ascii_fingerprint; // = "AB879940BD15B6B25691265F7384B271";
  static const uint8_t binary_fingerprint[16]; // = {0xAB,0x87,0x99,0x40,0xBD,0x15,0xB6,0xB2,0x56,0x91,0x26,0x5F,0x73,0x84,0xB2,0x71};

  MessageMailSendRequest() : addressee_(), title_(), context_() {
  }

  virtual ~MessageMailSendRequest() throw() {}

  std::string addressee_;
  std::string title_;
  std::string context_;

  void __set_addressee_(const std::string& val) {
    addressee_ = val;
  }

  void __set_title_(const std::string& val) {
    title_ = val;
  }

  void __set_context_(const std::string& val) {
    context_ = val;
  }

  bool operator == (const MessageMailSendRequest & rhs) const
  {
    if (!(addressee_ == rhs.addressee_))
      return false;
    if (!(title_ == rhs.title_))
      return false;
    if (!(context_ == rhs.context_))
      return false;
    return true;
  }
  bool operator != (const MessageMailSendRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailSendRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailSendRequest &a, MessageMailSendRequest &b);


class MessageMailSendResponse {
 public:

  static const char* ascii_fingerprint; // = "5892306F7B861249AE8E27C8ED619593";
  static const uint8_t binary_fingerprint[16]; // = {0x58,0x92,0x30,0x6F,0x7B,0x86,0x12,0x49,0xAE,0x8E,0x27,0xC8,0xED,0x61,0x95,0x93};

  MessageMailSendResponse() : result_(0) {
  }

  virtual ~MessageMailSendResponse() throw() {}

  bool result_;

  void __set_result_(const bool val) {
    result_ = val;
  }

  bool operator == (const MessageMailSendResponse & rhs) const
  {
    if (!(result_ == rhs.result_))
      return false;
    return true;
  }
  bool operator != (const MessageMailSendResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailSendResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailSendResponse &a, MessageMailSendResponse &b);


class MessageMailDelRequest {
 public:

  static const char* ascii_fingerprint; // = "A803C54EAD95E24D90C5E66FB98EA72B";
  static const uint8_t binary_fingerprint[16]; // = {0xA8,0x03,0xC5,0x4E,0xAD,0x95,0xE2,0x4D,0x90,0xC5,0xE6,0x6F,0xB9,0x8E,0xA7,0x2B};

  MessageMailDelRequest() {
  }

  virtual ~MessageMailDelRequest() throw() {}

  std::vector<int32_t>  del_mail_list_;

  void __set_del_mail_list_(const std::vector<int32_t> & val) {
    del_mail_list_ = val;
  }

  bool operator == (const MessageMailDelRequest & rhs) const
  {
    if (!(del_mail_list_ == rhs.del_mail_list_))
      return false;
    return true;
  }
  bool operator != (const MessageMailDelRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailDelRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailDelRequest &a, MessageMailDelRequest &b);


class MessageMailDelResponse {
 public:

  static const char* ascii_fingerprint; // = "5892306F7B861249AE8E27C8ED619593";
  static const uint8_t binary_fingerprint[16]; // = {0x58,0x92,0x30,0x6F,0x7B,0x86,0x12,0x49,0xAE,0x8E,0x27,0xC8,0xED,0x61,0x95,0x93};

  MessageMailDelResponse() : result_(0) {
  }

  virtual ~MessageMailDelResponse() throw() {}

  bool result_;

  void __set_result_(const bool val) {
    result_ = val;
  }

  bool operator == (const MessageMailDelResponse & rhs) const
  {
    if (!(result_ == rhs.result_))
      return false;
    return true;
  }
  bool operator != (const MessageMailDelResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailDelResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailDelResponse &a, MessageMailDelResponse &b);


class MessageMailReadRequest {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  MessageMailReadRequest() : mail_id_(0) {
  }

  virtual ~MessageMailReadRequest() throw() {}

  int32_t mail_id_;

  void __set_mail_id_(const int32_t val) {
    mail_id_ = val;
  }

  bool operator == (const MessageMailReadRequest & rhs) const
  {
    if (!(mail_id_ == rhs.mail_id_))
      return false;
    return true;
  }
  bool operator != (const MessageMailReadRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailReadRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailReadRequest &a, MessageMailReadRequest &b);


class MessageMailReadResponse {
 public:

  static const char* ascii_fingerprint; // = "D8319779DD0AC3352A315AB58713D84C";
  static const uint8_t binary_fingerprint[16]; // = {0xD8,0x31,0x97,0x79,0xDD,0x0A,0xC3,0x35,0x2A,0x31,0x5A,0xB5,0x87,0x13,0xD8,0x4C};

  MessageMailReadResponse() : context_() {
  }

  virtual ~MessageMailReadResponse() throw() {}

  std::string context_;
   ::entity::MailAffix affix_;

  void __set_context_(const std::string& val) {
    context_ = val;
  }

  void __set_affix_(const  ::entity::MailAffix& val) {
    affix_ = val;
  }

  bool operator == (const MessageMailReadResponse & rhs) const
  {
    if (!(context_ == rhs.context_))
      return false;
    if (!(affix_ == rhs.affix_))
      return false;
    return true;
  }
  bool operator != (const MessageMailReadResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailReadResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailReadResponse &a, MessageMailReadResponse &b);


class MessageMailPickAffixRequest {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  MessageMailPickAffixRequest() : mail_id_(0) {
  }

  virtual ~MessageMailPickAffixRequest() throw() {}

  int32_t mail_id_;

  void __set_mail_id_(const int32_t val) {
    mail_id_ = val;
  }

  bool operator == (const MessageMailPickAffixRequest & rhs) const
  {
    if (!(mail_id_ == rhs.mail_id_))
      return false;
    return true;
  }
  bool operator != (const MessageMailPickAffixRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailPickAffixRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailPickAffixRequest &a, MessageMailPickAffixRequest &b);


class MessageMailPickAffixResponse {
 public:

  static const char* ascii_fingerprint; // = "5892306F7B861249AE8E27C8ED619593";
  static const uint8_t binary_fingerprint[16]; // = {0x58,0x92,0x30,0x6F,0x7B,0x86,0x12,0x49,0xAE,0x8E,0x27,0xC8,0xED,0x61,0x95,0x93};

  MessageMailPickAffixResponse() : result_(0) {
  }

  virtual ~MessageMailPickAffixResponse() throw() {}

  bool result_;

  void __set_result_(const bool val) {
    result_ = val;
  }

  bool operator == (const MessageMailPickAffixResponse & rhs) const
  {
    if (!(result_ == rhs.result_))
      return false;
    return true;
  }
  bool operator != (const MessageMailPickAffixResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailPickAffixResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailPickAffixResponse &a, MessageMailPickAffixResponse &b);


class MessageMailDel {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  MessageMailDel() : id_(0) {
  }

  virtual ~MessageMailDel() throw() {}

  int32_t id_;

  void __set_id_(const int32_t val) {
    id_ = val;
  }

  bool operator == (const MessageMailDel & rhs) const
  {
    if (!(id_ == rhs.id_))
      return false;
    return true;
  }
  bool operator != (const MessageMailDel &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageMailDel & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MessageMailDel &a, MessageMailDel &b);

}} // namespace

#endif