include "mail.thrift"
include "mail_system.thrift"

namespace cpp session.protocol


///////////////////////////////
// 邮件发送请求
//type: MESSAGE_MAIL_SEND_REQUEST
//dir : game->session
struct MessageMailSendRequest {
  1 : required i64 addressee_id_;
  2 : required i64 addresser_id_;
  3 : required string addressee_;
  4 : required string addresser_;
  5 : required mail.MailType type_;
  6 : required mail.MailAffixType affix_type_;
  7 : required mail.MailAffix affix_;
  8 : required mail_system.SystemMailChildType child_type_;
  9 : required list<string> params_;
  10: required string title_;
  11: required string context_;
}

//////////////////////////////
// 邮件发送返回
//type: MESSAGE_MAIL_SEND_RESPONSE
//dir : session->game
struct MessageMailSendResponse {
  1 : required bool result_;
  2 : required i64 addresser_id_;
}

//////////////////////////////
// 通知邮件到达
//type: MESSAGE_MAIL_NOTICE
// dir: session->game
struct MessageMailNotice {
  1 : required string title_;                        // 标题
  2 : required string addresser_;                    // 发件人
  3 : required mail.MailReadState state_;            // 读取状态
  4 : required mail.MailAffixType affix_type_;       // 附件状态
  5 : required mail.MailType type_;                  // 邮件类型
  6 : required i64 time_;                            // 邮件创建时间
  7 : required i32 id_;                              // 邮件ID
  8 : required i64 addressee_id_;                    // 收件人ID
}

