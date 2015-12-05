#ifndef __GAME__SERVER__ATTR__LOAD__SPIRIT__CONFIGURE__H
#define __GAME__SERVER__ATTR__LOAD__SPIRIT__CONFIGURE__H

#include <string>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/attr/attr_configure.h"

namespace game {

namespace server {

namespace attr {

class LoadSpiritConfigure {
  typedef __gnu_cxx::hash_map<core::int32, SpiritCell> SpiritHashmap;
  typedef __gnu_cxx::hash_map<core::int32, SpiritFacadeCell> SpiritFacadeHashmap;

 public:
  LoadSpiritConfigure();
  ~LoadSpiritConfigure();

  bool LoadConfigure(const std::string &spirit_file, const std::string &spirit_facade_file);

  const SpiritCell *GetSpirit(core::int32 level) const;
  const SpiritFacadeCell *GetSpiritFacade(core::int32 id) const;

 private:
  SpiritHashmap spirits_;
  SpiritFacadeHashmap facades_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__LOAD__SPIRIT__CONFIGURE__H

