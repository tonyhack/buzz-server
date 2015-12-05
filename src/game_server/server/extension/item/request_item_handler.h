#ifndef __GAME__SERVER__ITEM__REQUEST__ITEM__HANDLER__H
#define __GAME__SERVER__ITEM__REQUEST__ITEM__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace item {

class RequestItemHandler : public core::Noncopyable {
 public:
  RequestItemHandler();
  ~RequestItemHandler();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestEquipWeardInfo(void *message, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__REQUEST__ITEM__HANDLER__H

