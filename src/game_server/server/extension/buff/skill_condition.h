//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-26 14:18:20.
// File name: skill_condition.h
//
// Description:
// Define skill condition.
//

#ifndef __GAME__SERVER__BUFF__SKILL__CONDITION__H
#define __GAME__SERVER__BUFF__SKILL__CONDITION__H

#include "game_server/server/extension/buff/condition.h"

namespace game {

namespace server {

namespace buff {

class SkillUseCondition : public Condition {
 public:
  SkillUseCondition();
  virtual ~SkillUseCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::SKILL_USE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute();

 private:
  bool OnEventUseSkill(const void *data, size_t size);
};

class SkillGroupUseCondition : public Condition {
 public:
  SkillGroupUseCondition();
  virtual ~SkillGroupUseCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::SKILL_GROUP_USE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute();

 private:
  bool OnEventUseSkill(const void *data, size_t size);
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__SKILL__CONDITION__H

