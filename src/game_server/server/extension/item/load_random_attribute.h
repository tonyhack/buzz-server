//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-28 14:23:19.
// File name: load_random_attribute.h
//
// Description:
// Define class LoadRandomAttribute.
//

#ifndef __GAME__SERVER__ITEM__LOAD__RANDOM__ATTRIBUTE__H
#define __GAME__SERVER__ITEM__LOAD__RANDOM__ATTRIBUTE__H

#include <vector>

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class LoadRandomAttribute : public core::Noncopyable {
 public:
  typedef std::vector<RandomAttributeCell> AttributeVector;
  typedef __gnu_cxx::hash_map<core::uint32, AttributeVector> AttributeHashmap;

  LoadRandomAttribute();
  ~LoadRandomAttribute();

  bool LoadConfigure(const std::string &file);

  // 随机出一个属性(这个属性可能为空)
  const RandomAttributeCell *RandomAttribute(core::uint32 id) const;

  const AttributeVector *GetRandomVector(core::uint32 id) const;

 private:
  bool CheckValid(core::uint8 type) const;
  bool CheckExist(core::uint8 type, const AttributeVector &cells) const;

  AttributeHashmap attrs_;
};

}

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__RANDOM__ATTRIBUTE__H

