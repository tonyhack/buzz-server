//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 13:43:39.
// File name: change_attr_action.h
//
// Description:
// Define class ChangeAttrAction.
//

#ifndef __GAME__SERVER__AI__CHANGE__ATTR__ACTION__H
#define __GAME__SERVER__AI__CHANGE__ATTR__ACTION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/action_node.h"
#include "game_server/server/extension/ai/ai_types.h"

namespace game {

namespace server {

namespace ai {

class ChangeAttrAction : public ActionNode {
 public:
  ChangeAttrAction();
  virtual ~ChangeAttrAction();

  virtual const std::string& GetTypeName() const {
    return ChangeAttrAction::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);

  virtual void OnEnter(BlackBoard *black_board);
  virtual RunningStatus::type OnExecute(BlackBoard *black_board);
  virtual void OnQuit(BlackBoard *black_board);

  AttrType::type type_;
  core::int32 increment_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHANGE__ATTR__ACTION__H
