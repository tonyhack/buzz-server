include "database_storage.thrift"
include "mail.thrift"

namespace cpp database.protocol

enum MailLoadField {
  ID = 0,
  TYPE,
  AFFIX_STATE,
  READ_STATE,
  TITLE,
  CREATE_TIME,
  ADDRESSER,

  MAX,
}

const list<string> kMailLoadField = [
  "mai_id",                              // 邮件ID
  "type",                                // 邮件类型
  "affix_state",                         // 附件状态
  "read_state",                          // 阅读状态
  "title",                               // 标题
  "create_time",                         // 创建时间
  "addresser",                           // 发件人
]

enum MailLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

struct MailHead {
   1 : required string title_;                        // 标题
   2 : required string addresser_;                    // 发件人
   3 : required mail.MailReadState state_;            // 读取状态
   4 : required mail.MailAffixType affix_type_;       // 附件状态
   5 : required mail.MailType type_;                  // 邮件类型
   6 : required i64 time_;                            // 邮件创建时间
   7 : required i32 id_;                              // 邮件ID
}

/////////////////////////
//登陆初始化邮件列表
struct StorageMailLoginRequest {
  1 : required i64 id_;                                 // 玩家ID
}

///////////////////////////
//返回邮件初始化列表
struct StorageMailLoginResponse {
  1 : required MailLoginResult result_;               // 结果
  2 : required list<MailHead> mail_list_;             // 邮件头列表
  3 : required i64 id_;                               // 邮件ID
}

