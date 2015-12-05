//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-25 13:36:19.
// File name: storage_playing_actor_login_response.h
//
// Description:
// Define class StoragePlayingActorLoginResponse.
//

#ifndef __GAME__SERVER__PLAYING__STORAGE__PLAYING__ACTOR__LOGIN__RESPONSE__H
#define __GAME__SERVER__PLAYING__STORAGE__PLAYING__ACTOR__LOGIN__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace playing {

class StoragePlayingActorLoginResponse : public core::Noncopyable {
 public:
  StoragePlayingActorLoginResponse();
  ~StoragePlayingActorLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__STORAGE__PLAYING__ACTOR__LOGIN__RESPONSE__H

