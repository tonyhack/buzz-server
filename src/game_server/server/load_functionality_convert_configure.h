#ifndef __GAME__SERVER__LOAD__FUNCTIONALITY__CONVERT__CONFIGURE__H
#define __GAME__SERVER__LOAD__FUNCTIONALITY__CONVERT__CONFIGURE__H

#include <map>
#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/functionality_types.h"

namespace game {

namespace server {

class LoadFunctionalityConvertConfigure {
  typedef std::map<int, std::string> FunctionalityMap;
 public:

  LoadFunctionalityConvertConfigure();
  ~LoadFunctionalityConvertConfigure();

  bool LoadConfigure(const std::string& file);

  const char *GetCell(core::int32 functionality_id);

 private:
  FunctionalityMap functionalitys_;
};

} // server

} // game

#endif  // GAME__SERVER__LOAD__FUNCTIONALITY__CONVERT__CONFIGURE__H
