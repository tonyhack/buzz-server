//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-22 19:14:29.
// File name: request_add_item.h
//
// Description:
// Define class RequestAddItem.
//

#ifndef __GAME__SERVER__ITEM__REQUEST__ADD__ITEM__H
#define __GAME__SERVER__ITEM__REQUEST__ADD__ITEM__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace item {

class RequestAddItem : public core::Noncopyable {
 public:
  RequestAddItem();
  ~RequestAddItem();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
  int OnRequestAddItems(void *message, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__REQUEST__ADD__ITEM__H

