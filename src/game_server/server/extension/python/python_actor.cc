//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 10:45:02.
// File name: python_actor.cc
//
// Description:
// Define class PythonActor.
//

#include "game_server/server/extension/python/python_actor.h"

namespace game {

namespace server {

namespace python {

PythonActor::PythonActor() : actor_(NULL) {}
PythonActor::~PythonActor() {}

bool PythonActor::Initialize(GameActor *actor) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 参数 actor 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->actor_ = actor;
  return true;
}

void PythonActor::Finalize() {
  this->actor_ = NULL;
}

core::int32 PythonActor::GetAttrEntityClient(int type) {
  if(this->actor_ == NULL ||
      type < entity::EntityClientFields::CLIENT_BEGIN ||
      type >= entity::EntityClientFields::CLIENT_END) {
    return 0;
  }
  return this->actor_->GetAttribute((entity::EntityClientFields::type)type);
}

core::int32 PythonActor::GetAttrEntityServer(int type) {
  if(this->actor_ == NULL ||
      type < entity::EntityServerFields::SERVER_BEGIN ||
      type >= entity::EntityServerFields::SERVER_END) {
    return 0;
  }
  return this->actor_->GetAttribute((entity::EntityServerFields::type)type);
}

core::int32 PythonActor::GetAttrRoleAoi(int type) {
  if(this->actor_ == NULL ||
      type < entity::RoleAoiFields::AOI_BEGIN ||
      type >= entity::RoleAoiFields::AOI_END) {
    return 0;
  }
  return this->actor_->GetAttribute((entity::RoleAoiFields::type)type);
}

core::int32 PythonActor::GetAttrRoleClient(int type) {
  if(this->actor_ == NULL ||
      type < entity::RoleClientFields::CLIENT_BEGIN ||
      type >= entity::RoleClientFields::CLIENT_END) {
    return 0;
  }
  return this->actor_->GetAttribute((entity::RoleClientFields::type)type);
}

core::int32 PythonActor::GetAttrRoleServer(int type) {
  if(this->actor_ == NULL ||
      type < entity::RoleServerFields::SERVER_BEGIN ||
      type >= entity::RoleServerFields::SERVER_END) {
    return 0;
  }
  return this->actor_->GetAttribute((entity::RoleServerFields::type)type);
}

core::int32 PythonActor::GetAttrActorAoi(int type) {
  if(this->actor_ == NULL ||
      type < entity::ActorAoiFields::AOI_BEGIN ||
      type >= entity::ActorAoiFields::AOI_END) {
    return 0;
  }
  return this->actor_->GetAttribute((entity::ActorAoiFields::type)type);
}

core::int32 PythonActor::GetAttrActorClient(int type) {
  if(this->actor_ == NULL ||
      type < entity::ActorClientFields::CLIENT_BEGIN ||
      type >= entity::ActorClientFields::CLIENT_END) {
    return 0;
  }
  return this->actor_->GetAttribute((entity::ActorClientFields::type)type);
}

core::int32 PythonActor::GetAttrActorServer(int type) {
  if(this->actor_ == NULL ||
      type < entity::ActorServerFields::SERVER_BEGIN ||
      type >= entity::ActorServerFields::SERVER_END) {
    return 0;
  }
  return this->actor_->GetAttribute((entity::ActorServerFields::type)type);
}

bool PythonActor::SetAttrEntityClient(int type, core::int32 value) {
  if(this->actor_ == NULL ||
      type < entity::EntityClientFields::CLIENT_BEGIN ||
      type >= entity::EntityClientFields::CLIENT_END) {
    return false;
  }
  return this->actor_->SetAttribute((entity::EntityClientFields::type)type, value);
}

bool PythonActor::SetAttrEntityServer(int type, core::int32 value) {
  if(this->actor_ == NULL ||
      type < entity::EntityServerFields::SERVER_BEGIN ||
      type >= entity::EntityServerFields::SERVER_END) {
    return false;
  }
  return this->actor_->SetAttribute((entity::EntityServerFields::type)type, value);
}

bool PythonActor::SetAttrRoleAoi(int type, core::int32 value) {
  if(this->actor_ == NULL ||
      type < entity::RoleAoiFields::AOI_BEGIN ||
      type >= entity::RoleAoiFields::AOI_END) {
    return false;
  }
  return this->actor_->SetAttribute((entity::RoleAoiFields::type)type, value);
}

bool PythonActor::SetAttrRoleClient(int type, core::int32 value) {
  if(this->actor_ == NULL ||
      type < entity::RoleClientFields::CLIENT_BEGIN ||
      type >= entity::RoleClientFields::CLIENT_END) {
    return false;
  }
  return this->actor_->SetAttribute((entity::RoleClientFields::type)type, value);
}

bool PythonActor::SetAttrRoleServer(int type, core::int32 value) {
  if(this->actor_ == NULL ||
      type < entity::RoleServerFields::SERVER_BEGIN ||
      type >= entity::RoleServerFields::SERVER_END) {
    return false;
  }
  return this->actor_->SetAttribute((entity::RoleServerFields::type)type, value);
}

bool PythonActor::SetAttrActorAoi(int type, core::int32 value) {
  if(this->actor_ == NULL ||
      type < entity::ActorAoiFields::AOI_BEGIN ||
      type >= entity::ActorAoiFields::AOI_END) {
    return false;
  }
  return this->actor_->SetAttribute((entity::ActorAoiFields::type)type, value);
}

bool PythonActor::SetAttrActorClient(int type, core::int32 value) {
  if(this->actor_ == NULL ||
      type < entity::ActorClientFields::CLIENT_BEGIN ||
      type >= entity::ActorClientFields::CLIENT_END) {
    return false;
  }
  return this->actor_->SetAttribute((entity::ActorClientFields::type)type, value);
}

bool PythonActor::SetAttrActorServer(int type, core::int32 value) {
  if(this->actor_ == NULL ||
      type < entity::ActorServerFields::SERVER_BEGIN ||
      type >= entity::ActorServerFields::SERVER_END) {
    return false;
  }
  return this->actor_->SetAttribute((entity::ActorServerFields::type)type, value);
}

}  // namespace python

}  // namespace server

}  // namespace game

