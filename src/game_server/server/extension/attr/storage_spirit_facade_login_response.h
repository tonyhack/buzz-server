#ifndef __GAME__SERVER__ATTR__STORAGE__SPIRIT__FACADE__LOGIN__RESPONSE__H
#define __GAME__SERVER__ATTR__STORAGE__SPIRIT__FACADE__LOGIN__RESPONSE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace attr {

class StorageSpiritFacadeLoginResponse : public core::Noncopyable {
 public:
  StorageSpiritFacadeLoginResponse();
  ~StorageSpiritFacadeLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__STORAGE__SPIRIT__FACADE__LOGIN__RESPONSE__H
