//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 09:52:47.
// File name: black_board.cc
//
// Description:
// Define class BlackBoard.
//

#include "game_server/server/extension/ai/black_board.h"

#include "entity/role_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/ai/ai_controller.h"
#include "game_server/server/extension/ai/behaviour_tree.h"
#include "game_server/server/extension/ai/behaviour_tree_manager.h"
#include "game_server/server/extension/ai/facade_request.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

BlackBoard::BlackBoard() : role_(NULL), target_x_(0), target_y_(0),
  path_size_(0), path_index_(0), enemy_count_(0), ai_running_(false),
  ai_(true), back_status_(true) {}
BlackBoard::~BlackBoard() {}

bool BlackBoard::Initialize(GameRole *role) {
  if(role == NULL) {
    global::LogError("%s:%d (%s) 参数 role 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->role_ = role;
  this->enemy_count_ = 0;
  this->ai_running_ = false;
  this->back_status_ = false;
  this->path_index_ = 0;
  this->path_size_ = 0;
  this->target_type_ = entity::EntityType::TYPE_MAX;
  this->target_id_ = 0;
  this->force_lock_target_ = false;
  this->dynamic_ai_ = 0;

  if(this->threat_.Initialize(this) == false) {
    LOG_ERROR("初始化 Threat 失败");
    return false;
  }
  if(this->variables_.Initialize() == false) {
    LOG_ERROR("初始化 Variables 失败");
    return false;
  }

  this->last_move_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  this->last_search_target_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  this->last_route_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());

  if(this->GetType() == entity::EntityType::TYPE_NPC) {
    const NpcCell *npc_cell =
      game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
          ((GameNpc *)this->role_)->GetTemplateID());
    if(npc_cell == NULL) {
      global::LogError("%s:%d (%s) npc(%u) 在Npc模板不存在",
          __FILE__, __LINE__, __FUNCTION__, ((GameNpc *)this->role_)->GetTemplateID());
      return false;
    }
    if(npc_cell->ai_ == 0) {
      this->ai_ = false;
    } else {
      BehaviourTree *tree = BehaviourTreeManager::GetInstance()->Get(npc_cell->ai_);
      if(tree == NULL) {
        global::LogError("%s:%d (%s) Npc的行为树AI(%u, %u) 不存在",
            __FILE__, __LINE__, __FUNCTION__, ((GameNpc *)this->role_)->GetTemplateID(),
            npc_cell->ai_);
        return false;
      }
      this->ai_ = true;
    }
  }
  
  return true;
}

void BlackBoard::Finalize() {
  this->selector_last_childs_.clear();
  this->ClearPaths();

  this->threat_.Finalize();
  this->variables_.Finalize();

  this->role_ = NULL;
}

void BlackBoard::Running() {
  if(this->GetType() == entity::EntityType::TYPE_NPC) {
    const NpcCell *npc_cell =
      game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
          ((GameNpc *)this->role_)->GetTemplateID());
    if(npc_cell == NULL) {
      global::LogError("%s:%d (%s) npc(%u) 在Npc模板不存在",
          __FILE__, __LINE__, __FUNCTION__, ((GameNpc *)this->role_)->GetTemplateID());
      return ;
    }
    BehaviourTree *tree = NULL;
    if (this->dynamic_ai_ != 0) {
      tree = BehaviourTreeManager::GetInstance()->Get(this->dynamic_ai_);
    } else {
      tree = BehaviourTreeManager::GetInstance()->Get(npc_cell->ai_);
    }
    if(tree == NULL) {
      LOG_ERROR("获取 BehaviourTree(%d,%d) 失败", this->dynamic_ai_, npc_cell->ai_);
      return;
    }
    tree->Running(this);
  }
}

void BlackBoard::OnEnter() {}

void BlackBoard::OnQuit() {
  this->SetTarget(entity::EntityType::TYPE_MAX, 0);
}

void BlackBoard::AddEnemyCount() {
  if(this->role_ == NULL) {
    return ;
  }
  // 死亡状态不加入AI控制器
  if(this->role_->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
    return ;
  }
  if(this->CheckHasAi() == false) {
    return ;
  }
  if(this->ai_running_ == false) {
    if(AiController::GetInstance()->AddBlackBoard(this) == false) {
      global::LogError("%s:%d (%s) 加入 Ai控制器 失败", 
          __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    this->ai_running_ = true;
  }
  ++this->enemy_count_;
}

void BlackBoard::SubEnemyCount() {
  if(this->CheckHasAi() == false) {
    return ;
  }
  if(this->enemy_count_ <= 0) {
    this->enemy_count_ = 0;
    return ;
  }
  --this->enemy_count_;
  if(this->enemy_count_ == 0) {
    AiController::GetInstance()->RemoveBlackBoard(this);
    this->ai_running_ = false;
  }
}

bool BlackBoard::CheckMove() const {
  if(this->role_ == NULL) {
    return false;
  }
  if(this->role_->CheckCommonStatus(entity::RoleCommonStatus::FORBID_MOVE)) {
    return false;
  }
  core::uint64 distance = this->last_move_time_.Distance(core::TimestampMillisecond(
        GlobalTimeTick::GetInstance()->TimeTick()));
  if(distance < (core::uint32)(1000 * game::server::Configure::kGridPixelWidth_ /
        this->role_->GetAttribute(entity::RoleAoiFields::MOVE_SPEED))) {
    return false;
  }

  return true;
}

bool BlackBoard::CheckRandomMove(core::uint32 interval, bool move_stamp) {
  if(this->role_ == NULL) {
    return false;
  }

  core::TimestampMillisecond stamp_now(GlobalTimeTick::GetInstance()->TimeTick());
  core::uint64 distance = this->last_move_time_.Distance(stamp_now);

  if(distance < interval) {
    return false;
  }

  if(move_stamp) {
    this->last_move_time_ = stamp_now;
  }

  return true;
}

bool BlackBoard::Move(entity::DirectionType::type type) {
  this->last_move_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());

  if(this->role_ == NULL) {
    return false;
  }

  request::RequestSceneRoleMove request;
  request.__set_type_(this->role_->GetType());
  request.__set_id_(this->role_->GetID());
  request.__set_dir_(type);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_ROLE_MOVE, &request, sizeof(request)) == -1) {
    global::LogError("%s:%d (%s) 请求角色(%u, %lu)移动失败",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(), this->role_->GetID());
    return false;
  }

  if(request.result_ == request::MoveResultType::SUCCESS) {
   // global::LogDebug("AI角色(%u,%lu) 移动到(%u,%u)", this->role_->GetType(),
   //     this->role_->GetID(), this->role_->GetPosx(), this->role_->GetPosy());
   return true;
  } else {
    global::LogDebug("AI角色(%u,%lu) 移动失败", this->role_->GetType(),
        this->role_->GetID());
    return false;
  }
}

