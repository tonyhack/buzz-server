//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-31 17:12:17.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __GAME__SERVER__SKILL__CONFIGURE__H
#define __GAME__SERVER__SKILL__CONFIGURE__H

#include <ext/hash_map>

#include "game_server/server/extension/skill/skill_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace skill {

class SkillKey {
 public:
  SkillKey() : id_(0), level_(0) {}
  SkillKey(core::uint32 id, core::uint32 level)
    : id_(id), level_(level) {}

  bool operator==(const SkillKey &key) const {
    return this->id_ == key.id_ && this->level_ == key.level_;
  }
  bool operator<(const SkillKey &key) const {
    if(this->id_ != key.id_) {
      return this->id_ < key.id_;
    } else {
      return this->level_ < key.level_;
    }
  } 

  core::uint32 id_;
  core::uint32 level_;
};

class HashFunction {
  public:
    size_t operator()(const SkillKey &key) const {
      size_t hash = key.id_;
      return (hash << 32) + key.level_;
    }
};

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

  typedef __gnu_cxx::hash_map<SkillKey, SkillCell, HashFunction> SkillHashmap;
  typedef __gnu_cxx::hash_map<core::uint32, HurtCell> HurtHashmap;

 public:
  bool LoadConfigure(const std::string &file);

  // 获取技能配置
  const SkillCell *GetSkill(core::uint32 id, core::uint16 level);
  // 获取伤害配置
  const HurtCell *GetHurt(core::uint32 id);
  // 技能角色池初始化与扩展数量
  core::uint32 role_pool_initial_;
  core::uint32 role_pool_extend_;

  // 技能池初始化与扩展数量
  core::uint32 skill_pool_initial_;
  core::uint32 skill_pool_extend_;

 private:
  // 加载配置文件
  bool LoadSkillConfigure(const std::string &file);
  bool LoadHurtConfigure(const std::string &file);
  bool LoadCampConfigure(const std::string &file);

  Configure();
  ~Configure();

  // 技能配置表
  SkillHashmap skills_;
  // 直接伤害配置表
  HurtHashmap hurts_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__CONFIGURE__H

