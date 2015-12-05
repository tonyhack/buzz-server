#include "session_server/server/admin/admin_session_pool.h"

namespace session {

namespace server {

namespace admin {

bool AdminSessionPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->sessions_.Initialize(initial_number, extend_number);
}

void AdminSessionPool::Finalize() {
  this->sessions_.Finalize();
}

AdminSession *AdminSessionPool::Allocate() {
  return this->sessions_.Allocate();
}

void AdminSessionPool::Deallocate(AdminSession *session) {
  this->sessions_.Deallocate(session);
}

}  // namespace admin

}  // namespace server

}  // namespace session

