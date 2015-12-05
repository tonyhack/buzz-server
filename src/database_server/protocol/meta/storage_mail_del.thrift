include "mail.thrift"

namespace cpp database.protocol

enum MailDelType {
  MAIL_DEL_TYPE_1 = 1,                                                 // 主动删除
  MAIL_DEL_TYPE_2,                                                     // 被动删除
}

////////////////////////////
//删除邮件请求
struct StorageDelMailRequest {
  1 : required i64 id_;                                                // 玩家ID
  2 : required list<i32> id_list_;                                     // 删除的邮件ID列表
  3 : required MailDelType del_type_;                                  // 邮件删除类型
}

/////////////////////////////
//删除邮件返回
struct StorageDelMailResponse {
  1 : required i64 id_;                                                // 玩家ID 
  2 : required list<i32> id_list_;                                     // 返回已经删除的ID列表
}

