//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-29 02:23:53.
// File name: storage_actor_save_response.h
//
// Description:
// Define class StorageActorSaveResponse.
//

#ifndef __GAME__SERVER__STORAGE__ACTOR__SAVE__RESPONSE__H
#define __GAME__SERVER__STORAGE__ACTOR__SAVE__RESPONSE__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "global/thrift_packet.h"

namespace game {

namespace server {

class StorageActorSaveResponse : public core::Noncopyable {
 public:
  StorageActorSaveResponse();
  ~StorageActorSaveResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__STORAGE__ACTOR__SAVE__RESPONSE__H

