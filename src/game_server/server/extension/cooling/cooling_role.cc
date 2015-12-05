//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 22:49:22.
// File name: cooling_role.cc
//
// Description:
// Define class CoolingRole.
//

#include "game_server/server/extension/cooling/cooling_role.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_cooling_save_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_role.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/cooling/configure.h"
#include "game_server/server/extension/cooling/cooling.h"
#include "game_server/server/extension/cooling/cooling_configure.h"
#include "game_server/server/extension/cooling/cooling_group.h"
#include "game_server/server/extension/cooling/cooling_group_pool.h"
#include "game_server/server/extension/cooling/cooling_pool.h"
#include "gateway_server/protocol/gateway_cooling_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace cooling {

CoolingRole::CoolingRole() : role_(NULL) {}

CoolingRole::~CoolingRole() {}

bool CoolingRole::Initialize(GameRole *role, const CoolingVector &coolings) {
  if(role == NULL) {
    global::LogError("%s:%d (%s) 参数 role 为空，初始化冷却 Role 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->role_ = role;

  // 冷却开启
  CoolingVector::const_iterator iterator = coolings.begin();
  for(; iterator != coolings.end(); ++iterator) {
    this->StartCooling(iterator->cooling_id_, iterator->cooling_time_);
  }

  this->Synchronize();

  return true;
}

void CoolingRole::Finalize() {
  // 回收冷却表中的所有对象
  while(this->coolings_.empty() == false) {
    CoolingHashMap::iterator iterator = this->coolings_.begin();
    if(iterator != this->coolings_.end()) {
      if(iterator->second) {
        iterator->second->Finalize();
        CoolingPool::GetInstance()->Deallocate(iterator->second);
        this->coolings_.erase(iterator);
      }
    }
  }

  // 回收冷却组表中的所有对象
  while(this->cooling_groups_.empty() == false) {
    CoolingGroupMap::iterator iterator = this->cooling_groups_.begin();
    if(iterator != this->cooling_groups_.end()) {
      if(iterator->second) {
        iterator->second->Finalize();
        CoolingGroupPool::GetInstance()->Deallocate(iterator->second);
        this->cooling_groups_.erase(iterator);
      }
    }
  }

  this->role_ = NULL;
}

void CoolingRole::StartCooling(core::uint32 id, core::int64 elapse,
    core::int64 add_value, core::int32 add_percent) {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) role_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到冷却配置
  const CoolingCell *configure =
    Configure::GetInstance()->GetCoolingCell(id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 冷却ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), id);
    return ;
  }

  Cooling *cooling = NULL;

  // 得到冷却对象
  CoolingHashMap::iterator iterator1 = this->coolings_.find(id);
  if(iterator1 == this->coolings_.end()) {
    // 从对象池中分配冷却对象
    cooling = CoolingPool::GetInstance()->Allocate();
    if(cooling == NULL) {
      global::LogError("%s:%d (%s) Role(%d, %lu) 分配冷却对象(%u) 失败",
          __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
          this->role_->GetID(), id);
      return ;
    }
    // 初始化冷却对象
    if(cooling->Initialize(id) == false) {
      CoolingPool::GetInstance()->Deallocate(cooling);
      global::LogError("%s:%d (%s) Role(%d, %lu) 初始化冷却对象(%u) 失败",
          __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), id);
      return ;
    }
    // 加入冷却表中
    if(this->AddCooling(id, cooling) == false) {
      CoolingPool::GetInstance()->Deallocate(cooling);
      global::LogError("%s:%d (%s) Role(%d, %lu) 加入冷却对象(%u) 到 coolings_ 失败",
          __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), id);
      return ;
    }
  } else {
    cooling = iterator1->second;
  }

  // 指针是否合法
  if(cooling == NULL) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 冷却ID(%u) 对应的对象为空",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), id);
    return ;
  }

  CoolingGroup *group = NULL;

  // 得到冷却组对象
  CoolingGroupMap::iterator iterator2 = this->cooling_groups_.find(configure->group_);
  if(iterator2 == this->cooling_groups_.end()) {
    // 从对象池中分配冷却组对象
    group = CoolingGroupPool::GetInstance()->Allocate();
    if(group == NULL) {
      global::LogError("%s:%d (%s) Role(%d, %lu) 分配冷却对象(%u) 失败",
          __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
          this->role_->GetID(), configure->group_);
      return ;
    }
    // 初始化冷却组对象
    if(group->Initialize(configure->group_) == false) {
      CoolingGroupPool::GetInstance()->Deallocate(group);
      global::LogError("%s:%d (%s) Role(%d, %lu) 初始化冷却组对象(%u) 失败",
          __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), configure->group_);
      return ;
    }
    // 加入冷却组表中
    if(this->AddCoolingGroup(configure->group_, group) == false) {
      group->Finalize();
      CoolingGroupPool::GetInstance()->Deallocate(group);
      global::LogError("%s:%d (%s) Role(%d, %lu) 加入冷却组对象(%u) 到 groups_ 失败",
          __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), configure->group_);
      return ;
    }
  } else {
    group = iterator2->second;
  }
  
  // 指针是否合法
  if(group == NULL) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 冷却组ID(%u) 对应的对象为空",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), configure->group_);
    return ;
  }

  // 冷却组配置
  const GroupCell *cell = Configure::GetInstance()->GetGroupCell(configure->group_);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 冷却组(%u)配置不存在",
        __FILE__, __LINE__, __FUNCTION__, configure->group_);
    return ;
  }

  // 冷却对象开始冷却
  cooling->Start(elapse, add_value, add_percent);

  // 冷却组对象同步冷却
  if(elapse < cell->time_) {
    core::TimestampMillisecond stamp;
    stamp.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
    stamp = stamp.GetStamp() + cell->time_ - elapse;
    if(group->GetEndTime().GetStamp() < stamp.GetStamp()) {
      group->Start(stamp);
    }
  }
}

