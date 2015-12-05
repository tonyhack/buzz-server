//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 16:43:18.
// File name: compress_null.cc
//
// Description:
// Define class CompressNull.
//


#include "coresh/compress/compress_null.h"

namespace coresh {

size_t CompressNull::GetCompressMaxSize(size_t size) const {
  return size;
}

size_t CompressNull::GetUncompressMaxSize(size_t size) const {
  return size;
}

int CompressNull::Compress(const char *in, size_t size, char *out, size_t &max_size) {
  return 0;
}

int CompressNull::Uncompress(const char *in, size_t size, char *out, size_t &max_size) {
  return 0;
}

}  // namespace coresh

