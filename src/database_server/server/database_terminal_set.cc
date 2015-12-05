//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-16 10:00:03.
// File name: database_terminal_set.cc
//
// Description:
// Define class DatabaseTerminalSet.
//

#include "database_server/server/database_terminal_set.h"

namespace database {

namespace server {

DatabaseTerminalSet::DatabaseTerminalSet() {}
DatabaseTerminalSet::~DatabaseTerminalSet() {}

bool DatabaseTerminalSet::OnTerminalLogin(const DatabaseTerminalPtr &terminal) {
  assert(terminal);
  TerminalGuid guid(terminal->GetSignature().type_, terminal->GetSignature().id_);
#ifdef _SESSION_LOGIN_DATABASE_FAILED
  if(terminal->GetSignature().type_ == global::protocol::ServerType::SESSION_SERVER) {
    return false;
  }
#endif  // _SESSION_LOGIN_DATABASE_FAILED

  core::AutoMutex auto_mutex(&this->mutex_);
  DatabaseTerminalMap::iterator iterator = this->terminals_.find(guid);
  if(iterator == this->terminals_.end()) {
    this->terminals_.insert(std::make_pair(guid, terminal));
    return true;
  }
  return false;
}

bool DatabaseTerminalSet::OnTerminalLogout(const DatabaseTerminalPtr &terminal) {
  assert(terminal);
  TerminalGuid guid(terminal->GetSignature().type_, terminal->GetSignature().id_);

  core::AutoMutex auto_mutex(&this->mutex_);
  DatabaseTerminalMap::iterator iterator = this->terminals_.find(guid);
  if(iterator != this->terminals_.end()) {
    this->terminals_.erase(iterator);
    return true;
  }
  return false;
}

DatabaseTerminalPtr DatabaseTerminalSet::GetTerminal(
    global::protocol::ServerType::type type, core::uint32 id) {
  TerminalGuid guid(type, id);

  core::AutoMutex auto_mutex(&this->mutex_);
  DatabaseTerminalMap::iterator iterator = this->terminals_.find(guid);
  if(iterator != this->terminals_.end()) {
    return iterator->second;
  }
  return DatabaseTerminalPtr();
}

}  // namespace server

}  // namespace database

