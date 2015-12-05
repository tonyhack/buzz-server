//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-16 09:52:03.
// File name: database_terminal_set.h
//
// Description:
// Define class DatabaseTerminalSet.
//

#ifndef __DATABASE__SERVER__TERMINAL__SET__h
#define __DATABASE__SERVER__TERMINAL__SET__h

#include <map>

#include "core/base/mutex.h"
#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/server/database_terminal.h"
#include "global/server_type_types.h"

namespace database {

namespace server {

class DatabaseTerminalSet : public core::Noncopyable {
  typedef std::pair<global::protocol::ServerType::type, core::uint32> TerminalGuid;
  typedef std::map<TerminalGuid, DatabaseTerminalPtr> DatabaseTerminalMap;

 public:
  DatabaseTerminalSet();
  ~DatabaseTerminalSet();

  // Login and logout.
  bool OnTerminalLogin(const DatabaseTerminalPtr &terminal);
  bool OnTerminalLogout(const DatabaseTerminalPtr &terminal);

  // Get from guid.
  DatabaseTerminalPtr GetTerminal(global::protocol::ServerType::type type,
      core::uint32 id);

 private:
  core::Mutex mutex_;

  DatabaseTerminalMap terminals_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__TERMINAL__SET__h

