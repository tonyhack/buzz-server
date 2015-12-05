#ifndef __SESSION__SERVER__SOCIAL__CONTACTS__MESSAGE__HANDLER__H
#define __SESSION__SERVER__SOCIAL__CONTACTS__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace social {

class ContactsMessageHandler : public core::Noncopyable {
 public:
  ContactsMessageHandler();
  ~ContactsMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 编辑签名请求
  void OnMessageSocialContactsEditSignatureRequest(core::uint64 id, const char *data, size_t size);
  // 新增联系人请求
  void OnMessageSocialContactsAddRequest(core::uint64 id, const char *data, size_t size);
  // 删除联系人请求
  void OnMessageSocialContactsRemoveRequest(core::uint64 id, const char *data, size_t size);
  // 移动联系人请求
  void OnMessageSocialContactsMoveRequest(core::uint64 id, const char *data, size_t size);
  // 推荐联系人请求
  void OnMessageSocialContactsRecommendRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif // __SESSION__SERVER__SOCIAL__CONTACTS__MESSAGE__HANDLER__H