bool BlackBoard::CheckSearchTarget(bool flag) {
  core::uint64 distance = this->last_search_target_time_.Distance(core::TimestampMillisecond(
        GlobalTimeTick::GetInstance()->TimeTick()));
  if(distance < BlackBoard::kMinSearchTargetInterval_) {
    return false;
  }
  if(flag == true) {
    this->last_search_target_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  }
  return true;
}


bool BlackBoard::CheckUseSkill(bool flag) {
  core::uint64 distance = this->last_use_skill_time_.Distance(
    core::TimestampMillisecond(GlobalTimeTick::GetInstance()->TimeTick()));
  if(distance < BlackBoard::kMinUseSkillInterval_) {
    return false;
  }
  if(flag == true) {
    this->last_use_skill_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  }
  return true;
}

bool BlackBoard::MoveTarget(core::int32 radius) {
  if(this->role_ == NULL) {
    return false;
  }

  GameRole *role = FacadeRequest::GetInstance()->GetRole(this->target_type_, this->target_id_);
  if(role == NULL) {
    return false;
  }

  core::uint32 x = 0, y = 0;
  role->GetPosition(x, y);

  if(this->Routing(x, y, radius) == false) {
    return false;
  }

  if(this->path_index_ < this->path_size_) {
    // 移动一步
    if(this->Move(this->paths_[this->path_index_++]) == false) {
      // 移动失败时，清除路点
      this->path_size_ = 0;
      this->path_index_ = 0;
    }
  } else {
    this->path_size_ = 0;
    this->path_index_ = 0;
  }

  return true;
}

bool BlackBoard::MoveBack(core::int32 radius) {
  if(this->role_ == NULL || this->role_->GetType() != entity::EntityType::TYPE_NPC) {
    return false;
  }

  GameNpc *npc = (GameNpc *)this->role_;

  // 得到出生点
  core::uint32 x = 0, y = 0;
  npc->GetBornPosition(x, y);

  // 寻路
  if(this->Routing(x, y, radius) == false) {
    // 寻路失败瞬移
    this->Jump(x, y);
    return true;
  }

  if(this->path_index_ < this->path_size_) {
    // 移动一步
    if(this->Move(this->paths_[this->path_index_++]) == false) {
      // 移动失败时，清除路点
      this->path_size_ = 0;
      this->path_index_ = 0;
    }
  } else {
    this->path_size_ = 0;
    this->path_index_ = 0;
  }

  return true;
}

bool BlackBoard::Jump(core::uint32 x, core::uint32 y) {
  if(this->role_ == NULL) {
    return false;
  }

  return FacadeRequest::GetInstance()->SceneRelocation(this->role_->GetType(),
      this->role_->GetID(), x, y);
}

bool BlackBoard::Routing(core::uint32 x, core::uint32 y, core::int32 radius) {
  if(this->role_ == NULL) {
    return false;
  }

  // 如果有路径，目标坐标没有变，才直接成功
  if(this->path_size_ == 0 || x != this->target_x_ || y != this->target_y_) {
    // 随机一个间隔时间
    core::uint32 rand_interval =
      ExtensionManager::GetInstance()->GetRandom().Random(BlackBoard::kMinRouteInterval_);
    // 寻路间隔
    core::uint64 distance = this->last_route_time_.Distance(core::TimestampMillisecond(
          GlobalTimeTick::GetInstance()->TimeTick()));
    if(distance < BlackBoard::kMinRouteInterval_ + rand_interval) {
      return true;
    }

    this->last_route_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
    this->target_x_ = x;
    this->target_y_ = y;
    // 寻路
    if(FacadeRequest::GetInstance()->Route(this->role_->GetPosx(), this->role_->GetPosy(),
          x, y, this->role_->GetScene(), this->paths_, (size_t &)this->path_size_,
          (size_t)kMaxPathSize_, radius) == false) {
      if(++this->route_fail_count_ >= BlackBoard::kMaxRouteFailCount_) {
        if(this->target_type_ == entity::EntityType::TYPE_ACTOR) {
          this->GetThreat().Remove(this->target_id_);
        }
        this->SetTarget(entity::EntityType::TYPE_MAX, 0);
        this->route_fail_count_ = 0;
      }
      return false;
    }
    this->path_index_ = 0;
  }

  this->route_fail_count_ = 0;
  return true;
}

}  // namespace ai

}  // namespace server

}  // namespace game

