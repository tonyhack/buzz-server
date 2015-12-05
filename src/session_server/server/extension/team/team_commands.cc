//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-02 16:31:16.
// File name: team_commands.cc
//
// Description:
// Define class TeamCommands.
//

#include "session_server/server/extension/team/team_commands.h"

#include "session_server/server/global_time_tick.h"

namespace session {

namespace server {

namespace team {

const time_t TeamCommands::kValidTime_ = 12;
const size_t TeamCommands::kCommandSize_ = 30;

TeamCommands::TeamCommands() {}
TeamCommands::~TeamCommands() {}

bool TeamCommands::Initialize() {
  return true;
}

void TeamCommands::Finalize() {
  this->commands_.clear();
}

bool TeamCommands::Add(core::uint64 actor, TeamCommandType::type type) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  // 找到相同的，如果已过期，更新值，如果没过期，则返回错误
  CommandKey key(actor, type);
  CommandMap::iterator iterator = this->commands_.find(key);
  if(iterator != this->commands_.end()) {
    if(now > iterator->second + TeamCommands::kValidTime_) {
      iterator->second = now;
      return true;
    } else {
      return false;
    }
  }

  // 列表长度小于最大长度，直接插入
  if(this->commands_.size() < TeamCommands::kCommandSize_) {
    this->commands_.insert(std::make_pair(key, now));
    return true;
  }

  // 从列表中找出最老的元素
  CommandMap::iterator iterator2 = this->commands_.begin();
  for(iterator = this->commands_.begin();
      iterator != this->commands_.end(); ++iterator) {
    if(iterator->second < iterator2->second) {
      iterator2 = iterator;
    }
  }

  // 如果查找成功，删除最老的元素，插入新的命令
  if(iterator2 != this->commands_.end()) {
    this->commands_.erase(iterator2);
    this->commands_.insert(std::make_pair(key, now));
    return true;
  }

  return false;
}

bool TeamCommands::Remove(core::uint64 actor, TeamCommandType::type type) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  bool ret = false;

  CommandKey key(actor, type);
  CommandMap::iterator iterator = this->commands_.find(key);
  if(iterator != this->commands_.end()) {
    ret = now < iterator->second + TeamCommands::kValidTime_;
    this->commands_.erase(iterator);
  }
  return ret;
}

}  // namespace team

}  // namespace server

}  // namespace session

