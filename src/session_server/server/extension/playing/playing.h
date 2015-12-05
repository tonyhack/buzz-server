//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-13 16:49:15.
// File name: playing.h
//
// Description:
// Define class Playing.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__H
#define __SESSION__SERVER__PLAYING__PLAYING__H

#include <set>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "session_server/server/extension/playing/playing_types.h"

namespace session {

namespace server {

namespace playing {

class Playing : public core::Noncopyable {
  typedef std::set<core::uint64> ActorSet;

 public:
  Playing();
  ~Playing();

  void Allocate(core::uint64 id);
  void Deallocate();

  bool Initialize(core::uint64 template_id);
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

  inline core::uint64 GetScene() const {
    return this->scene_;
  }
  inline void SetScene(core::uint64 scene) {
    this->scene_ = scene;
  }

  inline core::uint32 GetGameServer() const {
    return this->game_server_;
  }
  inline void SetGameServer(core::uint32 game_server) {
    this->game_server_ = game_server;
  }

  // 是否可以参加
  inline bool CheckJoin(core::uint64 actor) const {
    return this->actors_.find(actor) != this->actors_.end();
  }

  inline void SetStatus(StatusType::type type) {
    this->status_ = type;
  }
  inline StatusType::type GetStatus() const {
    return this->status_;
  }

  // 增加玩家
  bool AddActor(core::uint64 actor);

  // 清除
  void ClearActors();

  // 同步创建完成
  void BroadcastCreateFinish();

 private:
  core::uint32 template_id_;

  core::uint64 id_;

  core::uint64 scene_;

  core::uint32 game_server_;

  // 状态
  StatusType::type status_;

  // 玩家列表
  ActorSet actors_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__H

