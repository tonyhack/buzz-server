//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-01 13:52:45.
// File name: facade_request.h
//
// Description:
// Define class FacadeRequest.
//

#ifndef __GAME__SERVER__BUFF__FACADE__REQUEST__H
#define __GAME__SERVER__BUFF__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace buff {

class BuffRole;

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  BuffRole *GetBuffRole(entity::EntityType::type type, core::uint64 id);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__FACADE__REQUEST__H

