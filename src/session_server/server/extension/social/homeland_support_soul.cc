#include "session_server/server/extension/social/homeland_support_soul.h"

#include <cstdio>

#include "global/logging.h"
#include "global/common_functions.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor_manager.h"

namespace session {

namespace server {

namespace social {

HomelandSupportSoul::HomelandSupportSoul(core::uint32 soul_id,
    core::uint64 support_actor_id) :
    soul_id_(soul_id), support_actor_id_(support_actor_id), next_support_time_(0) {
}

bool HomelandSupportSoul::Initialize(const std::string &data) {
  std::vector<std::string> split_result;
  global::StringSplit(data.c_str(), ":", split_result);

  if (split_result.size() < 3) {
    return false;
  }

  // 检查 support_actor_id
  core::uint64 support_actor_id = atol(split_result[1].c_str());
  if (NULL == SessionActorManager::GetInstance()->GetBriefActor(support_actor_id)) {
    LOG_ERROR("actor_id(%lu) does not exist.", support_actor_id);
    return false;
  }

  this->soul_id_ = atoi(split_result[0].c_str());
  this->support_actor_id_ = support_actor_id;
  this->next_support_time_ = atol(split_result[2].c_str());

  return true;
}

int HomelandSupportSoul::Serialize(char *buff, size_t size) const {
  return snprintf(buff, size, "%u:%lu:%lu,",
      this->soul_id_, this->support_actor_id_, this->next_support_time_);
}

core::int32 HomelandSupportSoul::GetNextSupportCooling(time_t now) const {
  if (0 == now) {
    now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  }

  if (this->next_support_time_ > now) {
    return this->next_support_time_ - now;
  } else {
    return 0;
  }
}

void HomelandSupportSoul::SetNextSupportCooling(core::int32 cooling_time) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  
  this->next_support_time_ = now + cooling_time;
}

}  // namespace social

}  // namespace server

}  // namespace session

