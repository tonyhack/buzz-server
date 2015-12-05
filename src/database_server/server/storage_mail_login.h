
#ifndef __DATEBASE__SERVER__STORAGE__MAIL__LOGIN__H
#define __DATEBASE__SERVER__STORAGE__MAIL__LOGIN__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_mail_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageMailLogin : public core::Noncopyable {
 public:
  StorageMailLogin();
  ~StorageMailLogin();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageMailLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server
                             
}  // namespace database

#endif  // __DATEBASE__SERVER__STORAGE__MAIL__LOAD__H

