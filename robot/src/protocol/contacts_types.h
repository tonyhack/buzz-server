/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef contacts_TYPES_H
#define contacts_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace entity {

struct ContactsType {
  enum type {
    MIN = 0,
    FRIEND = 0,
    BLACKLIST = 1,
    MAX = 2
  };
};

extern const std::map<int, const char*> _ContactsType_VALUES_TO_NAMES;

} // namespace

#endif
