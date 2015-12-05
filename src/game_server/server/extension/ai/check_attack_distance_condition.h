//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 11:36:38.
// File name: check_attack_distance_condition.h
//
// Description:
// Define class CheckAttackDistanceCondition.
//

#ifndef __GAME__SERVER__CHECK__ATTACK__DISTANCE__CONDITION__H
#define __GAME__SERVER__CHECK__ATTACK__DISTANCE__CONDITION__H

#include "core/base/types.h"
#include "game_server/server/extension/ai/condition_node.h"

namespace game {

namespace server {

namespace ai {

class CheckAttackDistanceCondition : public ConditionNode {
 public:
  typedef CheckAttackDistanceCondition Type;

  CheckAttackDistanceCondition();
  ~CheckAttackDistanceCondition();

  virtual const std::string& GetTypeName() const {
    return CheckAttackDistanceCondition::kTypeName_;
  }

  static const std::string kTypeName_;

 private:
  virtual bool LoadConfigure(TiXmlElement *node);
  virtual bool DoCheckEvalute(BlackBoard *black_board) const;

  // 距离
  core::uint32 distance_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CHECK__ATTACK__DISTANCE__CONDITION__H

