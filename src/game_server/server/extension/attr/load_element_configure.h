#ifndef __GAME__SERVER__ATTR__LOAD__ELEMENT__CONFIGURE__H
#define __GAME__SERVER__ATTR__LOAD__ELEMENT__CONFIGURE__H

#include <cstddef>
#include <string>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/attr/attr_configure.h"

namespace game {

namespace server {

namespace attr {

class LoadElementConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::int32, size_t>  ElementIndexHashmap;
  typedef std::vector<ElementCell> ElementVector;

 public:
  LoadElementConfigure();
  ~LoadElementConfigure();

  bool LoadConfigure(const std::string &file);

  const ElementCell *GetElement(core::int32 id) const;
  const ElementCell *GetNextElement(core::int32 id) const;

 private:
  ElementIndexHashmap cells_index_;
  ElementVector cells_store_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__LOAD__ELEMENT__CONFIGURE__H