void CoolingRole::EndCooling(core::uint32 id) {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) role_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到冷却配置
  const CoolingCell *configure =
    Configure::GetInstance()->GetCoolingCell(id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 冷却ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), id);
    return ;
  }

  // 获取冷却对象
  Cooling *cooling = NULL;
  CoolingHashMap::iterator iterator1 = this->coolings_.find(id);
  if(iterator1 == this->coolings_.end()) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 结束冷却时找不到冷却对象(%d)",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), id);
    return ;
  }

  cooling = iterator1->second;

  if(cooling == NULL) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 冷却对象(%d) 为空",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), id);
    return ;
  }

  // 结束冷却
  cooling->End();

  // 获取冷却组对象
  CoolingGroup *group = NULL;
  CoolingGroupMap::iterator iterator2 = this->cooling_groups_.find(id);
  if(iterator2 == this->cooling_groups_.end()) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 结束冷却时找不到冷却组对象(%d)",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), configure->group_);
    return ;
  }

  group = iterator2->second;

  if(group == NULL) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 冷却组对象(%d) 为空",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), configure->group_);
    return ;
  }

  // 结束冷却组冷却
  group->End();
}

bool CoolingRole::CheckCoolingEnd(core::uint32 id) const {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) role_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 检测冷却ID
  CoolingHashMap::const_iterator iterator = this->coolings_.find(id);
  if(iterator != this->coolings_.end()) {
    if(iterator->second && iterator->second->CheckEnd() == false) {
      return false;
    }
  }

  // 得到冷却配置
  const CoolingCell *configure =
    Configure::GetInstance()->GetCoolingCell(id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 冷却ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID(), id);
    return false;
  }

  // 检测冷却组ID
  CoolingGroupMap::const_iterator iterator2 =
    this->cooling_groups_.find(configure->group_);
  if(iterator2 != this->cooling_groups_.end()) {
    if(iterator2->second && iterator2->second->CheckEnd() == false) {
      return false;
    }
  }

  return true;
}

