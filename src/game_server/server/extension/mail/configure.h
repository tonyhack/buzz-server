//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-03 18:16:24.
// File name: load_mail_configure.h
//
// Description:
// Define class Configure.
//

#ifndef __GAME__SERVER__MAIL__CONFIGURE__H
#define __GAME__SERVER__MAIL__CONFIGURE__H

#include <vector>

#include "core/base/types.h"
#include "entity/mail_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace mail {

class MailCell {
 public:
  core::int32 mail_type_;
  core::int32 expiry_time_;
  core::int32 context_max_;
};

class Configure : public  global::SingletonFactory<Configure> {
  typedef std::vector<MailCell> MailConfigVec;
 public:
  Configure();
  ~Configure();

  bool LoadConfigure(const std::string &file);

  bool LoadMailConfigure(const std::string &file);

  const MailCell *GetMailCell(entity::MailType::type type) const;
  
  core::int32 GetInitSize() const;

  core::int32 GetExtendSize() const;

 private:
  core::int32 mail_init_num_;
  core::int32 mail_extend_num_;
  MailConfigVec mails_;
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__MAIL__CONFIGURE__H

