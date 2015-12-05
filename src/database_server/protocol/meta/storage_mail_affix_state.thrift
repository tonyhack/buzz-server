include "mail.thrift"

namespace cpp database.protocol

//////////////////////////
// 变更附件状态
struct StorageMailAffixStateRequest {
  1 : required i64 id_;                               // 邮件玩家ID
  2 : required i32 mail_id_;                          // 邮件ID
  3 : required mail.MailAffixType type_;              // 邮件附件类型
}

//////////////////////////
// 相应附件变更状态
struct StorageMailAffixStateResponse {

}

