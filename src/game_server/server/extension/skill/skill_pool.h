//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-31 16:21:44.
// File name: skill_pool.h
//
// Description:
// Define class SkillPool.
//

#ifndef __GAME__SERVER__SKILL__SKILL__POOL__H
#define __GAME__SERVER__SKILL__SKILL__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace skill {

class Skill;

class SkillPool : public global::SingletonFactory<SkillPool> {
  friend class global::SingletonFactory<SkillPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Skill *Allocate();
  void Deallocate(Skill *skill);

 private:
  SkillPool() {}
  ~SkillPool() {}

  global::PoolTemplate<Skill> skills_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__POOL__H

