#ifndef __SESSION__SERVER__SOCIAL__ENEMY__MESSAGE__HANDLER__H
#define __SESSION__SERVER__SOCIAL__ENEMY__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace social {

class EnemyMessageHandler : public core::Noncopyable {
 public:
  EnemyMessageHandler();
  ~EnemyMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 新增仇人请求
  void OnMessageSocialEnemyAddRequest(core::uint64 id, const char *data, size_t size);
  // 删除仇人请求
  void OnMessageSocialEnemyRemoveRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__ENEMY__MESSAGE__HANDLER__H

