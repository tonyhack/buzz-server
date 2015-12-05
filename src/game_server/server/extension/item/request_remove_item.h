//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-20 14:02:57.
// File name: request_remove_item.h
//
// Description:
// Define class RequestRemoveItem.
//

#ifndef __GAME__SERVER__ITEM__REQUEST__REMOVE__ITEM__H
#define __GAME__SERVER__ITEM__REQUEST__REMOVE__ITEM__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace item {

class RequestRemoveItem : public core::Noncopyable {
 public:
  RequestRemoveItem();
  ~RequestRemoveItem();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
  int OnRemoveLocationIDRequest(void *message, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__REQUEST__REMOVE__ITEM__H

