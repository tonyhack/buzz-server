//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 17:08:19.
// File name: scene.cc
//
// Description:
// Define class Scene.
//

#include "game_server/server/extension/scene/scene.h"

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "entity/direction_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_entity.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_role.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/constant.h"
#include "game_server/server/extension/scene/facade_request.h"
#include "game_server/server/extension/scene/grid.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/extension/scene/scene_actor_pool.h"
#include "game_server/server/extension/scene/scene_item.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"
#include "game_server/server/extension/scene/screen.h"
#include "game_server/server/request/game_transport_request_types.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "global/channel.h"
#include "global/common_functions.h"
#include "global/game_misc.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/game_actor_protocol_types.h"

namespace game {

namespace server {

namespace scene {

static const size_t kNineScreen = 9;
static const size_t kMaxShiftScreen = 6;

static const size_t kMaxActorArraySize = 10240;
static core::uint64 actor_array1[kMaxActorArraySize];
static core::uint64 actor_array2[kMaxActorArraySize];
static GameEntity *entity_array1[kMaxActorArraySize];
static GameEntity *entity_array2[kMaxActorArraySize];

Scene::Scene() : screens_(NULL), map_id_(0), scene_id_(0),
    max_x_(0), max_y_(0), screen_max_x_(0), screen_max_y_(0), entity_builder_(this),
    map_attr_mask_(0), screen_width_(0) {}
Scene::~Scene() {}

bool Scene::Allocate(core::uint16 map_id) {
  const MapBlock *map_blocks = Configure::GetInstance()->GetMapBlock(map_id);
  if(map_blocks == NULL) {
    global::LogError("%s:%d (%s) Get map[%d] blocks failed.",
        __FILE__, __LINE__, __FUNCTION__, map_id);
    return false;
  }

  const ServerMapConfigure *server_map_configure =
    Configure::GetInstance()->GetServerMapConfigure();
  if(server_map_configure == NULL) {
    global::LogError("%s:%d (%s) 找不到配置 ServerMapConfigure",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  const MapInfo *map_info = server_map_configure->GetMapInfo(map_id);
  if(map_info == NULL) {
    global::LogError("%s:%d (%s) 找不到地图[%d]信息配置",
        __FILE__, __LINE__, __FUNCTION__, map_id);
    return false;
  }

  // 分配格子
  if(this->grids_.Allocate(map_blocks->max_x_, map_blocks->max_y_) == false) {
    global::LogError("%s:%d (%s) Allocate grids failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 初始化格子障碍信息
  size_t pos = 0;
  for(; pos < map_blocks->blocks_.size(); ++pos) {
    Grid *grid = this->grids_.Get(map_blocks->blocks_[pos].x_, map_blocks->blocks_[pos].y_);
    if(grid) {
      grid->SetBlockMask((BlockType::type)map_blocks->blocks_[pos].block_);
    }
  }

  this->max_x_ = map_blocks->max_x_;
  this->max_y_ = map_blocks->max_y_;

  if (map_info->divide_screen_ == true) {
    this->screen_width_ = kScreenWidth;
  } else {
    this->screen_width_ = this->max_x_ > this->max_y_ ? this->max_x_ : this->max_y_;
  }

  // 把地图划分为 20*20 大小的屏
  this->screen_max_x_ = map_blocks->max_x_ / this->screen_width_ +
    ((map_blocks->max_x_ % this->screen_width_) == 0 ? 0 : 1);
  this->screen_max_y_ = map_blocks->max_y_ / this->screen_width_ +
    ((map_blocks->max_y_ % this->screen_width_) == 0 ? 0 : 1);
  this->screen_length_ = this->screen_max_x_ * this->screen_max_y_;

  // 为屏分配内存
  this->screens_ = new (std::nothrow) Screen[this->screen_length_];
  if(this->screens_ == NULL) {
    global::LogError("%s:%d (%s) Allocate screen failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->map_id_ = map_id;

  return true;
}

void Scene::Deallocate() {
  delete [] this->screens_;
  this->grids_.Deallocate();
}

bool Scene::Initialize(core::uint64 scene_id, bool dynamic) {
  if(this->entity_builder_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 EntityBuilder 失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->grids_.Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize grid map failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->screens_ == NULL) {
    global::LogError("%s:%d (%s) Screen is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  size_t pos = 0;
  for(; pos < this->screen_length_; ++pos) {
    if(this->screens_[pos].Initialize() == false) {
      global::LogError("%s:%d (%s) Initialize screen failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
  }
  if(this->daynight_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize daynight failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->router_.Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize router failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->router_.SetScene(this);

  this->scene_id_ = scene_id;

  this->dynamic_ = dynamic;

  const ServerMapConfigure *server_map_configure =
    Configure::GetInstance()->GetServerMapConfigure();
  if(server_map_configure == NULL) {
    global::LogError("%s:%d (%s) 找不到配置 ServerMapConfigure",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  const MapInfo *map_info = server_map_configure->GetMapInfo(this->map_id_);
  if(map_info == NULL) {
    global::LogError("%s:%d (%s) 找不到地图[%d]信息配置",
        __FILE__, __LINE__, __FUNCTION__, this->scene_id_);
    return false;
  }
  
  this->map_attr_mask_ |= map_info->rookie_protect_ >= 1 ? 1 : 0;
  this->map_attr_mask_ |= (map_info->ban_change_pk_mode_ >= 1 ? 1 : 0) << 1; 
  this->map_attr_mask_ |= (map_info->die_protect_ >= 1 ? 1 : 0) << 2;
  this->map_attr_mask_ |= (map_info->punish_for_pk_ >= 1 ? 1 : 0) << 3;
  this->map_attr_mask_ |= (map_info->revive_stay_ >= 1 ? 1 : 0) << 4;

  this->logined_ = false;

  return true;
}

void Scene::Finalize() {
  this->scene_id_ = 0;
  size_t pos = 0;
  for(; pos < this->screen_length_; ++pos) {
    // 这里使用一份copy，因为DisappearNpc会删除Screen下容器中相应的元素
    Screen::EntitySet copy = this->screens_[pos].GetNpcs();
    Screen::EntitySet::const_iterator iterator1 = copy.begin();
    for(; iterator1 != copy.end(); ++iterator1) {
      if(*iterator1) {
        this->entity_builder_.DisappearNpc((*iterator1)->GetID(), 0);
      }
    }
    // TODO: 处理场景关闭时，玩家被踢出场景的功能
    this->screens_[pos].Finalize();
  }
  this->router_.Finalize();
  this->entity_builder_.Finalize();
  this->grids_.Finalize();
  this->daynight_.Finalize();
}

void Scene::ClearNpcs(core::int32 delay_secs, bool force_clear_reborn) {
  size_t pos = 0;
  for(; pos < this->screen_length_; ++pos) {
    // 这里使用一份copy，因为DisappearNpc会删除Screen下容器中相应的元素
    Screen::EntitySet copy = this->screens_[pos].GetNpcs();
    Screen::EntitySet::const_iterator iterator1 = copy.begin();
    for(; iterator1 != copy.end(); ++iterator1) {
      if(*iterator1) {
        this->entity_builder_.DisappearNpc((*iterator1)->GetID(), delay_secs);
      }
    }
  }

  if (force_clear_reborn == true) {
    this->entity_builder_.ClearBatchBornNpc();
  }
}

bool Scene::GetNearWalkPosition(core::uint32 &x, core::uint32 &y, core::uint32 radius) {
/*
  if(x >= this->max_x_ || y >= this->max_y_) {
    LOG_ERROR("参数 x/y 错误");
    return false;
  }
*/
  for(core::uint32 step = 0; step < radius; ++step) {
    for(core::uint32 count = 0; count <= step; ++count) {
      if((step % 2) == 0) {
        ++y;
      } else {
        --y;
      }
      if(this->CheckWalkBlock(x, y) == false) {
        return true;
      }
    }
    for(core::uint32 count = 0; count <= step; ++count) {
      if((step % 2) == 0) {
        ++x;
      } else {
        --x;
      }
      if(this->CheckWalkBlock(x, y) == false) {
        return true;
      }
    }
  }
  return false;
}

bool Scene::GetNearNpcWalkPosition(core::uint32 &x, core::uint32 &y, core::uint32 radius) {
/*
  if(x >= this->max_x_ || y >= this->max_y_) {
    LOG_ERROR("参数 x/y 错误");
    return false;
  }
*/
  for(core::uint32 step = 0; step < radius; ++step) {
    for(core::uint32 count = 0; count <= step; ++count) {
      if((step % 2) == 0) {
        ++y;
      } else {
        --y;
      }
      if(this->CheckNpcWalkBlock(x, y) == false) {
        return true;
      }
    }
    for(core::uint32 count = 0; count <= step; ++count) {
      if((step % 2) == 0) {
        ++x;
      } else {
        --x;
      }
      if(this->CheckNpcWalkBlock(x, y) == false) {
        return true;
      }
    }
  }
  return false;
}

bool Scene::AddEntity(GameEntity *entity, bool login) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加入屏
  if(this->AddScreen(entity) == false) {
    global::LogError("%s:%d (%s) 加入屏失败，实体(%d, %llu)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID());
    return false;
  }

  // 取得坐标
  core::uint32 x = 0, y = 0;
  entity->GetPosition(x, y);

  // 加入格子
  if(this->AddGrid(entity, x, y) == false) {
    global::LogError("%s:%d (%s) 加入格子失败，实体(%d, %llu)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID());
    this->RemoveScreen(entity);
    return false;
  }

  // 角色类型是玩家
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    // 分配一个场景玩家对象，初始化并加入场景的玩家管理器中
    SceneActor *actor = SceneActorPool::GetInstance()->Allocate();
    if(actor == NULL || actor->Initialize((GameActor *)entity, this) == false ||
        this->AddSceneActor(actor) == false) {
      global::LogError("%s:%d (%s) 玩家(%llu) 初始化或加入场景管理器失败",
          __FILE__, __LINE__, __FUNCTION__, entity->GetID());
      this->RemoveGrid(entity, x, y);
      this->RemoveScreen(entity);
      if(actor) {
        SceneActorPool::GetInstance()->Deallocate(actor);
      }

      // 不能死亡保护
      if(this->IsDieProtect() == false) {
        ((GameActor *)entity)->RemoveDieProtectBuff();
      }

      return false;
    }
    if(login == true) {
      // 通知客户端进入地图
      gateway::protocol::MessageActorEnterMap message;
      message.__set_map_(this->GetMapID());
      message.__set_x_(x);
      message.__set_y_(y);
      ((GameActor *)entity)->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_ACTOR_ENTER_MAP);
      // 把9屏中所有实体集同步给玩家
      this->SynchronizeCreateNineScreen((GameActor *)entity);
    }
  } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
    SceneNpc *npc = SceneNpcManager::GetInstance()->Get(entity->GetID());
    if(npc == NULL) {
      global::LogError("%s:%d (%s) 获取 NPC(%u, %lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, ((GameNpc *)entity)->GetTemplateID(),
          entity->GetID());
      return false;
    }
    // 设置场景对象
    npc->SetScene(this);
  } else if(entity->GetType() == entity::EntityType::TYPE_SCENE_ITEM) {
  }

  // 向周围9屏广播
  this->EnterSceneToNine(entity);

  // 发送角色靠近事件
  this->EnterSceneComingEvent(entity);

  return true;
}

bool Scene::RemoveEntity(GameEntity *entity) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 角色类型是玩家
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    SceneActor *actor = this->RemoveSceneActor(entity->GetID());
    if(actor == NULL) {
      global::LogError("%s:%d (%s) 删除场景玩家(%llu) 对象失败",
          __FILE__, __LINE__, __FUNCTION__, entity->GetID());
      return false;
    }
    actor->Finalize();
    SceneActorPool::GetInstance()->Deallocate(actor);
  }

  // 从屏索引删除
  if(this->RemoveScreen(entity) == false) {
    global::LogError("%s:%d (%s) 从屏删除失败，实体(%d, %llu)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID());
    return false;
  }

  // 取得坐标
  core::uint32 x = 0, y = 0;
  entity->GetPosition(x, y);

  // 从格子删除
  if(this->RemoveGrid(entity, x, y) == false) {
    global::LogError("%s:%d (%s) 从格子删除失败，实体(%d, %llu)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID());
    return false;
  }

  // 向周围9屏广播
  this->ExitSceneToNine(entity);

  // 发送远离事件
  this->ExitSceneLeavingEvent(entity);

  // 把周围9屏实体集销毁同步给玩家
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    this->SynchronizeDestoryNineScreen((GameActor *)entity);
  }

  return true;
}

bool Scene::CheckAddSceneItem(core::uint32 x, core::uint32 y) const {
  if(this->CheckPositionValid(x, y) == false) {
    return false;
  }
  const Grid *grid = this->grids_.Get(x, y);
  if(grid && grid->CanAddItem()) {
    return true;
  }
  return false;
}

bool Scene::CheckWalkBlock(core::uint32 x, core::uint32 y) const {
  if(this->CheckPositionValid(x, y) == false) {
    return true;
  }
  const Grid *grid = this->grids_.Get(x, y);
  if(grid && grid->CanWalk()) {
    return false;
  }
  return true;
}

bool Scene::CheckNpcWalkBlock(core::uint32 x, core::uint32 y) const {
  if(this->CheckPositionValid(x, y) == false) {
    return true;
  }
  const Grid *grid = this->grids_.Get(x, y);
  if(grid && grid->CanNpcWalk()) {
    return false;
  }
  return true;
}

bool Scene::CheckSafeArea(core::uint32 x, core::uint32 y) const {
  if(this->CheckPositionValid(x, y) == false) {
    return false;
  }
  const Grid *grid = this->grids_.Get(x, y);
  if(grid && grid->IsSafe()) {
    return true;
  }
  return false;
}

bool Scene::JumpMap(GameEntity *entity, core::uint16 dest_map, core::uint32 dest_x,
    core::uint32 dest_y) {
  if (entity == NULL) {
    LOG_ERROR("参数 entity 为空");
    return false;
  }

  session::protocol::MessageSceneActorJumpMapRequest request;
  request.__set_id_(entity->GetID());
  request.__set_map_(dest_map);
  request.__set_x_(dest_x);
  request.__set_y_(dest_y);

  SessionChannel::SendMessage(request,
      session::protocol::GameMessageType::MESSAGE_SCENE_ACTOR_JUMP_MAP_REQUEST);

  return true;
}

bool Scene::JumpScene(GameEntity *entity, core::uint64 dest_scene, core::uint32 dest_x,
    core::uint32 dest_y) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(entity->GetScene() != this->GetSceneID()) {
    global::LogError("%s:%d (%s) 参数 entity 不在当前地图，不能进行地图跳转",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(dest_scene);
  if(scene != NULL) {
    // 源坐标
    core::uint32 src_x = 0, src_y = 0;
    entity->GetPosition(src_x, src_y);
    // 获取本场景配置
    const ServerMapConfigure *server_map_configure =
      Configure::GetInstance()->GetServerMapConfigure();
    if(server_map_configure == NULL) {
      global::LogError("%s:%d (%s) 找不到配置 ServerMapConfigure",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    const MapInfo *map_info = server_map_configure->GetMapInfo(scene->GetMapID());
    if(map_info == NULL) {
      LOG_ERROR("找不到地图[%d]配置信息", scene->GetMapID());
      return false;
    }
    // 检测行走阻挡点
    if(entity->GetType() == entity::EntityType::TYPE_NPC && scene->CheckNpcWalkBlock(dest_x, dest_y)) {
      LOG_DEBUG("场景(%lu) 坐标(%u,%u) 阻挡", scene->GetSceneID(), dest_x, dest_y);
      // 获取最近的可行走点
      if(scene->GetNearNpcWalkPosition(dest_x, dest_y) == false) {
        LOG_ERROR("坐标x[%d]y[%d]是阻挡点", dest_x, dest_y);
        return false;
      }
    } else if(entity->GetType() != entity::EntityType::TYPE_NPC &&
        scene->CheckWalkBlock(dest_x, dest_y) == true) {
      LOG_DEBUG("场景(%lu) 坐标(%u,%u) 阻挡", scene->GetSceneID(), dest_x, dest_y);
      // 获取最近的可行走点
      if(scene->GetNearWalkPosition(dest_x, dest_y) == false) {
        dest_x = map_info->default_area_.x_;
        dest_y = map_info->default_area_.y_;
        if(scene->CheckWalkBlock(dest_x, dest_y) == true) {
          LOG_ERROR("此场景[%d],此点是阻挡点x[%d]y[%d]", scene->GetMapID(),
              dest_x, dest_y);
          return false;
        }
      }
    }

    // 如果是本场景则调用Relocation重置坐标
    if(dest_scene == this->GetSceneID()) {
      return this->Relocation(entity, dest_x, dest_y, true);
    }

    // 从源场景删除
    if(this->RemoveEntity(entity) == false) {
      global::LogError("%s:%d (%s) 场景删除角色失败，不能进行跳转",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }

    entity->SetPosition(dest_x, dest_y);
    if(scene->AddEntity(entity, false) == false) {
      entity->SetPosition(src_x, src_y);
      this->AddEntity(entity, false);
      global::LogError("%s:%d (%s) 场景加入角色出错，跳转失败",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }

    entity->SetScene(dest_scene);

    // 非动态地图保存上次所在地图
    if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
      if (this->CheckDynamic() == false) {
        GameActor *actor = (GameActor *)entity;
        actor->SetPreMap(this->GetMapID());
        actor->SetPrePosition(src_x, src_y);
      }
      // 通知客户端切换地图
      gateway::protocol::MessageSceneActorJump message;
      message.__set_map_(global::GameMisc::GetSceneMapID(dest_scene));
      message.__set_x_(dest_x);
      message.__set_y_(dest_y);
      ((GameActor *)entity)->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_SCENE_ACTOR_JUMP);

      // 把9屏中所有实体集同步给玩家
      scene->SynchronizeCreateNineScreen((GameActor *)entity);
    }

    // 发送跳转场景事件
    event::EventSceneJumpMap event;
    event.__set_type_(entity->GetType());
    event.__set_id_(entity->GetID());
    event.__set_src_scene_(this->GetSceneID());
    event.__set_dest_scene_(dest_scene);
    event.__set_dest_x_(dest_x);
    event.__set_dest_y_(dest_y);
    event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
    if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
      channel_type = event::ChannelType::CHANNEL_ACTOR;
    } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
      channel_type = event::ChannelType::CHANNEL_NPC;
    }
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SCENE_JUMP_MAP, entity->GetID(),
        channel_type, &event, sizeof(event));
    session::protocol::MessageSceneJumpSceneRequest request;
    request.__set_type_(entity->GetType());
    request.__set_id_(entity->GetID());
    request.__set_scene_(dest_scene);
    request.__set_x_(dest_x);
    request.__set_y_(dest_y);
    SessionChannel::SendMessage(request,
        session::protocol::GameMessageType::MESSAGE_SCENE_JUMP_SCENE_REQUEST);
  } else {
    if (entity->GetType() == entity::EntityType::TYPE_NPC) {
      // TODO: 发送request, 请求buff信息快照
      //
      GameNpc *game_npc = (GameNpc *)entity;
      // 配置信息
      const NpcCell *cell = game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
          game_npc->GetTemplateID());
      if(cell == NULL) {
        LOG_ERROR("配置中找不到 npc(%u) ", game_npc->GetTemplateID());
        return false;
      }
      if (cell->type_ == entity::NpcKind::TRANSPORT) {
        // TODO: 发送request, 请求transport信息快照
        request::RequestTransportGetSnapshot request;
        request.__set_transport_npc_id_(game_npc->GetID());
        if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TRANSPORT_GET_SNAPSHOT,
              &request, sizeof(request)) == -1) {
          LOG_ERROR("请求 REQUEST_TRANSPORT_GET_SNAPSHOT 失败");
          return false;
        }

        request::RequestTransportRemove transport_remove_request;
        transport_remove_request.__set_transport_npc_(game_npc->GetID());
        if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TRANSPORT_REMOVE,
              &transport_remove_request, sizeof(transport_remove_request)) == -1) {
          LOG_ERROR("请求 REQUEST_TRANSPORT_REMOVE 失败");
          return false;
        }

        session::protocol::MessageSceneJumpSceneRequest message;
        message.__set_type_(entity->GetType());
        message.__set_id_(entity->GetID());
        message.__set_scene_(dest_scene);
        message.__set_x_(dest_x);
        message.__set_y_(dest_y);
        message.__isset.npc_snapshot_ = true;
        message.npc_snapshot_.__set_template_id_(game_npc->GetTemplateID());
        message.npc_snapshot_.__set_current_hp_(game_npc->GetAttribute(entity::RoleAoiFields::CURRENT_HP));
        message.npc_snapshot_.__set_current_mp_(game_npc->GetAttribute(entity::RoleAoiFields::CURRENT_MP));
        message.npc_snapshot_.__set_transport_(request.snapshot_);
        SessionChannel::SendMessage(message,
            session::protocol::GameMessageType::MESSAGE_SCENE_JUMP_SCENE_REQUEST);
      }
    } else if (entity->GetType() == entity::EntityType::TYPE_ACTOR) {
      session::protocol::MessageSceneJumpSceneRequest request;
      request.__set_type_(entity->GetType());
      request.__set_id_(entity->GetID());
      request.__set_scene_(dest_scene);
      request.__set_x_(dest_x);
      request.__set_y_(dest_y);
      SessionChannel::SendMessage(request,
          session::protocol::GameMessageType::MESSAGE_SCENE_JUMP_SCENE_REQUEST);
    }
  }

  return true;
}

bool Scene::Jump(GameEntity *entity, core::uint32 dest_x, core::uint32 dest_y,
    core::uint8 grid_offs_x, core::uint8 grid_offs_y) {
  if(dest_x >= this->max_x_ || dest_y >= this->max_y_) {
    global::LogError("%s:%d (%s) 参数 dest_x/dest_y 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 玩家跳转
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    SceneActor *actor = this->GetSceneActor(entity->GetID());
    if(actor == NULL) {
      global::LogError("%s:%d (%s) 场景玩家(%llu) 在管理器中不存在",
          __FILE__, __LINE__, __FUNCTION__, entity->GetID());
      return false;
    }
    // 重置玩家移动合法性的检测
    actor->ResetCheck();
  }

  // 检测是否可以加入格子
  if(this->CheckAddGrid(entity, dest_x, dest_y) == false) {
    return false;
  }

  // 取得坐标
  core::uint32 src_x = 0, src_y = 0;
  entity->GetPosition(src_x, src_y);

  // 得到屏坐标
  core::uint32 src_screen_x = src_x / this->screen_width_;
  core::uint32 src_screen_y = src_y / this->screen_width_;
  core::uint32 dest_screen_x = dest_x / this->screen_width_;
  core::uint32 dest_screen_y = dest_y / this->screen_width_;

  Screen *src_screen[kNineScreen] = { NULL };
  Screen *dest_screen[kNineScreen] = { NULL };

  size_t src_screen_number = kNineScreen, dest_screen_number = kNineScreen;

  // 得到源九屏和目的九屏
  this->GetNineScreen(src_screen_x, src_screen_y, src_screen, src_screen_number);
  this->GetNineScreen(dest_screen_x, dest_screen_y, dest_screen, dest_screen_number);

  // TODO: src_screen 和 dest_screen 中应该去掉它们的交集

  // 退出、进入需要同步给玩家的实体数量
  size_t sync_create_number = 0;
  size_t sync_destory_number = 0;

  // 实体退出、进入广播
  size_t exit_array_index = 0, enter_array_index = 0;
  size_t max_screen_number =
    src_screen_number > dest_screen_number ? src_screen_number : dest_screen_number;
  for(size_t pos_x = 0; pos_x < max_screen_number; ++pos_x) {
    // 实体退出
    if(src_screen[pos_x]) {
      const Screen::EntitySet &actors = src_screen[pos_x]->GetActors();
      Screen::EntitySet::const_iterator iterator = actors.begin();
      for(; iterator != actors.end(); ++iterator) {
        if(exit_array_index >= kMaxActorArraySize) {
          break;
        }
        // 不能把自己加入这个删除列表
        if(*iterator && *iterator != entity) {
          actor_array1[exit_array_index++] = (*iterator)->GetID();
        }
      }
      // 得到此屏索引中所有的实体指针
      size_t count = kMaxActorArraySize - sync_destory_number;
      if(src_screen[pos_x]->GetEntitysExcept(entity,
            entity_array1 + sync_destory_number, count) == false) {
        global::LogError("%s:%d (%s) 获取实体集失败，可能由于屏索引中实体过多",
            __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
      sync_destory_number += count;
    }
    // 实体进入
    if(dest_screen[pos_x]) {
      const Screen::EntitySet &actors = dest_screen[pos_x]->GetActors();
      Screen::EntitySet::const_iterator iterator = actors.begin();
      for(; iterator != actors.end(); ++iterator) {
        if(enter_array_index >= kMaxActorArraySize) {
          break;
        }
        if(*iterator && *iterator != entity) {
          actor_array2[enter_array_index++] = (*iterator)->GetID();
        }
      }
      // 得到此屏索引中所有的实体指针
      size_t count = kMaxActorArraySize - sync_create_number;
      if(dest_screen[pos_x]->GetEntitysExcept(entity,
            entity_array2 + sync_create_number, count) == false) {
        global::LogError("%s:%d (%s) 获取实体集失败，可能由于屏索引中实体过多",
            __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
      sync_create_number += count;
    }
  }

  // 广播退出、进入
  // this->BroadcastDestory(entity, actor_array1, exit_array_index);
  // this->BroadcastCreate(entity, actor_array2, enter_array_index);
  // 从源格子删除
  if(this->RemoveGrid(entity, src_x, src_y) == false) {
    global::LogError("%s:%d (%s) 从格子删除失败，实体(%d, %llu)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID());
    return false;
  }

  // 加入新格子
  if(this->AddGrid(entity, dest_x, dest_y) == false) {
    this->AddGrid(entity, src_x, src_y);
    global::LogError("%s:%d (%s) 加入新格子(%u, %u) 失败，实体(%d, %llu)",
        __FILE__, __LINE__, __FUNCTION__, dest_x, dest_y,
        (core::uint32)entity->GetType(), entity->GetID());
    return false;
  }

  // 如果切屏
  if(src_screen_x != dest_screen_x || src_screen_y != dest_screen_y) {
    // 从源屏中删除
    if(this->RemoveScreen(entity) == false) {
      global::LogError("%s:%d (%s) 从屏删除失败，实体(%d, %llu)",
          __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
          entity->GetID());
      return false;
    }
  }

  entity->SetPosition(dest_x, dest_y);
  entity->SetPosOffset(grid_offs_x, grid_offs_y);

  // 如果切屏
  if(src_screen_x != dest_screen_x || src_screen_y != dest_screen_y) {
    // 加入到新屏中
    if(this->AddScreen(entity) == false) {
      global::LogError("%s:%d (%s) 加入新屏失败，实体(%d, %llu)",
          __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
          entity->GetID());
      return false;
    }
  }

  // 广播退出、进入
  this->BroadcastDestory(entity, actor_array1, exit_array_index);
  this->BroadcastCreate(entity, actor_array2, enter_array_index);

  // 如果是玩家移动，需要同步创建、销毁相关屏的实体集数据到当前玩家
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    // 同步需要销毁的实体集
    this->SynchronizeDestory(entity_array1, sync_destory_number, (GameActor *)entity);
    // 同步需要创建的实体集
    this->SynchronizeCreate(entity_array2, sync_create_number, (GameActor *)entity);

    // 需要通知客户端主玩家重新定位到新坐标
    gateway::protocol::MessageActorRelocation message;
    message.__set_id_(entity->GetStringID());
    message.__set_x_(dest_x);
    message.__set_y_(dest_y);
    ((GameActor *)entity)->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ACTOR_RELOCATION);
  }

  // 发送角色远离与靠近事件
  this->BroadcastLeavingEvent(entity, entity_array1, sync_destory_number);
  this->BroadcastComingEvent(entity, entity_array2, sync_create_number);

  // 广播位置改变事件
  event::EventSceneRoleChangeLocation event;
  event.__set_type_(entity->GetType());
  event.__set_id_(entity->GetID());
  event.__set_src_scene_(this->GetSceneID());
  event.__set_src_x_(src_x);
  event.__set_src_y_(src_y);
  event.__set_dest_scene_(this->GetSceneID());
  event.__set_dest_x_(dest_x);
  event.__set_dest_y_(dest_y);
  event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION, entity->GetID(),
      channel_type, &event, sizeof(event));

  return true;
}

bool Scene::Walk(GameEntity *entity, entity::DirectionType::type dir, core::uint8 grid_offset_x,
    core::uint8 grid_offset_y) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(entity->GetType() == entity::EntityType::TYPE_ACTOR ||
      entity->GetType() == entity::EntityType::TYPE_NPC) {
    // 是否在禁止移动状态
    if(((GameRole *)entity)->CheckCommonStatus(entity::RoleCommonStatus::FORBID_MOVE)) {
      global::LogError("%s:%d (%s) 角色禁止移动状态中，不能移动",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
  }

  if(dir < entity::DirectionType::MIN || dir >= entity::DirectionType::NONE) {
    global::LogError("%s:%d (%s) 参数 dir 值错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  core::int32 offset_x = entity::g_constants_constants.kDirectionOffsetX[dir];
  core::int32 offset_y = entity::g_constants_constants.kDirectionOffsetY[dir];

  // 得到当前坐标
  core::uint32 src_x = 0, src_y = 0, dest_x = 0, dest_y = 0;
  entity->GetPosition(src_x, src_y);

  // 出界检测
  if((src_x == 0 && offset_x < 0) || (src_y == 0 && offset_y < 0) ||
      ((src_x + offset_x) >= this->max_x_) || ((src_y + offset_y) >= this->max_y_)) {
    global::LogError("%s:%d (%s) 玩家移动失败，已到边界",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  dest_x = src_x + offset_x;
  dest_y = src_y + offset_y;

  // 如果是玩家移动，检测移动频率
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    SceneActor *actor = this->GetSceneActor(entity->GetID());
    if(actor == NULL) {
      global::LogError("%s:%d (%s) 场景玩家(%llu) 在管理器中不存在",
          __FILE__, __LINE__, __FUNCTION__, entity->GetID());
      return false;
    }
    // 检测移动频率是否过快
    if(actor->CheckMove() == false) {
      global::LogError("%s:%d (%s) 场景玩家(%llu) 移动异常",
          __FILE__, __LINE__, __FUNCTION__, entity->GetID());
      return false;
    }
  }

  // 检测是否可以加入格子
  if(this->CheckAddGrid(entity, dest_x, dest_y) == false) {
    return false;
  }

  // 从源格子删除
  if(this->RemoveGrid(entity, src_x, src_y) == false) {
    global::LogError("%s:%d (%s) 从格子删除失败，实体(%d, %llu)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID());
    return false;
  }

  // 加入新格子
  if(this->AddGrid(entity, dest_x, dest_y) == false) {
    this->AddGrid(entity, src_x, src_y);
    global::LogError("%s:%d (%s) 加入新格子(%u, %u) 失败，实体(%d, %llu)",
        __FILE__, __LINE__, __FUNCTION__, dest_x, dest_y,
        (core::uint32)entity->GetType(), entity->GetID());
    return false;
  }

  // 得到屏坐标
  core::uint32 src_screen_x = src_x / this->screen_width_;
  core::uint32 src_screen_y = src_y / this->screen_width_;
  core::uint32 dest_screen_x = dest_x / this->screen_width_;
  core::uint32 dest_screen_y = dest_y / this->screen_width_;

  // 移动同步屏、原9屏、新9屏
  Screen *sync_screen[kNineScreen] = { NULL };
  Screen *src_screen[kNineScreen] = { NULL };
  Screen *dest_screen[kNineScreen] = { NULL };

  // 移动之前坐标所在屏一定是需要移动广播的
  sync_screen[0] = this->GetScreen(src_screen_x, src_screen_y);

  // 屏坐标差值
  core::int32 diff_screen_x = (core::int32)dest_screen_x - (core::int32)src_screen_x;
  core::int32 diff_screen_y = (core::int32)dest_screen_y - (core::int32)src_screen_y;

  // 原屏、新屏周围9屏的数量(不可在边界上不足9屏)
  size_t src_screen_number = kNineScreen;
  size_t dest_screen_number = kNineScreen;
  // 需要同步移动消息的屏数量
  size_t sync_screen_number = kNineScreen;

  // 得到原屏周围9个屏
  this->GetNineScreen(src_screen_x, src_screen_y, src_screen, src_screen_number);

  if(diff_screen_x != 0 || diff_screen_y != 0) {
/*
    global::LogDebug("玩家[%llu]移动切屏 src_pos(%d, %d), dest_pos(%d, %d), src_screen(%d, %d), dest_screen(%d, %d)",
        entity->GetID(), src_x, src_y, dest_x, dest_y, src_screen_x, src_screen_y, dest_screen_x, dest_screen_y);
*/
    // 从源屏中删除
    if(this->RemoveScreen(entity) == false) {
      global::LogError("%s:%d (%s) 从屏删除失败，实体(%d, %llu)",
          __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
          entity->GetID());
      return false;
    }

    // 设置新坐标
    entity->SetPosition(dest_x, dest_y);
    entity->SetPosOffset(grid_offset_x, grid_offset_y);

    // 出现切屏，计算得到需要移动广播、创建广播、销毁广播
    // 得到新屏周围9个屏
    this->GetNineScreen(dest_screen_x, dest_screen_y, dest_screen, dest_screen_number);
    // 计算出切屏后，需要同步移动消息的几屏（可能是四屏也可能是六屏）
    if(diff_screen_x == 0) {
      // 1字方向，同步五屏
      sync_screen[1] = this->GetScreen(src_screen_x - 1, src_screen_y);
      sync_screen[2] = this->GetScreen(src_screen_x + 1, src_screen_y);
      sync_screen[3] = this->GetScreen(src_screen_x - 1, src_screen_y + diff_screen_y);
      sync_screen[4] = this->GetScreen(src_screen_x, src_screen_y + diff_screen_y);
      sync_screen[5] = this->GetScreen(src_screen_x + 1, src_screen_y + diff_screen_y);
      sync_screen_number = 6;
    } else if(diff_screen_y == 0) {
      // 一字方向，同步五屏
      sync_screen[1] = this->GetScreen(src_screen_x, src_screen_y - 1);
      sync_screen[2] = this->GetScreen(src_screen_x, src_screen_y + 1);
      sync_screen[3] = this->GetScreen(src_screen_x + diff_screen_x, src_screen_y - 1);
      sync_screen[4] = this->GetScreen(src_screen_x + diff_screen_x, src_screen_y);
      sync_screen[5] = this->GetScreen(src_screen_x + diff_screen_x, src_screen_y + 1);
      sync_screen_number = 6;
    } else {
      // X字方向，同步三屏
      sync_screen[1] = this->GetScreen(src_screen_x + diff_screen_x, src_screen_y);
      sync_screen[2] = this->GetScreen(src_screen_x, src_screen_y + diff_screen_y);
      sync_screen[3] = this->GetScreen(src_screen_x + diff_screen_x, src_screen_y + diff_screen_y);
      sync_screen_number = 4;
    }

    // 退出、进入需要同步给玩家的实体数量
    size_t sync_create_number = 0;
    size_t sync_destory_number = 0;

    // 实体退出、进入广播
    size_t exit_array_index = 0, enter_array_index = 0;
    for(size_t pos_x = 0; pos_x < kNineScreen; ++pos_x) {
      // 实体退出
      if(src_screen[pos_x]) {
        bool include = false;
        // for(size_t pos_y = 0; pos_y < kMaxShiftScreen; ++pos_y) {
        for(size_t pos_y = 0; pos_y <= sync_screen_number; ++pos_y) {
          if(sync_screen[pos_y] == src_screen[pos_x]) {
            include = true;
            break;
          }
        }
        if(include == false) {
          // src_screen[pos_x]不在需要同步的屏集合sync_screen中，是需要发送实体退出消息的
          const Screen::EntitySet &actors = src_screen[pos_x]->GetActors();
          Screen::EntitySet::const_iterator iterator = actors.begin();
          for(; iterator != actors.end(); ++iterator) {
            if(exit_array_index >= kMaxActorArraySize) {
              break;
            }
            if(*iterator) {
              actor_array1[exit_array_index++] = (*iterator)->GetID();
            }
          }
          // 得到此屏索引中所有的实体指针，用于广播给entity当前屏(需要被删除)
          // 中需要被删除的所有实体
          size_t count = kMaxActorArraySize - sync_destory_number;
          if(src_screen[pos_x]->GetEntitysExcept(entity,
                entity_array1 + sync_destory_number, count) == false) {
            global::LogError("%s:%d (%s) 获取实体集失败，可能由于屏索引中实体过多",
                __FILE__, __LINE__, __FUNCTION__);
            return false;
          }
          sync_destory_number += count;
        }
      }
      // 实体进入
      if(dest_screen[pos_x]) {
        bool include = false;
        // for(size_t pos_y = 0; pos_y < kMaxShiftScreen; ++pos_y) {
        for(size_t pos_y = 0; pos_y <= sync_screen_number; ++pos_y) {
          if(sync_screen[pos_y] == dest_screen[pos_x]) {
            include = true;
            break;
          }
        }
        if(include == false) {
          // dest_screen[pos_x]不在需要同步的屏集合sync_screen中，是需要发送实体进入消息的
          const Screen::EntitySet &actors = dest_screen[pos_x]->GetActors();
          Screen::EntitySet::const_iterator iterator = actors.begin();
          for(; iterator != actors.end(); ++iterator) {
            if(enter_array_index >= kMaxActorArraySize) {
              break;
            }
            if(*iterator) {
              actor_array2[enter_array_index++] = (*iterator)->GetID();
            }
          }
          // 得到此屏索引中所有的实体指针，用于广播给entity当前(需要创建)中
          // 需要被创建的所有实体
          size_t count = kMaxActorArraySize - sync_create_number;
          if(dest_screen[pos_x]->GetEntitysExcept(entity,
                entity_array2 + sync_create_number, count) == false) {
            global::LogError("%s:%d (%s) 获取实体集失败，可能由于屏索引中实体过多",
                __FILE__, __LINE__, __FUNCTION__);
            return false;
          }
          sync_create_number += count;
        }
      }
    }

    // 广播实体进入和退出到相应屏
    this->BroadcastDestory(entity, actor_array1, exit_array_index);
    this->BroadcastCreate(entity, actor_array2, enter_array_index);

    // 如果是玩家移动，需要同步创建、销毁相关屏的实体集数据到当前玩家
    if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
      // 同步需要销毁的实体集
      this->SynchronizeDestory(entity_array1, sync_destory_number, (GameActor *)entity);
      // 同步需要创建的实体集
      this->SynchronizeCreate(entity_array2, sync_create_number, (GameActor *)entity);
    }

    // 发送角色远离与靠近事件
    this->BroadcastLeavingEvent(entity, entity_array1, sync_destory_number);
    this->BroadcastComingEvent(entity, entity_array2, sync_create_number);

    // 广播移动同步
    size_t sync_array_index = 0;
    for(size_t pos_x = 0; pos_x < sync_screen_number; ++pos_x) {
      if(sync_screen[pos_x]) {
        const Screen::EntitySet &actors = sync_screen[pos_x]->GetActors();
        Screen::EntitySet::const_iterator iterator = actors.begin();
        for(; iterator != actors.end(); ++iterator) {
          if(sync_array_index >= kMaxActorArraySize) {
            break;
          }
          if(*iterator) {
            actor_array1[sync_array_index++] = (*iterator)->GetID();
          }
        }
      }
    }

    // 广播移动
    this->BroadcastMove(entity, dir, actor_array1, sync_array_index);
  } else {
    // 没有切屏，做移动同步即可
    size_t src_array_index = 0;
    for(size_t pos_x = 0; pos_x < src_screen_number; ++pos_x) {
      if(src_screen[pos_x]) {
        const Screen::EntitySet &actors = src_screen[pos_x]->GetActors();
        Screen::EntitySet::const_iterator iterator = actors.begin();
        for(; iterator != actors.end(); ++iterator) {
          if(src_array_index >= kMaxActorArraySize) {
            break;
          }
          if(*iterator && *iterator != entity) {
            actor_array1[src_array_index++] = (*iterator)->GetID();
          }
        }
      }
    }

    // 设置新坐标
    entity->SetPosition(dest_x, dest_y);
    entity->SetPosOffset(grid_offset_x, grid_offset_y);

    // 广播移动
    this->BroadcastMove(entity, dir, actor_array1, src_array_index);
  }

  // 切屏
  if(diff_screen_x != 0 || diff_screen_y != 0) {
    // 加入新屏
    if(this->AddScreen(entity) == false) {
      global::LogError("%s:%d (%s) 加入新屏失败，实体(%d, %llu)",
          __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
          entity->GetID());
      return false;
    }
  }

  // 如果是玩家移动，检测移动频率
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    SceneActor *actor = this->GetSceneActor(entity->GetID());
    if(actor == NULL) {
      global::LogError("%s:%d (%s) 场景玩家(%llu) 在管理器中不存在",
          __FILE__, __LINE__, __FUNCTION__, entity->GetID());
      return false;
    }

    bool right_angle = (dir == entity::DirectionType::EAST) ||
      (dir == entity::DirectionType::WEST) ||
      (dir == entity::DirectionType::SOUTH) ||
      (dir == entity::DirectionType::NORTH);

    actor->MoveCount(right_angle);
  }

  // global::LogDebug("玩家(%llu)移动到坐标(%u,%u)",
  //    entity->GetID(), dest_x, dest_y);

  // 广播位置改变事件
  event::EventSceneRoleChangeLocation event;
  event.__set_type_(entity->GetType());
  event.__set_id_(entity->GetID());
  event.__set_src_scene_(this->GetSceneID());
  event.__set_src_x_(src_x);
  event.__set_src_y_(src_y);
  event.__set_dest_scene_(this->GetSceneID());
  event.__set_dest_x_(dest_x);
  event.__set_dest_y_(dest_y);
  event::ChannelType::type channel_type = event::ChannelType::CHANNEL_MAX;
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    channel_type = event::ChannelType::CHANNEL_ACTOR;
  } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
    channel_type = event::ChannelType::CHANNEL_NPC;
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION, entity->GetID(),
      channel_type, &event, sizeof(event));

  return true;
}

bool Scene::Relocation(GameEntity *entity, core::uint32 x, core::uint32 y, bool broadcast,
    core::uint8 offset_x, core::uint8 offset_y) {
  if(x >= this->max_x_ || y >= this->max_y_) {
    global::LogError("%s:%d (%s) 参数 x/y 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    if(broadcast) {
      if(this->Jump(entity, x, y, offset_x, offset_y) == false) {
        return false;
      }
    } else {
      gateway::protocol::MessageActorRelocation message;
      message.__set_id_(entity->GetStringID());
      message.__set_x_(x);
      message.__set_y_(y);
      message.__set_offset_x_(offset_x);
      message.__set_offset_y_(offset_y);
      ((GameActor *)entity)->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_ACTOR_RELOCATION);
    }
  } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
    if(broadcast) {
      if(this->Jump(entity, x, y, offset_x, offset_y) == false) {
        return false;
      }
    }
  } else {
    global::LogError("%s:%d (%s) 实体类型错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  entity->SetPosition(x, y);
  entity->SetPosOffset(offset_x, offset_y);

  return true;
}

bool Scene::GetNineScreenActors(core::uint32 x, core::uint32 y,
    core::uint64 actors[], size_t &number) {
  // 得到屏坐标
  core::uint32 screen_x = x / this->screen_width_;
  core::uint32 screen_y = y / this->screen_width_;

  // 得到周围九屏
  size_t screen_number = kNineScreen;
  Screen *screens[kNineScreen] = { NULL };
  this->GetNineScreen(screen_x, screen_y, screens, screen_number);

  size_t max_number = number;
  number = 0;

  // 循环得到所有玩家
  size_t pos = 0;
  for(; pos < screen_number; ++pos) {
    if(screens[pos]) {
      const Screen::EntitySet &entitys = screens[pos]->GetActors();
      Screen::EntitySet::const_iterator iterator = entitys.begin();
      for(; iterator != entitys.end(); ++iterator) {
        if(number >= max_number) {
          break;
        }
        actors[number++] = (*iterator)->GetID();
      }
    }
  }

  return true;
}

SceneActor *Scene::GetActor(core::uint64 actor) {
  ActorHashmap::iterator iterator = this->actors_.find(actor);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool Scene::Router(const RouterCoord &src, const RouterCoord &dest,
    entity::DirectionType::type paths[], size_t &number, size_t max_path_size,
    core::int32 dest_radius) {
  if(this->router_.Router(src, dest, max_path_size, dest_radius) == false) {
    return false;
  }

  const entity::DirectionType::type *router_paths = this->router_.GetPaths(number);
  if(router_paths == NULL) {
    return false;
  }

  number = number < max_path_size ? number : max_path_size;

  if(this->router_.CheckReverse() == false) {
    for(size_t pos = 0; pos < number; ++pos) {
      paths[pos] = router_paths[pos];
    }
  } else {
    for(size_t pos = 0; pos < number; ++pos) {
      paths[pos] = this->router_.ReverseDirection(
          router_paths[number - pos - 1]);
    }
  }

  return true;
}

bool Scene::Router(const RouterCoord &src, const RouterCoord &dest,
    size_t max_path_size, core::int32 dest_radius) {
  return this->router_.Router(src, dest, max_path_size, dest_radius);
}

const entity::DirectionType::type *Scene::GetRouterPaths(size_t &size) {
  return this->router_.GetPaths(size);
}

void Scene::GetNpcs(std::vector<core::int64> &npcs) const {
  for(size_t pos = 0; pos < this->screen_length_; ++pos) {
    const Screen::EntitySet &screen_npcs = this->screens_[pos].GetNpcs();
    Screen::EntitySet::const_iterator iterator = screen_npcs.begin();
    for(; iterator != screen_npcs.end(); ++iterator) {
      if(*iterator) {
        npcs.push_back((*iterator)->GetID());
      }
    }
  }
}

void Scene::EnterSceneToNine(GameEntity *entity) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 取坐标
  core::uint32 x = 0, y = 0;
  entity->GetPosition(x, y);

  // 得到屏坐标
  core::uint32 screen_x = x / this->screen_width_;
  core::uint32 screen_y = y / this->screen_width_;

  Screen *screens[kNineScreen] = { NULL };

  size_t screen_number = kNineScreen;

  // 得到九屏
  this->GetNineScreen(screen_x, screen_y, screens, screen_number);

  size_t array_index = 0;

  for(size_t pos = 0; pos < screen_number; ++pos) {
    if(screens[pos]) {
      const Screen::EntitySet &actors = screens[pos]->GetActors();
      Screen::EntitySet::const_iterator iterator = actors.begin();
      for(; iterator != actors.end(); ++iterator) {
        if(array_index >= kMaxActorArraySize) {
          break;
        }
        if(*iterator) {
          actor_array1[array_index++] = (*iterator)->GetID();
        }
      }
    }
  }

  this->BroadcastCreate(entity, actor_array1, array_index);
}

void Scene::ExitSceneToNine(GameEntity *entity) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 取坐标
  core::uint32 x = 0, y = 0;
  entity->GetPosition(x, y);

  // 得到屏坐标
  core::uint32 screen_x = x / this->screen_width_;
  core::uint32 screen_y = y / this->screen_width_;

  Screen *screens[kNineScreen] = { NULL };

  size_t screen_number = kNineScreen;

  // 得到九屏
  this->GetNineScreen(screen_x, screen_y, screens, screen_number);

  size_t array_index = 0;

  for(size_t pos = 0; pos < screen_number; ++pos) {
    if(screens[pos]) {
      const Screen::EntitySet &actors = screens[pos]->GetActors();
      Screen::EntitySet::const_iterator iterator = actors.begin();
      for(; iterator != actors.end(); ++iterator) {
        if(array_index >= kMaxActorArraySize) {
          break;
        }
        if(*iterator) {
          actor_array2[array_index++] = (*iterator)->GetID();
        }
      }
    }
  }

  this->BroadcastDestory(entity, actor_array2, array_index);
}

void Scene::SynchronizeCreateNineScreen(GameActor *actor) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 参数 actor 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 取坐标
  core::uint32 x = 0, y = 0;
  actor->GetPosition(x, y);

  // 得到屏坐标
  core::uint32 screen_x = x / this->screen_width_;
  core::uint32 screen_y = y / this->screen_width_;

  Screen *screens[kNineScreen] = { NULL };

  size_t screen_number = kNineScreen;

  // 得到九屏
  this->GetNineScreen(screen_x, screen_y, screens, screen_number);

  size_t create_number = 0;

  // 得到九屏中所有实体
  for(size_t pos = 0; pos < screen_number; ++pos) {
    size_t count = kMaxActorArraySize - create_number;
    if(screens[pos]) {
      if(screens[pos]->GetEntitysExcept(actor,
            entity_array2 + create_number, count) == false) {
        global::LogError("%s:%d (%s) 获取实体集失败，可能由于屏索引中实体过多",
            __FILE__, __LINE__, __FUNCTION__);
        return ;
      }
      create_number += count;
    }
  }

  // 同步需要创建的实体集
  if(create_number > 0)
    this->SynchronizeCreate(entity_array2, create_number, actor);
}

void Scene::SynchronizeDestoryNineScreen(GameActor *actor) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 参数 actor 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 取坐标
  core::uint32 x = 0, y = 0;
  actor->GetPosition(x, y);

  // 得到屏坐标
  core::uint32 screen_x = x / this->screen_width_;
  core::uint32 screen_y = y / this->screen_width_;

  Screen *screens[kNineScreen] = { NULL };

  size_t screen_number = kNineScreen;

  // 得到九屏
  this->GetNineScreen(screen_x, screen_y, screens, screen_number);

  size_t destory_number = 0;

  // 得到九屏中所有实体
  for(size_t pos = 0; pos < screen_number; ++pos) {
    size_t count = kMaxActorArraySize - destory_number;
    if(screens[pos]) {
      if(screens[pos]->GetEntitysExcept(actor,
            entity_array1 + destory_number, count) == false) {
        global::LogError("%s:%d (%s) 获取实体集失败，可能由于屏索引中实体过多",
            __FILE__, __LINE__, __FUNCTION__);
        return ;
      }
      destory_number += count;
    }
  }

  // 同步需要创建的实体集
  this->SynchronizeDestory(entity_array1, destory_number, actor);
}

bool Scene::AddScreen(GameEntity *entity) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 取得坐标
  core::uint32 x = 0, y = 0;
  entity->GetPosition(x, y);

  // 取得屏
  Screen *screen = this->GetPositionScreen(x, y);
  if(screen == NULL) {
    global::LogError("%s:%d (%s) 取得 Entity(%d, %llu) 所在屏失败，坐标(%u, %u)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID(), x, y);
    return false;
  }

  // 加入到屏索引中
  if(screen->Add(entity) == false) {
    global::LogError("%s:%d (%s) Entity(%d, %llu) 加入屏失败，坐标(%u, %u)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID(), x, y);
    return false;
  }

  return true;
}

bool Scene::RemoveScreen(GameEntity *entity) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 取得坐标
  core::uint32 x = 0, y = 0;
  entity->GetPosition(x, y);

  // 取得屏坐标
  Screen *screen = this->GetPositionScreen(x, y);
  if(screen == NULL) {
    global::LogError("%s:%d (%s) 取得 Entity(%d, %llu) 所在屏失败，坐标(%u, %u)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID(), x, y);
    return false;
  }

  // 从屏索引中删除
  if(screen->Remove(entity) == false) {
    global::LogError("%s:%d (%s) Entity(%d, %llu) 从屏中删除失败，坐标(%u, %u)",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID(), x, y);
    return false;
  }

  return true;
}


bool Scene::AddGrid(GameEntity *entity, core::uint32 dest_x, core::uint32 dest_y) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 得到对应格子
  Grid *grid = this->grids_.Get(dest_x, dest_y);
  if(grid == NULL) {
    global::LogError("%s:%d (%s) Entity(%d, %llu) 加入格子失败，坐标(%u, %u) 无格子",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID(), dest_x, dest_y);
    return false;
  }
 
  // 格子是否为障碍
  if((entity->GetType() == entity::EntityType::TYPE_NPC && grid->CanNpcWalk() == false) ||
      grid->CanWalk() == false) {
    global::LogError("%s:%d (%s) Entity(%d, %llu) 加入格子失败，坐标(%u, %u) 为障碍",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID(), dest_x, dest_y);
    return false;
  }

  // 加入格子列表
  grid->AddEntity(entity);

  return true;
}

bool Scene::RemoveGrid(GameEntity *entity, core::uint32 src_x, core::uint32 src_y) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 得到对应格子
  Grid *grid = this->grids_.Get(src_x, src_y);
  if(grid == NULL) {
    global::LogError("%s:%d (%s) Entity(%d, %llu) 从格子删除失败, 坐标(%u, %u) 无格子",
        __FILE__, __LINE__, __FUNCTION__, (core::uint32)entity->GetType(),
        entity->GetID(), src_x, src_y);
    return false;
  }

  // 从格子列表中删除
  grid->RemoveEntity(entity);

  return true;
}

bool Scene::CheckAddGrid(GameEntity *entity, core::uint32 dest_x, core::uint32 dest_y) {
  if(entity == NULL) {
    return false;
  }

  // 取得坐标
  core::uint32 src_x = 0, src_y = 0;
  entity->GetPosition(src_x, src_y);

  // 得到目标坐标所在的格子
  Grid *grid = this->grids_.Get(dest_x, dest_y);
  if(grid == NULL) {
    return false;
  }

  // 是否可以行走
  if((entity->GetType() == entity::EntityType::TYPE_NPC && grid->CanNpcWalk() == false) ||
      (entity->GetType() != entity::EntityType::TYPE_NPC && grid->CanWalk() == false)) {
    return false;
  }

  // 得到源坐标所在的格子
  grid = this->grids_.Get(src_x, src_y);
  if(grid == NULL) {
    return false;
  }

  return true;
}

Screen *Scene::GetPositionScreen(core::uint32 x, core::uint32 y) {
  if(this->CheckPositionValid(x, y) == false) {
    return NULL;
  }
  x /= this->screen_width_;
  y /= this->screen_width_;
  return this->GetScreen(x, y);
}

Screen *Scene::GetScreen(core::uint32 screen_x, core::uint32 screen_y) {
  if(screen_x >= this->screen_max_x_ || screen_y >= this->screen_max_y_) {
    return NULL;
  }
  return &this->screens_[screen_y * this->screen_max_x_ + screen_x];
}

bool Scene::CheckPositionValid(core::uint32 x, core::uint32 y) const {
  if(x <= this->max_x_ && y < this->max_y_) {
    return true;
  }
  return false;
}

void Scene::GetNineScreen(core::uint32 screen_x, core::uint32 screen_y,
    Screen *screens[], size_t &screen_number) {
  // 把当前屏填入
  screens[0] = this->GetScreen(screen_x, screen_y);

  size_t max_screen_size = screen_number;
  screen_number = 1;
  core::int32 x = 0, y = 0;

  // 填入其它八个方向的屏
  for(size_t pos = 0; pos < entity::DirectionType::NONE; ++pos) {
    if(screen_number >= max_screen_size) {
      return ;
    }
    // 八方向偏移
    core::int32 offset_x = entity::g_constants_constants.kDirectionOffsetX[pos];
    core::int32 offset_y = entity::g_constants_constants.kDirectionOffsetY[pos];

    x = (core::int32)screen_x + offset_x;
    y = (core::int32)screen_y + offset_y;

    if(x >= 0 && y >= 0) {
      Screen *screen = this->GetScreen(x, y);
      if(screen) {
        screens[screen_number++] = screen;
      }
    }
  }
}

void Scene::BroadcastCreate(GameEntity *entity, core::uint64 actors[], size_t number) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(number <= 0) {
    return ;
  }

  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    GameActor *actor = (GameActor *)entity;
    gateway::protocol::MessageCreateActorSynchronize aoi;
    aoi.__set_name_(actor->GetName());
    aoi.__set_id_(actor->GetStringID());
    aoi.__set_role_aoi_fields_(actor->GetRoleAoiAttributes());
    aoi.__set_actor_aoi_fields_(actor->GetActorAoiAttributes());
    actor->GetPosition((core::uint32 &)aoi.pos_x_, (core::uint32 &)aoi.pos_y_);
    core::uint8 offset_x = 0, offset_y = 0;
    actor->GetPosOffset(offset_x, offset_y);
    if(offset_x != 0 || offset_y != 0) {
      aoi.__set_offset_x_(offset_x);
      aoi.__set_offset_y_(offset_y);
    }
    GameServerSingleton::GetInstance().MulticastMessage(aoi,
        gateway::protocol::MessageType::MESSAGE_CREATE_ACTOR_SYNCHRONIZE,
        actors, number);
  } else if(entity->GetType() == entity::EntityType::TYPE_NPC) {
    GameNpc *npc = (GameNpc *)entity;
    gateway::protocol::MessageCreateNpcSynchronize aoi;
    aoi.__set_template_id_(npc->GetTemplateID());
    aoi.__set_id_(npc->GetStringID());
    aoi.__set_role_aoi_fields_(npc->GetRoleAoiAttributes());
    aoi.__set_npc_aoi_fields_(npc->GetNpcAoiAttributes());
    npc->GetPosition((core::uint32 &)aoi.pos_x_, (core::uint32 &)aoi.pos_y_);
    GameServerSingleton::GetInstance().MulticastMessage(aoi,
        gateway::protocol::MessageType::MESSAGE_CREATE_NPC_SYNCHRONIZE,
        actors, number);
  } else if(entity->GetType() == entity::EntityType::TYPE_SCENE_ITEM) {
    SceneItem *item = (SceneItem *)entity;
    gateway::protocol::MessageCreateSceneItemSynchronize aoi;
    aoi.__set_id_(item->GetStringID());
    aoi.__set_scene_item_aoi_fields_(item->GetSceneItemAoiAttribute());
    item->GetPosition((core::uint32 &)aoi.pos_x_, (core::uint32 &)aoi.pos_y_);
    aoi.__set_owner_type_(item->GetOwnerType());
    const SceneItem::OwnerVector &owners = item->GetOwners();
    for(SceneItem::OwnerVector::const_iterator iterator = owners.begin();
        iterator != owners.end(); ++iterator) {
      aoi.owner_actors_.push_back(global::ToString(*iterator));
    }
    GameServerSingleton::GetInstance().MulticastMessage(aoi,
        gateway::protocol::MessageType::MESSAGE_CREATE_SCENE_ITEM_SYNCHRONIZE,
        actors, number);
  } else {
    global::LogError("%s:%d (%s) 实体类型(%d) 错误",
        __FILE__, __LINE__, __FUNCTION__, (int)entity->GetType());
    return ;
  }

  // 发送广播实体创建事件
  event::EventSceneBroadcastCreateRole event;
  event.__set_type_(entity->GetType());
  event.__set_id_(entity->GetID());
  for(size_t pos = 0; pos < number; ++pos) {
    event.actors_.push_back(actors[pos]);
  }
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SCENE_BROADCAST_CREATE_ROLE, this->GetSceneID(),
      event::ChannelType::CHANNEL_SCENE, &event, sizeof(event));
}

void Scene::BroadcastDestory(GameEntity *entity, core::uint64 actors[], size_t number) {
  if(number > 0) {
    gateway::protocol::MessageDestoryRoleSynchronize message;
    message.__set_id_(entity->GetStringID());
    message.__set_type_(entity->GetType());
    GameServerSingleton::GetInstance().MulticastMessage(message,
        gateway::protocol::MessageType::MESSAGE_DESTORY_ROLE_SYNCHRONIZE,
        actors, number);
  }
}

void Scene::BroadcastComingEvent(GameEntity *entity, GameEntity *entitys[], size_t number) {
  if(entity == NULL || number <= 0) {
    return ;
  }

  event::EventSceneRoleComing event;
  event.__set_type_(entity->GetType());
  event.__set_id_(entity->GetID());

  event::SceneRolePair pair;
  size_t pos = 0;
  for(; pos < number; ++pos) {
    if(entitys[pos] && entitys[pos] != entity) {
      pair.__set_type_(entitys[pos]->GetType());
      pair.__set_id_(entitys[pos]->GetID());
      event.roles_.push_back(pair);
    }
  }

  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SCENE_ROLE_COMING, this->GetSceneID(),
      event::ChannelType::CHANNEL_SCENE, &event, sizeof(event));
}

void Scene::BroadcastLeavingEvent(GameEntity *entity, GameEntity *entitys[], size_t number) {
  if(entity == NULL || number <= 0) {
    return ;
  }

  event::EventSceneRoleLeaving event;
  event.__set_type_(entity->GetType());
  event.__set_id_(entity->GetID());

  event::SceneRolePair pair;
  size_t pos = 0;
  for(; pos < number; ++pos) {
    if(entitys[pos] && entitys[pos] != entity) {
      pair.__set_type_(entitys[pos]->GetType());
      pair.__set_id_(entitys[pos]->GetID());
      event.roles_.push_back(pair);
    }
  }

  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SCENE_ROLE_LEAVING, this->GetSceneID(),
      event::ChannelType::CHANNEL_SCENE, &event, sizeof(event));
}

void Scene::BroadcastDayNightChange() {
  ActorHashmap::iterator iter = this->actors_.begin(); 
  size_t scene_array_index = 0;
  for(; iter != this->actors_.end(); ++iter) {
    if(iter->second) {
      actor_array1[scene_array_index] = iter->second->GetActorID();
      scene_array_index++;
    }
  }
  gateway::protocol::MessageSceneDayNightChange message; 
  message.__set_status_(
      static_cast<gateway::protocol::DayNightStatus::type>(this->daynight_.GetCurStatus()));

  GameServerSingleton::GetInstance().MulticastMessage(message,
      gateway::protocol::MessageType::MESSAGE_SCENE_DAYNIGHT_CHANGE,
      actor_array1, scene_array_index);
}

void Scene::EnterSceneComingEvent(GameEntity *entity) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 取坐标
  core::uint32 x = 0, y = 0;
  entity->GetPosition(x, y);

