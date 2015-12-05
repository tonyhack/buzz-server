//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-22 14:25:47.
// File name: request_scene_forbid_drop_item.h
//
// Description:
// Define class RequestSceneForbidDropItem.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__FORBID__DROP__ITEM__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__FORBID__DROP__ITEM__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSceneForbidDropItem : public core::Noncopyable {
 public:
  RequestSceneForbidDropItem();
  ~RequestSceneForbidDropItem();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *data, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__FORBID__DROP__ITEM__H

