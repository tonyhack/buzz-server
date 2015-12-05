/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef error_TYPES_H
#define error_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace entity {

struct ErrorType {
  enum type {
    MIN = 0,
    ACTOR_BEGIN = 0,
    ACTOR_OFFLINE = 1,
    ACTOR_LEVEL_LOW = 2,
    ACTOR_GOLD_LOW = 3,
    ACTOR_FREEDOM_DOLLAR_LOW = 4,
    ACTOR_RESTRICT_DOLLAR_LOW = 5,
    ITEM_BEGIN = 200,
    PACKET_FULL = 201,
    ITEM_NUMBER = 202,
    SKILL_BEGIN = 400,
    BUFF_BEGIN = 500,
    SCENE_BEGIN = 600,
    SOU_BEGIN = 700,
    TASK_BEGIN = 800,
    TASK_ACCPED_RISK = 801,
    TASK_ACCPED_SAME_TRIP = 802,
    ATTR_BEGIN = 900,
    CHAT_BEGIN = 1000,
    MAIL_BEGIN = 1100,
    MAIL_ACTOR_INVALID = 1101,
    MAIL_ACTOR_BLACKLIST = 1102,
    MAIL_CONTEXT_SIZE = 1103,
    SOCIAL_BEGIN = 1200,
    TEAM_BEGIN = 1300,
    TEAM_HAVE_TEAM = 1301,
    GUILD_BEGIN = 1400,
    MAX = 1401
  };
};

extern const std::map<int, const char*> _ErrorType_VALUES_TO_NAMES;

} // namespace

#endif