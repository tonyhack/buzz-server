//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 10:34:58.
// File name: singleton_factory.h
//
// Description: 
// Define class SingletonFactory.
//

#ifndef __GLOBAL__SINGLETON__FACTORY__H
#define __GLOBAL__SINGLETON__FACTORY__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace global {

template <typename _T>
class SingletonFactory : public core::Noncopyable {
 public:
  static _T* GetInstance() {
    if(!g_Instance) {
      g_Instance = new _T;
    }
    return g_Instance;
  }

  static void ReleaseInstance() {
    if(g_Instance) {
      delete g_Instance;
      g_Instance = NULL;
    }
  }

 protected:
  SingletonFactory() {};
  ~SingletonFactory() {};

 private:
  static _T* g_Instance;
};

template <typename _T>
_T* SingletonFactory<_T>::g_Instance = NULL;

}  // namespace global

#endif  // __GLOBAL__SINGLETON__FACTORY__H

