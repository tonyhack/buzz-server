//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 15:02:46.
// File name: compressor.h
//
// Description:
// Define class Compressor.
//

#ifndef __CORESH__COMPRESSOR__H
#define __CORESH__COMPRESSOR__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace coresh {

struct CompressType {
  enum Type {
    TYPE_NULL = 0,
  };
};

class Compressor : public core::Noncopyable {
 public:
  Compressor() {}
  virtual ~Compressor() {}

  virtual CompressType::Type GetType() const = 0;

  // Get max size.
  virtual size_t GetCompressMaxSize(size_t size) const = 0;
  virtual size_t GetUncompressMaxSize(size_t size) const = 0;

  // Compress/Uncompress.
  // Return >0 : successful.
  // Return =0 : with no need for packeting.
  // Return <0 : failure.
  virtual int Compress(const char *in, size_t size, char *out, size_t &max_size) = 0;
  virtual int Uncompress(const char *in, size_t size, char *out, size_t &max_size) = 0;
};

}  // namespace coresh

#endif  // __CORESH__COMPRESSOR__H

