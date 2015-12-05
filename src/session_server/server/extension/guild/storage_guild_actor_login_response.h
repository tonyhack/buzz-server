#ifndef __SESSION__SERVER__GUILD__STORAGE__GUILD__ACTOR__LOGIN__RESPONSE__H
#define __SESSION__SERVER__GUILD__STORAGE__GUILD__ACTOR__LOGIN__RESPONSE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace session {

namespace server {

namespace guild {

class StorageGuildActorLoginResponse : public core::Noncopyable {
 public:
  StorageGuildActorLoginResponse();
  ~StorageGuildActorLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__STORAGE__GUILD__ACTOR__LOGIN__RESPONSE__H

