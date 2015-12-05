namespace cpp entity
namespace py ccentity.mail
namespace as3 com.dreamfire.sanguo.data.definition.entity

////////////////////////////////////////////////
// 邮件类型

enum MailType {
  INVALID = 0,                     // 无效的邮件
  SYSTEM,                          // 系统邮件
  OPERATING,                       // 运营邮件
  GENERAL,                         // 普通邮件
}

////////////////////////////////////////////////
// 附件类型

enum MailAffixType {
  NONE = 0,                        // 没有附件
  EXIST,                           // 有附件
  PICKED,                          // 附件已被领取
}


////////////////////////////////////////////////////////////////////////////
// 邮件附件
struct MailAffix {
  1 : required i32 item1_,                          // 附件物品1
  2 : required i32 item1_num_,                      // 附件物品数目1
  3 : required i32 item2_,                          // 附件物品2
  4 : required i32 item2_num_,                      // 附件物品数目2
  5 : required i32 item3_,                          // 附件物品3
  6 : required i32 item3_num_,                      // 附件物品数目3
  7 : required i32 item4_,                          // 附件物品4
  8 : required i32 item4_num_,                      // 附件物品数目4 
  9 : required i32 item5_,                          // 附件物品4
  10: required i32 item5_num_,                      // 附件物品数目4
  11: required i32 gold_,                           // 金币
  12: required i32 restrict_dollars_,               // 礼券  
}

enum MailReadState {
  UNREAD = 0,                                      // 未读邮件
  READ,                                            // 已读取邮件
}


