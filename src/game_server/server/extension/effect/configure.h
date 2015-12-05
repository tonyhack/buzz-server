//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 23:34:31.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __GAME__SERVER__EFFECT__CONFIGURE__H
#define __GAME__SERVER__EFFECT__CONFIGURE__H

#include <string>

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace effect {

class EffectConfigure {
 public:
  EffectConfigure()
    : type_(0), param1_(0), param2_(0),
    param3_(0) {}

  core::int16 type_;
  core::int32 param1_;
  core::int32 param2_;
  core::int32 param3_;
};

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

  typedef __gnu_cxx::hash_map<core::uint32, EffectConfigure> EffectHashmap;

 public:
  // 加载配置文件
  bool LoadConfigure(const std::string &file);

  // 获取效果信息
  const EffectConfigure *GetEffectConfigure(core::uint32 effect_id);

 private:
  Configure() {}
  ~Configure() {}

  EffectHashmap effects_;
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__CONFIGURE__H

