include "mail.thrift"
include "storage_mail_login.thrift"

namespace cpp database.protocol

enum MailAddField {
  ID = 0,
  ACTOR_ID,
  CREATE_TIME,

  MAX,
}

const list<string> kMailAddField = [
  "@ret_mail_id";
  "@actor_id";
  "@param_create_time";
]

//////////////////////////
//添加邮件
struct StorageAddMailRequest {
  1 : required mail.MailAffix affix_;                             // 邮件附件
  2 : required i64 addressee_id_;                                 // 收件人id
  3 : required i64 addresser_id_;                                 // 发件人ID
  4 : required string addresser_;                                 // 发件人名字
  5 : required string title_;                                     // 邮件标题
  6 : required string context_;                                   // 邮件正文
  7 : required mail.MailType type_;                               // 邮件类型
}

//////////////////////////
//响应发件请求
struct StorageAddMailResponse {
  1 : required storage_mail_login.MailHead mail_head_;            // 邮件头部
  2 : required i64 addressee_id_;                                 // 收件人ID
  3 : required i64 addresser_id_;                                 // 发件人ID
}

