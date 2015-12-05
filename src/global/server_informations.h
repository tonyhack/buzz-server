//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-14 14:30:27.
// File name: server_informations.h
//
// Description:
// Define server informations.
//

#ifndef __GLOBAL__SERVER__INFORMATIONS__H
#define __GLOBAL__SERVER__INFORMATIONS__H

#include <map>
#include <string>
#include <vector>

#include "core/base/types.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"

namespace global {

class AutoSaveInformation {
 public:
  AutoSaveInformation() : actor_(0) {}
  ~AutoSaveInformation() {}

  core::uint32 actor_;
};

class ServerInformation {
 public:
  ServerInformation() : id_(0), port_(0) {}
  virtual ~ServerInformation() {}

  core::uint32 id_;
  std::string ip_;
  core::uint16 port_;

  std::string log_;
  std::string core_;
  std::string coresh_;
};

class DatabaseServerInformation : public ServerInformation {
 public:
  DatabaseServerInformation() : db_thread_number_(0),
    request_queue_size_(0), request_queue_reduce_waterline_(0) {}
  virtual ~DatabaseServerInformation() {}

  core::uint32 db_thread_number_;
  core::uint32 request_queue_size_;
  core::int32 request_queue_reduce_waterline_;
};

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

class GatewayServerInformation : public ServerInformation {
 public:
  GatewayServerInformation()
    : user_pool_initial_(0), user_pool_extend_(0), max_user_number_(0) {}
  ~GatewayServerInformation() {}

  size_t user_pool_initial_;
  size_t user_pool_extend_;
  size_t max_user_number_;
};

class SessionServerInformation : public ServerInformation {
 public:
  SessionServerInformation()
    : user_pool_initial_(0), user_pool_extend_(0), max_user_number_(0) {}
  ~SessionServerInformation() {}

  std::string admin_ip_;
  core::uint16 admin_port_;
  size_t user_pool_initial_;
  size_t user_pool_extend_;
  size_t max_user_number_;
};

class GameServerInformation : public ServerInformation {
 public:
  GameServerInformation()
    : user_pool_initial_(0), user_pool_extend_(0),
    npc_pool_initial_(0), npc_pool_extend_(0) {}
  ~GameServerInformation() {}

  size_t user_pool_initial_;
  size_t user_pool_extend_;

  size_t npc_pool_initial_;
  size_t npc_pool_extend_;
};

class GameDatabaseInformation {
 public:
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
  DatabaseInformation game_database_;
  ProcedureMap procedures_;
};

}  // namespace global

#endif  // __GLOBAL__SERVER__INFORMATIONS__H

