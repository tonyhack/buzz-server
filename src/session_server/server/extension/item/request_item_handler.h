#ifndef __SESSION__SERVER__REQUEST__ITEM__HANDLER__H
#define __SESSION__SERVER__REQUEST__ITEM__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace session {

namespace server {

namespace item {

class RequestItemHandler : public core::Noncopyable {
 public:
  RequestItemHandler();
  ~RequestItemHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestMallTreasureConfigureData(void *message, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__REQUEST__ITEM__HANDLER__H

