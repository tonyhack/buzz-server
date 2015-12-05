//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-04 20:47:54.
// File name: storage_actor_login_response.h
//
// Description:
// Define class StorageActorLoginResponse.
//

#ifndef __GAME__SERVER__STORAGE__ACTOR__LOGIN__RESPONSE__H
#define __GAME__SERVER__STORAGE__ACTOR__LOGIN__RESPONSE__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "global/thrift_packet.h"

namespace game {

namespace server {

class StorageActorLoginResponse : public core::Noncopyable {
 public:
  StorageActorLoginResponse();
  ~StorageActorLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);

  void ResponseFailed(core::uint64 actor);
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__STORAGE__ACTOR__LOGIN__RESPONSE__H

