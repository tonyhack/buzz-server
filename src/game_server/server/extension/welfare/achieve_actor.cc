
#include "game_server/server/extension/welfare/achieve_actor.h"

#include "database_server/client/database_client.h"
#include "game_server/server/extension/welfare/configure.h"
#include "game_server/server/extension/welfare/load_task_configure.h"
#include "game_server/server/extension/welfare/task_pool.h"
#include "game_server/server/extension/welfare/task.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

AchieveActor::AchieveActor() {}

AchieveActor::~AchieveActor() {}

bool AchieveActor::Initialize(std::vector<database::protocol::StorageAchieveField> &achieves, 
    std::vector<database::protocol::StorageAchieveFinishField> &field) {
  if(this->actor_ == NULL) {
    LOG_ERROR("成就加载时，游戏玩家对象没有找到");
    return false;
  }
  gateway::protocol::MessageAchieveInitilize message;
  // 获取配置
  const LoadTaskConfigure::AchieveMap &achieves_config = Configure::GetInstance()->GetAchievesCell();
  for(LoadTaskConfigure::AchieveMap::const_iterator itr = achieves_config.begin();
      itr != achieves_config.end(); ++itr) {
    if(this->achieves_map_.find(itr->first) != this->achieves_map_.end()) {
      LOG_ERROR("成就配置文件错误(%d)", itr->first);
      continue ;
    }
    bool finished = false; bool going = false;
    // 查找完成的成就
    for(size_t finish = 0; finish < field.size(); ++finish) {
      if(field[finish].achieve_id_ == itr->first) {
        this->finish_achieves_.push_back(AchieveFinish(field[finish].achieve_id_, field[finish].finish_time_));
        gateway::protocol::AchieveFinish message_finish;
        message_finish.__set_finish_time_(global::ToString(field[finish].finish_time_));
        message_finish.__set_id_(field[finish].achieve_id_);
        message.finishs_.push_back(message_finish);
        this->AddAttr(field[finish].achieve_id_, false);
        finished = true;
        break;
      }
    }
    if(finished == true) {
      continue ;
    }
    Task *task = TaskPool::GetInstance()->Allocate();
    if(task == NULL) {
      LOG_ERROR("成就任务(%d)内存分配失败", itr->first);
      return false;
    }
    // 查找正在进行的成就
    for(size_t add = 0; add < achieves.size(); ++add) {
      if(achieves[add].achieve_id_ == itr->first) {
        if(task->Initialize(this, itr->first, achieves[add]) == false) {
          task->Finalize();
          TaskPool::GetInstance()->Deallocate(task);
          return false;
        }
        gateway::protocol::AchieveCondition export_task;
        task->ExportConditionVar(export_task);
        message.conditions_.push_back(export_task);
        going = true;
      }
    }
    if(going == false) {
      database::protocol::StorageAchieveField temp_field;
      if(task->Initialize(this, itr->first, temp_field) == false) {
        task->Finalize();
        TaskPool::GetInstance()->Deallocate(task);
        return false;
      }
    }
    // 加入正在进行的列表
    this->achieves_map_.insert(std::make_pair(itr->first, task));
  }
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ACHIEVE_INITILIZE);

  return true;
}

bool AchieveActor::InitializeActor(GameActor *actor, database::protocol::StorageWelfareField &field) {
  if(actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return false;
  }
  this->actor_ = actor;
  this->title_id_ = field.title_id_;
  return true;
}

void AchieveActor::Finalize() {
  for(AchieveMap::iterator itr = this->achieves_map_.begin(); itr != this->achieves_map_.end(); ++itr) {
    if(itr->second !=NULL) {
      itr->second->Finalize();
      TaskPool::GetInstance()->Deallocate(itr->second);
    } else {
      LOG_ERROR("内存释放出错");
    }
  }
  this->achieves_map_.clear();
  this->finish_achieves_.clear();
  for(EventAchieveMap::iterator itr = this->events_.begin(); itr != this->events_.end(); ++itr) {
      itr->second.clear();
  }
  this->events_.clear();
  this->actor_ = NULL;
}

