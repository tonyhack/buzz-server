//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 16:32:33.
// File name: encode_null.h
//
// Description:
// Define class EncodeNull.
//

#ifndef __CORESH__ENCODE__NULL__H
#define __CORESH__ENCODE__NULL__H

#include "coresh/encoder.h"

namespace coresh {

class EncodeNull : public Encoder {
 public:
  EncodeNull() {}
  virtual ~EncodeNull() {}

  virtual EncodeType::Type GetType() const {
    return EncodeType::TYPE_NULL;
  }

  // Get max size.
  virtual size_t GetEncodeMaxSize(size_t size) const;
  virtual size_t GetUnencodeMaxSize(size_t size) const;

  // Encode/Unencode.
  // Return >0 : successful.
  // Return =0 : with no need for packeting.
  // Return <0 : failure.
  virtual int Encode(const char *in, size_t size, char *out, size_t &max_size);
  virtual int Unecode(const char *in, size_t size, char *out, size_t &max_size);
};

}  // namespace coresh

#endif  // __CORESH__ENCODE__NULL__H

