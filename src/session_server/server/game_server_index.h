//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-27 15:08:19.
// File name: game_server_index.h
//
// Description:
// Define class GameServerIndex.
//

#ifndef __SESSION__SERVER__GAME__SERVER__INDEX__H
#define __SESSION__SERVER__GAME__SERVER__INDEX__H

#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

class GameServerData {
 public:
  GameServerData() : user_number_(0) {}
  ~GameServerData() {}

  inline core::uint32 GetUserNumber() const {
    return this->user_number_;
  }
  inline void SetUserNumber(core::uint32 number) {
    this->user_number_ = number;
  }

  const GameServerData &operator=(const GameServerData &data) {
    this->SetUserNumber(data.GetUserNumber());
    return *this;
  }

 private:
  core::uint32 user_number_;
};

class GameServerIndex : public core::Noncopyable {
  typedef std::map<core::uint32, GameServerData> GameServerDataMap;

 public:
  GameServerIndex();
  ~GameServerIndex();

  bool Add(core::uint32 id);
  bool Update(core::uint32 id, const GameServerData &data);
  bool Remove(core::uint32 id);
  core::uint32 GetServer() const;

 private:
  GameServerDataMap servers_;
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GAME__SERVER__INDEX__H

