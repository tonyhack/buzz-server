//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-23 18:35:11.
// File name: threat.cc
//
// Description:
//

#include "game_server/server/extension/ai/threat.h"

#include "coresh/communicator.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_ai_event_types.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

Threat::Threat() {};
Threat::~Threat() {};

bool Threat::Initialize(BlackBoard *black_board) {
  if(black_board == NULL) {
    LOG_ERROR("参数 black_board 错误");
    return false;
  }

  this->max_hate_ = -1;
  this->max_hate_id_ = 0;
  this->threat_map_.clear();

  this->black_board_ = black_board;

  return true;
}


void Threat::Finalize() {
  this->RemoveAll();

  this->max_hate_ = -1;
  this->max_hate_id_ = 0;

  this->black_board_ = NULL;
}

bool Threat::Add(core::uint64 id, core::int32 value) {
  if(this->black_board_ == NULL) {
    LOG_ERROR("black_board_ 为 NULL");
    return false;
  }

  if (this->threat_map_.empty() == true) {
    request::RequestSkillSetFightingStatus request;
    request.__set_type_(entity::EntityType::TYPE_NPC);
    request.__set_id_(this->black_board_->GetID());
    request.__set_status_(true);
    if (ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_SKILL_SET_FIGHTING_STATUS,
          &request, sizeof(request)) == -1) {
      LOG_ERROR("REQUEST_SKILL_SET_FIGHTING_STATUS error");
      return false;
    }
  }

  ThreatMap::iterator iterator = threat_map_.find(id);
  if(iterator == threat_map_.end()) {
    threat_map_.insert(ThreatMap::value_type(id, value));
    if(this->max_hate_id_ == 0 || value > this->max_hate_) {
      this->max_hate_ = value;
      this->max_hate_id_ = id;
    }
    // 发送仇恨开始事件
    event::EventAiThreatStart event;
    event.__set_threat_role_type_(entity::EntityType::TYPE_ACTOR);
    event.__set_threat_role_id_(id);
    event.__set_npc_(this->black_board_->GetID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_AI_THREAT_START, id,
        event::ChannelType::CHANNEL_ACTOR, &event,
        sizeof(event::EventAiThreatStart));
  } else {
    iterator->second += value;
    if(iterator->second > this->max_hate_) {
      this->max_hate_ = iterator->second;
      this->max_hate_id_ = id;
    }
    /*
    if(iterator->second <= 0) {
      this->Remove(id);
    }
    */
  }
  return true;
}

bool Threat::Remove(core::uint64 id) {
  if(this->black_board_ == NULL) {
    LOG_ERROR("black_board_ is null");
    return false;
  }
  ThreatMap::iterator iterator = threat_map_.find(id);
  if(iterator == threat_map_.end()) {
    return false;
  } else {
    entity::EntityType::type target_type = entity::EntityType::TYPE_MAX;
    core::uint64 target_id = 0;
    this->black_board_->GetTarget(target_type, target_id);
    if(target_id == id) {
      this->black_board_->SetTarget(entity::EntityType::TYPE_MAX, 0);
    }
    // 删除对此玩家仇恨，找出下一个仇恨值高的玩家
    threat_map_.erase(iterator);
    if(id == this->max_hate_id_) {
      this->max_hate_id_ = 0;
      this->max_hate_ = -1;
      for(ThreatMap::iterator iterator = this->threat_map_.begin();
          iterator != this->threat_map_.end(); ++iterator) {
        if(this->max_hate_ < iterator->second) {
          this->max_hate_ = iterator->second;
          this->max_hate_id_ = iterator->first;
        }
      }
    }
    // 发送仇恨结束事件
    event::EventAiThreatEnd event;
    event.__set_threat_role_type_(entity::EntityType::TYPE_ACTOR);
    event.__set_threat_role_id_(id);
    event.__set_npc_(this->black_board_->GetID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_AI_THREAT_END, id,
        event::ChannelType::CHANNEL_ACTOR, &event,
        sizeof(event::EventAiThreatEnd));

    if (this->threat_map_.empty() == true) {
      request::RequestSkillSetFightingStatus request;
      request.__set_type_(entity::EntityType::TYPE_NPC);
      request.__set_id_(this->black_board_->GetID());
      request.__set_status_(false);
      if (ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_SKILL_SET_FIGHTING_STATUS,
            &request, sizeof(request)) == -1) {
        LOG_ERROR("REQUEST_SKILL_SET_FIGHTING_STATUS error");
        return false;
      }
    }
  }

  return true;
}

void Threat::RemoveAll() {
  if (this->threat_map_.empty() == true) {
    return;
  }

  for(ThreatMap::iterator iterator = this->threat_map_.begin();
      iterator != this->threat_map_.end(); ++iterator) {
    // 发送仇恨结束事件
    event::EventAiThreatEnd event;
    event.__set_threat_role_type_(entity::EntityType::TYPE_ACTOR);
    event.__set_threat_role_id_(iterator->first);
    event.__set_npc_(this->black_board_->GetID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_AI_THREAT_END, iterator->first,
        event::ChannelType::CHANNEL_ACTOR, &event,
        sizeof(event::EventAiThreatEnd));
  }

  threat_map_.clear(); 
  this->black_board_->SetTarget(entity::EntityType::TYPE_MAX, 0);

  request::RequestSkillSetFightingStatus request;
  request.__set_type_(entity::EntityType::TYPE_NPC);
  request.__set_id_(this->black_board_->GetID());
  request.__set_status_(false);
  if (ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SKILL_SET_FIGHTING_STATUS,
        &request, sizeof(request)) == -1) {
    LOG_ERROR("REQUEST_SKILL_SET_FIGHTING_STATUS error");
    return;
  }
}

core::int32 Threat::GetThreat(const core::uint64 id) const {
  ThreatMap::const_iterator iterator = threat_map_.find(id);
  if(iterator == threat_map_.end()) {
    return 0;
  }
  return iterator->second;
}

core::uint64 Threat::GetMaxThreatActor() const {
  return this->max_hate_id_;
}

} // namespace ai

} // namepsace server

} // namespace game

