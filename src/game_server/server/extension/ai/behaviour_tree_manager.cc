//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 10:16:54.
// File name: behaviour_tree_manager.cc
//
// Description:
// Define class BehaviourTreeManager.
//

#include "game_server/server/extension/ai/behaviour_tree_manager.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension/ai/behaviour_factory.h"
#include "game_server/server/extension/ai/behaviour_tree.h"
#include "global/logging.h"
#include "global/server_configure.h"

#include "game_server/server/extension/ai/activity_range_condition_allocator.h"
#include "game_server/server/extension/ai/check_attack_distance_condition_allocator.h"
#include "game_server/server/extension/ai/check_attr_condition_allocator.h"
#include "game_server/server/extension/ai/check_back_state_condition_allocator.h"
#include "game_server/server/extension/ai/check_birth_place_condition_allocator.h"
#include "game_server/server/extension/ai/check_distance_condition_allocator.h"
#include "game_server/server/extension/ai/check_interval_seconds_condition_allocator.h"
#include "game_server/server/extension/ai/check_threat_condition_allocator.h"
#include "game_server/server/extension/ai/check_var_condition_allocator.h"
#include "game_server/server/extension/ai/target_exist_condition_allocator.h"
#include "game_server/server/extension/ai/add_buff_action_allocator.h"
#include "game_server/server/extension/ai/change_attr_action_allocator.h"
#include "game_server/server/extension/ai/choose_target_action_allocator.h"
#include "game_server/server/extension/ai/clear_target_action_allocator.h"
#include "game_server/server/extension/ai/delete_me_action_allocator.h"
#include "game_server/server/extension/ai/go_back_action_allocator.h"
#include "game_server/server/extension/ai/log_action_allocator.h"
#include "game_server/server/extension/ai/move_target_action_allocator.h"
#include "game_server/server/extension/ai/random_move_action_allocator.h"
#include "game_server/server/extension/ai/remove_target_action_allocator.h"
#include "game_server/server/extension/ai/reset_interval_seconds_action_allocator.h"
#include "game_server/server/extension/ai/search_target_action_allocator.h"
#include "game_server/server/extension/ai/set_back_state_action_allocator.h"
#include "game_server/server/extension/ai/set_var_action_allocator.h"
#include "game_server/server/extension/ai/simple_move_target_action_allocator.h"
#include "game_server/server/extension/ai/simple_use_skill_action_allocator.h"
#include "game_server/server/extension/ai/summon_npc_action_allocator.h"
#include "game_server/server/extension/ai/selector_decision_allocator.h"
#include "game_server/server/extension/ai/sequence_decision_allocator.h"

