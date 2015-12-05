//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-02 15:49:03.
// File name: team_commands.h
//
// Description:
// Define class TeamCommands.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__COMMANDS__H
#define __SESSION__SERVER__TEAM__TEAM__COMMANDS__H

#include <time.h>

#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "session_server/server/extension/team/team_types.h"

namespace session {

namespace server {

namespace team {

class TeamCommands : public core::Noncopyable {
  typedef std::pair<core::uint64, TeamCommandType::type> CommandKey;
  typedef std::map<CommandKey, time_t> CommandMap;

 public:
  TeamCommands();
  ~TeamCommands();

  bool Initialize();
  void Finalize();

  // 增加
  bool Add(core::uint64 actor, TeamCommandType::type type);
  // 删除(返回删除的元素是否存在且没有超时)
  bool Remove(core::uint64 actor, TeamCommandType::type type);

 private:
  // 命令列表
  CommandMap commands_;

  static const time_t kValidTime_;
  static const size_t kCommandSize_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__COMMANDS__H

