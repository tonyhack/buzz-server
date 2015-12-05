#ifndef __SESSION__SERVER__SOCIAL__STORAGE__SOCIAL__ACTOR__LOGIN__RESPONSE__H
#define __SESSION__SERVER__SOCIAL__STORAGE__SOCIAL__ACTOR__LOGIN__RESPONSE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace session {

namespace server {

namespace social {

class StorageSocialActorLoginResponse : public core::Noncopyable {
 public:
  StorageSocialActorLoginResponse();
  ~StorageSocialActorLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__STORAGE__SOCIAL__ACTOR__LOGIN__RESPONSE__H

