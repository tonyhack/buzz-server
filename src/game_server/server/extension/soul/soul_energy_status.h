//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-09 10:09:08.
// File name: soul_energy_status.h
//
// Description:
// Define status of energy.
//

#ifndef __GAME__SERVER__SOUL__SOUL__ENERGY__STATUS__H
#define __GAME__SERVER__SOUL__SOUL__ENERGY__STATUS__H

#include "entity/soul_types.h"

#include "game_server/server/event/game_event_types.h"

namespace game {

namespace server {

namespace soul {

class SoulEnergy;

class EnergyStatus {
 public:
  EnergyStatus() {}
  virtual ~EnergyStatus() {}

  virtual bool Initialize(SoulEnergy *energy);
  virtual void Finalize();

  virtual entity::SoulEnergyStatus::type GetType() = 0;

  virtual void OnEnter() = 0;
  virtual void OnLeave() = 0;
  virtual void OnLoadFinish() = 0;

  virtual void OnEvent(event::EventType::type type, const void *message,
      size_t size) {}
  virtual void OnTimer() {}

 protected:
  SoulEnergy *energy_;
};

class EnergyStatusIncrement : public EnergyStatus {
 public:
  EnergyStatusIncrement() {}
  virtual ~EnergyStatusIncrement() {}

  virtual entity::SoulEnergyStatus::type GetType() {
    return entity::SoulEnergyStatus::INCREMENT;
  }

  virtual void OnEnter();
  virtual void OnLeave();
  virtual void OnLoadFinish();

  virtual void OnTimer();
  virtual void OnEvent(event::EventType::type type, const void *message,
      size_t size);

 private:
  void Explode();
};

class EnergyStatusExplosion : public EnergyStatus {
 public:
  EnergyStatusExplosion() {}
  virtual ~EnergyStatusExplosion() {}

  virtual entity::SoulEnergyStatus::type GetType() {
    return entity::SoulEnergyStatus::EXPLOSION;
  }

  virtual void OnEnter();
  virtual void OnLeave();
  virtual void OnLoadFinish();

  virtual void OnEvent(event::EventType::type type, const void *message,
      size_t size);

 private:
  void OnEventRoleCommonStatusChange(const void *message, size_t size);
};

class EnergyStatusDecrement : public EnergyStatus {
 public:
  EnergyStatusDecrement() {}
  virtual ~EnergyStatusDecrement() {}

  virtual entity::SoulEnergyStatus::type GetType() {
    return entity::SoulEnergyStatus::DECREMENT;
  }

  virtual void OnEnter();
  virtual void OnLeave();
  virtual void OnLoadFinish();

  virtual void OnTimer();
  virtual void OnEvent(event::EventType::type type, const void *message,
      size_t size);

 private:
  void Depleted();
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__ENERGY__STATUS__H

