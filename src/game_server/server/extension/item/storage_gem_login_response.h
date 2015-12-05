#ifndef __GAME__SERVER__ITEM__STORAGE__GEM__LOGIN__RESPONSE__H
#define __GAME__SERVER__ITEM__STORAGE__GEM__LOGIN__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace item {

class StorageGemLoginResponse : public core::Noncopyable {
 public:
  StorageGemLoginResponse();
  ~StorageGemLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__STORAGE__GEM__LOGIN__RESPONSE__H

