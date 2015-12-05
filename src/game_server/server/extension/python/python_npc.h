//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-02 19:40:13.
// File name: python_npc.h
//
// Description:
// Define class PythonNpc.
//

#ifndef __GAME__SERVER__PYTHON__PYTHON__NPC__H
#define __GAME__SERVER__PYTHON__PYTHON__NPC__H

#include <boost/shared_ptr.hpp>

#include "core/base/types.h"
#include "game_server/server/game_npc.h"

namespace game {

namespace server {

namespace python {

class PythonNpc {
 public:
  PythonNpc();
  ~PythonNpc();

  bool Initialize(GameNpc *npc);
  void Finalize();

  // 获取属性
  core::int32 GetAttrEntityClient(int type);
  core::int32 GetAttrEntityServer(int type);
  core::int32 GetAttrRoleAoi(int type);
  core::int32 GetAttrRoleClient(int type);
  core::int32 GetAttrRoleServer(int type);
  core::int32 GetAttrNpcAoi(int type);
  core::int32 GetAttrNpcClient(int type);

  // 设置属性
  bool SetAttrEntityClient(int type, core::int32 value);
  bool SetAttrEntityServer(int type, core::int32 value);
  bool SetAttrRoleAoi(int type, core::int32 value);
  bool SetAttrRoleClient(int type, core::int32 value);
  bool SetAttrRoleServer(int type, core::int32 value);
  bool SetAttrNpcAoi(int type, core::int32 value);
  bool SetAttrNpcClient(int type, core::int32 value);

  inline core::uint32 GetPosx() const {
    if(this->npc_) {
      return this->npc_->GetPosx();
    }
    return 0;
  }

  inline core::uint32 GetPosy() const {
    if(this->npc_) {
      return this->npc_->GetPosy();
    }
    return 0;
  }

  inline core::uint64 GetID() const {
    if(this->npc_) {
      return this->npc_->GetID();
    }
    return 0;
  }

 private:
  GameNpc *npc_;
};

typedef boost::shared_ptr<PythonNpc> PythonNpcPtr;

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__PYTHON__NPC__H

