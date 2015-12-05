
#ifndef __GAME__SERVER__WELFARE__STORAGE__LOGIN__RESPONSE__H
#define __GAME__SERVER__WELFARE__STORAGE__LOGIN__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace welfare {

class StorageLoginResponse : public core::Noncopyable {
 public:
  StorageLoginResponse();
  ~StorageLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);

  void OnActorResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);

};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__STORAGE__LOGIN__RESPONSE__H
