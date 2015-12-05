//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 15:01:41.
// File name: server_configure.h
//
// Description:
// Define class ServerConfigure.
//

#ifndef __LOGIN__SERVER__SERVER__CONFIGURE__H
#define __LOGIN__SERVER__SERVER__CONFIGURE__H

#include <map>
#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace login {

namespace server {

class DatabaseInformation {
 public:
  DatabaseInformation() : port_(0) {}
  ~DatabaseInformation() {}

  std::string ip_;
  core::uint32 port_;
  std::string user_;
  std::string passwd_;
  std::string db_;
};

class GameDatabaseInformation {
 public:
  typedef std::vector<DatabaseInformation> DatabaseVector;
  typedef std::map<core::uint16, DatabaseVector> DatabaseMap;
  typedef std::map<int, std::string> ProcedureMap;

  GameDatabaseInformation() {}
  ~GameDatabaseInformation() {}

  bool InsertProcedure(int type, const std::string &procedure) {
    ProcedureMap::iterator iterator = this->procedures_.find(type);
    if(iterator == this->procedures_.end()) {
      this->procedures_.insert(std::make_pair(type, procedure));
      return true;
    }
    return false;
  }

  bool GetProcedure(int type, std::string &procedure) {
    ProcedureMap::iterator iterator = this->procedures_.find(type);
    if(iterator != this->procedures_.end()) {
      procedure = iterator->second;
      return true;
    }
    return false;
  }

  DatabaseInformation center_database_;
  DatabaseMap databases_;
  ProcedureMap procedures_;
};

class ServerInformation {
 public:
  ServerInformation() {}
  ~ServerInformation() {}

  std::string ip_;
  core::uint16 port_;
};

class ServerConfigure : public global::SingletonFactory<ServerConfigure> {
  friend class global::SingletonFactory<ServerConfigure>;

 public:
  ServerConfigure();
  ~ServerConfigure();

  bool LoadFile(const char *file);

  inline const GameDatabaseInformation *GetDatabaseInfo() const {
    return &this->database_;
  }
  inline const ServerInformation *GetGatewayInfo() const {
    return &this->gateway_;
  }
  inline const ServerInformation *GetLocalServer() const {
    return &this->login_;
  }

  inline const std::string &GetLog() const {
    return this->log_;
  }
  inline const std::string &GetCore() const {
    return this->core_;
  }
  inline const std::string &GetDirtyWordFile() const {
    return this->dirty_word_file_;
  }

 private:
  GameDatabaseInformation database_;

  ServerInformation gateway_;
  ServerInformation login_;

  std::string log_;
  std::string core_;
  std::string dirty_word_file_;
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__SERVER__CONFIGURE__H