  // 得到屏坐标
  core::uint32 screen_x = x / this->screen_width_;
  core::uint32 screen_y = y / this->screen_width_;

  Screen *screens[kNineScreen] = { NULL };

  size_t screen_number = kNineScreen;

  // 得到九屏
  this->GetNineScreen(screen_x, screen_y, screens, screen_number);

  size_t array_index = 0;

  for(size_t pos = 0; pos < screen_number; ++pos) {
    if(screens[pos]) {
      const Screen::EntitySet &actors = screens[pos]->GetActors();
      Screen::EntitySet::const_iterator iterator = actors.begin();
      for(; iterator != actors.end(); ++iterator) {
        if(array_index >= kMaxActorArraySize) {
          break;
        }
        if(*iterator) {
          entity_array1[array_index++] = *iterator;
        }
      }
      const Screen::EntitySet &npcs = screens[pos]->GetNpcs();
      iterator = npcs.begin();
      for(; iterator != npcs.end(); ++iterator) {
        if(array_index >= kMaxActorArraySize) {
          break;
        }
        if(*iterator) {
          entity_array1[array_index++] = *iterator;
        }
      }
    }
  }

  this->BroadcastComingEvent(entity, entity_array1, array_index);
}

void Scene::ExitSceneLeavingEvent(GameEntity *entity) {
  if(entity == NULL) {
    global::LogError("%s:%d (%s) 参数 entity 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 取坐标
  core::uint32 x = 0, y = 0;
  entity->GetPosition(x, y);

  // 得到屏坐标
  core::uint32 screen_x = x / this->screen_width_;
  core::uint32 screen_y = y / this->screen_width_;

  Screen *screens[kNineScreen] = { NULL };

  size_t screen_number = kNineScreen;

  // 得到九屏
  this->GetNineScreen(screen_x, screen_y, screens, screen_number);

  size_t array_index = 0;

  for(size_t pos = 0; pos < screen_number; ++pos) {
    if(screens[pos]) {
      const Screen::EntitySet &actors = screens[pos]->GetActors();
      Screen::EntitySet::const_iterator iterator = actors.begin();
      for(; iterator != actors.end(); ++iterator) {
        if(array_index >= kMaxActorArraySize) {
          break;
        }
        if(*iterator) {
          entity_array2[array_index++] = *iterator;
        }
      }
      const Screen::EntitySet &npcs = screens[pos]->GetNpcs();
      iterator = npcs.begin();
      for(; iterator != npcs.end(); ++iterator) {
        if(array_index >= kMaxActorArraySize) {
          break;
        }
        if(*iterator) {
          entity_array2[array_index++] = *iterator;
        }
      }
    }
  }

  this->BroadcastLeavingEvent(entity, entity_array2, array_index);
}

void Scene::BroadcastMove(GameEntity *entity, entity::DirectionType::type dir,
    core::uint64 actors[], size_t number) {
  gateway::protocol::MessageRoleMoveSynchronize message;
  message.__set_type_(entity->GetType());
  message.__set_id_(entity->GetStringID());
  message.__set_dir_(dir);
  if(entity->GetType() == entity::EntityType::TYPE_ACTOR) {
    core::uint8 offset_x = 0, offset_y = 0;
    entity->GetPosOffset(offset_x, offset_y);
    message.__set_offset_x_(offset_x);
    message.__set_offset_y_(offset_y);
    ((GameActor *)entity)->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ROLE_MOVE_SYNCHRONIZE);
  }
  GameServerSingleton::GetInstance().MulticastMessage(message,
      gateway::protocol::MessageType::MESSAGE_ROLE_MOVE_SYNCHRONIZE,
      actors, number);
}

void Scene::SynchronizeCreate(GameEntity *entitys[], size_t number, GameActor *actor) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 参数 actor 为空", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(number <= 0) {
    return ;
  }

