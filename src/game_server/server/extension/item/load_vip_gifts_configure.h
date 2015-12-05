//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-21 23:50:12.
// File name: load_vip_gifts_configure.h
//
// Description:
//

#ifndef __GAME__SERVER__ITEM__LOAD__VIP__GIFTS__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__VIP__GIFTS__CONFIGURE__H

#include <ext/hash_map>
#include <vector>

#include "core/base/noncopyable.h"
#include "entity/vip_types.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class VipGiftAward {
 public:
  VipGiftAward() : type_(0), id_(0), number_(0) {}

  // 类型
  core::uint32 type_;

  // ID/数量
  core::uint32 id_;
  core::uint32 number_;

  // 是否绑定(0:非绑, 1:绑定)
  core::uint32 bind_;
};

class LoadVipGiftsConfigure : public core::Noncopyable {
 public:
  typedef std::vector<VipGiftAward> Awards;

  LoadVipGiftsConfigure();
  ~LoadVipGiftsConfigure();

  bool LoadConfigure(const std::string &file);

  const Awards* GetAwards(entity::VipType::type type) const;

 private:
  Awards awards_[entity::VipType::MAX-1];
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__VIP__GIFTS__CONFIGURE__H

