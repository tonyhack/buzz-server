//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 14:23:16.
// File name: skill_idle_status.h
//
// Description:
// Define class SkillIdleStatus.
//

#ifndef __GAME__SERVER__SKILL__SKILL__IDLE__STATUS__H
#define __GAME__SERVER__SKILL__SKILL__IDLE__STATUS__H

#include "game_server/server/extension/skill/skill_status.h"
#include "game_server/server/request/game_request_types.h"

namespace game {

namespace server {

namespace skill {

class SkillIdleStatus : public SkillStatus {
 public:
  SkillIdleStatus();
  virtual ~SkillIdleStatus();

  bool Initialize(SkillRole *role);
  void Finalize();

  // 当前状态
  virtual SkillStatusType::type GetStatus() const {
    return SkillStatusType::IDLE;
  }

  // 状态进入/退出
  virtual void OnEnter();
  virtual void OnLeave();

  // 消息处理
  virtual int OnExecute(request::RequestType::type type,
      const void *data, size_t size);

 private:
  // 使用技能
  int OnUseSkill(const void *data, size_t size);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__IDLE__STATUS__H