  gateway::protocol::AoiActorData actor_data;
  gateway::protocol::AoiNpcData npc_data;
  gateway::protocol::AoiSceneItemData item_data;
  gateway::protocol::MessageCreateAoiRoles message;

  event::EventSceneCreateAoiRoles event;
  event::SceneRolePair entity;

  size_t pos = 0;
  for(; pos < number; ++pos) {
    if(entitys[pos] == NULL) {
      continue;
    }
    if(entitys[pos]->GetType() == entity::EntityType::TYPE_ACTOR) {
      actor_data.role_aoi_fields_ = ((GameActor *)entitys[pos])->GetRoleAoiAttributes();
      actor_data.actor_aoi_fields_ = ((GameActor *)(entitys[pos]))->GetActorAoiAttributes();
      actor_data.__set_id_(entitys[pos]->GetStringID());
      actor_data.__set_name_(((GameActor *)(entitys[pos]))->GetName());
      entitys[pos]->GetPosition((core::uint32 &)actor_data.pos_x_, (core::uint32 &)actor_data.pos_y_);
      core::uint8 offset_x = 0, offset_y = 0;
      entitys[pos]->GetPosOffset(offset_x, offset_y);
      if(offset_x != 0 || offset_y != 0) {
        actor_data.__set_offset_x_(offset_x);
        actor_data.__set_offset_y_(offset_y);
      } else {
        actor_data.__isset.offset_x_ = false;
        actor_data.__isset.offset_y_ = false;
      }
      message.actors_.push_back(actor_data);
    } else if(entitys[pos]->GetType() == entity::EntityType::TYPE_NPC) {
      npc_data.__set_id_(entitys[pos]->GetStringID());
      npc_data.__set_template_id_(((GameNpc *)entitys[pos])->GetTemplateID());
      npc_data.role_aoi_fields_ = ((GameNpc *)entitys[pos])->GetRoleAoiAttributes();
      npc_data.npc_aoi_fields_ = ((GameNpc *)entitys[pos])->GetNpcAoiAttributes();
      entitys[pos]->GetPosition((core::uint32 &)npc_data.pos_x_, (core::uint32 &)npc_data.pos_y_);
      message.npcs_.push_back(npc_data);
    } else if(entitys[pos]->GetType() == entity::EntityType::TYPE_SCENE_ITEM) {
      item_data.__set_id_(entitys[pos]->GetStringID());
      item_data.__set_item_aoi_fields_(((SceneItem *)entitys[pos])->GetSceneItemAoiAttribute());
      item_data.__set_owner_type_(((SceneItem *)entitys[pos])->GetOwnerType());
      entitys[pos]->GetPosition((core::uint32 &)item_data.pos_x_, (core::uint32 &)item_data.pos_y_);
      const SceneItem::OwnerVector &owners = ((SceneItem *)entitys[pos])->GetOwners();
      for(SceneItem::OwnerVector::const_iterator iterator = owners.begin();
          iterator != owners.end(); ++iterator) {
        item_data.owner_actors_.push_back(global::ToString(*iterator));
      }
      message.items_.push_back(item_data);
    } else {
      global::LogError("%s:%d (%s) 实体类型错误",
          __FILE__, __LINE__, __FUNCTION__);
      continue;
    }

    entity.__set_type_(entitys[pos]->GetType());
    entity.__set_id_(entitys[pos]->GetID());
    event.roles_.push_back(entity);

    if(message.actors_.size() + message.npcs_.size() + message.items_.size() >= kMaxCreateEntitySize) {
      actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_CREATE_AOI_ROLES);
      message.actors_.clear();
      message.npcs_.clear();
      message.items_.clear();
    }
  }

