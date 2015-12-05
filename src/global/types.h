//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-04 14:52:21.
// File name: types.h
//
// Description:
// Define types.
//

#ifndef __GLOBAL__TYPES__H
#define __GLOBAL__TYPES__H

#include "core/base/types.h"

namespace global {

// Max name size is 25, including '\0'.
static const size_t kMaxNameSize = 25;

const static core::int32 kPercentDenom = 10000;

const static core::uint32 kMaxTeleporterDistance = 5;

}  // namespace global

#endif  // __GLOBAL__TYPES__H

