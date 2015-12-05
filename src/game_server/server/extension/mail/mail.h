#ifndef __GAME__SERVER__MAIL__MAIL__H
#define __GAME__SERVER__MAIL__MAIL__H

#include <vector>
#include <string>

#include "core/base/types.h"
#include "core/timer_handler.h"
#include "entity/mail_constants.h"
#include "entity/mail_types.h"
#include "game_server/server/game_entity.h"

namespace game {

namespace server {

namespace mail {

class MailActor;

class Mail : public core::TimerHandler, public core::Noncopyable {
  enum { DISABLE_TIMER = 0, };

 public:
  Mail();
  ~Mail();

  bool Initialize(MailActor* actor);
  void Finalize();

  inline bool isValid() {
    return (entity::MailAffixType::type)type_ == entity::MailAffixType::NONE ? false : true; 
  }
  inline entity::MailType::type GetMailType() const {
    return (entity::MailType::type)type_;
  }
  inline entity::MailReadState::type GetMailReadState() const {
    return (entity::MailReadState::type)read_state_;
  }
  inline core::int32 GetMailID() const {
    return id_;
  }
  inline bool GetMailReadSend() const {
    return read_send_;
  }
  inline entity::MailAffixType::type GetMailAffixType() const {
    return (entity::MailAffixType::type)affix_type_;
  }
  inline core::uint64 GetMailCreateTime() const {
    return create_time_;
  }
  inline void SetMailCreateTime(core::uint64 create_time) {
    this->create_time_ = create_time;
  }
  inline void SetDbId(core::int32 id) {
    this->id_ = id;
  }
  inline void SetMailType(entity::MailType::type type) {
    this->type_ = (core::int8)type;
  }
  inline void SetMailAffixType(entity::MailAffixType::type affix_type) {
    this->affix_type_ = (core::int8)affix_type;
  }
  inline void SetMailInvalid() {
    this->type_ = (core::int8)entity::MailType::INVALID;
  }
  inline void SetMailReadState(entity::MailReadState::type type) {
    this->read_state_ = type;
  }
  inline void SetMailReadSend() {
    this->read_send_ = true;
  }
  // 开启邮件时间
  void StartExpireTimer();
  /*
  // 创建邮件
  void CreateMail(entity::MailType::type mail_type, entity::MailAffixType::type mail_affix_type,
      core::int32 mail_ID, core::uint64 create_time);
  */
  // 定时器触发
  virtual void OnTimer(core::uint32 id);
  // 删除邮件
  void DeleteMail();

 private:
  // 玩家
  MailActor* actor_;
  // 邮件创建时间
  core::int64 create_time_;
  // 数据库返回ID
  core::int32 id_;
  // 邮件类型
  core::int8 type_;
  // 附件类型
  core::int8 affix_type_;
  // 邮件读取状态
  core::int8 read_state_;
  // 本次邮件是否已经发送过邮件正文
  bool read_send_;
};

}  // namespace mail

}  // namespace server

}  // namespace game

#endif // __GAME__SERVER__MAIL__MAIL__H
