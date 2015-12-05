//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 16:37:16.
// File name: encode_null.cc
//
// Description:
// Define class EncodeNull.
//

#include "coresh/encode/encode_null.h"

namespace coresh {

size_t EncodeNull::GetEncodeMaxSize(size_t size) const {
  return size;
}

size_t EncodeNull::GetUnencodeMaxSize(size_t size) const {
  return size;
}

int EncodeNull::Encode(const char *in, size_t size, char *out, size_t &max_size) {
  return 0;
}

int EncodeNull::Unecode(const char *in, size_t size, char *out, size_t &max_size) {
  return 0;
}

}  // namespace coresh

