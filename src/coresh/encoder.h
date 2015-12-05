//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 15:41:53.
// File name: encoder.h
//
// Description:
// Define class Encoder.
//

#ifndef __CORESH__ENCODER__H
#define __CORESH__ENCODER__H

#include "stddef.h"

#include "core/base/noncopyable.h"

namespace coresh {

struct EncodeType {
  enum Type {
    TYPE_NULL = 0,
  };
};

class Encoder : public core::Noncopyable {
 public:
  Encoder() {}
  virtual ~Encoder() {}

  virtual EncodeType::Type GetType() const = 0;

  // Get max size.
  virtual size_t GetEncodeMaxSize(size_t size) const = 0;
  virtual size_t GetUnencodeMaxSize(size_t size) const = 0;

  // Encode/Unencode.
  // Return >0 : successful.
  // Return =0 : with no need for packeting.
  // Return <0 : failure.
  virtual int Encode(const char *in, size_t size, char *out, size_t &max_size) = 0;
  virtual int Unecode(const char *in, size_t size, char *out, size_t &max_size) = 0;
};

}  // namespace coresh

#endif  // __CORESH__ENCODER__H

