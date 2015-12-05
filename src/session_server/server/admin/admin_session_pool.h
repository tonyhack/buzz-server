#ifndef __SESSION__SERVER__ADMIN__ADMIN__SESSION__POOL__H
#define __SESSION__SERVER__ADMIN__ADMIN__SESSION__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"
#include "session_server/server/admin/admin_session.h"

namespace session {

namespace server {

namespace admin {

class AdminSessionPool : public global::SingletonFactory<AdminSessionPool> {
  friend class global::SingletonFactory<AdminSessionPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  AdminSession *Allocate();
  void Deallocate(AdminSession *session);

 private:
  AdminSessionPool() {}
  ~AdminSessionPool() {}

  global::PoolTemplate<AdminSession> sessions_;
};

}  // namespace admin

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__ADMIN__ADMIN__SESSION__POOL__H

