//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-07 20:18:02.
// File name: black_board.h
//
// Description:
// Define class BlackBoard.
//

#ifndef __GAME__SERVER__AI__BLACK__BOARD__H
#define __GAME__SERVER__AI__BLACK__BOARD__H

#include <map>

#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"
#include "entity/direction_types.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension/ai/behaviour_node.h"
#include "game_server/server/extension/ai/threat.h"
#include "game_server/server/extension/ai/variables.h"

namespace game {

namespace server {

namespace ai {

class BlackBoard : public core::Noncopyable {
  typedef std::map<core::uint32, size_t> SelectorLastChildMap;

  public:
  BlackBoard();
  ~BlackBoard();

  bool Initialize(GameRole *role);
  void Finalize();

  // 设置/获取 目标
  inline void SetTarget(entity::EntityType::type type,
      core::uint64 id) {
    if (this->GetForceLockTarget() == false) {
      this->target_type_ = type;
      this->target_id_ = id;
    }
  }
  inline void GetTarget(entity::EntityType::type &type,
      core::uint64 &id) const {
    type = this->target_type_;
    id = this->target_id_;
  }

  inline void SetForceLockTarget(bool lock) {
    this->force_lock_target_ = lock;
  }
  inline bool GetForceLockTarget() const {
    return this->force_lock_target_;
  }

  inline entity::EntityType::type GetType() const {
    if(this->role_ == NULL) {
      return entity::EntityType::TYPE_MAX;
    }
    return this->role_->GetType();
  }
  inline core::uint64 GetID() const {
    if(this->role_ == NULL) {
      return 0;
    }
    return this->role_->GetID();
  }

  inline GameRole *GetGameRole() {
    return this->role_;
  }

  inline bool CheckHasAi() const {
    return this->ai_;
  }

  // 寻路失败次数
  inline size_t GetRouteFailCount() const {
    return this->route_fail_count_;
  }

  // 检测是否位于回归状态
  inline bool CheckBackStatus() const {
    return this->back_status_;
  }
  inline void SetBackStatus(bool status) {
    this->back_status_ = status;
  }

  inline void GetTargetPosition(core::uint32 &x, core::uint32 &y) {
    x = this->target_x_;
    y = this->target_y_;
  }

  inline Variables &GetVariables() {
    return this->variables_;
  }

  inline core::int32 GetDynamicAi() const {
    return this->dynamic_ai_;
  }

  inline void SetDynamicAi(core::int32 ai) {
    this->dynamic_ai_ = ai;
  }

  void Running();

  // 进入/离开 AI控制
  void OnEnter();
  void OnQuit();

  // 增加/减少 周围敌人数量
  void AddEnemyCount();
  void SubEnemyCount();

  // 检测是否可移动
  bool CheckMove() const;

  // 检测是否可以随机移动
  bool CheckRandomMove(core::uint32 interval, bool move_stamp = false);

  // 移动
  bool Move(entity::DirectionType::type type);

  // 检测是否可以搜索目标
  bool CheckSearchTarget(bool flag = false);

  // 检测是否可以使用技能
  bool CheckUseSkill(bool flag = false);

  // 移动到目标
  bool MoveTarget(core::int32 radius);

  // 回到出生点
  bool MoveBack(core::int32 radius);

  void ClearPaths() {
    this->path_index_ = 0;
    this->path_size_ = 0;
  }

  // 设置行为树中选择决策结点的上次选中子树的索引
  inline void SetSelectorLastChild(core::uint32 selector_id,
      size_t last_child_index) {
    this->selector_last_childs_[selector_id] = last_child_index;
  }

  // 获取行为树中选择决策结点的上次选中子树的索引
  inline size_t &GetSelectorLastChild(core::uint32 selector_id) {
    return this->selector_last_childs_[selector_id];
  }

  // 得到怪物仇恨列表
  inline Threat& GetThreat() {
    return threat_;
  }

 private:
  // 直接跳转
  bool Jump(core::uint32 x, core::uint32 y);

  // 寻路
  bool Routing(core::uint32 x, core::uint32 y, core::int32 radius);

  GameRole *role_;

  // 目标
  entity::EntityType::type target_type_;
  core::uint64 target_id_;

  // 强制锁定目标
  bool force_lock_target_;

  // 目标坐标
  core::uint32 target_x_;
  core::uint32 target_y_;

  // 最大路径个数
  static const core::uint32 kMaxPathSize_ = 64;

  // 寻路点
  entity::DirectionType::type paths_[kMaxPathSize_];

  // 当前路径个数
  core::uint32 path_size_;

  // 当前路径
  core::uint32 path_index_;

  // 敌人计数(周围)
  core::uint32 enemy_count_;

  // 上次移动时间
  core::TimestampMillisecond last_move_time_;

  // 上次寻路时间
  core::TimestampMillisecond last_route_time_;

  // 最小寻路间隔
  static const core::uint32 kMinRouteInterval_ = 500;

  // 上次搜索目标时间
  core::TimestampMillisecond last_search_target_time_;

  // 最小搜索目标的间隔时间
  static const core::uint32 kMinSearchTargetInterval_ = 2000;

  // 上次使用技能时间
  core::TimestampMillisecond last_use_skill_time_;

  // 最小使用技能的间隔时间
  static const core::uint32 kMinUseSkillInterval_ = 1000;

  // 是否在运行AI
  bool ai_running_;

  // 是否有AI
  bool ai_;

  // 寻路失败次数
  size_t route_fail_count_;

  // 回归状态
  bool back_status_;

  // 用于记录行为树中选择决策结点的上次选中子树的索引
  SelectorLastChildMap selector_last_childs_;

  // 怪物仇恨值
  Threat threat_;

  // 变量表
  Variables variables_;

  // 动态AI
  core::int32 dynamic_ai_;

  static const size_t kMaxRouteFailCount_ = 5;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__BLACK__BOARD__H

