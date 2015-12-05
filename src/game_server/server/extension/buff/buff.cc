//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-25 12:24:42.
// File name: buff.cc
//
// Description:
// Define class Buff.
//

#include "game_server/server/extension/buff/buff.h"

#include "coresh/communicator.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/event/game_buff_event_types.h"
#include "game_server/server/extension/buff/condition.h"
#include "game_server/server/extension/buff/condition_pool.h"
#include "game_server/server/extension/buff/configure.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/request/game_effect_request_types.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace buff {

Buff::Buff() : buff_role_(NULL), buff_cell_(NULL), during_time_(0), stack_count_(0) {}
Buff::~Buff() {}

bool Buff::Initialize(BuffRole *role, core::int32 buff_id, core::uint32 during_time,
    entity::EntityType::type type, core::uint64 id) {
  if(role == NULL) {
    LOG_ERROR("参数 role 错误");
    return false;
  }
  this->buff_role_ = role;

  this->buff_cell_ = Configure::GetInstance()->GetBuffCell(buff_id);
  if(this->buff_cell_ == NULL) {
    global::LogError("%s:%d (%s) 初始化BUFF失败，不存在的buff(%u)",
        __FILE__, __LINE__, __FUNCTION__, buff_id);
    return false;
  }

  // 效果条件
  std::vector<core::int32>::const_iterator iterator_effect =
    this->buff_cell_->effect_conditions_.begin();
  for(; iterator_effect != this->buff_cell_->effect_conditions_.end(); ++iterator_effect) {
    const BuffConditionCell *cell = Configure::GetInstance()->GetConditionCell(*iterator_effect);
    if(cell == NULL) {
      LOG_ERROR("获取 BuffConditionCell(%d) 失败", *iterator_effect);
      return false;
    }
    Condition *condition = ConditionPool::GetInstance()->Allocate(cell->type_);
    if(condition == NULL) {
      LOG_ERROR("分配 Condition(%d) 失败", cell->type_);
      return false;
    }
    if(condition->Initialize(*iterator_effect, this->buff_role_) == false) {
      LOG_ERROR("初始化 Condition(%d) 失败", *iterator_effect);
      ConditionPool::GetInstance()->Deallocate(condition);
      return false;
    }
    this->effect_conditions_.push_back(condition);
  }

  // BUFF条件
  std::vector<core::int32>::const_iterator iterator_buff =
    this->buff_cell_->buff_conditions_.begin();
  for(; iterator_buff != this->buff_cell_->buff_conditions_.end(); ++iterator_buff) {
    const BuffConditionCell *cell = Configure::GetInstance()->GetConditionCell(*iterator_buff);
    if(cell == NULL) {
      LOG_ERROR("获取 BuffConditionCell(%d) 失败", *iterator_buff);
      return false;
    }
    Condition *condition = ConditionPool::GetInstance()->Allocate(cell->type_);
    if(condition == NULL) {
      LOG_ERROR("分配 Condition(%d) 失败", cell->type_);
      return false;
    }
    if(condition->Initialize(*iterator_buff, this->buff_role_) == false) {
      LOG_ERROR("初始化 Condition(%d) 失败", *iterator_buff);
      ConditionPool::GetInstance()->Deallocate(condition);
      return false;
    }
    this->buff_conditions_.push_back(condition);
  }

  this->effect_condition_ = false;

  this->end_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  if(during_time == 0) {
    during_time = this->buff_cell_->during_time_;
  }
  this->during_time_ = during_time;

  this->SetSource(type, id);

  return true;
}

