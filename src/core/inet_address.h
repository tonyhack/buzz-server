//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:39:49.
// File name: inet_address.h
//
// Description:
// Define class InetAddress.
// Simple wrapper of ipv4.
//

#ifndef __CORE__INET__ADDRESS__H
#define __CORE__INET__ADDRESS__H

#include <netinet/in.h>
#include <string.h>

#include <string>

#include "core/base/types.h"

namespace core {

class InetAddress {
 public:
  InetAddress();
  explicit InetAddress(uint16 port);
  InetAddress(const std::string &ip, uint16 port);
  explicit InetAddress(const struct sockaddr_in &address);

  const InetAddress &operator = (const InetAddress &address);

  inline const struct sockaddr_in &GetPrototype() const {
    return this->address_;
  }

  inline void SetPrototype(const struct sockaddr_in &address) {
    memcpy(&this->address_, &address, sizeof(struct sockaddr_in));
  }

  inline size_t GetSize() const { return sizeof(this->address_); }

  std::string ToString() const;

  void GetAddressInfo(std::string &ip, uint16 &port) const;
  void GetAddressInfo(char *ip, size_t size, uint16 &port) const;

 private:
  struct sockaddr_in address_;
};

}  // namespace core

#endif  // __CORE__INET__ADDRESS__H

