//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-09 11:32:24.
// File name: delete_me_action.h
//
// Description:
// Define class DeleteMeAction.
//

#ifndef __GAME__SERVER__AI__DELETE__ME__ACTION__H
#define __GAME__SERVER__AI__DELETE__ME__ACTION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"

namespace game {

namespace server {

namespace ai {

class DeleteMeAction : public ActionNode {
 public:
  DeleteMeAction();
  virtual ~DeleteMeAction();

  virtual const std::string& GetTypeName() const {
    return DeleteMeAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__DELETE__ME__ACTION__H

