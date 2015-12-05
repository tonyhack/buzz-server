//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-14 10:10:49.
// File name: load_drop_configure.h
//
// Description:
// Define class LoadDropConfigure.
//

#ifndef __GAME__SERVER__LOAD__DROP__CONFIGURE__H
#define __GAME__SERVER__LOAD__DROP__CONFIGURE__H

#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/resource_types.h"

namespace game {

namespace server {

class DropCell {
 public:
  DropCell() {}
  ~DropCell() {}

  // 机率
  core::int32 odds_;
  // 子ID
  core::int32 sub_id_;
};

class DropSubCell {
 public:
  DropSubCell() {}
  ~DropSubCell() {}

  // 机率
  core::int32 odds_;
  // 资源类型
  entity::ResourceType::type resource_type_;
  // 资源ID
  core::int32 resource_id_;
  // 绑定(0: 非绑定)
  core::int32 bind_;
  // 最大最小数量
  core::int32 max_;
  core::int32 min_;
  // 强化等级
  core::int32 intensify_;
};

class DropElement {
 public:
  DropElement() {}
  ~DropElement() {}

  // 资源类型
  entity::ResourceType::type resource_type_;
  // 资源ID
  core::int32 resource_id_;
  // 绑定(0: 非绑定)
  core::int32 bind_;
  // 数量
  core::int32 number_;
  // 强化等级
  core::int32 intensify_level_;
};

class LoadDropConfigure : public core::Noncopyable {
  typedef std::vector<DropCell> DropVector;
  typedef std::vector<DropSubCell> DropSubVector;

  typedef std::map<core::int32, DropVector> DropMap;
  typedef std::map<core::int32, DropSubVector> DropSubMap;

 public:
  typedef std::vector<DropElement> DropElementVector;

  LoadDropConfigure();
  ~LoadDropConfigure();

  bool LoadDropConfig(const std::string &file);
  bool LoadDropSubConfig(const std::string &file);

  const DropElementVector &Drop(core::int32 id) const;

 private:
  DropMap drops_;
  DropSubMap drop_subs_;

  mutable DropElementVector elements_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__LOAD__DROP__CONFIGURE__H

