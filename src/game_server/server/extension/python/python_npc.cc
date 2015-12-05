//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-02 19:44:59.
// File name: python_npc.cc
//
// Description:
// Define class PythonNpc.
//

#include "game_server/server/extension/python/python_npc.h"

namespace game {

namespace server {

namespace python {

PythonNpc::PythonNpc() : npc_(NULL) {}
PythonNpc::~PythonNpc() {}

bool PythonNpc::Initialize(GameNpc *npc) {
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 参数 npc 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->npc_ = npc;
  return true;
}

void PythonNpc::Finalize() {
  this->npc_ = NULL;
}

core::int32 PythonNpc::GetAttrEntityClient(int type) {
  if(this->npc_ == NULL ||
      type < entity::EntityClientFields::CLIENT_BEGIN ||
      type >= entity::EntityClientFields::CLIENT_END) {
    return 0;
  }
  return this->npc_->GetAttribute((entity::EntityClientFields::type)type);
}

core::int32 PythonNpc::GetAttrEntityServer(int type) {
  if(this->npc_ == NULL ||
      type < entity::EntityServerFields::SERVER_BEGIN ||
      type >= entity::EntityServerFields::SERVER_END) {
    return 0;
  }
  return this->npc_->GetAttribute((entity::EntityServerFields::type)type);
}

core::int32 PythonNpc::GetAttrRoleAoi(int type) {
  if(this->npc_ == NULL ||
      type < entity::RoleAoiFields::AOI_BEGIN ||
      type >= entity::RoleAoiFields::AOI_END) {
    return 0;
  }
  return this->npc_->GetAttribute((entity::RoleAoiFields::type)type);
}

core::int32 PythonNpc::GetAttrRoleClient(int type) {
  if(this->npc_ == NULL ||
      type < entity::RoleClientFields::CLIENT_BEGIN ||
      type >= entity::RoleClientFields::CLIENT_END) {
    return 0;
  }
  return this->npc_->GetAttribute((entity::RoleClientFields::type)type);
}

core::int32 PythonNpc::GetAttrRoleServer(int type) {
  if(this->npc_ == NULL ||
      type < entity::RoleServerFields::SERVER_BEGIN ||
      type >= entity::RoleServerFields::SERVER_END) {
    return 0;
  }
  return this->npc_->GetAttribute((entity::RoleServerFields::type)type);
}

core::int32 PythonNpc::GetAttrNpcAoi(int type) {
  if(this->npc_ == NULL ||
      type < entity::NpcAoiFields::AOI_BEGIN ||
      type >= entity::NpcAoiFields::AOI_END) {
    return 0;
  }
  return this->npc_->GetAttribute((entity::NpcAoiFields::type)type);
}

core::int32 PythonNpc::GetAttrNpcClient(int type) {
  if(this->npc_ == NULL ||
      type < entity::NpcClientFields::CLIENT_BEGIN ||
      type >= entity::NpcClientFields::CLIENT_END) {
    return 0;
  }
  return this->npc_->GetAttribute((entity::NpcClientFields::type)type);
}

bool PythonNpc::SetAttrEntityClient(int type, core::int32 value) {
  if(this->npc_ == NULL ||
      type < entity::EntityClientFields::CLIENT_BEGIN ||
      type >= entity::EntityClientFields::CLIENT_END) {
    return false;
  }
  return this->npc_->SetAttribute((entity::EntityClientFields::type)type, value);
}

bool PythonNpc::SetAttrEntityServer(int type, core::int32 value) {
  if(this->npc_ == NULL ||
      type < entity::EntityServerFields::SERVER_BEGIN ||
      type >= entity::EntityServerFields::SERVER_END) {
    return false;
  }
  return this->npc_->SetAttribute((entity::EntityServerFields::type)type, value);
}

bool PythonNpc::SetAttrRoleAoi(int type, core::int32 value) {
  if(this->npc_ == NULL ||
      type < entity::RoleAoiFields::AOI_BEGIN ||
      type >= entity::RoleAoiFields::AOI_END) {
    return false;
  }
  return this->npc_->SetAttribute((entity::RoleAoiFields::type)type, value);
}

bool PythonNpc::SetAttrRoleClient(int type, core::int32 value) {
  if(this->npc_ == NULL ||
      type < entity::RoleClientFields::CLIENT_BEGIN ||
      type >= entity::RoleClientFields::CLIENT_END) {
    return false;
  }
  return this->npc_->SetAttribute((entity::RoleClientFields::type)type, value);
}

bool PythonNpc::SetAttrRoleServer(int type, core::int32 value) {
  if(this->npc_ == NULL ||
      type < entity::RoleServerFields::SERVER_BEGIN ||
      type >= entity::RoleServerFields::SERVER_END) {
    return false;
  }
  return this->npc_->SetAttribute((entity::RoleServerFields::type)type, value);
}

bool PythonNpc::SetAttrNpcAoi(int type, core::int32 value) {
  if(this->npc_ == NULL ||
      type < entity::NpcAoiFields::AOI_BEGIN ||
      type >= entity::NpcAoiFields::AOI_END) {
    return false;
  }
  return this->npc_->SetAttribute((entity::NpcAoiFields::type)type, value);
}

bool PythonNpc::SetAttrNpcClient(int type, core::int32 value) {
  if(this->npc_ == NULL ||
      type < entity::NpcClientFields::CLIENT_BEGIN ||
      type >= entity::NpcClientFields::CLIENT_END) {
    return false;
  }
  return this->npc_->SetAttribute((entity::NpcClientFields::type)type, value);
}


}  // namespace python

}  // namespace server

}  // namespace game

