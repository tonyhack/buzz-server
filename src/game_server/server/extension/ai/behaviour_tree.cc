//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 16:32:04.
// File name: behaviour_tree.cc
//
// Description:
// Define class BehaviourTree.
//

#include "game_server/server/extension/ai/behaviour_tree.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension/ai/behaviour_node_factory.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

static const char *kDecisionTypeStr = "decision";
static const size_t kStrlenDecision = 9;

static const char *kBehaviourTypeStr = "behaviour";
static const size_t kStrlenBehaviour = 10;

BehaviourTree::BehaviourTree() : root_(NULL), id_(0) {}
BehaviourTree::~BehaviourTree() {}

bool BehaviourTree::Initialize(core::uint32 id, const std::string &file) {
  if(this->LoadConfigure(file) == false) {
    return false;
  }
  this->id_ = id;
  return true;
}

void BehaviourTree::Deallocate() {
  if(this->root_) {
    this->root_->Deallocate();
  }
}

bool BehaviourTree::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载配置文件(%s)出错", 
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载根结点
  TiXmlElement *root_node = doc.FirstChildElement("node");
  if(root_node == NULL) {
    global::LogError("%s:%d (%s) 找不到树根结点，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  const char *desc = root_node->Attribute("desc");
  if(desc == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [node-desc]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *type1 = root_node->Attribute("type1");
  if(type1 == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [node-type1, desc=%s]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, desc, file.c_str());
    return false;
  }
  const char *type2 = root_node->Attribute("type2");
  if(type2 == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [node-type2, desc=%s]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, desc, file.c_str());
    return false;
  }
  const char *name = root_node->Attribute("name");
  if(name == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [node-name, desc=%s]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, desc, file.c_str());
    return false;
  }
  this->root_ = BehaviourNodeFactory::GetInstance()->Allocate(type1, type2,
      name, NULL, root_node);
  if(this->root_ == NULL) {
    global::LogError("%s:%d (%s) 分配结点(type1=%s, type2=%s, name=%s)失败, desc=%s",
        __FILE__, __LINE__, __FUNCTION__, type1, type2, name, desc);
    return false;
  }

  return true;
}

void BehaviourTree::Running(BlackBoard *black_board) {
  if(this->root_ && this->root_->CheckEvaluate(black_board)) {
    this->root_->OnRunning(black_board);
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game

