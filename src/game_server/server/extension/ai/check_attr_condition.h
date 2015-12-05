//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 14:04:41.
// File name: check_attr_condition.h
//
// Description:
// Define class CheckAttrCondition.
//

#ifndef __GAME__SERVER__AI__CHECK__ATTR__CONDITION__H
#define __GAME__SERVER__AI__CHECK__ATTR__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/condition_node.h"
#include "game_server/server/extension/ai/ai_types.h"

namespace game {

namespace server {

namespace ai {

class CheckAttrCondition : public ConditionNode {
 public:
  typedef CheckAttrCondition Type;

  CheckAttrCondition();
  virtual ~CheckAttrCondition();

  virtual const std::string& GetTypeName() const {
    return CheckAttrCondition::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);
  virtual bool DoCheckEvalute(BlackBoard *black_board) const;

  AttrConditionType::type condition_type_;
  RelationalOperator::type operator_type_;
  core::int32 value_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__ATTR__CONDITION__H

