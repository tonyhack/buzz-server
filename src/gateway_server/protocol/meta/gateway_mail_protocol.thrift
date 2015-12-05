include "gateway_protocol.thrift"
include "mail.thrift"


namespace cpp gateway.protocol
namespace as3 com.dreamfire.sanguo.data.definition.protocol.gateway

struct MailContext {
  1 : required string title_;                        // 标题
  2 : required string addresser_;                    // 发件人
  3 : required mail.MailReadState state_;            // 读取状态
  4 : required mail.MailAffixType affix_type_;       // 附件状态
  5 : required mail.MailType type_;                  // 邮件类型
  6 : required string time_;                         // 邮件创建时间
  7 : required i32 id_;                              // 邮件ID
}



////////////////////////////////////////
// 初始化邮件列表
// type: MESSAGE_MAIL_INITIALIZE
// dir: gateway->client
struct MessageMailInitialize {
  1 : required list<MailContext> mail_list_;         // 邮件列表
}

////////////////////////////////////////
// 服务器转发邮件
// type: MESSAGE_MAIL_ADD
// dir: gateway->client

struct MessageMailAdd {
  1 : required MailContext mail_;                    // 下发邮件
}

///////////////////////////////////////
// 客户端发送邮件
// type: MESSAGE_MAIL_SEND_REQUEST
// dir: client->gateway
struct MessageMailSendRequest {
  1 : required string addressee_;                    // 收件人名字
  2 : required string title_;                        // 邮件标题
  3 : required string context_;                      // 邮件正文
}

//////////////////////////////////////
// 服务器端向客户端确认发送邮件成功与否
// type: MESSAGE_MAIL_SEND_RESPONSE
// dir: gateway->client
struct MessageMailSendResponse {
  1 : required bool result_;                         // 成功与否
}

//////////////////////////////////////
// 玩家删除邮件
// type: MESSAGE_MAIL_DEL_REQUEST
// dir: client->getway
struct MessageMailDelRequest {
  1 : required list<i32> del_mail_list_;             // 删除邮件列表
}

///////////////////////////////////////
// 玩家删除邮件返回结果
// type: MESSAGE_MAIL_DEL_RESPONSE
// dir: gateway->client
struct MessageMailDelResponse {
  1 : required bool result_;                         // 删除邮件返回
}

/////////////////////////////////////////
// 玩家读取邮件
// type: MESSAGE_MAIL_READ_REQUEST
// dir: client->gateway
struct MessageMailReadRequest {
  1 : required i32 mail_id_;                        // 邮件ID
}

////////////////////////////////////////
// 玩家读取邮件返回
// type: MESSAGE_MAIL_READ_RESPONSE
// dir: gateway->client
struct MessageMailReadResponse {
  1 : required string context_;                     // 邮件正文内容
  2 : required mail.MailAffix affix_;               // 附件内容
}

////////////////////////////////////////
// 请求提取附件
// type: MESSAGE_MAIL_PICK_AFFIX_REQUEST
// dir: client->gateway
struct MessageMailPickAffixRequest {
  1 : required i32 mail_id_;                        // 邮件ID
}

////////////////////////////////////////
// 提取附件结果
// type: MESSAGE_MAIL_PICK_AFFIX_RESPONSE
// dir: gateway->client
struct MessageMailPickAffixResponse {
  1 : required bool result_;                        // 提取附件返回结果
}

//////////////////////////////////////
// 删除邮件
// type: MESSAGE_MAIL_DEL
// dir: gateway->client
struct MessageMailDel {
  1 : required i32 id_;                              // 删除邮件ID
}

