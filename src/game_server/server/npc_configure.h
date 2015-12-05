//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-18 19:11:34.
// File name: npc_configure.h
//
// Description:
// Define class NpcConfigure.
//

#ifndef __GAME__SERVER__NPC__CONFIGURE__H
#define __GAME__SERVER__NPC__CONFIGURE__H

#include <string>

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/npc_types.h"

namespace game {

namespace server {

class NpcCell {
 public:
  NpcCell() {}

  // 类型
  core::int32 type_;
  // 名称
  std::string name_;
  // 等级
  core::int32 level_;
  // 职业
  core::int32 vocation_;
  // 生命值
  core::int32 hp_;
  // 魔法值
  core::int32 mp_;
  // 移动速度
  core::int32 move_speed_;
  // 物理攻击
  core::int32 phy_attrack_;
  // 物理防御
  core::int32 phy_defence_;
  // 法术攻击
  core::int32 magic_attrack_;
  // 法术防御
  core::int32 magic_defence_;
  // 闪避
  core::int32 dodge_;
  // 暴击
  core::int32 crit_;
  // 会心
  core::int32 ignore_defence_;
  // AI
  core::int32 ai_;
  // 默认攻击距离
  core::int32 attack_distance_;
  // 死亡后的消失时间(秒)
  core::int32 disappear_time_;
  // 重生时间(秒)
  core::int32 reborn_;
  // 读条ID
  core::int32 bar_id_;
  // 技能
  std::vector<core::int32> skills_;
  // 阵营id
  core::int32 camp_id_;
  // 掉落ID
  core::int32 drop_id_;
  // 归属类型
  entity::NpcOwnerType::type owner_type_;
  // 获取经验的类型
  core::int32 exp_type_;
  // 经验值
  core::int32 exp_;
  // 模型
  entity::NpcMobType::type mob_type_;
};

class NpcConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint32, NpcCell> NpcHashmap;

 public:
  NpcConfigure();
  ~NpcConfigure();

  bool LoadConfigure(const std::string &file);
  const NpcCell *GetNpc(core::uint32 template_id) const;

 private:
  NpcHashmap npcs_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__NPC__CONFIGURE__H

