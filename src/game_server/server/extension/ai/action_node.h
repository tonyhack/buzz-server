//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 14:36:10.
// File name: action_node.h
//
// Description:
// Define class ActionNode.
//

#ifndef __GAME__SERVER__AI__ACTION__NODE__H
#define __GAME__SERVER__AI__ACTION__NODE__H

#include <string>

#include "game_server/server/extension/ai/behaviour_node.h"

namespace game {

namespace server {

namespace ai {

struct ActionNodeStatus {
  enum type {
    ENTER = 0,
    EXECUTE,
    QUIT,

    MAX,
  };
};

class ActionNode : public BehaviourNode {
 public:
  typedef ActionNode Type;

  ActionNode();
  virtual ~ActionNode();

  virtual bool Initialize(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate();

  virtual void OnTransition(BlackBoard *black_board);
  virtual RunningStatus::type OnRunning(BlackBoard *black_board);

  virtual const std::string &GetTypeName() const = 0;

 protected:
  // 加载配置
  virtual bool LoadConfigure(TiXmlElement *node) = 0;

  // 进入/执行/退出
  virtual void OnEnter(BlackBoard *black_board) = 0;
  virtual RunningStatus::type OnExecute(BlackBoard *black_board) = 0;
  virtual void OnQuit(BlackBoard *black_board) = 0;

  // 当前状态
  ActionNodeStatus::type status_;
  // 是否需要做退出动作
  bool need_quit_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__ACTION__NODE__H

