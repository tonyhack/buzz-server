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

#ifndef __GAME__SERVER__BUFF__CONFIGURE__H
#define __GAME__SERVER__BUFF__CONFIGURE__H

#include <list>
#include <memory.h>
#include <string>

#include <ext/hash_map>

#include "core/base/types.h"
#include "entity/buff_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace buff {

class BuffCell {
 public:
  typedef std::pair<core::int32, core::int32> ConditionBuffPair;

  BuffCell() {}
  ~BuffCell() {}

  core::int32 id_;
  // 类型
  entity::BuffType::type type_;
  // 驱散组
  core::int32 clear_group_;
  // 条件组
  core::int32 condition_group_;
  // 组
  core::int32 group_;
  core::int32 group_level_;
  // 广播类型
  entity::BuffBroadcastType::type broadcast_type_;
  // 持续时间
  core::int32 during_time_;
  // 叠加层数
  core::int32 stack_count_;
  // 增益
  core::int32 buff_debuff_;
  // 结果条件
  core::int32 end_conditions_;
  // 效果集
  std::vector<core::int32> effects_;
  // 叠加上限效果集
  std::vector<core::int32> max_stack_effects_;
  // 效果条件集
  std::vector<core::int32> effect_conditions_;
  // 条件效果集
  std::vector<core::int32> condition_effects_;
  // BUFF条件集
  std::vector<core::int32> buff_conditions_;
  // 条件BUFF集
  std::vector<ConditionBuffPair> condition_buffs_;
  // 间隔时间
  core::int32 interval_time_;
  // 间隔效果(根据type_字段意义不同)
  std::vector<core::int32> interval_effects_;
  // 保存标志
  bool save_db_;
};

class BuffConditionCell {
 public:
  BuffConditionCell() {}
  ~BuffConditionCell() {}

  // 条件类型
  entity::BuffConditionType::type type_;
  // 参数
  core::int32 param1_;
};

class PoolInitial {
 public:
  PoolInitial() : initial_number_(0), extend_number_(0) {}
  core::int32 initial_number_;
  core::int32 extend_number_;
};

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

  typedef __gnu_cxx::hash_map<core::uint32, BuffCell> BuffHashmap;
  typedef __gnu_cxx::hash_map<core::int32, BuffConditionCell> ConditionHashmap;

 public:
  // 加载配置文件
  bool LoadConfigure(const std::string &file);

  bool LoadBuffConfigure(const std::string &file);
  bool LoadConditionConfigure(const std::string &file);

  // 获取 BUFF 配置
  const BuffCell *GetBuffCell(core::uint32 buff_id) const;

  // 获取 BUFF 条件配置
  const BuffConditionCell *GetConditionCell(core::int32 id) const;

  // 对象池初始化信息
  const PoolInitial &GetBuffPoolInitial() {
    return this->buff_pool_;
  }

  const PoolInitial &GetConditionPoolInitial() {
    return this->condition_pool_;
  }

 private:
  Configure() {}
  ~Configure() {}

  PoolInitial buff_pool_;
  PoolInitial condition_pool_;

  BuffHashmap buffs_;
  ConditionHashmap conditions_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__CONFIGURE__H

