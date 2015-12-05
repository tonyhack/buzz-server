//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-08 22:29:16.
// File name: transport_npc.h
//
// Description:
// Define class TransportNpc.
//

#ifndef __GAME__SERVER__TRANSPORT__TRANSPORT__NPC__H
#define __GAME__SERVER__TRANSPORT__TRANSPORT__NPC__H

#include "core/base/noncopyable.h"
#include "game_server/server/game_npc.h"

namespace game {

namespace server {

namespace transport {

class TransportNpc : public core::Noncopyable {
 public:
  TransportNpc();
  ~TransportNpc();

  bool Initialize(GameNpc *npc);
  void Finalize();

  void Lose();
  void Regain();

  inline GameNpc *GetNpc() {
    return this->npc_;
  }
  inline core::uint64 GetID() {
    return this->npc_->GetID();
  }

  inline core::uint64 GetMasterActor() const {
    return this->master_actor_;
  }
  inline void SetMasterActor(core::uint64 actor) {
    this->master_actor_ = actor;
  }

  inline const std::string &GetMasterName() const {
    return this->master_name_;
  }
  inline void SetMasterName(const std::string &name) {
    this->master_name_ = name;
  }

  inline bool GetLost() const {
    return this->lost_;
  }
  inline void SetLost(bool lost) {
    this->lost_ = lost;
  }

 private:
  GameNpc *npc_;

  core::uint64 master_actor_;
  std::string master_name_;

  bool lost_;
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__TRANSPORT__NPC__H