void Buff::Finalize() {
  if(this->during_time_ > 0) {
    GameServerSingleton::GetInstance().RemoveTimer(BUFF_TIMER, this);
  }

  // DOT类BUFF的定时器
  if(this->buff_cell_->interval_time_ > 0 && (this->buff_cell_->type_ == entity::BuffType::DOT_BUFF ||
      this->buff_cell_->type_ == entity::BuffType::DOT_HURT)) {
    GameServerSingleton::GetInstance().RemoveTimer(INTERVAL_TIMER, this);
  }

  ConditionVector::iterator iterator1 = this->effect_conditions_.begin();
  for(; iterator1 != this->effect_conditions_.end(); ++iterator1) {
    if(*iterator1) {
      (*iterator1)->Finalize();
      ConditionPool::GetInstance()->Deallocate(*iterator1);
    }
  }
  this->effect_conditions_.clear();

  ConditionVector::iterator iterator2 = this->buff_conditions_.begin();
  for(; iterator2 != this->buff_conditions_.end(); ++iterator2) {
    if(*iterator2) {
      (*iterator2)->Finalize();
      ConditionPool::GetInstance()->Deallocate(*iterator2);
    }
  }
  this->buff_conditions_.clear();

  this->during_time_ = 0;
  this->buff_role_ = NULL;
  this->buff_cell_ = NULL;
  this->stack_count_ = 0;
}