  if(message.actors_.empty() == false || message.npcs_.empty() == false || message.items_.empty() == false) {
    actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_CREATE_AOI_ROLES);
  }

  // 发送创建视野内角色集事件
  if(event.roles_.empty() == false) {
    event.__set_actor_(actor->GetID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SCENE_CREATE_AOI_ROLES, this->GetSceneID(),
        event::ChannelType::CHANNEL_SCENE, &event, sizeof(event));
  }
}

void Scene::SynchronizeDestory(GameEntity *entitys[], size_t number, GameActor *actor) {
  gateway::protocol::MessageDestoryAoiRoles message;
  for(size_t pos = 0; pos < number; ++pos) {
    if(entitys[pos] == NULL) {
      continue;
    }
    if(entitys[pos]->GetType() == entity::EntityType::TYPE_ACTOR) {
      message.actors_.push_back(entitys[pos]->GetStringID());
    } else if(entitys[pos]->GetType() == entity::EntityType::TYPE_NPC) {
      message.npcs_.push_back(entitys[pos]->GetStringID());
    } else if(entitys[pos]->GetType() == entity::EntityType::TYPE_SCENE_ITEM) {
      message.items_.push_back(entitys[pos]->GetStringID());
    } else {
      global::LogError("%s:%d (%s) 实体类型错误",
          __FILE__, __LINE__, __FUNCTION__);
    }

    if(message.actors_.size() + message.npcs_.size() + message.items_.size() >= kMaxDestoryEntitySize) {
      actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_DESTORY_AOI_ROLES);
      message.actors_.clear();
      message.npcs_.clear();
      message.items_.clear();
    }
  }

  if(message.actors_.empty() == false || message.npcs_.empty() == false || message.items_.empty() == false) {
    actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_DESTORY_AOI_ROLES);
  }
}

