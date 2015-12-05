//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 19:04:00.
// File name: skill_boot_status.h
//
// Description:
// Define class SkillBootStatus.
//

#ifndef __GAME__SERVER__SKILL__SKILL__BOOT__STATUS__H
#define __GAME__SERVER__SKILL__SKILL__BOOT__STATUS__H

#include "core/timer_handler.h"
#include "game_server/server/extension/skill/skill_status.h"
#include "game_server/server/game_role.h"

namespace game {

namespace server {

namespace skill {

class SkillBootStatus : public SkillStatus, public core::TimerHandler {
 public:
  SkillBootStatus();
  virtual ~SkillBootStatus();

  bool Initialize(SkillRole *role);
  void Finalize();

  // 当前状态
  virtual SkillStatusType::type GetStatus() const {
    return SkillStatusType::BOOT;
  }

  // 状态 进入/退出
  virtual void OnEnter();
  virtual void OnLeave();

  // 消息处理
  virtual int OnExecute(request::RequestType::type type,
      const void *data, size_t size);

  // 定时器回调
  virtual void OnTimer(core::uint32 id);

  // 事件处理
  virtual void OnEvent(event::EventType::type type, const void *message,
      size_t size);

  // 通知客户端打断技能条
  void BreakSynchronize();

 private:
  // 改变角色的状态
  void RoleForbidMove(GameRole *game_role, bool forbid);
  // 角色状态变更事件
  void OnEventRoleCommonStatusChange(const void *message, size_t size);
  // 最小引导时间(小于这个时间忽略引导)
  static const core::int32 kBootMinMsecs_ = 100;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__BOOT__STATUS__H

