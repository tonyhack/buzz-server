#ifndef __SESSION__SERVER__AUCTION__STORAGE__AUCTION__ACTOR__LOGIN__RESPONSE__H
#define __SESSION__SERVER__AUCTION__STORAGE__AUCTION__ACTOR__LOGIN__RESPONSE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace session {

namespace server {

namespace auction {

class StorageAuctionActorLoginResponse : public core::Noncopyable {
 public:
  StorageAuctionActorLoginResponse();
  ~StorageAuctionActorLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__STORAGE__AUCTION__ACTOR__LOGIN__RESPONSE__H

