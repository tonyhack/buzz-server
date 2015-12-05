

#ifndef __DATEBASE__SERVER__STORAGE__MAIL__DEL__H
#define __DATEBASE__SERVER__STORAGE__MAIL__DEL__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_mail_del_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageMailDel : public core::Noncopyable {
 public:
  StorageMailDel();
  ~StorageMailDel();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageDelMailResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server
                             
}  // namespace database

#endif  // __DATEBASE__SERVER__STORAGE__MAIL__DEL__H

