//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-07 15:58:32.
// File name: load_soul_step_configure.h
//
// Description:
// Define class LoadSoulStepConfigure.
//

#ifndef __GAME__SERVER__SOUL__LOAD__SOUL__STEP__CONFIGURE__H
#define __GAME__SERVER__SOUL__LOAD__SOUL__STEP__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/soul/soul_configure.h"

namespace game {

namespace server {

namespace soul {

class SoulStepKey {
 public:
  SoulStepKey(core::int32 group = 0, core::int32 level = 0, core::int32 step = 0) {
    this->group_ = group;
    this->level_ = level;
    this->step_ = step;
  }
  inline bool operator<(const SoulStepKey &key) const {
    if (this->group_ != key.group_) {
      return this->group_ < key.group_;
    } else if (this->level_ != key.level_) {
      return this->level_ < key.level_;
    } else {
      return this->step_ < key.step_;
    }
  }
  core::int32 group_;
  core::int32 level_;
  core::int32 step_;
};

class LoadSoulStepConfigure : public core::Noncopyable {
  typedef std::map<SoulStepKey, SoulStepCell> StepMap;

 public:
  LoadSoulStepConfigure();
  ~LoadSoulStepConfigure();

  bool LoadConfigure(const std::string &file);
  const SoulStepCell *GetStep(core::int32 group, core::int32 level,
      core::int32 step) const;

 private:
  StepMap steps_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__LOAD__SOUL__STEP__CONFIGURE__H

