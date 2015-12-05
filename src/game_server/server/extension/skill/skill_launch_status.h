//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-05 10:03:09.
// File name: skill_launch_status.h
//
// Description:
// Define class SkillLaunchStatus.
//

#ifndef __GAME__SERVER__SKILL__SKILL__LAUNCH__STATUS__H
#define __GAME__SERVER__SKILL__SKILL__LAUNCH__STATUS__H

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "game_server/server/extension/skill/skill_status.h"
#include "game_server/server/request/game_request_types.h"

namespace game {

namespace server {

namespace skill {

class SkillRole;

class SkillLaunchStatus : public SkillStatus, public core::TimerHandler {
 public:
  SkillLaunchStatus();
  virtual ~SkillLaunchStatus();

  bool Initialize(SkillRole *role);
  void Finalize();

  // 当前状态
  virtual SkillStatusType::type GetStatus() const {
    return SkillStatusType::LAUNCH;
  }

  // 状态 进入/退出
  virtual void OnEnter();
  virtual void OnLeave();

  // 消息处理
  virtual int OnExecute(request::RequestType::type type,
      const void *data, size_t size);

  // 定时器回调
  virtual void OnTimer(core::uint32 id);

 private:
  // 最小发射时间(飞行时间小于这个时间，直接转到结束状态)
  static const core::uint32 kLaunchMinMsecs_ = 100;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__LAUNCH__STATUS__H

