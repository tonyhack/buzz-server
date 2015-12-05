//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-08 12:22:58.
// File name: soul_energy.h
//
// Description:
// Define class SoulEnergy.
//

#ifndef __GAME__SERVER__SOUL__SOUL__ENERGY__H
#define __GAME__SERVER__SOUL__SOUL__ENERGY__H

#include "core/timer_handler.h"
#include "database_server/protocol/storage_soul_actor_login_types.h"
#include "entity/soul_types.h"
#include "game_server/server/extension/soul/soul_energy_status.h"

namespace game {

namespace server {

namespace soul {

class SoulActor;

class SoulEnergy : public core::TimerHandler {
  enum { TIMER_ID_ENERGY = 0, };

 public:
  SoulEnergy();
  virtual ~SoulEnergy();

  bool Initialize(SoulActor *actor,
      const database::protocol::StorageSoulActorField &db_field);
  void Finalize();
  void Save(database::protocol::StorageSoulActorField &db_field);

  bool OnLoadFinish();

  void GotoStatus(entity::SoulEnergyStatus::type status);

  virtual void OnTimer(core::uint32 id);

  inline EnergyStatus *GetCurrentStatus() {
    return this->current_status_;
  }

  inline SoulActor *GetActor() {
    return this->actor_;
  }

 private:
  SoulActor *actor_;
  core::int32 load_current_energy_;
  bool timer_start_;

  EnergyStatusIncrement status_increment_;
  EnergyStatusExplosion status_explosion_;
  EnergyStatusDecrement status_decrement_;

  EnergyStatus *current_status_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__ENERGY__H

