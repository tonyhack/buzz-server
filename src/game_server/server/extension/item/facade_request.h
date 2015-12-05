//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-01 18:12:51.
// File name: facade_request.h
//
// Description:
// Define class FacadeRequest.
//

#ifndef __GAME__SERVER__ITEM__FACADE__REQUEST__H
#define __GAME__SERVER__ITEM__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace item {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  // 所谓检测是否在冷却
  bool CheckCooling(entity::EntityType::type type, core::uint64 id,
      core::uint32 cooling);

  // 开启冷却
  void StartCooling(entity::EntityType::type type, core::uint64 id,
      core::uint32 cooling);

  // 加效果
  void StartEffect(entity::EntityType::type type, core::uint64 id, core::int32 effect_id);

  // 关闭效果
  void StopEffect(entity::EntityType::type type, core::uint64 id, core::int32 effect_id);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__FACADE__REQUEST__H

