//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-31 16:37:47.
// File name: gateway_user_pool.h
//
// Description:
// Define class GatewayUserPool.
//

#ifndef __GATEWAY__SERVER__GATEWAY__USER__POOL__H
#define __GATEWAY__SERVER__GATEWAY__USER__POOL__H

#include "core/base/noncopyable.h"
#include "gateway_server/server/gateway_user.h"
#include "global/pool_template.h"

namespace gateway {

namespace server {

typedef global::PoolTemplate<GatewayUser> GatewayUserPool;

class GatewayUserPoolSingleton : public core::Noncopyable {
 public:
  GatewayUserPoolSingleton() {}
  ~GatewayUserPoolSingleton() {}

  static inline GatewayUserPool &GetInstance() {
    return GatewayUserPoolSingleton::instance_;
  }

 private:
  static GatewayUserPool instance_;
};

}  // namespace server

}  // namespace gateway

#endif  // __GATEWAY__SERVER__GATEWAY__USER__POOL__H