bool Scene::AddSceneActor(SceneActor *actor) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 参数 actor 为 NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  ActorHashmap::iterator iterator = this->actors_.find(actor->GetActorID());
  if(iterator != this->actors_.end()) {
    return false;
  }
  this->actors_.insert(std::make_pair(actor->GetActorID(), actor));
  return true;
}

SceneActor *Scene::RemoveSceneActor(core::uint64 id) {
  SceneActor *actor = NULL;
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }
  return actor;
}

SceneActor *Scene::GetSceneActor(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

request::PKCheckResultType::type Scene::PKCheck(GameActor *src_actor, GameActor *dest_actor) {
  if(src_actor == NULL || dest_actor == NULL) {
    return request::PKCheckResultType::ERROR_ARGS;
  }

  // 源在安全区
  core::uint32 cur_x = 0, cur_y = 0;
  src_actor->GetPosition(cur_x, cur_y);

  if(this->CheckSafeArea(cur_x, cur_y)) {
    return request::PKCheckResultType::ERROR_SAFE_AREA;
  }
  // 目标在安全区
  dest_actor->GetPosition(cur_x, cur_y);
  if(this->CheckSafeArea(cur_x, cur_y)) { 
    return request::PKCheckResultType::ERROR_SAFE_AREA;
  }

  // 新手保护  
  if(this->IsRookieProtect()) {
    if(src_actor->GetAttribute(entity::RoleAoiFields::LEVEL) <
        MISC_CONF()->rookie_protect_level_) {   
      return request::PKCheckResultType::ERROR_ROOKIE_PROTECT; 
    }  
    if(dest_actor->GetAttribute(entity::RoleAoiFields::LEVEL) < 
        MISC_CONF()->rookie_protect_level_) {  
      return request::PKCheckResultType::ERROR_ROOKIE_PROTECT; 
    }
  } 

  // 判断死亡保护   
  if(this->IsDieProtect()) {
    if(entity::RoleAfterDieStatus::PROTECT == 
        dest_actor->CheckCommonStatus(entity::RoleCommonStatus::FORBID_BEATEN_MAP) )
      return request::PKCheckResultType::ERROR_DIE_PROTECT_TIME;
  }

  // 判断攻击方式    
  core::int8 pkmode =  (core::int8)src_actor->GetAttribute(entity::ActorClientFields::PK_MODE);  
  core::int32 pkchoose = (core::int32)(src_actor->GetAttribute(entity::ActorClientFields::PK_MODE) &
      0x0000FF00) >> 8; 

  switch(pkmode) {
    case entity::ActorPKModeType::PEACE:
      return request::PKCheckResultType::ERROR_PK_MODE;
    case entity::ActorPKModeType::ATTACK:
      // 不攻击同队
      if(pkchoose & entity::ActorPKChooseType::NOT_ATTACK_TEAMMATE) {
        core::uint64 src_team = FacadeRequest::GetInstance()->GetTeamID(src_actor->GetID());
        core::uint64 dest_team = FacadeRequest::GetInstance()->GetTeamID(dest_actor->GetID());
        if(src_team != 0 && src_team == dest_team) {
          return request::PKCheckResultType::ERROR_PK_MODE; 
        }
      }
      // 不攻击同公会
      if((pkchoose & entity::ActorPKChooseType::NOT_ATTACK_SAME_GUILD)) {
        core::uint64 src_guild = FacadeRequest::GetInstance()->GetGuildID(src_actor->GetID());
        core::uint64 dest_guild = FacadeRequest::GetInstance()->GetGuildID(dest_actor->GetID());
        if(src_guild != 0 && src_guild == dest_guild) {
          return request::PKCheckResultType::ERROR_PK_MODE;
        }
      }
      // 不攻击非褐名
      if(pkchoose & entity::ActorPKChooseType::NOT_ATTACK_NO_BROWN_ACTOR) {
        if(dest_actor->GetBrownName() == false) {
          return request::PKCheckResultType::ERROR_PK_MODE;
        }
      }
      break;
    case entity::ActorPKModeType::BATTLE:
      break;
    default:
      return request::PKCheckResultType::ERROR_ARGS;
  }
  return request::PKCheckResultType::SUCCESS;
}

}  // namespace scene

}  // namespace server

}  // namespace game

