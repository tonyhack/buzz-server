//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-13 10:40:32.
// File name: playing_scene.h
//
// Description:
// Define class Playing.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__H
#define __GAME__SERVER__PLAYING__PLAYING__H

#include <cstddef>
#include <map>
#include <set>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/playing/playing_types.h"

namespace game {

namespace server {

namespace playing {

class PlayingActor;

class Playing : public core::Noncopyable {
 public:
  typedef std::set<core::uint64> ActorSet;

  Playing();
  virtual ~Playing();

  bool Initialize(core::uint32 template_id, core::uint64 id);
  void Finalize();

  inline core::uint64 GetID() const {
    return this->id_;
  }
  inline void SetID(core::uint64 id) {
    this->id_ = id;
  }

  inline core::uint32 GetTemplateID() const {
    return this->template_id_;
  }
  inline void SetTeamplateID(core::uint32 template_id) {
    this->template_id_ = template_id;
  }

  inline core::uint64 GetSceneID() const {
    return this->scene_;
  }
  inline void SetSceneID(core::uint64 scene) {
    this->scene_ = scene;
  }

  inline StatusType::type GetStatus() const {
    return this->status_;
  }
  inline void SetStatus(StatusType::type status) {
    this->status_ = status;
  }

  inline const ActorSet &GetActors() const {
    return this->actors_;
  }

  inline size_t GetActorSize() const {
    return this->actors_.size();
  }

  bool AddActor(core::uint64 actor);
  void RemoveActor(core::uint64 actor);

  void KickoutActors();

  void RequireDestory();

 private:
  core::uint64 id_;

  core::uint32 template_id_;

  core::uint64 scene_;

  StatusType::type status_;

  ActorSet actors_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__H

