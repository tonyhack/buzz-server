//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-31 16:34:42.
// File name: skill_role_pool.h
//
// Description:
// Define class SkillRolePool.
//

#ifndef __GAME__SERVER__SKILL__SKILL__ROLE__POOL__H
#define __GAME__SERVER__SKILL__SKILL__ROLE__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace skill {

class SkillRole;

class SkillRolePool : public global::SingletonFactory<SkillRolePool> {
  friend class global::SingletonFactory<SkillRolePool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  SkillRole *Allocate();
  void Deallocate(SkillRole *role);

 private:
  SkillRolePool() {}
  ~SkillRolePool() {}

  global::PoolTemplate<SkillRole> roles_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__ROLE__POOL__H

