//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-23 15:46:35.
// File name: simple_encrypt.h
//
// Description:
// Define class SimpleEncrypt.
//

#ifndef __GLOBAL__SIMPLE__ENCRYPT__H
#define __GLOBAL__SIMPLE__ENCRYPT__H

#include "global/random_generator.h"

namespace global {

class SimpleEncrypt {
 public:
  static void Initialize();

  // in: string to encrypt/unencrypt.
  // key_buffer: the key.
  // code: random code.
  // out: output after encrypting/unencrypting.
  // size: size of out.
  static void Encrypt(const char *in, const char *key_buffer,
      int code, char *out, int &size);
  static void Unencrypt(const char *in, const char *key_buffer,
      int code, char *out, int &size);

  static void MakeRandomKey(char *buffer, int size);

 private:
  static void BinToStr(char *str);
  static void StrToBin(char *str, int len);

  static RandomGenerator random_;
};

}  // namespace global

#endif  // __GLOBAL__SIMPLE__ENCRYPT__H

