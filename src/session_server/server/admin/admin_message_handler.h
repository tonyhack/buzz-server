#ifndef __SESSION__SERVER__ADMIN__ADMIN__MESSAGE__HANDLER__H
#define __SESSION__SERVER__ADMIN__ADMIN__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"

namespace session {

namespace server {

namespace admin {

class AdminSession;

class AdminMessageHandler : public core::Noncopyable {
 public:
  AdminMessageHandler();
  ~AdminMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 玩家人数查询请求
  void OnMessageAdminUserNumberQueryRequest(AdminSession *session,
      const char *data, size_t size);
};

}  // namespace admin

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__ADMIN__ADMIN__MESSAGE__HANDLER__H