void AchieveActor::Save() {
  
  database::protocol::StorageAchieveSave save;
  save.__set_actor_(this->GetActorID());
  for(AchieveMap::iterator itr = this->achieves_map_.begin(); itr != this->achieves_map_.end();
      ++itr) {
    database::protocol::StorageAchieveField field;
    if(itr->second != NULL) {
      itr->second->Save(field);
      save.achieves_.push_back(field);
    }
  }
  // 完成的列表
  for(size_t i = 0; i < this->finish_achieves_.size(); ++i) {
    database::protocol::StorageAchieveFinishField temp_field;
    temp_field.__set_achieve_id_(this->finish_achieves_[i].id_);
    temp_field.__set_finish_time_(this->finish_achieves_[i].finish_time_);
    save.finish_achieves_.push_back(temp_field);
  }

  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(save,
        database::protocol::StorageType::STORAGE_ACHIEVE_SAVE, this->GetActorID()) == false) {
    LOG_ERROR("保存成就列表失败");
  }
}

void AchieveActor::SaveActor(database::protocol::StorageWelfareField &field) {
  field.__set_title_id_(this->title_id_);
}

bool AchieveActor::CheckFinishExist(core::int32 achieve_id) {
  for(size_t i = 0; this->finish_achieves_.size(); ++i) {
    if(this->finish_achieves_[i].id_ == achieve_id) {
      return true;
    }
  }
  return false;
}

bool AchieveActor::CheckFinishAchieve(core::int32 achieve_id) {
  AchieveMap::iterator itr = this->achieves_map_.find(achieve_id);
  if(itr == this->achieves_map_.end()) {
    LOG_ERROR("成就任务(%d)不在列表中", achieve_id);
    return false;
  }
  return itr->second->CheckFinish();
}

bool AchieveActor::FinishAchieve(core::int32 achieve_id) {
  // 更换列表
  AchieveMap::iterator itr = this->achieves_map_.find(achieve_id);
  if(itr == this->achieves_map_.end()) {
    LOG_ERROR("成就任务(%d)不在列表中", achieve_id);
    return false;
  }
  if(itr->second == NULL) {
    LOG_ERROR("成就任务没有找到");
    return false;
  }
  this->finish_achieves_.push_back(AchieveFinish(achieve_id, itr->second->GetFinishTime()));
  itr->second->Finalize();
  TaskPool::GetInstance()->Deallocate(itr->second);
  this->achieves_map_.erase(itr);
  this->AddAttr(achieve_id, true);
  return true;
}

void AchieveActor::OnEvent(event::EventType::type type, const void *data, size_t size) {
  EventAchieveMap::iterator event_itr = this->events_.find(type);
  if(event_itr == this->events_.end()) {
    return ;
  }
  // 处理成就
  AchieveIds &ids = event_itr->second;
  for(AchieveIds::iterator itr = ids.begin(); itr != ids.end();) {
    AchieveMap::iterator task_itr = this->achieves_map_.find(*itr);
    if(task_itr == this->achieves_map_.end()) {
      itr = ids.erase(itr);
    } else {
      // 如果成就完成了, 就删除
      if(task_itr->second && task_itr->second->CheckFinish() == false) {
        task_itr->second->OnEvent(type, data, size);
      }
      ++itr;
    }
  }
}


