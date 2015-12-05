#include "session_server/server/extension/social/homeland_supported_soul.h"

#include <cstdio>

#include "global/logging.h"
#include "global/common_functions.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor_manager.h"

namespace session {

namespace server {

namespace social {

HomelandSupportedSoul::HomelandSupportedSoul(core::uint64 actor_id,
    core::uint32 soul_id, core::int32 soul_level) :
    actor_id_(actor_id), soul_id_(soul_id),
    soul_level_(soul_level), leave_time_(0) {
}

bool HomelandSupportedSoul::Initialize(const std::string &data) {
  std::vector<std::string> split_result;
  global::StringSplit(data.c_str(), ":", split_result);

  if (split_result.size() < 4) {
    return false;
  }

  // 检查 actor_id
  core::uint64 actor_id = atol(split_result[0].c_str());
  if (NULL == SessionActorManager::GetInstance()->GetBriefActor(actor_id)) {
    LOG_ERROR("actor_id(%lu) does not exist.", actor_id);
    return false;
  }

  this->actor_id_ = actor_id;
  this->soul_id_ = atoi(split_result[1].c_str());
  this->soul_level_ = atoi(split_result[2].c_str());
  this->leave_time_ = atol(split_result[3].c_str());

  return true;
}

int HomelandSupportedSoul::Serialize(char *buff, size_t size) const {
  return snprintf(buff, size, "%lu:%u:%d:%lu,",
      this->actor_id_, this->soul_id_, this->soul_level_, this->leave_time_);
}

core::int32 HomelandSupportedSoul::GetLeaveCooling(time_t now) const {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->leave_time_ > now) {
    return this->leave_time_ - now;
  } else {
    return 0;
  }
}

void HomelandSupportedSoul::SetLeaveCooling(core::int32 cooling_time) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  this->leave_time_ = now + cooling_time;
}

}  // namespace social

}  // namespace server

}  // namespace session

