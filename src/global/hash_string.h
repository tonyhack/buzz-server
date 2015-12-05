//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-27 15:21:54.
// File name: hash_string.h
//
// Description:
// Define class HashString.
//

#ifndef __GLOBAL__HASH__STRING__H
#define __GLOBAL__HASH__STRING__H

#include <string>

namespace global {

template<class _Key>
struct hash { };

inline size_t hash_string(const char* __s) {
  unsigned long __h = 0;
  for ( ; *__s; ++__s)
    __h = 5 * __h + *__s;
  return size_t(__h);
} 

template<>
struct hash<std::string> {
  size_t operator()(const std::string &str) const {
    return hash_string(str.c_str());
  }
};

}  // namespace global

#endif  // __GLOBAL__HASH__STRING__H

