//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:44:26.
// File name: auto_mutex.h
//
// Description:
// Define class AutoMutex.
//

#ifndef __CORE__AUTO__MUTEX__H
#define __CORE__AUTO__MUTEX__H

#include "core/base/scoped_mutex.h"

namespace core {

class Mutex;

typedef ScopedMutex<Mutex> AutoMutex;

}  // namespace core

#endif  // __CORE__AUTO__MUTEX__H

