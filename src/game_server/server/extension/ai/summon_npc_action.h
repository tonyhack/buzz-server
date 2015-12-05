//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:44:56.
// File name: summon_npc_action.h
//
// Description:
// Define class SummonNpcAction.
//

#ifndef __GAME__SERVER__AI__SUMMON__NPC__ACTION__H
#define __GAME__SERVER__AI__SUMMON__NPC__ACTION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"
#include "game_server/server/extension/ai/ai_types.h"

namespace game {

namespace server {

namespace ai {

class SummonNpcAction : public ActionNode {
 public:
  SummonNpcAction();
  virtual ~SummonNpcAction();

  virtual const std::string& GetTypeName() const {
    return SummonNpcAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  core::int32 npc_template_;
  TargetType::type position_type_;
  core::int32 radius_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SUMMON__NPC__ACTION__H

