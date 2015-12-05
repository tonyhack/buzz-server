//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 10:17:39.
// File name: behaviour_node.h
//
// Description:
// Define class BehaviourNode.
//

#ifndef __GAME__SERVER__AI__BEHAVIOUR__NODE__H
#define __GAME__SERVER__AI__BEHAVIOUR__NODE__H

#include <cstddef>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/tinyxml/tinyxml.h"

namespace game {

namespace server {

namespace ai {

class BlackBoard;

static const size_t kChildrenNumber = 32;

struct RunningStatus {
  enum type {
    FINISH = 0,
    EXECUTING,
    ERROR,
  };
};

class BehaviourNode : public core::Noncopyable {
 public:
  typedef BehaviourNode Type;
  typedef std::vector<BehaviourNode *> NodeVector;

  BehaviourNode();
  virtual ~BehaviourNode();

  virtual bool Initialize(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate();

  // 条件检测
  virtual bool CheckEvaluate(BlackBoard *black_board) const { return true; }
  // 结点转移回调
  virtual void OnTransition(BlackBoard *black_board) {}

  // Ai运行
  virtual RunningStatus::type OnRunning(BlackBoard *black_board) {
    return RunningStatus::FINISH;
  }

  virtual void OnAddChildNode(BehaviourNode *node) {}

  // 增加子结点
  inline bool AddChildNode(BehaviourNode *node) {
    if(node == NULL ||
        this->children_.size() >= kChildrenNumber) {
      return false;
    }
    this->children_.push_back(node);
    this->OnAddChildNode(node);
    return true;
  }
  // 删除某个子结点
  void RemoveChildNode(BehaviourNode *node) {
    if(node == NULL) {
      return ;
    }
    NodeVector::iterator iterator = this->children_.begin();
    for(; iterator != this->children_.end(); ++iterator) {
      if(*iterator == node) {
        this->children_.erase(iterator);
        return ;
      }
    }
  }

  // 获取/设置 运行状态
  inline RunningStatus::type GetRunningStatus() const {
    return this->running_status_;
  }
  inline void SetRunningStatus(RunningStatus::type type) {
    this->running_status_ = type;
  }

 protected:
  // 索引是否合法
  inline bool CheckIndexValid(size_t index) const {
    return index < this->children_.size();
  }
  // 父结点
  BehaviourNode *parent_;

  // 子结点
  NodeVector children_;

  // 运行状态
  RunningStatus::type running_status_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__BEHAVIOUR__NODE__H