void Buff::UpdateTimer() {
  if(this->buff_cell_ == NULL) {
    global::LogError("%s:%d (%s) buff_cell_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(this->during_time_ > 0) {
    GameServerSingleton::GetInstance().RemoveTimer(BUFF_TIMER, this);
  }
  this->during_time_ = this->buff_cell_->during_time_;
  if(this->during_time_ > 0) {
    GameServerSingleton::GetInstance().AddTimer(BUFF_TIMER, this->during_time_ * 1000,
        1, this, "Buff::Start");
  }
  // 记录结束时间
  this->end_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec() + this->during_time_;
}

bool Buff::Start() {
  if(this->buff_cell_ == NULL || this->buff_role_ == NULL) {
    global::LogError("%s:%d (%s) buff_cell_/buff_role_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  GameRole *game_role = this->buff_role_->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return false;
  }

  if(this->stack_count_ == 0) {
    if(this->during_time_ > 0) {
      GameServerSingleton::GetInstance().AddTimer(BUFF_TIMER, this->during_time_ * 1000,
          1, this, "Buff::Start");
    }
    // 记录结束时间
    this->end_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec() + this->during_time_;

    // DOT类BUFF的定时器
    if(this->buff_cell_->interval_time_ > 0 && (this->buff_cell_->type_ == entity::BuffType::DOT_BUFF ||
        this->buff_cell_->type_ == entity::BuffType::DOT_HURT)) {
      GameServerSingleton::GetInstance().AddTimer(INTERVAL_TIMER,
          this->buff_cell_->interval_time_ * 1000, -1, this, "Buff::Start");
    }
  } else {
    // 更新定时器
    this->UpdateTimer();
  }

  // 如果叠加次数没有到上限，开启效果
  if((core::uint32)this->buff_cell_->stack_count_ > this->stack_count_) {
    ++this->stack_count_;
    // 请求开启效果
    request::RequestEffectStart effect_start;
    effect_start.__set_entity_type_(this->buff_role_->GetType());
    effect_start.__set_entity_id_(this->buff_role_->GetID());

    std::vector<core::int32>::const_iterator iterator;
    iterator = this->buff_cell_->effects_.begin();
    for (; iterator != this->buff_cell_->effects_.end(); ++iterator) {
      effect_start.__set_effect_id_(*iterator);
      if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_EFFECT_START, &effect_start, sizeof(effect_start)) != 0) {
        global::LogError("%s:%d (%s) 请求 REQUEST_EFFECT_START 失败",
            __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
    }

    // 叠加上限效果
    if((core::uint32)this->buff_cell_->stack_count_ == this->stack_count_) {
      std::vector<core::int32>::const_iterator iterator = this->buff_cell_->max_stack_effects_.begin();
      for(; iterator != this->buff_cell_->max_stack_effects_.end(); ++iterator) {
        effect_start.__set_effect_id_(*iterator);
        if(ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_EFFECT_START, &effect_start, sizeof(effect_start))!= 0) {
          global::LogError("%s:%d (%s) 请求 REQUEST_EFFECT_START 失败",
              __FILE__, __LINE__, __FUNCTION__);
          return false;
        }
      }
    }
  }

  bool check_condition = this->CheckEffectCondition();
  this->ExecuteConditionEffect();
  if(this->effect_condition_ == false && check_condition == true) {
    this->StartConditionEffect();
  }

  check_condition = this->CheckBuffCondition();
    this->ExecuteConditionBuff();
  if(check_condition == true) {
    this->StartConditionBuff();
  }

  // 广播消息
  if(this->buff_cell_->broadcast_type_ == entity::BuffBroadcastType::BROAD) {
    gateway::protocol::MessageAddBuff add_buff;
    add_buff.__set_type_(game_role->GetType());
    add_buff.__set_id_(game_role->GetStringID());
    add_buff.__set_buff_(this->GetBuffID());
    add_buff.__set_stack_count_(this->GetStackCount());
    game_role->SendMessageAround(add_buff,
        gateway::protocol::MessageType::MESSAGE_ADD_BUFF, true);
  } else if(this->buff_cell_->broadcast_type_ == entity::BuffBroadcastType::SINGLE &&
    game_role->GetType() == entity::EntityType::TYPE_ACTOR) {
    gateway::protocol::MessageAddBuff add_buff;
    add_buff.__set_type_(game_role->GetType());
    add_buff.__set_id_(game_role->GetStringID());
    add_buff.__set_buff_(this->GetBuffID());
    add_buff.__set_stack_count_(this->GetStackCount());
    ((GameActor *)game_role)->SendMessage(add_buff,
      gateway::protocol::MessageType::MESSAGE_ADD_BUFF);
  }

  event::EventBuffStart event;
  event.__set_type_(this->buff_role_->GetType());
  event.__set_id_(this->buff_role_->GetID());
  event.__set_buff_(this->GetBuffID());
  event.__set_stack_count_(this->GetStackCount());
  event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
  if(this->buff_role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(this->buff_role_->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(event::EventType::EVENT_BUFF_START,
      this->buff_role_->GetID(), channel_type, &event, sizeof(event));

  return true;
}

bool Buff::End() {
  if(this->buff_cell_ == NULL || this->buff_role_ == NULL) {
    global::LogError("%s:%d (%s) buff_cell_/buff_role_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  GameRole *game_role = this->buff_role_->GetRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole 失败");
    return false;
  }

  if(this->during_time_ > 0) {
    GameServerSingleton::GetInstance().RemoveTimer(BUFF_TIMER, this);
  }

  // DOT类BUFF的定时器
  if(this->buff_cell_->interval_time_ > 0 && (this->buff_cell_->type_ == entity::BuffType::DOT_BUFF ||
      this->buff_cell_->type_ == entity::BuffType::DOT_HURT)) {
    GameServerSingleton::GetInstance().RemoveTimer(INTERVAL_TIMER, this);
  }

  // 设置结时间
  this->end_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  // 请求关闭效果
  request::RequestEffectEnd effect_end;
  effect_end.__set_entity_type_(this->buff_role_->GetType());
  effect_end.__set_entity_id_(this->buff_role_->GetID());

  // 结束叠加上限效果
  if((core::uint32)this->buff_cell_->stack_count_ == this->stack_count_) {
    std::vector<core::int32>::const_iterator iterator = this->buff_cell_->max_stack_effects_.begin();
    for (; iterator != this->buff_cell_->max_stack_effects_.end(); ++iterator) {
      effect_end.__set_effect_id_(*iterator);
      if(ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_EFFECT_END, &effect_end, sizeof(effect_end)) != 0) {
        global::LogError("%s:%d (%s) 请求 REQUEST_EFFECT_START 失败",
            __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
    }
  }

  while(this->stack_count_-- > 0) {
    std::vector<core::int32>::const_iterator iterator = this->buff_cell_->effects_.begin();
    for (; iterator != this->buff_cell_->effects_.end(); ++iterator) {
      effect_end.__set_effect_id_(*iterator);
      if(ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_EFFECT_END, &effect_end, sizeof(effect_end)) != 0) {
        global::LogError("%s:%d (%s) 请求 REQUEST_EFFECT_START 失败",
            __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
    }
  }

  if(this->effect_condition_ == true) {
    this->StopConditionEffect();
  }

  // 广播消息
  if(this->buff_cell_->broadcast_type_ == entity::BuffBroadcastType::BROAD) {
    gateway::protocol::MessageRemoveBuff remove_buff;
    remove_buff.__set_type_(game_role->GetType());
    remove_buff.__set_id_(game_role->GetStringID());
    remove_buff.__set_buff_(this->GetBuffID());
    game_role->SendMessageAround(remove_buff,
        gateway::protocol::MessageType::MESSAGE_REMOVE_BUFF, true);
  } else if(this->buff_cell_->broadcast_type_ == entity::BuffBroadcastType::SINGLE &&
    game_role->GetType() == entity::EntityType::TYPE_ACTOR) {
    gateway::protocol::MessageRemoveBuff remove_buff;
    remove_buff.__set_type_(game_role->GetType());
    remove_buff.__set_id_(game_role->GetStringID());
    remove_buff.__set_buff_(this->GetBuffID());
    ((GameActor *)game_role)->SendMessage(remove_buff,
      gateway::protocol::MessageType::MESSAGE_REMOVE_BUFF);
  }

  event::EventBuffEnd event;
  event.__set_type_(this->buff_role_->GetType());
  event.__set_id_(this->buff_role_->GetID());
  event.__set_buff_(this->GetBuffID());
  event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
  if(this->buff_role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(this->buff_role_->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(event::EventType::EVENT_BUFF_END,
      this->buff_role_->GetID(), channel_type, &event, sizeof(event));

  return true;
}

core::int32 Buff::GetLeftTime() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if(now <= this->end_time_) {
    return this->end_time_ - now;
  }

  return 0;
}

bool Buff::CheckEffectCondition() const {
  ConditionVector::const_iterator iterator = this->effect_conditions_.begin();
  for(; iterator != this->effect_conditions_.end(); ++iterator) {
    if(*iterator && (*iterator)->Check() == false) {
      return false;
    }
  }

  return true;
}

bool Buff::CheckBuffCondition() const {
  ConditionVector::const_iterator iterator = this->buff_conditions_.begin();
  for(; iterator != this->buff_conditions_.end(); ++iterator) {
    if(*iterator && (*iterator)->Check() == false) {
      return false;
    }
  }

  return true;
}

void Buff::ExecuteConditionEffect() {
  ConditionVector::iterator iterator2 = this->effect_conditions_.begin();
  for(; iterator2 != this->effect_conditions_.end(); ++iterator2) {
    if(*iterator2) {
      (*iterator2)->OnExecute();
    }
  }
}

void Buff::ExecuteConditionBuff() {
  ConditionVector::iterator iterator2 = this->buff_conditions_.begin();
  for(; iterator2 != this->buff_conditions_.end(); ++iterator2) {
    if(*iterator2) {
      (*iterator2)->OnExecute();
    }
  }
}

void Buff::StartConditionEffect() {
  this->effect_condition_ = true;

  if(this->buff_cell_ == NULL || this->buff_role_ == NULL) {
    LOG_ERROR("获取 buff_cell_/buff_role_ 失败");
    return ;
  }

  std::vector<core::int32>::const_iterator iterator =
    this->buff_cell_->condition_effects_.begin();
  for(; iterator != this->buff_cell_->condition_effects_.end(); ++iterator) {
    request::RequestEffectStart request;
    request.__set_entity_type_(this->buff_role_->GetType());
    request.__set_entity_id_(this->buff_role_->GetID());
    request.__set_effect_id_(*iterator);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_EFFECT_START, &request, sizeof(request)) != 0) {
      LOG_ERROR("请求 REQUEST_EFFECT_START 失败");
      return;
    }
  }
}

void Buff::StopConditionEffect() {
  this->effect_condition_ = false;

  if(this->buff_cell_ == NULL || this->buff_role_ == NULL) {
    LOG_ERROR("获取 buff_cell_/buff_role_ 失败");
    return ;
  }

  std::vector<core::int32>::const_iterator iterator =
    this->buff_cell_->condition_effects_.begin();
  for(; iterator != this->buff_cell_->condition_effects_.end(); ++iterator) {
    request::RequestEffectEnd request;
    request.__set_entity_type_(this->buff_role_->GetType());
    request.__set_entity_id_(this->buff_role_->GetID());
    request.__set_effect_id_(*iterator);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_EFFECT_END, &request, sizeof(request)) != 0) {
      LOG_ERROR("请求 REQUEST_EFFECT_END 失败");
      return;
    }
  }
}

void Buff::StartConditionBuff() {
  if(this->buff_cell_ == NULL || this->buff_role_ == NULL) {
    LOG_ERROR("获取 buff_cell_/buff_role_ 失败");
    return ;
  }

  std::vector<BuffCell::ConditionBuffPair>::const_iterator iterator =
    this->buff_cell_->condition_buffs_.begin();
  for(; iterator != this->buff_cell_->condition_buffs_.end(); ++iterator) {
    core::uint32 rand = ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
    if(rand <= (core::uint32)iterator->second) {
      this->buff_role_->StartBuff(this->source_type_, this->source_id_, iterator->first);
    }
  }
}

void Buff::OnEvent(event::EventType::type type, const void *data, size_t size) {
  bool event_succeed = false;

  ConditionVector::iterator iterator1 = this->effect_conditions_.begin();
  for(; iterator1 != this->effect_conditions_.end(); ++iterator1) {
    if(*iterator1 && (*iterator1)->OnEvent(type, data, size) == true) {
      event_succeed = true;
    }
  }

  if(event_succeed == true) {
    bool check_condition = this->CheckEffectCondition();
    this->ExecuteConditionEffect();
    if(this->effect_condition_ == false && check_condition == true) {
      this->StartConditionEffect();
    } else if(this->effect_condition_ == true && check_condition == false) {
      this->StopConditionEffect();
    }
  }

  event_succeed = false;
  ConditionVector::iterator iterator2 = this->buff_conditions_.begin();
  for(; iterator2 != this->buff_conditions_.end(); ++iterator2) {
    if(*iterator2 && (*iterator2)->OnEvent(type, data, size) == true) {
      event_succeed = true;
    }
  }

  if(event_succeed == true) {
    bool check_condition = this->CheckBuffCondition();
    this->ExecuteConditionBuff();
    if(check_condition == true) {
      this->StartConditionBuff();
    }
  }
}

void Buff::OnTimer(core::uint32 id) {
  switch(id) {
    case BUFF_TIMER:
      this->OnTimerBuffEnd();
      break;
    case INTERVAL_TIMER:
      this->OnTimerInterval();
      break;
    default:
      break;
  }
}

void Buff::OnTimerBuffEnd() {
  this->during_time_ = 0;

  if(this->buff_role_ == NULL) {
    global::LogError("%s:%d (%s) 定时器回调时找不到 buff_role_ 对象",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  this->buff_role_->EndBuff(this->GetBuffID());
}

static void RequestIntervalEffect(BuffRole *role, entity::EntityType::type src_type,
    core::int64 src_id, const BuffCell *cell, core::int32 current_buff) {
  if(cell->type_ == entity::BuffType::DOT_BUFF) {
    std::vector<core::int32>::const_iterator iterator = cell->interval_effects_.begin();
    for(; iterator != cell->interval_effects_.end(); ++iterator) {
      if(*iterator != current_buff) {
        role->StartBuff(src_type, src_id, *iterator);
      }
    }
  } else if(cell->type_ == entity::BuffType::DOT_HURT) {
    std::vector<core::int32>::const_iterator iterator =
      cell->interval_effects_.begin();
    for(; iterator != cell->interval_effects_.end(); ++iterator) {
      request::RequestSkillHurt request;
      request.__set_src_type_(src_type);
      request.__set_src_id_(src_id);
      request.__set_type_(role->GetType());
      request.__set_id_(role->GetID());
      request.__set_hurt_id_(*iterator);
      if(ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_SKILL_HURT, &request, sizeof(request)) == -1) {
        LOG_ERROR("请求 REQUEST_SKILL_HURT 失败");
        return ;
      }
    }
  }
}

void Buff::OnTimerInterval() {
  if(this->buff_cell_ == NULL || this->buff_role_ == NULL) {
    LOG_ERROR("找不到 buff_cell_/buff_role_ 对象");
    return ;
  }

  RequestIntervalEffect(this->buff_role_, this->source_type_, this->source_id_,
      this->buff_cell_, this->GetBuffID());
}

}  // namespace buff

}  // namespace server

}  // namespace game

