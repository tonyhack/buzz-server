#ifndef __SESSION__SERVER__ADMIN__ADMIN__SESSION__MANAGER__H
#define __SESSION__SERVER__ADMIN__ADMIN__SESSION__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "coresh/connection.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace admin {

class AdminSession;

class AdminSessionManager : public global::SingletonFactory<AdminSessionManager> {
  friend class global::SingletonFactory<AdminSessionManager>;

 public:
  typedef __gnu_cxx::hash_map<core::uint64, AdminSession *> AdminSessionHashmap;

  bool Initialize();
  void Finalize();

  AdminSession *CreateSession(const coresh::ConnectionPtr &connection);
  AdminSession *GetSession(core::uint64 session_id);
  void CloseSession(core::uint64 session_id);

 private:
  AdminSessionManager();
  ~AdminSessionManager();

  uint64_t session_id_allocator_;
  AdminSessionHashmap sessions_;
};

}  // namespace admin

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__ADMIN__ADMIN__SESSION__MANAGER__H

