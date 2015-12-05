#ifndef __GAME__SERVER__ATTR__STORAGE__ATTR__ACTOR__LOGIN__RESPONSE__H
#define __GAME__SERVER__ATTR__STORAGE__ATTR__ACTOR__LOGIN__RESPONSE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace attr {

class StorageAttrActorLoginResponse : public core::Noncopyable {
 public:
  StorageAttrActorLoginResponse();
  ~StorageAttrActorLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__STORAGE__ATTR__ACTOR__LOGIN__RESPONSE__H
