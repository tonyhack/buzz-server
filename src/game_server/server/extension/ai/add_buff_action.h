//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 18:52:07.
// File name: add_buff_action.h
//
// Description:
// Define class AddBuffAction.
//

#ifndef __GAME__SERVER__AI__ADD__BUFF__ACTION__H
#define __GAME__SERVER__AI__ADD__BUFF__ACTION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"
#include "game_server/server/extension/ai/ai_types.h"

namespace game {

namespace server {

namespace ai {

class AddBuffAction : public ActionNode {
 public:
  AddBuffAction();
  virtual ~AddBuffAction();

  virtual const std::string& GetTypeName() const {
    return AddBuffAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  core::int32 buff_id_;
  TargetType::type target_type_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__ADD__BUFF__ACTION__H

