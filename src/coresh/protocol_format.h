//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-28 10:24:00.
// File name: protocol_format.h
//
// Description:
// Define protocol format.
//

#ifndef __CORESH__PROTOCOL__FORMAT__H
#define __CORESH__PROTOCOL__FORMAT__H

#include "core/base/types.h"

namespace coresh {

typedef core::uint32 ProtocolHead;

static const core::uint32 kProtocolHeadSize = sizeof(ProtocolHead);

static const core::uint32 kProtocolSizeMask     = 0x00FFFFFF;
static const core::uint32 kProtocolEncodeMask   = 0xF0000000;
static const core::uint32 kProtocolCompressMask = 0x0F000000;

inline void SetProtocolEncode(ProtocolHead head, core::uint32 type) {
  head &= ~kProtocolEncodeMask;
  head |= (type << 28) & kProtocolEncodeMask;
}

inline core::uint32 GetProtocolEncode(ProtocolHead head) {
  return (head & kProtocolEncodeMask) >> 28;
}

inline void SetProtocolCompress(ProtocolHead head, core::uint32 type) {
  head &= ~kProtocolCompressMask;
  head |= (type << 24) & kProtocolCompressMask;
}

inline core::uint32 GetProtocolCompress(ProtocolHead head) {
  return (head & kProtocolCompressMask) >> 24;
}

inline const ProtocolHead *ProtocolHeadCast(const char *protocol) {
  return (ProtocolHead *)protocol;
}

inline ProtocolHead *ProtocolHeadCast(char *protocol) {
  return (ProtocolHead *)protocol;
}

inline void SetProtocolHead(char *protocol, ProtocolHead head) {
  *ProtocolHeadCast(protocol) = head;
}

inline char *GetProtocolData(char *protocol) {
  return protocol + kProtocolHeadSize;
}

inline const char *GetProtocolData(const char *protocol) {
  return protocol + kProtocolHeadSize;
}

inline core::uint32 GetProtocolSize(const char *protocol) {
  return ((*ProtocolHeadCast(protocol) & kProtocolSizeMask) + kProtocolHeadSize);
}

}  // namespace coresh

#endif  // __CORESH__PROTOCOL__FORMAT__H

