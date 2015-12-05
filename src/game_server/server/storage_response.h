//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-27 10:49:25.
// File name: storage_response.h
//
// Description:
// Define class StorageResponse.
//

#ifndef __GAME__SERVER__STORAGE__RESPONSE__H
#define __GAME__SERVER__STORAGE__RESPONSE__H

#include "core/base/noncopyable.h"
#include "game_server/server/storage_actor_login_response.h"
#include "game_server/server/storage_actor_save_response.h"
#include "global/thrift_packet.h"

namespace game {

namespace server {

class StorageResponse : public core::Noncopyable {
 public:
  StorageResponse();
  ~StorageResponse();

  bool Initialize();
  void Finalize();

 private:
  StorageActorLoginResponse actor_login_;
  StorageActorSaveResponse actor_save_;

  global::ThriftPacket thrift_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__STORAGE__RESPONSE__H

