//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-28 18:48:30.
// File name: communicator_proxy.cc
//
// Description:
// Define class CommunicatorProxy.
//

#include "game_server/server/extension/python/communicator_proxy.h"

#include <boost/bind.hpp>
#include <boost/python.hpp>

#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_ai_event_types.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/event/game_system_event_types.h"
#include "game_server/server/extension/python/python_engine.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/time_consuming.h"

namespace game {

namespace server {

namespace python {

CommunicatorProxy::CommunicatorProxy() {}
CommunicatorProxy::~CommunicatorProxy() {}

bool CommunicatorProxy::Initialize() {
  this->calling_ = false;
  return true;
}

void CommunicatorProxy::Finalize() {
  CommunicatorHashmap::iterator iterator = this->communicators_.begin();
  for(; iterator != this->communicators_.end(); ++iterator) {
    coresh::CommunicatorSingleton::GetInstance().Unfollow(iterator->second.first);
  }
}

void CommunicatorProxy::Follow(core::uint32 message_type, core::uint64 channel,
    core::uint8 channel_type, const char *python_method) {
  if(python_method == NULL) {
    global::LogError("%s:%d (%s) 参数 python_method 为NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  coresh::communicator::CommunicatorKey key(message_type, channel, channel_type);
  CommunicatorHashmap::iterator iterator = this->communicators_.find(key);
  if(iterator == this->communicators_.end()) {
    EventCallback callback;
    if(this->GetEventCallback(message_type, callback) == false) {
      global::LogError("%s:%d (%s) 事件(%u) 找不到对应的回调函数",
          __FILE__, __LINE__, __FUNCTION__, message_type);
      return ;
    }
    coresh::Communicator::Connection connection =
      coresh::CommunicatorSingleton::GetInstance().Follow(message_type,
          channel, channel_type, callback);
    PythonMethodSet set;
    set.insert(python_method);
    CommunicatorPair pair(connection, set);
    this->communicators_.insert(std::make_pair(key, pair));
  } else {
    iterator->second.second.insert(python_method);
  }

  global::LogDebug("CommunicatorProxy Follow(message=%u, channel=%lu, channel_tp=%u, python_method=%s)",
      message_type, channel, channel_type, python_method);
}

void CommunicatorProxy::Unfollow(core::uint32 message_type, core::uint64 channel,
    core::uint8 channel_type, const char *python_method) {
  if(python_method == NULL) {
    global::LogError("%s:%d (%s) 参数 python_method 为NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  coresh::communicator::CommunicatorKey key(message_type, channel, channel_type);
  CommunicatorHashmap::iterator iterator = this->communicators_.find(key);
  if(iterator != this->communicators_.end()) {
    if(this->calling_ == true) {
      this->unfollow_communicators_.push_back(
          PythonCommunicatorKey(key, python_method));
    } else {
      iterator->second.second.erase(python_method);
      if(iterator->second.second.empty()) {
        coresh::CommunicatorSingleton::GetInstance().Unfollow(iterator->second.first);
        this->communicators_.erase(iterator);
      }
      global::LogDebug("CommunicatorProxy Unfollow(message=%u, channel=%lu, channel_tp=%u, python_method=%s)",
          message_type, channel, channel_type, python_method);
    }
  }
}

bool CommunicatorProxy::GetEventCallback(core::uint32 message_type, EventCallback &callback) {
  switch(message_type) {
    case event::EventType::EVENT_SYSTEM_LOGIN_SESSION:
      callback = boost::bind(&CommunicatorProxy::OnEventSystemLoginSession,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_LOGIN:
      callback = boost::bind(&CommunicatorProxy::OnEventActorLogin,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_LOGOUT:
      callback = boost::bind(&CommunicatorProxy::OnEventActorLogout,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_SAVE:
      callback = boost::bind(&CommunicatorProxy::OnEventActorSave,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH:
      callback = boost::bind(&CommunicatorProxy::OnEventActorExtensionLoadFinish,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_LOAD_FINISH:
      callback = boost::bind(&CommunicatorProxy::OnEventActorLoadFinish,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_DAILY_CLEAN:
      callback = boost::bind(&CommunicatorProxy::OnEventActorDailyClean,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_TALK_NPC:
      callback = boost::bind(&CommunicatorProxy::OnEventActorTalkNpc,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_LEVEL_UP:
      callback = boost::bind(&CommunicatorProxy::OnEventActorLevelUp,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE:
      callback = boost::bind(&CommunicatorProxy::OnEventActorFunctionalityEnable,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ACTOR_KILL_NPC:
      callback = boost::bind(&CommunicatorProxy::OnEventActorKillNpc,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_BASIC_STATUS_CHANGE:
      callback = boost::bind(&CommunicatorProxy::OnEventBasicStatusChange,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ROLE_KILLED:
      callback = boost::bind(&CommunicatorProxy::OnEventRoleKilled,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ROLE_CHANGE_HP:
      callback = boost::bind(&CommunicatorProxy::OnEventRoleChangeHp,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ROLE_CHANGE_MAX_HP:
      callback = boost::bind(&CommunicatorProxy::OnEventRoleChangeMaxHp,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ROLE_CHANGE_MP:
      callback = boost::bind(&CommunicatorProxy::OnEventRoleChangeMp,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ROLE_CHANGE_MAX_MP:
      callback = boost::bind(&CommunicatorProxy::OnEventRoleChangeMaxMp,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ROLE_ENTER_FIGHTING_STATUS:
      callback = boost::bind(&CommunicatorProxy::OnEventRoleEnterFightingStatus,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_ROLE_LEAVE_FIGHTING_STATUS:
      callback = boost::bind(&CommunicatorProxy::OnEventRoleLeaveFightingStatus,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_NPC_CREATE:
      callback = boost::bind(&CommunicatorProxy::OnEventNpcCreate,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_NPC_DESTROY:
      callback = boost::bind(&CommunicatorProxy::OnEventNpcDestory,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_SCENE_BROADCAST_CREATE_ROLE:
      callback = boost::bind(&CommunicatorProxy::OnEventSceneBroadcastCreateRole,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_SCENE_CREATE_AOI_ROLES:
      callback = boost::bind(&CommunicatorProxy::OnEventSceneCreateAoiRoles,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_SCENE_ROLE_COMING:
      callback = boost::bind(&CommunicatorProxy::OnRoleComingEvent,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_SCENE_ROLE_LEAVING:
      callback = boost::bind(&CommunicatorProxy::OnRoleLeavingEvent,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION:
      callback = boost::bind(&CommunicatorProxy::OnRoleChangeLocation,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_SCENE_JUMP_MAP:
      callback = boost::bind(&CommunicatorProxy::OnEventSceneJumpMap,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_SKILL_USE_SKILL:
      callback = boost::bind(&CommunicatorProxy::OnEventSkillUseSkill,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_SKILL_USE_APPEAR:
      callback = boost::bind(&CommunicatorProxy::OnEventSkillUseAppear,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_SKILL_ATTACK_HIT:
      callback = boost::bind(&CommunicatorProxy::onEventSkillAttackHit,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_CREATE:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingCreate,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_DESTORY:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingDestory,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_ACTOR_ENTER:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingActorEnter,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_ACTOR_LEAVE:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingActorLeave,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_ACTOR_REQUEST_FINISH:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingActorRequestFinish,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_ACTOR_REQUEST_COMPLETE:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingActorRequestComplete,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_REQUEST_UPDATE_RECORD:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingRequestUpdateRecord,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_REQUEST_AUTO_FINISH:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingRequestAutoFinish,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_REQUEST_ADD_AWARD:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingRequestAddAward,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_ACTOR_FINISH:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingActorFinish,
          this, _1, _2, _3, _4);
      break;
    case event::EventType::EVENT_PLAYING_ACTOR_REQUEST_AWARD:
      callback = boost::bind(&CommunicatorProxy::OnEventPlayingActorRequestAward,
          this, _1, _2, _3, _4);
      break;
    default:
      return false;
  }
  return true;
}

void CommunicatorProxy::CallPython(const std::string &method, core::uint32 message_type,
    core::uint64 channel, core::uint8 channel_type, const char *message, size_t size) {
  try {

    // global::TimeConsuming consuming("CallPython:" + method);

    boost::python::call_method<void>(PythonEngine::GetInstance()->GetModule()->get(),
        method.c_str(), message_type, channel, channel_type, std::string(message, size));

  } catch(boost::python::error_already_set &) {

    // TODO: 替换为异常信息写入日志
    PyErr_Print();
    global::LogError("%s:%d (%s) 调用 python 函数(%s)失败",
        __FILE__, __LINE__, __FUNCTION__, method.c_str());

  }
}

void CommunicatorProxy::PythonCallback(core::uint32 message_type, core::uint64 channel,
    core::uint8 channel_type, const char *message, size_t size) {
  coresh::communicator::CommunicatorKey key_all(message_type,
      coresh::Communicator::GetChannelAll(), channel_type);
  CommunicatorHashmap::iterator iterator_all = this->communicators_.find(key_all);
  if(iterator_all != this->communicators_.end()) {
    this->calling_ = true;
    PythonMethodSet::iterator iterator2 = iterator_all->second.second.begin();
    for(; iterator2 != iterator_all->second.second.end(); ++iterator2) {
      this->CallPython(*iterator2, message_type, channel, channel_type, message, size);
    }
    this->calling_ = false;
    this->DoSafeUnfollow();
  }
  if(channel != coresh::Communicator::GetChannelAll()) {
    coresh::communicator::CommunicatorKey key(message_type, channel, channel_type);
    CommunicatorHashmap::iterator iterator = this->communicators_.find(key);
    if(iterator != this->communicators_.end()) {
      this->calling_ = true;
      PythonMethodSet::iterator iterator2 = iterator->second.second.begin();
      for(; iterator2 != iterator->second.second.end(); ++iterator2) {
        this->CallPython(*iterator2, message_type, channel, channel_type, message, size);
      }
      this->calling_ = false;
      this->DoSafeUnfollow();
    }
  }
}

///////////////////////////
// 系统相关
void CommunicatorProxy::OnEventSystemLoginSession(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventSystemLoginSession *event = (event::EventSystemLoginSession *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SYSTEM_LOGIN_SESSION, channel,
      channel_type, serialize, ret_size);
}

///////////////////////////
// 玩家相关事件
void CommunicatorProxy::OnEventActorLogin(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorLogin *event = (event::EventActorLogin *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_LOGIN, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventActorLogout(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorLogout *event = (event::EventActorLogout *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_LOGOUT, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventActorSave(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorSave *event = (event::EventActorSave *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_SAVE, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventActorExtensionLoadFinish(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorExtensionLoadFinish *event =
    (event::EventActorExtensionLoadFinish *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventActorLoadFinish(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorLoadFinish *event = (event::EventActorLoadFinish *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_LOAD_FINISH, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventActorDailyClean(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorDailyClean *event = (event::EventActorDailyClean *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_DAILY_CLEAN, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventActorTalkNpc(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorTalkNpc *event = (event::EventActorTalkNpc *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_TALK_NPC, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventActorLevelUp(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorLevelUp *event = (event::EventActorLevelUp *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_LEVEL_UP, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventActorFunctionalityEnable(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorFunctionalityEnable *event = (event::EventActorFunctionalityEnable *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventActorKillNpc(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventActorKillNpc *event = (event::EventActorKillNpc *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ACTOR_KILL_NPC, channel,
      channel_type, serialize, ret_size);
}

///////////////////////////
// Role 相关事件
void CommunicatorProxy::OnEventBasicStatusChange(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventBasicStatusChange *event = (event::EventBasicStatusChange *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_BASIC_STATUS_CHANGE, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventRoleKilled(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventRoleKilled *event = (event::EventRoleKilled *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ROLE_KILLED, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventRoleChangeHp(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventRoleChangeHp *event = (event::EventRoleChangeHp *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ROLE_CHANGE_HP, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventRoleChangeMaxHp(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventRoleChangeMaxHp *event = (event::EventRoleChangeMaxHp *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ROLE_CHANGE_MAX_HP, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventRoleChangeMp(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventRoleChangeMp *event = (event::EventRoleChangeMp *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ROLE_CHANGE_MP, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventRoleChangeMaxMp(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventRoleChangeMaxMp *event = (event::EventRoleChangeMaxMp *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ROLE_CHANGE_MAX_MP, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventRoleEnterFightingStatus(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventRoleEnterFightingStatus *event = (event::EventRoleEnterFightingStatus *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ROLE_ENTER_FIGHTING_STATUS, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventRoleLeaveFightingStatus(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventRoleLeaveFightingStatus *event = (event::EventRoleLeaveFightingStatus *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ROLE_LEAVE_FIGHTING_STATUS, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventRoleEnterPvpStatus(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventRoleEnterPvpStatus *event = (event::EventRoleEnterPvpStatus *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ROLE_ENTER_PVP_STATUS, channel,
      channel_type, serialize, ret_size);
}

void CommunicatorProxy::OnEventRoleLeavePvpStatus(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventRoleLeavePvpStatus *event = (event::EventRoleLeavePvpStatus *)message;
  size_t ret_size = 0;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, ret_size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_ROLE_LEAVE_PVP_STATUS, channel,
      channel_type, serialize, ret_size);
}

///////////////////////////
// NPC 相关事件
void CommunicatorProxy::OnEventNpcCreate(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventNpcCreate *event = (event::EventNpcCreate *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_NPC_CREATE, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventNpcDestory(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventNpcDestory *event = (event::EventNpcDestory *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_NPC_DESTROY, channel,
      channel_type, serialize, size);
}

///////////////////////////
// 场景相关事件
void CommunicatorProxy::OnEventSceneBroadcastCreateRole(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventSceneBroadcastCreateRole *event =
    (event::EventSceneBroadcastCreateRole *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SCENE_BROADCAST_CREATE_ROLE, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventSceneCreateAoiRoles(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventSceneCreateAoiRoles *event =
    (event::EventSceneCreateAoiRoles *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SCENE_CREATE_AOI_ROLES, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnRoleComingEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventSceneRoleComing *event =
      (event::EventSceneRoleComing *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SCENE_CREATE_AOI_ROLES, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnRoleLeavingEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventSceneRoleLeaving *event =
      (event::EventSceneRoleLeaving *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SCENE_CREATE_AOI_ROLES, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnRoleChangeLocation(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventSceneRoleChangeLocation *event =
      (event::EventSceneRoleChangeLocation *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventSceneJumpMap(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventSceneJumpMap *event = (event::EventSceneJumpMap *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SCENE_JUMP_MAP, channel,
      channel_type, serialize, size);
}

///////////////////////////
// 技能事件
void CommunicatorProxy::OnEventSkillUseSkill(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventUseSkill *event = (event::EventUseSkill *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SKILL_USE_SKILL, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventSkillUseAppear(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventUseAppearSkill *event = (event::EventUseAppearSkill *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SKILL_USE_APPEAR, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::onEventSkillAttackHit(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventSkillAttackHit *event = (event::EventSkillAttackHit *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_SKILL_ATTACK_HIT, channel,
      channel_type, serialize, size);
}

///////////////////////////
// 副本事件
void CommunicatorProxy::OnEventPlayingCreate(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingCreate *event = (event::EventPlayingCreate *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_CREATE, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingDestory(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingDestory *event = (event::EventPlayingDestory *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_DESTORY, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingActorEnter(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingActorEnter *event = (event::EventPlayingActorEnter *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_ACTOR_ENTER, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingActorLeave(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingActorLeave *event = (event::EventPlayingActorLeave *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_ACTOR_LEAVE, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingActorRequestFinish(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingActorRequestFinish *event = (event::EventPlayingActorRequestFinish *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_ACTOR_REQUEST_FINISH, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingActorRequestComplete(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingActorRequestComplete *event =
    (event::EventPlayingActorRequestComplete *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_ACTOR_REQUEST_COMPLETE, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingRequestUpdateRecord(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingRequestUpdateRecord *event =
    (event::EventPlayingRequestUpdateRecord *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_REQUEST_UPDATE_RECORD, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingRequestAutoFinish(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingRequestAutoFinish *event = 
    (event::EventPlayingRequestAutoFinish *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if (serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_REQUEST_AUTO_FINISH, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingRequestAddAward(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingRequestAddAward *event =
    (event::EventPlayingRequestAddAward *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_REQUEST_ADD_AWARD, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingActorFinish(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingActorFinish *event =
    (event::EventPlayingActorFinish *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_ACTOR_FINISH, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventPlayingActorRequestAward(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  event::EventPlayingActorRequestAward *event =
    (event::EventPlayingActorRequestAward *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_PLAYING_ACTOR_REQUEST_AWARD, channel,
      channel_type, serialize, size);
}

///////////////////////////
// AI事件
void CommunicatorProxy::OnEventAiThreatStart(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  event::EventAiThreatStart*event =
    (event::EventAiThreatStart *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_AI_THREAT_START, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::OnEventAiThreatEnd(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  event::EventAiThreatEnd*event =
    (event::EventAiThreatEnd *)message;
  const char *serialize = global::GlobalPacket::GetPacket()->Serialize(*event, size);
  if(serialize == NULL) {
    LOG_ERROR("消息序列化失败");
    return ;
  }
  this->PythonCallback(event::EventType::EVENT_AI_THREAT_END, channel,
      channel_type, serialize, size);
}

void CommunicatorProxy::DoSafeUnfollow() {
  PythonCommunicatorVector::iterator iterator = this->unfollow_communicators_.begin();
  for(; iterator != this->unfollow_communicators_.end(); ++iterator) {
    CommunicatorHashmap::iterator iterator2 = this->communicators_.find(iterator->first);
    if(iterator2 != this->communicators_.end()) {
      iterator2->second.second.erase(iterator->second);
      if(iterator2->second.second.empty()) {
        coresh::CommunicatorSingleton::GetInstance().Unfollow(iterator2->second.first);
        this->communicators_.erase(iterator2);
      }
      global::LogDebug("CommunicatorProxy Unfollow(message=%u, channel=%lu, channel_tp=%u, python_method=%s)",
          iterator->first.message_, iterator->first.channel_, iterator->first.channel_type_,
          iterator->second.c_str());
    }
  }
  this->unfollow_communicators_.clear();
}

}  // namespace python

}  // namespace server

}  // namespace game

