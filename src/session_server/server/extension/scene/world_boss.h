//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-05 19:58:22.
// File name: world_boss.h
//
// Description:
//
#ifndef __SESSION__SERVER__WORLD_BOSS__H
#define __SESSION__SERVER__WORLD_BOSS__H

#include <cstdio>
#include <string>

#include "core/base/types.h"
#include "core/base/noncopyable.h"

namespace session {

namespace server {

namespace scene {

class Scene;

class WorldBoss : public core::Noncopyable{
 public:
  WorldBoss();
  ~WorldBoss();

  bool Initialize(Scene *scene);
  void Finalize();

  inline std::string& GetKiller() {
    return this->killer_;
  }
  inline bool GetLive() const {
    return this->live_;
  }
  inline core::uint32 GetTemplateID() {
    return this->boss_template_;
  }

  void OnWorldBossDie(std::string &killer);

 public:
  void BroadCastKillBoss();
  void BroadCastSummonBoss(core::int32 boss_template, core::int32 x, core::int32 y, core::int32 width, core::int32 heigth);

 private:
  std::string killer_;
  core::uint32 boss_template_;
  Scene *scene_;
  bool live_;
};

} // namepsace scene

} // namespace server

} // namespace session

#endif // __SESSION__SERVER__WORLD_BOSS__H
