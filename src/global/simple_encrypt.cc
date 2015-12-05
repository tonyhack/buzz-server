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

#include "global/simple_encrypt.h"

#include <string.h>

namespace global {

RandomGenerator SimpleEncrypt::random_;

void SimpleEncrypt::Initialize() {
  random_.Initialize();
}

void SimpleEncrypt::BinToStr(char *str) {
#define MAKE_BYTE(high, low) (((high) << 4) | (low))

  char temp[512] = { 0 };

  int str_len = strlen(str);
  unsigned char *ustr = (unsigned char *)str;
  for(int i = 0, j = 0; (i + 1) < str_len; i += 2) {
    unsigned char high = ustr[i];
    unsigned char low = ustr[i + 1];

    if(high <= 57) {
      high -= 48;
    } else {
      high -= 55;
    }

    if(low <= 57) {
      low -= 48;
    } else {
      low -= 55;
    }

    temp[j++] = MAKE_BYTE(high, low);
  }

  strncpy(str, temp, str_len);
}

void SimpleEncrypt::StrToBin(char *str, int len) {
#define GET_HIGH_BYTE(value) ((value) >> 4)
#define GET_LOW_BYTE(value) ((value) & 0x0F)

  char temp[512] = { 0 };

  int str_len = strlen(str);
  unsigned char *ustr = (unsigned char *)str;
  for(int i = 0, j = 0; i < str_len; ++i) {
    unsigned char high = GET_HIGH_BYTE(ustr[i]);
    unsigned char low = GET_LOW_BYTE(ustr[i]);

    if(high <= 9) {
      temp[j++] = high + 48;
    } else {
      temp[j++] = high + 55;
    }

    if(low <= 9) {
      temp[j++] = low + 48;
    } else {
      temp[j++] = low + 55;
    }
  }

  strncpy(str, temp, len);
}

void SimpleEncrypt::Encrypt(const char *in, const char *key_buffer,
    int code, char *out, int &size) {
  if(in == NULL || key_buffer == NULL || out == NULL) {
    return ;
  }

  char temp[512] = { 0 };
  strncpy(temp, in, sizeof(temp));

  int in_len = strlen(temp);

  char *key = (char *)key_buffer + ((code + 7) % 19);
  int key_len = strlen(key);

  int rand[9] = { 22, 12, 8, 33, 1, 9, 7, 15, 18 };

  int rand_pos = 0;
  int key_pos = 0;
  for(int i = 0; i < in_len; ++i, ++key_pos, ++rand_pos) {
    key_pos = key_pos % key_len;
    rand_pos = rand_pos % (sizeof(rand) / sizeof(int));

    temp[i] += key[key_pos] % 23;
    temp[i] += rand[rand_pos];
  }

  size = 2 * in_len + 1;
  StrToBin(temp, size);

  strncpy(out, temp, size);
}

void SimpleEncrypt::Unencrypt(const char *in, const char *key_buffer,
    int code, char *out, int &size) {
  if(in == NULL || key_buffer == NULL || out == NULL) {
    return ;
  }

  char temp[512] = { 0 };
  strncpy(temp, in, sizeof(temp));

  BinToStr(temp);
  int in_len = strlen(temp);

  char *key = (char *)key_buffer + ((code + 7) % 19);
  int key_len = strlen(key);

  int rand[9] = { 22, 12, 8, 33, 1, 9, 7, 15, 18 };

  int rand_pos = 0;
  int key_pos = 0;
  for(int i = 0; i < in_len; ++i, ++key_pos, ++rand_pos) {
    key_pos = key_pos % key_len;
    rand_pos = rand_pos % (sizeof(rand) / sizeof(int));

    temp[i] -= key[key_pos] % 23;
    temp[i] -= rand[rand_pos];
  }

  size = in_len + 1;
  strncpy(out, temp, size);
}

void SimpleEncrypt::MakeRandomKey(char *buffer, int size) {
  for(int i = 0; i < size; ++i, ++buffer) {
    *buffer = (random_.Random() % 127) + 1;
  }
}

}  // namespace global

