//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-31 17:00:00.
// File name: storage_response_handler.h
//
// Description:
// Define class StorageResponseHandler.
//

#ifndef __GATEWAY__SERVER__STORAGE__RESPONSE__HANDLER__H
#define __GATEWAY__SERVER__STORAGE__RESPONSE__HANDLER__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"
#include "global/thrift_packet.h"

namespace gateway {

namespace server {

class StorageResponseHandler : public core::Noncopyable {
 public:
  StorageResponseHandler();
  ~StorageResponseHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnCheckSessionResponse(const std::string &request,
      const std::string &response, database::protocol::ResponseType::type result);

  void OnActorOnlineResponse(const std::string &request,
        const std::string &response, database::protocol::ResponseType::type result);

  global::ThriftPacket thrift_;
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__STORAGE__RESPONSE__HANDLER__H

