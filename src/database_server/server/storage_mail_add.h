#ifndef __DATEBASE__SERVER__STORAGE__MAIL__ADD__H
#define __DATEBASE__SERVER__STORAGE__MAIL__ADD__H

#include "core/base/noncopyable.h"

#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_mail_add_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageMailAdd : public core::Noncopyable {
 public:
  StorageMailAdd();
  ~StorageMailAdd();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageAddMailResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server
                             
}  // namespace database

#endif  // __DATEBASE__SERVER__STORAGE__MAIL__ADD__H

