#ifndef __GAME__SERVER__MAIL__MAIL__DATABASE__RESPONSE__H
#define __GAME__SERVER__MAIL__MAIL__DATABASE__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace mail {

class MailDatabaseResponse : public core::Noncopyable {
 public:
  MailDatabaseResponse();
  ~MailDatabaseResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponseAddMail(const std::string &req, const std::string &res,
     database::protocol::ResponseType::type result);
  void OnResponseDelMail(const std::string &req, const std::string &res,
     database::protocol::ResponseType::type result);
  void OnResponsePickAffx(const std::string &req, const std::string &res,
     database::protocol::ResponseType::type result);
  void OnResponseReadMail(const std::string &req, const std::string &res,
     database::protocol::ResponseType::type result);
  void OnResponseGetAllMails(const std::string &req, const std::string &res,
     database::protocol::ResponseType::type result);
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__MAIL__MAIL__DATABASE__RESPONSE__H

