#ifndef __GAME__SERVER__WELFARE__FACEDE__EXTENSION__H
#define __GAME__SERVER__WELFARE__FACEDE__EXTENSION__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/achieve_types.h"
#include "game_server/server/extension/welfare/load_task_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace welfare {

class FacedeRequest : public global::SingletonFactory<FacedeRequest> {
 friend class global::SingletonFactory<FacedeRequest>;

 public:
  FacedeRequest();
  ~FacedeRequest();

  core::uint32 GetNpcTemplateID(core::uint64 id);

  bool GetBuffExist(core::uint64 id, core::uint32 buff_id);

  core::int32 RequestActorEquipNum(core::uint64 id, 
      entity::AchieveWearEquipType::type type, core::uint32 num);

  bool AddItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, bool bind = false);

  bool Awards(core::uint64 actor, const Award *awards, core::int32 num);

  core::int32 GetSpiritFacadeType(core::uint64 actor);

 private:

};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__FACEDE__EXTENSION__H
