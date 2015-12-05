#ifndef __GAME__SERVER__ATTR__LOAD__ATTR__BONUS__CONFIGURE__H
#define __GAME__SERVER__ATTR__LOAD__ATTR__BONUS__CONFIGURE__H

#include <string>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/attr/attr_configure.h"

namespace game {

namespace server {

namespace attr {

class LoadAttrBonusConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::int32, AttrBonusCell>  AttrBonusHashmap;

 public:
  LoadAttrBonusConfigure();
  ~LoadAttrBonusConfigure();

  bool LoadConfigure(const std::string &file);

  const AttrBonusCell *GetAttrBonus(core::int32 id) const;

 private:
  AttrBonusHashmap cells_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__LOAD__ATTR__BONUS__CONFIGURE__H

