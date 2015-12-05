include "mail.thrift"

namespace cpp database.protocol

enum MailReadField {
  CONTEXT = 0,
  ITEM1,
  ITEM1_NUM,
  ITEM2,
  ITEM2_NUM,
  ITEM3,
  ITEM3_NUM,
  ITEM4,
  ITEM4_NUM,
  ITEM5,
  ITEM5_NUM,
  GOLD,
  FREEDOM_DOLLARS,

  MAX,
}

const list<string> kMailReadField = [
  "context",
  "item1",
  "item1_num"
  "item2",
  "item2_num",
  "item3",
  "item3_num",
  "item4",
  "item4_num",
  "item5",
  "item5_num",
  "gold",
  "freedom_dollars",
]

//////////////////////////
// 读取邮件请求
struct StorageReadMailRequest {
   1 : required i32 mail_id_;                                    // 邮件ID
   2 : required i64 id_;                                         // 玩家ID
}

/////////////////////////
// 读取邮件相应
struct StorageReadMailResponse {
   1 : required mail.MailAffix affix_;                           // 邮件附件
   2 : required string context_;                                 // 邮件正文
   3 : required i64 id_;                                         // 玩家ID
   4 : required i32 mail_id_;                                    // 邮件ID
}

