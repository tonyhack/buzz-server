#include "session_server/server/extension/guild/guild_event.h"

#include <cstdio>
#include <cstring>

#include "entity/guild_types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor_manager.h"

namespace session {

namespace server {

namespace guild {

GuildEvent::GuildEvent() : type_(0) {
}

GuildEvent::GuildEvent(core::int32 type) : type_(type) {
  this->timestamp_ =
    global::ToString(GlobalTimeTick::GetInstance()->TimeTick().GetRealSec());
}

GuildEvent::~GuildEvent() {
}

bool GuildEvent::Initialize(const std::string &data) {
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

void GuildEvent::PushEventVar(const std::string &event_var) {
  this->event_vars_.push_back(event_var);
}

void GuildEvent::PushEventVar(core::uint64 event_var) {
  this->event_vars_.push_back(global::ToString(event_var));
}

void GuildEvent::PushEventVar(core::int32 event_var) {
  this->event_vars_.push_back(global::ToString(event_var));
}

void GuildEvent::TranslateEventVar(std::vector<std::string> *result) const {
  // 0 no translate
  // 1 actor_id -> actor_name
  static const char *translate_table[] = {
    "1",                     // CREATE_GUILD
    "1",                     // JOIN_GUILD
    "1",                     // LEAVE_GUILD
    "110",                   // SET_MEMBER_POSITION
    "110",                   // RESET_MEMBER_POSITION
    "10",                    // GIVE_UP_POSITION
    "11",                    // GIVE_UP_LEADER_POSITION
    "0",                     // GUILD_UPGRADE
    "0",                     // GUILD_SHOP_UPGRADE
    "0",                     // GUILD_COLLEGE_UPGRADE 
    "0",                     // GUILD_BARRACK_UPGRADE 
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

}  // namespace guild

}  // namespace server

}  // namespace session

