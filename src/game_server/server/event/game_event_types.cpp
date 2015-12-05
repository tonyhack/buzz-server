/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "game_event_types.h"

#include <algorithm>

namespace game { namespace server { namespace event {

int _kEventTypeValues[] = {
  EventType::EVENT_MIN,
  EventType::EVENT_SYSTEM_LOGIN_SESSION,
  EventType::EVENT_ACTOR_LOGIN,
  EventType::EVENT_ACTOR_LOGOUT,
  EventType::EVENT_ACTOR_SAVE,
  EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH,
  EventType::EVENT_ACTOR_LOAD_FINISH,
  EventType::EVENT_ACTOR_DAILY_CLEAN,
  EventType::EVENT_ACTOR_WEEKLY_CLEAN,
  EventType::EVENT_ACTOR_TALK_NPC,
  EventType::EVENT_ACTOR_LEVEL_UP,
  EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE,
  EventType::EVENT_ACTOR_KILL_NPC,
  EventType::EVENT_ACTOR_COMPARE,
  EventType::EVENT_ACTOR_VIP_CHANGE,
  EventType::EVENT_ACTOR_RESOURCE_CHANGE,
  EventType::EVENT_ACTOR_RELIVE,
  EventType::EVENT_ACTOR_FCM_STATUS_CHANGE,
  EventType::EVENT_BASIC_STATUS_CHANGE,
  EventType::EVENT_ROLE_COMMON_STATUS_CHANGE,
  EventType::EVENT_ROLE_KILLED,
  EventType::EVENT_ROLE_CHANGE_HP,
  EventType::EVENT_ROLE_CHANGE_MAX_HP,
  EventType::EVENT_ROLE_CHANGE_MP,
  EventType::EVENT_ROLE_CHANGE_MAX_MP,
  EventType::EVENT_ROLE_ENTER_FIGHTING_STATUS,
  EventType::EVENT_ROLE_LEAVE_FIGHTING_STATUS,
  EventType::EVENT_ROLE_ENTER_PVP_STATUS,
  EventType::EVENT_ROLE_LEAVE_PVP_STATUS,
  EventType::EVENT_ROLE_CHANGE_VOCATION,
  EventType::EVENT_ROLE_CHANGE_CURRENT_ENERGY,
  EventType::EVENT_ROLE_CHANGE_MAX_ENERGY,
  EventType::EVENT_NPC_CREATE,
  EventType::EVENT_NPC_DESTROY,
  EventType::EVENT_SCENE_BROADCAST_CREATE_ROLE,
  EventType::EVENT_SCENE_CREATE_AOI_ROLES,
  EventType::EVENT_SCENE_ROLE_COMING,
  EventType::EVENT_SCENE_ROLE_LEAVING,
  EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION,
  EventType::EVENT_SCENE_JUMP_MAP,
  EventType::EVENT_SCENE_GATHER,
  EventType::EVENT_SKILL_USE_SKILL,
  EventType::EVENT_SKILL_ATTACKED_SKILL,
  EventType::EVENT_SKILL_USE_APPEAR,
  EventType::EVENT_SKILL_ATTACK_HIT,
  EventType::EVENT_SKILL_ATTACKED_HIT,
  EventType::EVENT_BUFF_START,
  EventType::EVENT_BUFF_END,
  EventType::EVENT_PLAYING_CREATE,
  EventType::EVENT_PLAYING_DESTORY,
  EventType::EVENT_PLAYING_ACTOR_ENTER,
  EventType::EVENT_PLAYING_ACTOR_LEAVE,
  EventType::EVENT_PLAYING_ACTOR_REQUEST_FINISH,
  EventType::EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,
  EventType::EVENT_PLAYING_REQUEST_UPDATE_RECORD,
  EventType::EVENT_PLAYING_REQUEST_AUTO_FINISH,
  EventType::EVENT_PLAYING_REQUEST_ADD_AWARD,
  EventType::EVENT_PLAYING_ACTOR_FINISH,
  EventType::EVENT_PLAYING_ACTOR_REQUEST_AWARD,
  EventType::EVENT_ITEM_USE,
  EventType::EVENT_ITEM_EQUIP_STRENGTH,
  EventType::EVENT_ITEM_EQUIP_REFINE,
  EventType::EVENT_ITEM_EQUIP_INLAY,
  EventType::EVENT_ITEM_WEAR_EQUIP,
  EventType::EVENT_ITEM_DIVINE_GEM,
  EventType::EVENT_ATTR_SPIRIT_FEED,
  EventType::EVENT_ATTR_CONSTELLATION_UP,
  EventType::EVENT_ATTR_SPIRIT_UPGRADE,
  EventType::EVENT_ATTR_SWORD_UPGRADE,
  EventType::EVENT_ATTR_NOBLE_UPGRADE,
  EventType::EVENT_SOUL_ADD,
  EventType::EVENT_SOUL_SET,
  EventType::EVENT_SOUL_UPGRADE_STEP,
  EventType::EVENT_SOUL_UPGRADE_LEVEL,
  EventType::EVENT_AI_THREAT_START,
  EventType::EVENT_AI_THREAT_END,
  EventType::EVENT_BAR_FINISH,
  EventType::EVENT_TASK_FINISH,
  EventType::EVENT_TASK_ACCEPT,
  EventType::EVENT_SOCIAL_HOMELAND_BATTLE_STRIKE,
  EventType::EVENT_SOCIAL_HOMELAND_TREE_WATER,
  EventType::EVENT_TRANSPORT_FAILURE,
  EventType::EVENT_MAX
};
const char* _kEventTypeNames[] = {
  "EVENT_MIN",
  "EVENT_SYSTEM_LOGIN_SESSION",
  "EVENT_ACTOR_LOGIN",
  "EVENT_ACTOR_LOGOUT",
  "EVENT_ACTOR_SAVE",
  "EVENT_ACTOR_EXTENSION_LOAD_FINISH",
  "EVENT_ACTOR_LOAD_FINISH",
  "EVENT_ACTOR_DAILY_CLEAN",
  "EVENT_ACTOR_WEEKLY_CLEAN",
  "EVENT_ACTOR_TALK_NPC",
  "EVENT_ACTOR_LEVEL_UP",
  "EVENT_ACTOR_FUNCTIONALITY_ENABLE",
  "EVENT_ACTOR_KILL_NPC",
  "EVENT_ACTOR_COMPARE",
  "EVENT_ACTOR_VIP_CHANGE",
  "EVENT_ACTOR_RESOURCE_CHANGE",
  "EVENT_ACTOR_RELIVE",
  "EVENT_ACTOR_FCM_STATUS_CHANGE",
  "EVENT_BASIC_STATUS_CHANGE",
  "EVENT_ROLE_COMMON_STATUS_CHANGE",
  "EVENT_ROLE_KILLED",
  "EVENT_ROLE_CHANGE_HP",
  "EVENT_ROLE_CHANGE_MAX_HP",
  "EVENT_ROLE_CHANGE_MP",
  "EVENT_ROLE_CHANGE_MAX_MP",
  "EVENT_ROLE_ENTER_FIGHTING_STATUS",
  "EVENT_ROLE_LEAVE_FIGHTING_STATUS",
  "EVENT_ROLE_ENTER_PVP_STATUS",
  "EVENT_ROLE_LEAVE_PVP_STATUS",
  "EVENT_ROLE_CHANGE_VOCATION",
  "EVENT_ROLE_CHANGE_CURRENT_ENERGY",
  "EVENT_ROLE_CHANGE_MAX_ENERGY",
  "EVENT_NPC_CREATE",
  "EVENT_NPC_DESTROY",
  "EVENT_SCENE_BROADCAST_CREATE_ROLE",
  "EVENT_SCENE_CREATE_AOI_ROLES",
  "EVENT_SCENE_ROLE_COMING",
  "EVENT_SCENE_ROLE_LEAVING",
  "EVENT_SCENE_ROLE_CHANGE_LOCATION",
  "EVENT_SCENE_JUMP_MAP",
  "EVENT_SCENE_GATHER",
  "EVENT_SKILL_USE_SKILL",
  "EVENT_SKILL_ATTACKED_SKILL",
  "EVENT_SKILL_USE_APPEAR",
  "EVENT_SKILL_ATTACK_HIT",
  "EVENT_SKILL_ATTACKED_HIT",
  "EVENT_BUFF_START",
  "EVENT_BUFF_END",
  "EVENT_PLAYING_CREATE",
  "EVENT_PLAYING_DESTORY",
  "EVENT_PLAYING_ACTOR_ENTER",
  "EVENT_PLAYING_ACTOR_LEAVE",
  "EVENT_PLAYING_ACTOR_REQUEST_FINISH",
  "EVENT_PLAYING_ACTOR_REQUEST_COMPLETE",
  "EVENT_PLAYING_REQUEST_UPDATE_RECORD",
  "EVENT_PLAYING_REQUEST_AUTO_FINISH",
  "EVENT_PLAYING_REQUEST_ADD_AWARD",
  "EVENT_PLAYING_ACTOR_FINISH",
  "EVENT_PLAYING_ACTOR_REQUEST_AWARD",
  "EVENT_ITEM_USE",
  "EVENT_ITEM_EQUIP_STRENGTH",
  "EVENT_ITEM_EQUIP_REFINE",
  "EVENT_ITEM_EQUIP_INLAY",
  "EVENT_ITEM_WEAR_EQUIP",
  "EVENT_ITEM_DIVINE_GEM",
  "EVENT_ATTR_SPIRIT_FEED",
  "EVENT_ATTR_CONSTELLATION_UP",
  "EVENT_ATTR_SPIRIT_UPGRADE",
  "EVENT_ATTR_SWORD_UPGRADE",
  "EVENT_ATTR_NOBLE_UPGRADE",
  "EVENT_SOUL_ADD",
  "EVENT_SOUL_SET",
  "EVENT_SOUL_UPGRADE_STEP",
  "EVENT_SOUL_UPGRADE_LEVEL",
  "EVENT_AI_THREAT_START",
  "EVENT_AI_THREAT_END",
  "EVENT_BAR_FINISH",
  "EVENT_TASK_FINISH",
  "EVENT_TASK_ACCEPT",
  "EVENT_SOCIAL_HOMELAND_BATTLE_STRIKE",
  "EVENT_SOCIAL_HOMELAND_TREE_WATER",
  "EVENT_TRANSPORT_FAILURE",
  "EVENT_MAX"
};
const std::map<int, const char*> _EventType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(83, _kEventTypeValues, _kEventTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kChannelTypeValues[] = {
  ChannelType::CHANNEL_SYSTEM,
  ChannelType::CHANNEL_ACTOR,
  ChannelType::CHANNEL_NPC,
  ChannelType::CHANNEL_SCENE,
  ChannelType::CHANNEL_PLAYING,
  ChannelType::CHANNEL_MAX
};
const char* _kChannelTypeNames[] = {
  "CHANNEL_SYSTEM",
  "CHANNEL_ACTOR",
  "CHANNEL_NPC",
  "CHANNEL_SCENE",
  "CHANNEL_PLAYING",
  "CHANNEL_MAX"
};
const std::map<int, const char*> _ChannelType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(6, _kChannelTypeValues, _kChannelTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

}}} // namespace