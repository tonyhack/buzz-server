#include "session_server/server/admin/admin_session_manager.h"

#include "global/logging.h"
#include "session_server/server/admin/admin_session.h"
#include "session_server/server/admin/admin_session_pool.h"

namespace session {

namespace server {

namespace admin {

AdminSessionManager::AdminSessionManager() : session_id_allocator_(0) {}
AdminSessionManager::~AdminSessionManager() {}

bool AdminSessionManager::Initialize() {
  return true;
}

void AdminSessionManager::Finalize() {
  for (AdminSessionHashmap::iterator iter = this->sessions_.begin();
       iter != this->sessions_.end(); ++iter) {
    iter->second->Finalize();
    AdminSessionPool::GetInstance()->Deallocate(iter->second);
  }
}

AdminSession *AdminSessionManager::CreateSession(const coresh::ConnectionPtr &connection) {
  AdminSession *session = AdminSessionPool::GetInstance()->Allocate();
  if (NULL == session) {
    LOG_ERROR("Allocate AdminSession from AdminSessionPool failed.");
    return NULL;
  }

  if (session->Initialize(++this->session_id_allocator_, connection) == false) {
    AdminSessionPool::GetInstance()->Deallocate(session);
    LOG_ERROR("Init AdminSession(%lu) failed.", this->session_id_allocator_);
    return NULL;
  }

  if (this->sessions_.insert(std::make_pair(session->GetID(), session)).second== false) {
    LOG_ERROR("Add AdminSession(%lu) failed.", session->GetID());
    session->Finalize();
    AdminSessionPool::GetInstance()->Deallocate(session);
    return NULL;
  }

  LOG_INFO("AdminSession(%lu) created.", session->GetID());

  return session;
}

AdminSession *AdminSessionManager::GetSession(core::uint64 session_id) {
  AdminSessionHashmap::iterator iter = this->sessions_.find(session_id);
  if (iter != this->sessions_.end()) {
    return iter->second;
  }

  return NULL;
}

void AdminSessionManager::CloseSession(core::uint64 session_id) {
  AdminSessionHashmap::iterator iter = this->sessions_.find(session_id);
  if (iter == this->sessions_.end()) {
    LOG_ERROR("Get AdminSession(%lu) from AdminSessionManager failed.", session_id);
    return;
  }
  AdminSession *session = iter->second;

  this->sessions_.erase(iter);
  session->Finalize();
  AdminSessionPool::GetInstance()->Deallocate(session);

  LOG_INFO("AdminSession(%lu) closed.", session_id);
}

}  // namespace admin

}  // namespace server

}  // namespace session

