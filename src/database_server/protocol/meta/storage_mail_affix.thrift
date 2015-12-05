include "mail.thrift"

namespace cpp database.protocol

enum MailAffixField {
  ITEM1 = 0,
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

const list<string> kMailAffixField =  [
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
//提取附件
struct StoragePickAffixRequest {
  1 : required i64 id_;                                 // 玩家ID
  2 : required i32 mail_id_;                            // 邮件ID
}

////////////////////////
//提取附件相应
struct StoragePickAffixResponse {
  1 : required mail.MailAffix affix_;                   // 邮件附件
  2 : required i32 mail_id_;                            // 邮件id
  3 : required i64 id_;                                 // 邮件玩家ID
}

