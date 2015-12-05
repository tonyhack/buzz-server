#ifndef __SESSION__SERVER__GUILD__FACADE__REQUEST__H
#define __SESSION__SERVER__GUILD__FACADE__REQUEST__H

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace guild {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  bool SendDismissMail(core::uint64 actor_id);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__FACADE__REQUEST__H