void CoolingRole::Save() {
  // TODO: 优化存储效率，设置一个 save 标识，开始冷却时，则设置这个标识，
  // 存储时判断是否冷却完成，冷却完成则重置这个标识，当需要存储时，判断
  // 这个标识是否被设置，如果设置则保存，如果没有设置(重置)，直接返回。
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) role_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(this->role_->GetType() != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  // Now stamp.
  core::TimestampMillisecond now;
  now.Stamp(GlobalTimeTick::GetInstance()->TimeTick());

  // 填充消息
  size_t number = 0;
  database::protocol::StorageCoolingSaveRequest request;
  request.__set_id_(this->role_->GetID());
  CoolingHashMap::iterator iterator = this->coolings_.begin();
  for(; iterator != this->coolings_.end(); ++iterator) {
    if(number++ >= CoolingRole::max_save_number_) {
      // 发送消息
      if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
            request, database::protocol::StorageType::STORAGE_COOLING_SAVE,
            this->role_->GetID()) == false) {
        global::LogError("%s:%d (%s) Role(%d, %lu) 保存冷却信息失败",
            __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
            this->role_->GetID());
        return ;
      }
      request.fields_.clear();
      request.__isset.fields_ = false;
      number = 0;
    }
    if(iterator->second) {
      database::protocol::StorageCoolingField field;
      field.__set_cooling_id_(iterator->first);
      field.__set_cooling_time_(iterator->second->GetElapseTime());
      request.fields_.push_back(field);
      request.__isset.fields_ = true;
    }
  }

  // 发送消息
  if(request.__isset.fields_ == true &&
      database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_COOLING_SAVE,
        this->role_->GetID()) == false) {
    global::LogError("%s:%d (%s) Role(%d, %lu) 保存冷却信息失败",
        __FILE__, __LINE__, __FUNCTION__, this->role_->GetType(),
        this->role_->GetID());
    return ;
  }

  global::LogInfo("玩家(%lu) 保存冷却信息", this->role_->GetID());
}

void CoolingRole::Synchronize() {
  if(this->role_ && this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
    gateway::protocol::MessageCoolingInitialize message;
    entity::CoolingFields field;
    CoolingHashMap::const_iterator iterator = this->coolings_.begin();
    for(; iterator != this->coolings_.end(); ++iterator) {
      if(iterator->second && iterator->second->GetElapseTime() > 0) {
        field.__set_id_(iterator->first);
        field.__set_elapse_time_(global::ToString(iterator->second->GetElapseTime()));
        message.coolings_.push_back(field);
      }
    }
    ((GameActor *)(this->role_))->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_COOLING_INITIALIZE);
    LOG_DEBUG("CoolingRole::Synchronize");
  }
}

bool CoolingRole::AddCooling(core::uint32 id, Cooling *cooling) {
  if(cooling == NULL) {
    global::LogError("%s:%d (%s) 参数 cooling 为空，增加冷却对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  CoolingHashMap::const_iterator iterator = this->coolings_.find(id);
  if(iterator != this->coolings_.end()) {
    global::LogError("%s:%d (%s) 增加冷却对象(%u)失败，已存在",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  this->coolings_.insert(std::make_pair(id, cooling));

  return true;
}

Cooling *CoolingRole::RemoveCooling(core::uint32 id) {
  CoolingHashMap::iterator iterator = this->coolings_.find(id);
  if(iterator == this->coolings_.end()) {
    global::LogError("%s:%d (%s) 冷却对象(%u)不存在，移除失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }

  Cooling *cooling = iterator->second;
  this->coolings_.erase(iterator);

  return cooling;
}

bool CoolingRole::AddCoolingGroup(core::uint32 id, CoolingGroup *group) {
  if(group == NULL) {
    global::LogError("%s:%d (%s) 参数 group 为空，增加冷却组对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  CoolingGroupMap::const_iterator iterator = this->cooling_groups_.find(id);
  if(iterator != this->cooling_groups_.end()) {
    global::LogError("%s:%d (%s) 增加冷却组对象(%u)失败，已存在",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  this->cooling_groups_.insert(std::make_pair(id, group));

  return true;
}

CoolingGroup *CoolingRole::RemoveCoolingGroup(core::uint32 id) {
  CoolingGroupMap::iterator iterator = this->cooling_groups_.find(id);
  if(iterator == this->cooling_groups_.end()) {
    global::LogError("%s:%d (%s) 冷却组对象(%u)不存在，移除失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }

  CoolingGroup *group = iterator->second;
  this->cooling_groups_.erase(iterator);

  return group;
}

}  // namespace cooling

}  // namespace server

}  // namespace game

