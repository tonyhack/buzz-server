#include "session_server/server/extension/social/homeland_invader.h"

#include <cstdio>

#include "global/logging.h"
#include "global/common_functions.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor_manager.h"

namespace session {

namespace server {

namespace social {

HomelandInvader::HomelandInvader(core::uint64 actor_id, time_t invade_time) :
    actor_id_(actor_id), invade_time_(invade_time) {}

bool HomelandInvader::Initialize(const std::string &data) {
  std::vector<std::string> split_result;
  global::StringSplit(data.c_str(), ":", split_result);

  if (split_result.size() < 2) {
    return false;
  }

  // 检查 actor_id
  core::uint64 actor_id = atol(split_result[0].c_str());
  if (NULL == SessionActorManager::GetInstance()->GetBriefActor(actor_id)) {
    LOG_ERROR("actor_id(%lu) does not exist.", actor_id);
    return false;
  }

  this->actor_id_ = actor_id;
  this->invade_time_ = atol(split_result[1].c_str());

  return true;
}

int HomelandInvader::Serialize(char *buff, size_t size) const {
  return snprintf(buff, size, "%lu:%lu,",
      this->actor_id_, this->invade_time_);
}

}  // namespace social

}  // namespace server

}  // namespace session

