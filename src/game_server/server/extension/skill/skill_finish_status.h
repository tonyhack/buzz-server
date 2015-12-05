//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-05 11:28:02.
// File name: skill_finish_status.h
//
// Description:
// Define class SkillFinishStatus.
//

#ifndef __GAME__SERVER__SKILL__SKILL__FINISH__STATUS__H
#define __GAME__SERVER__SKILL__SKILL__FINISH__STATUS__H

#include "game_server/server/extension/skill/skill_status.h"

namespace game {

namespace server {

namespace skill {

class SkillFinishStatus : public SkillStatus {
 public:
  SkillFinishStatus();
  virtual ~SkillFinishStatus();

  bool Initialize(SkillRole *role);
  void Finalize();

  // 当前状态
  virtual SkillStatusType::type GetStatus() const {
    return SkillStatusType::FINISH;
  }

  // 状态 进入/退出
  virtual void OnEnter();
  virtual void OnLeave();

  // 消息处理
  virtual int OnExecute(request::RequestType::type type,
      const void *data, size_t size);

 private:
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__FINISH__STATUS__H

