#ifndef __SESSION__SERVER__SOCIAL__CONTACTS__REQUEST__HANDLER__H
#define __SESSION__SERVER__SOCIAL__CONTACTS__REQUEST__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace social {

class ContactsRequestHandler : public core::Noncopyable {
 public:
  ContactsRequestHandler();
  ~ContactsRequestHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestSocialCheckInBlacklist(void *message, size_t size);
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__CONTACTS__REQUEST__HANDLER__H

