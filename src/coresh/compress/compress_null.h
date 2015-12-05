//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 16:40:06.
// File name: compress_null.h
//
// Description:
// Define class CompressNull.
//

#ifndef __CORESH__COMPRESS__NULL__H
#define __CORESH__COMPRESS__NULL__H

#include "coresh/compressor.h"

namespace coresh {

class CompressNull : public Compressor {
 public:
  CompressNull() {}
  virtual ~CompressNull() {}

  virtual CompressType::Type GetType() const {
    return CompressType::TYPE_NULL;
  }

  // Get max size.
  virtual size_t GetCompressMaxSize(size_t size) const;
  virtual size_t GetUncompressMaxSize(size_t size) const;

  // Compress/Uncompress.
  // Return >0 : successful.
  // Return =0 : with no need for packeting.
  // Return <0 : failure.
  virtual int Compress(const char *in, size_t size, char *out, size_t &max_size);
  virtual int Uncompress(const char *in, size_t size, char *out, size_t &max_size);
};

}  // namespace coresh

#endif  // __CORESH__COMPRESS__NULL__H

