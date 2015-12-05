//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-29 22:13:43.
// File name: storage_cooling_save.h
//
// Description:
// Define class StorageSkillSave.
//

#ifndef __DATABASE__SERVER__STORAGE__SKILL__SAVE__H
#define __DATABASE__SERVER__STORAGE__SKILL__SAVE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_skill_save_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageSkillSave : public core::Noncopyable {
 public:
  StorageSkillSave();
  ~StorageSkillSave();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageSkillSaveResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__SKILL__SAVE__H

