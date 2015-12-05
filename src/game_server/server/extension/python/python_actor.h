//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 10:25:12.
// File name: python_actor.h
//
// Description:
// Define class PythonActor.
//

#ifndef __GAME__SERVER__PYTHON__PYTHON__ACTOR__H
#define __GAME__SERVER__PYTHON__PYTHON__ACTOR__H

#include <boost/shared_ptr.hpp>

#include "core/base/types.h"
#include "entity/role_types.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace python {

class PythonActor {
 public:
  PythonActor();
  ~PythonActor();

  bool Initialize(GameActor *actor);
  void Finalize();

  // 获取属性
  core::int32 GetAttrEntityClient(int type);
  core::int32 GetAttrEntityServer(int type);
  core::int32 GetAttrRoleAoi(int type);
  core::int32 GetAttrRoleClient(int type);
  core::int32 GetAttrRoleServer(int type);
  core::int32 GetAttrActorAoi(int type);
  core::int32 GetAttrActorClient(int type);
  core::int32 GetAttrActorServer(int type);

  // 设置属性
  bool SetAttrEntityClient(int type, core::int32 value);
  bool SetAttrEntityServer(int type, core::int32 value);
  bool SetAttrRoleAoi(int type, core::int32 value);
  bool SetAttrRoleClient(int type, core::int32 value);
  bool SetAttrRoleServer(int type, core::int32 value);
  bool SetAttrActorAoi(int type, core::int32 value);
  bool SetAttrActorClient(int type, core::int32 value);
  bool SetAttrActorServer(int type, core::int32 value);

  inline std::string GetName() const {
    if(this->actor_) {
      return this->actor_->GetName();
    }
    return "";
  }

  inline int GetBasicStatus() const {
    if(this->actor_) {
      return this->actor_->GetBasicStatus();
    }
    return entity::RoleBasicStatus::STATUS_MAX;
  }

  // 增加资源
  inline void AddResource(int type, core::int32 value) {
    if(this->actor_) {
      this->actor_->AddResource((entity::ResourceID::type)type, value);
    }
  }

  inline core::uint32 GetPosx() const {
    if(this->actor_) {
      return this->actor_->GetPosx();
    }
    return 0;
  }

  inline core::uint32 GetPosy() const {
    if(this->actor_) {
      return this->actor_->GetPosy();
    }
    return 0;
  }

  inline core::uint64 GetID() const {
    if(this->actor_) {
      return this->actor_->GetID();
    }
    return 0;
  }

 private:
  GameActor *actor_;
};

typedef boost::shared_ptr<PythonActor> PythonActorPtr;

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__PYTHON__ACTOR__H

