//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:45:57.
// File name: common.cc
//
// Description:
// common define.
//

#include "core/base/common.h"

#include <ctype.h>

namespace core {

char *strupr(char *str) { 
  char *ptr = str; 

  while (*ptr != '\0') { 
    if (islower(*ptr))  
      *ptr = toupper(*ptr); 
    ptr++; 
  } 

  return str; 
} 

}  // namespace core

