#include "session_server/server/brief_actor.h"

#include "coresh/communicator.h"
#include "global/configure/configure.h"
#include "global/logging.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/global_time_tick.h"

namespace session {

namespace server {

BriefActor::BriefActor(core::uint64 id, const std::string &name, core::int32 level,
    entity::VocationType::type vocation, entity::GenderType::type gender,
    core::uint16 map, const std::string &func_state,
    core::int32 axe_fight_score,
    core::int32 shoot_fight_score,
    core::int32 magic_fight_score,
    time_t last_logout_time,
    entity::VipType::type vip_type,
    time_t vip_expire_time)
  : id_(id), name_(name), level_(level), vocation_(vocation), gender_(gender),
    map_(map), last_logout_time_(last_logout_time), vip_type_(vip_type),
    vip_expire_time_(vip_expire_time)
{
  // 初始化系统功能开启状态
  this->func_states_.resize(entity::FunctionalityType::MAX, 0);
  for(size_t i = 0; i < func_state.size(); ++i) {
    if('1' == func_state[i]) {
      this->func_states_[i] = 1;
    }
  }
  // 初始化战斗力
  this->fight_scores_.resize(entity::FightScoreType::MAX, 0);
  this->fight_scores_[entity::FightScoreType::AXE] = axe_fight_score;
  this->fight_scores_[entity::FightScoreType::SHOOT] = shoot_fight_score;
  this->fight_scores_[entity::FightScoreType::MAGIC] = magic_fight_score;
}

bool BriefActor::CheckFunctionalityState(entity::FunctionalityType::type type) const {
  return this->func_states_[type];
}

void BriefActor::SetFunctionalityState(entity::FunctionalityType::type type, bool state) {
  if(state == (bool)this->func_states_[type]) {
    return;
  }

  this->func_states_[type] = state;

  // 发送玩家系统功能开启事件
  if(true == state) {
    event::EventActorFunctionalityEnable event;
    event.__set_actor_id_(this->GetID());
    event.__set_functionality_id_(type);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE , this->GetID(),
        event::ChannelType::CHANNEL_ACTOR, &event,
        sizeof(event::EventActorFunctionalityEnable));
  }
}

core::int32 BriefActor::GetFightScore(entity::FightScoreType::type type) const {
  return this->fight_scores_[type];
}

void BriefActor::SetFightScore(entity::FightScoreType::type type, core::int32 score) {
  this->fight_scores_[type] = score;
}

core::int32 BriefActor::GetCurrentFightScore() const {
  if (entity::VocationType::AXE == this->vocation_) {
    return this->fight_scores_[entity::FightScoreType::AXE];
  } else if (entity::VocationType::SHOOT == this->vocation_) {
    return this->fight_scores_[entity::FightScoreType::SHOOT];
  } else if (entity::VocationType::MAGIC == this->vocation_) {
    return this->fight_scores_[entity::FightScoreType::MAGIC];
  } else {
    LOG_ERROR("Vocation is invalid.");
    return 0;
  }
}

time_t BriefActor::VipTimeRemainderToExpired(time_t last_logout_time, 
    core::int32 remainder_time) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if(now - last_logout_time >= remainder_time) {
    return now;
  }
  return (now - last_logout_time - remainder_time + now);
}

const global::configure::VipCell *BriefActor::GetVipPrivilege(
    entity::VipPrivilegeType::type type) const {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if(now >= this->vip_expire_time_) {
    LOG_INFO("vip 已过期(%lu)", this->vip_expire_time_);
    return NULL;
  }
  return VIP_CONF()->GetCell(this->vip_type_, type);
}
const global::configure::VipCell *BriefActor::GetVipPrivilege(
    entity::VipPrivilegeType::type type, core::int32 key) const {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if(now >= this->vip_expire_time_) {
    LOG_INFO("vip 已过期(%lu)", this->vip_expire_time_);
    return NULL;
  }
  return VIP_CONF()->GetCell(this->vip_type_, type, key);
}

}  // namespace server

}  // namespace session

