//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:44:13.
// File name: version.h
//
// Description:
// Define version of kernel.
//

#ifndef __CORE__VERSION__H
#define __CORE__VERSION__H

#include <linux/version.h>

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif  // KERNEL_VERSION

#ifdef LINUX_VERSION_CODE
#if(LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 30))
#define __CORE_KERNEL_EVENTFD__
#define __CORE_KERNEL_TIMERFD__
#define __CORE_KERNEL_SIGNALFD__
#endif
#endif  // LINUX_VERSION_CODE

#endif  // __CORE__VERSION__H

