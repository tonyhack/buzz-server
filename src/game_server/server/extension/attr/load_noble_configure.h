#ifndef __GAME__SERVER__ATTR__LOAD__NOBLE__CONFIGURE__H
#define __GAME__SERVER__ATTR__LOAD__NOBLE__CONFIGURE__H

#include <string>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/attr/attr_configure.h"

namespace game {

namespace server {

namespace attr {

class LoadNobleConfigure {
  typedef __gnu_cxx::hash_map<core::int32, size_t> NobleIndexHashmap;
  typedef std::vector<NobleCell> NobleVector;

 public:
  LoadNobleConfigure();
  ~LoadNobleConfigure();

  bool LoadConfigure(const std::string &file);

  const NobleCell *GetNoble(core::int32 id) const;
  const NobleCell *GetFirstNoble() const;
  const NobleCell *GetNextNoble(core::int32 id) const;

 private:
  NobleIndexHashmap cells_index_;
  NobleVector cells_store_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__LOAD__NOBLE__CONFIGURE__H

