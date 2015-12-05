#include "session_server/server/extension/social/homeland_event.h"

#include <cstdio>
#include <cstring>

#include "entity/homeland_types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor_manager.h"

namespace session {

namespace server {

namespace social {

HomelandEvent::HomelandEvent() : type_(0) {
}

HomelandEvent::HomelandEvent(core::int32 type) : type_(type) {
  this->timestamp_ =
    global::ToString(GlobalTimeTick::GetInstance()->TimeTick().GetRealSec());
}

HomelandEvent::~HomelandEvent() {
}

bool HomelandEvent::Initialize(const std::string &data) {
  std::vector<std::string> split_result;
  global::StringSplit(data.c_str(), ":", split_result);

  if (split_result.size() < 2) {
    LOG_ERROR("Event data is invalid.");
    return false;
  }

  this->type_ = atoi(split_result[0].c_str());
  this->timestamp_ = split_result[1].c_str();

  for (size_t i = 2; i < split_result.size(); ++i) {
    this->PushEventVar(split_result[i]);
  }

  return true;
}

void HomelandEvent::PushEventVar(const std::string &event_var) {
  this->event_vars_.push_back(event_var);
}

void HomelandEvent::PushEventVar(core::uint64 event_var) {
  this->event_vars_.push_back(global::ToString(event_var));
}

void HomelandEvent::PushEventVar(core::uint32 event_var) {
  this->event_vars_.push_back(global::ToString(event_var));
}

void HomelandEvent::PushEventVar(core::int32 event_var) {
  this->event_vars_.push_back(global::ToString(event_var));
}

void HomelandEvent::TranslateEventVar(std::vector<std::string> *result) const {
  // 0 no translate
  // 1 actor_id -> actor_name
  static const char *translate_table[] = {
    "1",                     // HOMELAND_VISIT
    "000",                   // CHURCH_PRAY
    "100",                   // TREE_ASSIST_WATER
    "1",                     // TREE_ASSIST_WATERED
    "10",                    // TREE_STEAL
    "10",                    // TREE_STOLEN
    "010",                   // STRONGHOLD_SUPPORT
    "10",                    // STRONGHOLD_SUPPORTED
    "1",                     // BATTLE_DEFENCE_SUCCESS
    "10",                    // BATTLE_DEFENCE_FAILED
    "10",                    // BATTLE_ATTACK_SUCCESS
    "1",                     // BATTLE_ATTACK_FAILED
  };

  for (size_t i = 0; i < this->event_vars_.size(); ++i) {
    if ((size_t)this->type_ < sizeof(translate_table) / sizeof(char *) &&
        i < strlen(translate_table[this->type_])) {
      // 转换为玩家名
      if (translate_table[this->type_][i] == '1') {
        BriefActor *brief_actor =
          SessionActorManager::GetInstance()->GetBriefActor(atol(this->event_vars_[i].c_str()));
        if (NULL == brief_actor) {
          LOG_ERROR("Get BriefActor(%s) from SessionActorManager failed.",
                    this->event_vars_[i].c_str());
          return;
        }
        result->push_back(brief_actor->GetName());
      } else {
        result->push_back(this->event_vars_[i]);
      }
    } else {
      LOG_ERROR("Invalid type[%d] or index[%zd].", this->type_, i);
      return;
    }
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