bool AchieveActor::AddEventDispatch(core::int32 achieve_id, 
    entity::AchieveConditionType::type type) {
  switch(type) {
    case entity::AchieveConditionType::MIN:
      break;
    case entity::AchieveConditionType::SOUL_UNLOCK:
      break;
    case entity::AchieveConditionType::SOUL_UPGRAGE_LEVEL:
      break;
    case entity::AchieveConditionType::SKILL_USE:
      this->AddEvent(event::EventType::EVENT_SKILL_USE_SKILL, achieve_id);
      break;
    case entity::AchieveConditionType::FUNCTIONALITY_OPEN:
      this->AddEvent(event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE, achieve_id);
      break;
    case entity::AchieveConditionType::SPIRIT_UPGRADE_LEVEL:
      this->AddEvent(event::EventType::EVENT_ATTR_SPIRIT_UPGRADE, achieve_id);
      break;
    case entity::AchieveConditionType::SPIRIT_COLLECT_SHAPE:
      break;
    case entity::AchieveConditionType::KILL_ROLE:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::BAG_EXPAND:
      break;
    case entity::AchieveConditionType::KILLED_SELF:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::EQUIP_STRENGTH:
      this->AddEvent(event::EventType::EVENT_ITEM_EQUIP_STRENGTH, achieve_id);
      break;
    case entity::AchieveConditionType::WEAR_EQUIP:
      this->AddEvent(event::EventType::EVENT_ITEM_WEAR_EQUIP, achieve_id);
      break;
    case entity::AchieveConditionType::GEM_OPT:
      break;
    case entity::AchieveConditionType::GEM_STRENGTH:
      break;
    case entity::AchieveConditionType::RELIVE_WAY:
      this->AddEvent(event::EventType::EVENT_ACTOR_RELIVE, achieve_id);
      break;
    case entity::AchieveConditionType::VOCATION_SET:
      break;
    case entity::AchieveConditionType::SOUL_SCHEME:
      this->AddEvent(event::EventType::EVENT_SOUL_SET, achieve_id);
      break;
    case entity::AchieveConditionType::SWORD_UPGRADE:
      break;
    case entity::AchieveConditionType::SWORD_STRENGTH:
      break;
    case entity::AchieveConditionType::HOMELAND_BUILDING_UPGRADE:
      break;
    case entity::AchieveConditionType::HOMELAND_REVENUE:
      break;
    case entity::AchieveConditionType::HOMELAND_USE:
      break;
    case entity::AchieveConditionType::HOMELAND_WATER:
      break;
    case entity::AchieveConditionType::HOMELAND_OPT:
      break;
    case entity::AchieveConditionType::HOMELAND_ATTACK_NUM:
      break;
    case entity::AchieveConditionType::CORPS_OPT:
      break;
    case entity::AchieveConditionType::CORPS_JOB:
      break;
    case entity::AchieveConditionType::CORPS_GET_HOLY_CITY:
      break;
    case entity::AchieveConditionType::TO_SENCE:
      this->AddEvent(event::EventType::EVENT_SCENE_JUMP_MAP, achieve_id);
      break;
    case entity::AchieveConditionType::NOLIBITY_GET_LEVEL:
      break;
    case entity::AchieveConditionType::USE_ITEM:
      this->AddEvent(event::EventType::EVENT_ITEM_USE, achieve_id);
      break;
    case entity::AchieveConditionType::RESOURCE:
      this->AddEvent(event::EventType::EVENT_ACTOR_RESOURCE_CHANGE, achieve_id);
      break;
    case entity::AchieveConditionType::ACTOR_GET_VALUE:
      break;
    case entity::AchieveConditionType::PASS_STORY_COPY:
      break;
    case entity::AchieveConditionType::SELF_KILLED_SKILL:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::KILLED_SELF_ACTOR_FIGHTING:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::KILLED_SELF_ACTOR_LEVEL:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::KILLED_SELF_ACTOR_APPEAR:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::KILL_ACTOR_FIGHTING:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::KILL_ACTOR_LEVEL:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::KILL_ACTOR_APPEAR:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::KILL_ACTOR_SEX:
      this->AddEvent(event::EventType::EVENT_ROLE_KILLED, achieve_id);
      break;
    case entity::AchieveConditionType::VIP_STATUS:
      this->AddEvent(event::EventType::EVENT_ACTOR_VIP_CHANGE, achieve_id);
      break;
    case entity::AchieveConditionType::MAX:
      break;
  }
  return true;
}

void AchieveActor::AddEvent(event::EventType::type type, core::int32 achieve_id) {
  EventAchieveMap::iterator itr = this->events_.find(type);
  if(itr == this->events_.end()) {
    AchieveIds tempVecs;
    tempVecs.push_back(achieve_id);
    this->events_.insert(std::make_pair(type, tempVecs));
  } else {
    for(size_t i = 0; i < itr->second.size(); ++i) {
      if(itr->second[i] == achieve_id) {
        return ;
      }
    }
    itr->second.push_back(achieve_id);
  }
}

void AchieveActor::AddAttr(core::int32 achieve_id, bool sync) {
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return ;
  }

  const AchieveTaskCell *cell = Configure::GetInstance()->GetAchieveCell(achieve_id);
  if(cell == NULL) {
    LOG_ERROR("成就(%d)配置没有找到", achieve_id);
    return ;
  }

  GameActor *game_actor = this->actor_;
  game_actor->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE)
      + cell->physique_, sync);
  game_actor->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE)
      + cell->strength_, sync);
  game_actor->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE)
      + cell->intelligence_, sync);
  game_actor->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE)
      + cell->agile_, sync);
  game_actor->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
      game_actor->GetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE)
      + cell->spirit_, sync);
}

}  // namespace welfare

}  // namespace server

}  // namespace game