namespace game {

namespace server {

namespace ai {

bool BehaviourTreeManager::Initialize(const std::string &file) {
  if(ConditionFactory::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) ConditionFactory 不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->RegistAllocator();

  if(this->Parse(file) == false) {
    global::LogError("%s:%d (%s) 配置文件(%s) 解析失败",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    this->CancelRegistAllocator();
    return false;
  }

  return true;
}

void BehaviourTreeManager::Finalize() {
  BehaviourTreeHashmap::iterator iterator = this->behaviour_trees_.begin();
  for(; iterator != this->behaviour_trees_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->Deallocate();
      delete iterator->second;
    }
  }
  this->behaviour_trees_.clear();
  this->CancelRegistAllocator();
}

BehaviourTree *BehaviourTreeManager::Get(core::uint32 id) {
  BehaviourTreeHashmap::iterator iterator = this->behaviour_trees_.find(id);
  if(iterator == this->behaviour_trees_.end()) {
    return NULL;
  }
  return iterator->second;
}

bool BehaviourTreeManager::Add(BehaviourTree *tree) {
  if(tree == NULL) {
    return false;
  }
  if(this->behaviour_trees_.find(tree->GetID()) != this->behaviour_trees_.end()) {
    return false;
  }
  this->behaviour_trees_.insert(std::make_pair(tree->GetID(), tree));
  return true;
}

BehaviourTree *BehaviourTreeManager::Remove(core::uint32 id) {
  BehaviourTree *tree = NULL;
  BehaviourTreeHashmap::iterator iterator = this->behaviour_trees_.find(id);
  if(iterator != this->behaviour_trees_.end()) {
    tree = iterator->second;
    this->behaviour_trees_.erase(iterator);
  }
  return tree;
}

void BehaviourTreeManager::RegistAllocator() {
  /////////////////////////
  // condition
  ConditionFactory::GetInstance()->Register(ActivityRangeCondition::kTypeName_,
      new (std::nothrow) ActivityRangeConditionAllocator);
  ConditionFactory::GetInstance()->Register(CheckAttackDistanceCondition::kTypeName_,
      new (std::nothrow) CheckAttackDistanceConditionAllocator);
  ConditionFactory::GetInstance()->Register(CheckAttrCondition::kTypeName_,
      new (std::nothrow) CheckAttrConditionAllocator);
  ConditionFactory::GetInstance()->Register(CheckBackStateCondition::kTypeName_,
      new (std::nothrow) CheckBackStateConditionAllocator);
  ConditionFactory::GetInstance()->Register(CheckBirthPlaceCondition::kTypeName_,
      new (std::nothrow) CheckBirthPlaceConditionAllocator);
  ConditionFactory::GetInstance()->Register(CheckDistanceCondition::kTypeName_,
      new (std::nothrow) CheckDistanceConditionAllocator);
  ConditionFactory::GetInstance()->Register(CheckIntervalSecondsCondition::kTypeName_,
      new (std::nothrow) CheckIntervalSecondsConditionAllocator);
  ConditionFactory::GetInstance()->Register(CheckThreatCondition::kTypeName_,
      new (std::nothrow) CheckThreatConditionAllocator);
  ConditionFactory::GetInstance()->Register(CheckVarCondition::kTypeName_,
      new (std::nothrow) CheckVarConditionAllocator);
  ConditionFactory::GetInstance()->Register(TargetExistCondition::kTypeName_,
      new (std::nothrow) TargetExistConditionAllocator);

  /////////////////////////
  // action
  ActionFactory::GetInstance()->Register(AddBuffAction::kTypeName_,
      new (std::nothrow) AddBuffActionAllocator);
  ActionFactory::GetInstance()->Register(ChangeAttrAction::kTypeName_,
      new (std::nothrow) ChangeAttrActionAllocator);
  ActionFactory::GetInstance()->Register(ChooseTargetAction::kTypeName_,
      new (std::nothrow) ChooseTargetActionAllocator);
  ActionFactory::GetInstance()->Register(ClearTargetAction::kTypeName_,
      new (std::nothrow) ClearTargetActionAllocator);
  ActionFactory::GetInstance()->Register(DeleteMeAction::kTypeName_,
      new (std::nothrow) DeleteMeActionAllocator);
  ActionFactory::GetInstance()->Register(GoBackAction::kTypeName_,
      new (std::nothrow) GoBackActionAllocator);
  ActionFactory::GetInstance()->Register(LogAction::kTypeName_,
      new (std::nothrow) LogActionAllocator);
  ActionFactory::GetInstance()->Register(MoveTargetAction::kTypeName_,
      new (std::nothrow) MoveTargetActionAllocator);
  ActionFactory::GetInstance()->Register(RandomMoveAction::kTypeName_,
      new (std::nothrow) RandomMoveActionAllocator);
  ActionFactory::GetInstance()->Register(RemoveTargetAction::kTypeName_,
      new (std::nothrow) RemoveTargetActionAllocator);
  ActionFactory::GetInstance()->Register(ResetIntervalSecondsAction::kTypeName_,
      new (std::nothrow) ResetIntervalSecondsActionAllocator);
  ActionFactory::GetInstance()->Register(SimpleMoveTargetAction::kTypeName_,
      new (std::nothrow) SimpleMoveTargetActionAllocator);
  ActionFactory::GetInstance()->Register(SearchTargetAction::kTypeName_,
      new (std::nothrow) SearchTargetActionAllocator);
  ActionFactory::GetInstance()->Register(SetBackStateAction::kTypeName_,
      new (std::nothrow) SetBackStateActionAllocator);
  ActionFactory::GetInstance()->Register(SetVarAction::kTypeName_,
      new (std::nothrow) SetVarActionAllocator);
  ActionFactory::GetInstance()->Register(SimpleUseSkillAction::kTypeName_,
      new (std::nothrow) SimpleUseSkillActionAllocator);
  ActionFactory::GetInstance()->Register(SummonNpcAction::kTypeName_,
      new (std::nothrow) SummonNpcActionAllocator);

  /////////////////////////
  // decision
  DecisionFactory::GetInstance()->Register("selector",
      new (std::nothrow) SelectorDecisionAllocator);
  DecisionFactory::GetInstance()->Register("sequence",
      new (std::nothrow) SequenceDecisionAllocator);
}

void BehaviourTreeManager::CancelRegistAllocator() {
  /////////////////////////
  // condition
  delete ConditionFactory::GetInstance()->GetAllocator(ActivityRangeCondition::kTypeName_);
  ConditionFactory::GetInstance()->CancelRegister(ActivityRangeCondition::kTypeName_);

  delete ConditionFactory::GetInstance()->GetAllocator(CheckAttackDistanceCondition::kTypeName_);
  ConditionFactory::GetInstance()->CancelRegister(CheckAttackDistanceCondition::kTypeName_);

  delete ConditionFactory::GetInstance()->GetAllocator(CheckAttrCondition::kTypeName_);
  ConditionFactory::GetInstance()->CancelRegister(CheckAttrCondition::kTypeName_);

  delete ConditionFactory::GetInstance()->GetAllocator(CheckBackStateCondition::kTypeName_);
  ConditionFactory::GetInstance()->CancelRegister(CheckBackStateCondition::kTypeName_);

  delete ConditionFactory::GetInstance()->GetAllocator(CheckBirthPlaceCondition::kTypeName_);
  ConditionFactory::GetInstance()->CancelRegister(CheckBirthPlaceCondition::kTypeName_);

  delete ConditionFactory::GetInstance()->GetAllocator(CheckDistanceCondition::kTypeName_);
  ConditionFactory::GetInstance()->CancelRegister(CheckDistanceCondition::kTypeName_);

  delete ConditionFactory::GetInstance()->GetAllocator(CheckIntervalSecondsCondition::kTypeName_);
  ConditionFactory::GetInstance()->CancelRegister(CheckIntervalSecondsCondition::kTypeName_);

  delete ConditionFactory::GetInstance()->GetAllocator(CheckThreatCondition::kTypeName_);
  ConditionFactory::GetInstance()->CancelRegister(CheckThreatCondition::kTypeName_);

  delete ConditionFactory::GetInstance()->GetAllocator(CheckVarCondition::kTypeName_);
  ConditionFactory::GetInstance()->CancelRegister(CheckVarCondition::kTypeName_);

  delete ConditionFactory::GetInstance()->GetAllocator(TargetExistCondition::kTypeName_);;
  ConditionFactory::GetInstance()->CancelRegister(TargetExistCondition::kTypeName_);;

  /////////////////////////
  // action
  delete ActionFactory::GetInstance()->GetAllocator(AddBuffAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(AddBuffAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(ChangeAttrAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(ChangeAttrAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(ChooseTargetAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(ChooseTargetAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(ClearTargetAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(ClearTargetAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(DeleteMeAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(DeleteMeAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(GoBackAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(GoBackAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(LogAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(LogAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(MoveTargetAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(MoveTargetAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(RandomMoveAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(RandomMoveAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(RemoveTargetAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(RemoveTargetAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(ResetIntervalSecondsAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(ResetIntervalSecondsAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(SimpleMoveTargetAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(SimpleMoveTargetAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(SearchTargetAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(SearchTargetAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(SetBackStateAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(SetBackStateAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(SetVarAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(SetVarAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(SimpleUseSkillAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(SimpleUseSkillAction::kTypeName_);

  delete ActionFactory::GetInstance()->GetAllocator(SummonNpcAction::kTypeName_);
  ActionFactory::GetInstance()->CancelRegister(SummonNpcAction::kTypeName_);

  /////////////////////////
  // decision
  delete DecisionFactory::GetInstance()->GetAllocator("selector");
  DecisionFactory::GetInstance()->CancelRegister("selector");

  delete DecisionFactory::GetInstance()->GetAllocator("sequence");
  DecisionFactory::GetInstance()->CancelRegister("sequence");
}

bool BehaviourTreeManager::Parse(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载配置文件(%s) 出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 data 结点标签
  TiXmlElement *data_node = doc.FirstChildElement("data");
  if(data_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 cell 结点标签
  TiXmlElement *cell_node = data_node->FirstChildElement("cell");
  while(cell_node) {
    int id = 0;
    if(cell_node->Attribute("id", &id) == NULL) {
      global::LogError("%s:%d (%s) 找不到属性 [cell-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    const char *str = cell_node->Attribute("file");
    if(str == NULL) {
      global::LogError("%s:%d (%s) AI(%u) 找不到属性 [cell-file]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
      return false;
    }
    // 分配 BehaviourTree
    BehaviourTree *tree = new (std::nothrow) BehaviourTree();
    if(tree == NULL) {
      global::LogError("%s:%d (%s) 创建 BehaviourTree 失败",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    // 初始化 BehaviourTree
    if(tree->Initialize(id, global::ServerConfigureSingleton::GetInstance().GetConfigueDir()
          + std::string(str)) == false) {
      delete tree;
      global::LogError("%s:%d (%s) 初始化 BehaviourTree 失败",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    // 增加到管理器中
    if(this->Add(tree) == false) {
      tree->Deallocate();
      delete tree;
      global::LogError("%s:%d (%s) 初始化 BehaviourTree 失败",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    cell_node = cell_node->NextSiblingElement("cell");
  }

  return true;
}

}  // namespace ai

}  // namespace server

}  // namespace game

