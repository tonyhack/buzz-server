//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-29 11:35:00.
// File name: storage_response.h
//
// Description:
// Define class StorageResponse.
//

#ifndef __SESSION__SERVER__STORAGE__RESPONSE__H
#define __SESSION__SERVER__STORAGE__RESPONSE__H

#include "core/base/noncopyable.h"
#include "session_server/server/storage_actor_login_response.h"

namespace session {

namespace server {

class StorageResponse : public core::Noncopyable {
 public:
  StorageResponse();
  ~StorageResponse();

  bool Initialize();
  void Finalize();

 private:
  StorageActorLoginResponse actor_login_;
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__STORAGE__RESPONSE__H

