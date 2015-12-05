//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 11:01:53.
// File name: decision_allocator.cc
//
// Description:
// Define class DecisionAllocator.
//

#include "game_server/server/extension/ai/decision_allocator.h"

#include "game_server/server/extension/ai/behaviour_node.h"
#include "game_server/server/extension/ai/behaviour_node_factory.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

static const char *kDecisionTypeStr = "decision";
static const size_t kStrlenDecision = 9;

static const char *kBehaviourTypeStr = "behaviour";
static const size_t kStrlenBehaviour = 10;

static const char *kConditionTypeStr = "condition";
static const size_t kStrlenCondition = 10;

static const char *kActionTypeStr = "action";
static const size_t kStrlenAction = 7;

DecisionAllocator::DecisionAllocator() {}
DecisionAllocator::~DecisionAllocator() {}

BehaviourNode *DecisionAllocator::Allocate(BehaviourNode *parent, TiXmlElement *node) {
  // 分配当前结点
  BehaviourNode *behaviour_node = this->AllocateSelf(parent, node);
  if(behaviour_node == NULL) {
    global::LogError("%s:%d (%s) AllocateSelf 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  bool parse = true;

  BehaviourNode *parent_node = behaviour_node;
  BehaviourNode *current_node = NULL;

  // 解析和分配子结点
  TiXmlElement *child_node = node->FirstChildElement("node");
  while(child_node) {
    const char *desc = child_node->Attribute("desc");
    if(desc == NULL) {
      global::LogError("%s:%d (%s) 找不到结点[node-desc]",
          __FILE__, __LINE__, __FUNCTION__);
      parse = false;
      break;
    }
    const char *type1 = child_node->Attribute("type1");
    if(type1 == NULL) {
      global::LogError("%s:%d (%s) 找不到结点[node-type1], desc=%s",
          __FILE__, __LINE__, __FUNCTION__, desc);
      parse = false;
      break;
    }
    const char *type2 = child_node->Attribute("type2");
    if(type2 == NULL) {
      global::LogError("%s:%d (%s) 找不到结点[node-type1], desc=%s",
          __FILE__, __LINE__, __FUNCTION__, desc);
      parse = false;
      break;
    }
    const char *name = child_node->Attribute("name");
    if(name == NULL) {
      global::LogError("%s:%d (%s) 找不到结点[node-name], desc=%s",
          __FILE__, __LINE__, __FUNCTION__, desc);
      parse = false;
      break;
    }
    // 根据配置类型分配结点
    current_node = BehaviourNodeFactory::GetInstance()->Allocate(type1, type2,
        name, parent_node, child_node);
    if(current_node == NULL) {
      global::LogError("%s:%d (%s) 分配结点(type1=%s, type2=%s, name=%s)失败, desc=%s",
          __FILE__, __LINE__, __FUNCTION__, type1, type2, name, desc);
      parse = false;
      break;
    }
    // 把节点加入到树中
    if(parent_node->AddChildNode(current_node) == false) {
      global::LogError("%s:%d (%s) 结点(type1=%s, type2=%s, name=%s)加入树失败, desc=%s",
          __FILE__, __LINE__, __FUNCTION__, type1, type2, name, desc);
      parse = false;
      // 加入失败时，删除所在结点
      current_node->Deallocate();
      break;
    }
    child_node = child_node->NextSiblingElement("node");
  }

  // 解析失败，删除当前结点
  if(parse == false) {
    behaviour_node->Deallocate();
    behaviour_node = NULL;
  }

  return behaviour_node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

