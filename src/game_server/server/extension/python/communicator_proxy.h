//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-28 18:13:37.
// File name: communicator_proxy.h
//
// Description:
// Define class CommunicatorProxy.
//

#ifndef __GAME__SERVER__PYTHON__COMMNICATOR__PROXY__H
#define __GAME__SERVER__PYTHON__COMMNICATOR__PROXY__H

#include <set>
#include <string>

#include <ext/hash_map>

#include <boost/shared_ptr.hpp>

#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace python {

class CommunicatorProxy {
  typedef std::set<std::string> PythonMethodSet;
  typedef std::pair<coresh::Communicator::Connection, PythonMethodSet> CommunicatorPair;
  typedef __gnu_cxx::hash_map<coresh::communicator::CommunicatorKey, CommunicatorPair,
          coresh::communicator::HashFunction> CommunicatorHashmap;

  typedef std::pair<coresh::communicator::CommunicatorKey, std::string> PythonCommunicatorKey;
  typedef std::vector<PythonCommunicatorKey> PythonCommunicatorVector;

  typedef boost::function<void (core::uint64, core::uint8, const void *, size_t)> EventCallback;

 public:
  CommunicatorProxy();
  ~CommunicatorProxy();

  bool Initialize();
  void Finalize();

  void Follow(core::uint32 message_type, core::uint64 channel,
      core::uint8 channel_type, const char *python_method);
  void Unfollow(core::uint32 message_type, core::uint64 channel,
      core::uint8 channel_type, const char *python_method);

 private:
  bool GetEventCallback(core::uint32 message_type, EventCallback &callback);

  // 调用 Python 函数
  void CallPython(const std::string &method, core::uint32 message_type,
      core::uint64 channel, core::uint8 channel_type, const char *message, size_t size);

  // Python 事件回调
  void PythonCallback(core::uint32 message_type, core::uint64 channel,
      core::uint8 channel_type, const char *message, size_t size);

  ///////////////////////////
  // 系统相关
  void OnEventSystemLoginSession(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  ///////////////////////////
  // 玩家相关事件
  void OnEventActorLogin(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorLogout(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorSave(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorExtensionLoadFinish(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorLoadFinish(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorDailyClean(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorTalkNpc(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorLevelUp(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorFunctionalityEnable(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorKillNpc(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  ///////////////////////////
  // Role 相关事件
  void OnEventBasicStatusChange(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleKilled(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleChangeHp(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleChangeMaxHp(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleChangeMp(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleChangeMaxMp(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleEnterFightingStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleLeaveFightingStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleEnterPvpStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleLeavePvpStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  ///////////////////////////
  // NPC 相关事件
  void OnEventNpcCreate(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventNpcDestory(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  ///////////////////////////
  // 场景相关事件
  void OnEventSceneBroadcastCreateRole(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSceneCreateAoiRoles(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnRoleComingEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnRoleLeavingEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnRoleChangeLocation(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSceneJumpMap(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  ///////////////////////////
  // 技能事件
  void OnEventSkillUseSkill(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSkillUseAppear(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void onEventSkillAttackHit(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  ///////////////////////////
  // 副本事件
  void OnEventPlayingCreate(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingDestory(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingActorEnter(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingActorLeave(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingActorRequestFinish(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingActorRequestComplete(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingRequestUpdateRecord(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingRequestAutoFinish(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingRequestAddAward(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingActorFinish(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventPlayingActorRequestAward(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  ///////////////////////////
  // AI事件
  void OnEventAiThreatStart(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventAiThreatEnd(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void DoSafeUnfollow();

  CommunicatorHashmap communicators_;

  PythonCommunicatorVector unfollow_communicators_;

  // 是否正在回调
  bool calling_;
};

typedef boost::shared_ptr<CommunicatorProxy> CommunicatorProxyPtr;

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__COMMNICATOR__PROXY__H

