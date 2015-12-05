//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-09 12:01:37.
// File name: simple_use_skill_action.h
//
// Description:
// Define class SimpleUseSkillAction.
//

#ifndef __GAME__SERVER__AI__SIMPLE__USE__SKILL__ACTION__H
#define __GAME__SERVER__AI__SIMPLE__USE__SKILL__ACTION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"
#include "game_server/server/extension/ai/ai_types.h"

namespace game {

namespace server {

namespace ai {

class SimpleUseSkillAction : public ActionNode {
 public:
  SimpleUseSkillAction();
  virtual ~SimpleUseSkillAction();

  virtual const std::string& GetTypeName() const {
    return SimpleUseSkillAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  core::int32 skill_id_;
  SkillTargetType::type target_type_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SIMPLE__USE__SKILL__ACTION__H

