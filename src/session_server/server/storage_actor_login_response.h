//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-26 11:15:16.
// File name: storage_actor_login_response.h
//
// Description:
// Define class StorageActorLoginResponse.
//

#ifndef __SESSION__SERVER__STORAGE__ACTOR__LOGIN__RESPONSE__H
#define __SESSION__SERVER__STORAGE__ACTOR__LOGIN__RESPONSE__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/database_protocol_types.h"

namespace session {

namespace server {

class SessionActor;

class StorageActorLoginResponse : public core::Noncopyable {
 public:
  StorageActorLoginResponse();
  ~StorageActorLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);

  void OnResponseFailure(SessionActor *actor);
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__STORAGE__ACTOR__LOGIN__RESPONSE__H

