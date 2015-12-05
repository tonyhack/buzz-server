//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-12 19:20:12.
// File name: according_level_configure.h
//
// Description:
// Define class AccordingLevelConfigure.
//

#ifndef __GAME__SERVER__ACCORDING__LEVEL__CONFIGURE__H
#define __GAME__SERVER__ACCORDING__LEVEL__CONFIGURE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

class AccordingLevelCell {
 public:
  AccordingLevelCell() {}
  ~AccordingLevelCell() {}

  // 经验
  core::int32 exp_;
  // 灵气上限
  core::int32 max_nimbus_;
  // 移动速度
  core::int32 speed_;
  // 闪避系数
  core::int32 dodge_coe_;
  // 暴击系数
  core::int32 crit_effect_coe_;
  // 伤害浮动系数
  core::int32 hurt_random_per_;
  // 幻化能量上限
  core::int32 max_energy_;
  // 每次恢复能量值
  core::int32 energy_increment_;
  // 冥想经验收益
  core::int32 zazen_add_exp_;
  // 冥想灵气收益
  core::int32 zazen_add_nimbus_;
};

// 根据等级变化的配置
class AccordingLevelConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, AccordingLevelCell> CellHashmap;

 public:
  AccordingLevelConfigure();
  ~AccordingLevelConfigure();

  bool LoadConfigure(const std::string &file);

  const AccordingLevelCell *GetCell(core::uint32 level) const;

 private:
  CellHashmap cells_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ACCORDING__LEVEL__CONFIGURE__H

