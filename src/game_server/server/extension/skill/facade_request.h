//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-11 10:38:01.
// File name: facade_request.h
//
// Description:
// Define class FacadeRequest.
//

#ifndef __GAME__SERVER__SKILL__FACADE__REQUEST__H
#define __GAME__SERVER__SKILL__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "entity/soul_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace skill {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  // 增加一个BUFF
  bool AddBuff(entity::EntityType::type src_type, core::uint64 src_id,
      entity::EntityType::type dest_type, core::uint64 dest_id, core::uint32 buff);

  // 删除一个BUFF
  bool RemoveBuff(entity::EntityType::type type, core::uint64 id, core::uint32 buff);

  // BUFF是否存在
  bool CheckBuffExist(entity::EntityType::type type, core::uint64 id, core::int32 buff);

  // 是否冷却中
  bool CheckCooling(entity::EntityType::type type, core::uint64 id,
      core::uint32 cooling);

  // 冷却
  bool Cooling(entity::EntityType::type type, core::uint64 id,
      core::uint32 cooling);

  // 检测是否是同队
  bool CheckSameTeam(core::uint64 actor1_id, core::uint64 actor2_id);

  // 获取玩家能量状态
  entity::SoulEnergyStatus::type GetEnergyStatus(core::uint64 actor);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__FACADE__REQUEST__H

