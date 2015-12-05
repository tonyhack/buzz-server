//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 17:09:53.
// File name: variables.h
//
// Description:
// Define class Variables.
//

#ifndef __GAME__SERVER__AI__VARIABLE__H
#define __GAME__SERVER__AI__VARIABLE__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/hash_string.h"

namespace game {

namespace server {

namespace ai {

class Variables {
  typedef __gnu_cxx::hash_map<std::string, core::int64,
          global::hash<std::string> > VariableHashmap;

 public:
  Variables();
  ~Variables();

  bool Initialize();
  void Finalize();

  void Set(const std::string &key, core::int64 value);
  core::int64 Get(const std::string &key) const;

  core::int64 Increase(const std::string &key, core::int64 increment);
  core::int64 Decrease(const std::string &key, core::int64 decrement);

 private:
  VariableHashmap variables_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__VARIABLE__H

