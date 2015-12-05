include "game_request.thrift"
include "mail.thrift"
include "mail_system.thrift"

namespace cpp game.server.request
namespace py ccrequest.mail

///////////////////////////////////
// 邮件发送
// REQUEST_MAIL_SEND
struct RequestMailSend {
  1 : required i64 id_;                                              // 收件人ID
  2 : optional mail.MailType type_;                                  // 邮件类型
  3 : required mail_system.SystemMailChildType child_type_;          // 系统邮件子类型
  4 : required list<string> params_;                                 // 邮件参数信息，第一个为配置ID
  5 : optional mail.MailAffix affix_;                                // 附件
  6 : optional string context_;                                      // 正文(系统邮件为空)
}

