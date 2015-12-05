//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 19:30:16.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __GAME__SERVER__COOLING__CONFIGURE__H
#define __GAME__SERVER__COOLING__CONFIGURE__H

#include <string>

#include <ext/hash_map>

#include "core/base/types.h"
#include "game_server/server/extension/cooling/cooling_configure.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace cooling {

class PoolInitial {
 public:
  PoolInitial() : initial_number_(0), extend_number_(0) {}
  size_t initial_number_;
  size_t extend_number_;
};

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

  typedef __gnu_cxx::hash_map<core::uint32, CoolingCell> CoolingHashmap;
  typedef __gnu_cxx::hash_map<core::uint32, GroupCell> GroupHashmap;

 public:
  // 加载配置文件
  bool LoadConfigure(const std::string &file);

  // 获取冷却信息
  const CoolingCell *GetCoolingCell(core::uint32 cooling_id);
  // 获取组信息
  const GroupCell *GetGroupCell(core::uint32 group_id);

  // 得到冷却对象池初始化信息
  const PoolInitial &GetCoolingPoolInitial() const {
    return this->cooling_pool_;
  }
  // 得到冷却组对象池初始化信息
  const PoolInitial &GetCoolingGroupPoolInitial() const {
    return this->cooling_group_pool_;
  }

 private:
  // 加载冷却配置
  bool LoadCoolingConfigure(const std::string &file);
  // 加载组配置
  bool LoadGroupConfigure(const std::string &file);

  Configure() {}
  ~Configure() {}

  // 冷却配置
  CoolingHashmap coolings_;
  // 组配置
  GroupHashmap groups_;

  // 冷却对象池初始数据
  PoolInitial cooling_pool_;

  // 冷却组对象池初始数据
  PoolInitial cooling_group_pool_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__CONFIGURE__H

