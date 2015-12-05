//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-19 14:18:28.
// File name: npc_builder.cc
//
// Description:
// Define class NpcBuilder.
//

#include "game_server/server/extension/scene/npc_builder.h"

#include "coresh/communicator.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/game_npc_pool.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_npc_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

NpcBuilder::NpcBuilder() {}

NpcBuilder::~NpcBuilder() {}

bool NpcBuilder::Initialize(Scene *scene) {
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 参数 scene 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->scene_ = scene;

  // 服务器配置
  const ServerMapConfigure *server_map =
    Configure::GetInstance()->GetServerMapConfigure();
  if(server_map == NULL) {
    global::LogError("%s:%d (%s) 找不到服务器配置(%u)",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 地图配置
  const MapInfo *map_info = server_map->GetMapInfo(this->scene_->GetMapID());
  if(map_info == NULL) {
    global::LogError("%s:%d (%s) 找不到地图配置信息(%u)",
        __FILE__, __LINE__, __FUNCTION__, this->scene_->GetMapID());
    return false;
  }

  // npc 配置
  const MapInfo::NpcVector &npcs = map_info->GetNpcs();
  MapInfo::NpcVector::const_iterator iterator = npcs.begin();
  for(; iterator != npcs.end(); ++iterator) {
    for(core::uint32 i = 0; i < iterator->number_; ++i) {
      core::int32 x = 0;
      core::int32 y = 0;
      core::int32 result_x = 0;
      core::int32 result_y = 0;
      if(this->RandCoordinate(iterator->x_, iterator->y_, 
            iterator->width_, iterator->heigth_, result_x, result_y) == false) {
        LOG_ERROR("随机NPC坐标点错误[%d]", iterator->npc_);
        return false;
      }
      this->SummonNpc(iterator->npc_, result_x, result_y,
          iterator->dir_, 1);
    }
  }

  return true;
}

bool NpcBuilder::RandCoordinate(core::int32 x, core::int32 y, core::int32 width,
    core::int32 heigth, core::int32 &result_x, core::int32 &result_y) {
  if(width != 0) {
    result_x = ExtensionManager::GetInstance()->GetRandom().Random(width);
  }
  if(heigth != 0) {
    result_y = ExtensionManager::GetInstance()->GetRandom().Random(heigth);
  }
  result_x -= width/2;
  result_y -= heigth/2;

  result_x = x + result_x;
  if(result_x <= 0) {
    result_x = 1;
  }
  if((core::uint32)result_x >= this->scene_->GetMaxX()) {
    result_x = this->scene_->GetMaxX() - 1;
  }

  result_y = y + result_y;
  if(result_y <= 0) {
    result_y = 1;
  }
  if((core::uint32)result_y >= this->scene_->GetMaxY()) {
    result_y = this->scene_->GetMaxY() - 1;
  }

  return true;
}

void NpcBuilder::ClearBatchBorn() {
  this->npcs_.clear();
}

void NpcBuilder::Finalize() {
  this->npcs_.clear();
}

GameNpc *NpcBuilder::SummonNpc(core::uint32 template_id, core::uint32 x,
    core::uint32 y, entity::DirectionType::type dir, core::uint32 delay) {
  if(delay == 0) {
    return this->SummonNpcImmediate(template_id, x, y, dir); 
  } else {
    NpcBuildInfo info;
    info.template_id_ = template_id;
    info.x_ = x;
    info.y_ = y;
    info.dir_ = dir;
    this->AddNpc(info, delay);
    return NULL;
  }
}

bool NpcBuilder::DisappearNpc(core::uint64 id, core::uint32 disappear_delay) {
  if(disappear_delay == 0) {
    return this->DisappearNpc(id);
  } else {
    time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
    DisappearNpcInfo info;
    info.id_ = id;
    info.time_stamp_ = now + disappear_delay;
    this->disappear_npcs_.insert(info);
    return true;
  }
}

void NpcBuilder::BatchBorn() {
  size_t number = 0;
  time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
  NpcMultimap::iterator iterator = this->npcs_.begin();
  for(; iterator != this->npcs_.end();) {
    if(iterator->first > now || number >= NpcBuilder::kMaxBatchNumber) {
      return ;
    }
    this->SummonNpcImmediate(iterator->second.template_id_, iterator->second.x_,
        iterator->second.y_, iterator->second.dir_);
    this->npcs_.erase(iterator++);
    ++number;
  }
}

void NpcBuilder::BathchDisappear() {
  time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
  DisappearNpcSet::iterator iterator = this->disappear_npcs_.begin();
  for(; iterator != this->disappear_npcs_.end();) {
    if(iterator->time_stamp_ > now) {
      return ;
    }
    this->DisappearNpc(iterator->id_);
    this->disappear_npcs_.erase(iterator++);
  }
}

GameNpc *NpcBuilder::SummonNpcImmediate(core::uint32 template_id, core::uint32 x,
    core::uint32 y, entity::DirectionType::type dir) {
  if(this->scene_ == NULL) {
    global::LogError("%s:%d (%s) 场景指针 scene_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // Npc 配置
  const NpcCell *cell =
    game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(template_id);
  if(cell == NULL) {
    LOG_ERROR("找不到npc(%u)", template_id);
    return NULL;
  }
  
  // 处理NPC方向
  core::uint32 temp_dir = entity::DirectionType::NONE;
  if(dir == entity::DirectionType::NONE) {
    temp_dir = ExtensionManager::GetInstance()->GetRandom().Random(entity::DirectionType::NONE);
  } else {
    temp_dir = dir;
  }

  // 分配 npc 对象
  GameNpc *npc = GameNpcPool::GetInstance()->Allocate();
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 分配 npc对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化 npc 对象
  if(npc->Initialize(template_id, (entity::DirectionType::type)temp_dir) == false) {
    GameNpcPool::GetInstance()->Deallocate(npc);
    global::LogError("%s:%d (%s) 初始化 npc对象 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 加入 npc 管理器
  if(GameNpcManager::GetInstance()->Add(npc) == false) {
    npc->Finalize();
    GameNpcPool::GetInstance()->Deallocate(npc);
    global::LogError("%s:%d (%s) 加入 npc管理器 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  if(this->scene_->CheckNpcWalkBlock(x, y) == true) {
    if(this->scene_->GetNearNpcWalkPosition(x, y) == false) {
      global::LogError("%s:%d (%s) npc(%d) 寻找地图(%d) 坐标(%d,%d) 失败",
          __FILE__, __LINE__, __FUNCTION__, template_id, this->scene_->GetMapID(), x, y);
      npc->Finalize();
      GameNpcPool::GetInstance()->Deallocate(npc);
      return NULL;
    }
  }

  npc->SetPosition(x, y);
  // 发送创建 npc 事件
  event::EventNpcCreate event;
  event.__set_id_(npc->GetID());
  event.__set_reborn_(cell->reborn_ > 0);
  event.__set_reborn_secs_(cell->reborn_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_NPC_CREATE, npc->GetID(),
      event::ChannelType::CHANNEL_NPC, &event, sizeof(event));

  // npc 模块创建是否全部成功
  if(npc->GetLoadProgress().LoadFinish() == false) {
    global::LogError("%s:%d (%s) 加载 npc 模块失败",
        __FILE__, __LINE__, __FUNCTION__);
    this->DestoryNpcEvent(npc->GetID());
    GameNpcManager::GetInstance()->Remove(npc->GetID());
    npc->Finalize();
    GameNpcPool::GetInstance()->Deallocate(npc);
    return NULL;
  }

  // 进入场景
  if(this->scene_->AddEntity(npc) == false) {
    global::LogError("%s:%d (%s) npc(%d) 加入场景失败",
        __FILE__, __LINE__, __FUNCTION__, template_id);
    this->DestoryNpcEvent(npc->GetID());
    GameNpcManager::GetInstance()->Remove(npc->GetID());
    npc->Finalize();
    GameNpcPool::GetInstance()->Deallocate(npc);
    return NULL;
  }

  npc->SetScene(this->scene_->GetSceneID());

  global::LogDebug("[召唤NPC] npc(%d, %ld) 地图(%u) 坐标(%u,%u)创建成功",
      template_id, npc->GetID(), this->scene_->GetMapID(), x, y);

  return npc;
}

bool NpcBuilder::DisappearNpc(core::uint64 id) {
  GameNpc *npc = GameNpcManager::GetInstance()->Get(id);
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 获取 GameNpc 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(npc->GetScene());
  if(scene != NULL) {
    if(scene->RemoveEntity(npc) == false) {
      global::LogError("%s:%d (%s) GameNpc(%s) 从场景(%lu)移除失败",
          __FILE__, __LINE__, __FUNCTION__, npc->GetTemplateID(), scene->GetSceneID());
      return false;
    }
  } else {
    global::LogError("%s:%d (%s) 获取 GameNpc(%u) 所在 Scene 失败",
        __FILE__, __LINE__, __FUNCTION__, npc->GetTemplateID());
  }

  // 发送销毁事件
  this->DestoryNpcEvent(npc->GetID());

  // 从管理器中删除，还回对象池
  GameNpcManager::GetInstance()->Remove(npc->GetID());
  npc->Finalize();
  GameNpcPool::GetInstance()->Deallocate(npc);

  return true;
}

void NpcBuilder::AddNpc(const NpcBuildInfo &info, core::uint32 delay) {
  time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
  this->npcs_.insert(std::make_pair(now + delay, info));
}

void NpcBuilder::DestoryNpcEvent(core::uint64 id) {
  // 发送销毁 npc 事件
  event::EventNpcDestory event;
  event.__set_id_(id);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_NPC_DESTROY, id,
      event::ChannelType::CHANNEL_NPC, &event, sizeof(event));
}

}  // namespace scene

}  // namespace server

}  // namespace game

