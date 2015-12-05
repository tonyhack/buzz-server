//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-06 11:20:38.
// File name: scene.h
//
// Description:
// Define class Scene.
//

#ifndef __SESSION__SERVER__SCENE__SCENE__H
#define __SESSION__SERVER__SCENE__SCENE__H

#include <set>

#include "core/base/noncopyable.h"
#include "global/game_misc.h"
#include "session_server/server/extension/scene/world_boss.h"

namespace session {

namespace server {

namespace scene {

class Scene : public core::Noncopyable {
 public:
  typedef std::set<core::uint64> ActorSet;

  Scene();
  ~Scene();

  bool Initialize(core::uint64 id, bool dynamic);
  void Finalize();

  inline core::uint64 GetID() const {
    return this->id_;
  }
  inline core::uint16 GetMapID() const {
    return global::GameMisc::GetSceneMapID(this->GetID());
  }

  inline core::uint16 GetBranchID() const {
    return global::GameMisc::GetSceneSequenceID(this->GetID());
  }

  inline core::uint32 GetGameServer() const {
    return this->game_server_;
  }
  inline void SetGameServer(core::uint32 game_server) {
    this->game_server_ = game_server;
  }

  inline size_t GetActorSize() const {
    return this->actors_.size();
  }
  const ActorSet &GetActorSet() const {
    return this->actors_;
  }
  WorldBoss& GetWorldBoss() {
    return this->world_boss_;
  }

  // 增加玩家
  bool AddActor(core::uint64 id);
  bool RemoveActor(core::uint64 id);

  // 判定场景类型
  bool CheckDynamic() {
    return this->dynamic_;
  }

 private:
  WorldBoss world_boss_;

  core::uint64 id_;

  core::uint32 game_server_;

  ActorSet actors_;

  bool dynamic_;
};

}  // namespace scene

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__SCENE__H

