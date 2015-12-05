//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-12 09:41:47.
// File name: scene_actor.h
//
// Description:
// Define class SceneActor.
//

#ifndef __GAME__SERVER__SCENE__SCENE__ACTOR__H
#define __GAME__SERVER__SCENE__SCENE__ACTOR__H

#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"
#include "core/base/types.h"
#include "entity/actor_types.h"
#include "game_server/server/extension/scene/zazen_actor.h"
#include "game_server/server/game_actor.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"

namespace game {

namespace server {

namespace scene {

class Scene;
class SceneNpc;

class SceneActor : public core::Noncopyable,
  public core::TimerHandler {
  enum { TIMER_ID_REVIVE = 0, TIMER_ID_CONSECUTIVE_KILL = 1,};

 public:
  SceneActor();
  virtual ~SceneActor();

  bool Initialize(GameActor *actor, Scene *scene);
  void Finalize();

  // 检测移动
  bool CheckMove();

  // 移动计数
  void MoveCount(bool right_angle = true);

  // 重置
  void ResetCheck();

  // 死亡
  void OnDeath();
  // 复活
  bool Revive(bool relocation);

  // 被玩家杀死
  void AfterKilled(SceneActor* attacker);

  // 被NPC杀死
  void AfterKilledByNpc(SceneNpc* scene_npc);

  // 杀死NPC
  void KilledNpc(SceneNpc *scene_npc);

  // 玩家选择复活方式
  bool ChooseReviveMode(entity::ReviveType::type type, bool buy = false);

  //连斩定时器调用
  void OnConsecutiveKill();

  inline core::uint64 GetActorID() const {
    if(this->actor_) {
      return this->actor_->GetID();
    }
    return 0;
  }

  inline entity::EntityType::type GetType() const {
    if(this->actor_) {
      return this->actor_->GetType();
    }
    return entity::EntityType::TYPE_NPC; 
  }

  // 获得名字
  inline std::string GetActorName() const {
    if(this->actor_) {
      return this->actor_->GetName();
    }
    return "";
  }

  virtual void OnTimer(core::uint32 id);

  core::uint64 GetGatherNpc() const {
    return this->gather_npc_id_;
  }

  void SetGatherNpc(core::uint64 id) {
    this->gather_npc_id_ = id;
  }

  GameActor *GetActor() {
    return this->actor_;
  }

  Scene *GetScene() {
    return this->scene_;
  }

  ZazenActor &GetZazen() {
    return this->zazen_;
  }

  bool GetPVPFightingStatus() const {
    return this->pvp_fighting_status_;
  }

  void SetPVPFightingStatus(bool pvp_status) {
    this->pvp_fighting_status_ = pvp_status;
  }

 private:
  // 所在场景
  Scene *scene_;

  // 玩家对象
  GameActor *actor_;

  // 玩家打坐对象
  ZazenActor zazen_;

  // 未检测的连续移动次数
  core::uint32 move_count_;
  // 上次检测的时间戳
  core::TimestampMillisecond timestamp_;

  core::uint32 speed_;

  static const core::uint32 kCheckFrequency_ = 5;

  // 上次移动时间戳
  core::TimestampMillisecond last_move_timestamp_;

  // 之前 N 次移动的时间戳组
  core::uint32 move_time_[kCheckFrequency_];
  core::uint32 standard_time_[kCheckFrequency_];

  // 检测步数内的总移动时间
  core::int32 total_move_time_;
  core::int32 total_standard_time_;

  // 读条时NPC
  core::uint64 gather_npc_id_;

  // 连斩次数
  core::int32 consecutive_kill_num_;
  // 上一次连斩的时间
  time_t last_consecutive_time_;
  // 连斩的BUFF表
  std::vector<int> consecutive_kill_buffs_;
  
  // 玩家PVP状态
  bool pvp_fighting_status_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__ACTOR__H

