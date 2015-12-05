//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-14 16:26:46.
// File name: scene_extension.cc
//
// Description:
// Define class SceneExtension.
//

#include "game_server/server/extension/scene/scene_extension.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/entity_enumerator.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_actor_pool.h"
#include "game_server/server/extension/scene/scene_builder.h"
#include "game_server/server/extension/scene/scene_item_manager.h"
#include "game_server/server/extension/scene/scene_item_pool.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"
#include "game_server/server/extension/scene/scene_npc_pool.h"
#include "game_server/server/extension/scene/scene_pool.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace scene {

SceneExtension::SceneExtension() {}
SceneExtension::~SceneExtension() {}

bool SceneExtension::Initialize() {
  // 服务器配置信息
  const global::GameServerInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameServers(
        GameServerSingleton::GetInstance().GetSignature().id_);
  if(information == NULL) {
    global::LogError("%s:%d (%s) 获取游戏服务器配置信息失败.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(SceneBuilder::GetInstance() == NULL) {
    LOG_ERROR("SceneBuilder 不存在");
    return false;
  }
  // 创建初始化场景管理器
  if(SceneManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 场景管理器对象不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(SceneManager::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 场景管理器初始化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 场景对象池创建与初始化
  if(ScenePool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 场景对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(ScenePool::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 场景对象池初始化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(SceneActorPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 场景玩家对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 初始化场景玩家对象池
  if(SceneActorPool::GetInstance()->Initialize(information->user_pool_initial_,
        information->user_pool_initial_) == false) {
    global::LogError("%s:%d (%s) 场景玩家对象池初始化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 初始化场景NPC管理器
  if(SceneNpcManager::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 场景NPC管理器不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 初始化场景NPC对象池
  if(SceneNpcPool::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 场景NPC对象池不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(SceneNpcPool::GetInstance()->Initialize(information->npc_pool_initial_,
        information->npc_pool_initial_) == false) {
    global::LogError("%s:%d (%s) 场景NPC对象池初始化失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(SceneItemManager::GetInstance() == NULL) {
    LOG_ERROR("SceneItemManager 不存在");
    return false;
  }
  if(SceneItemPool::GetInstance() == NULL) {
    LOG_ERROR("SceneItemPool 不存在");
    return false;
  }
  if(SceneItemPool::GetInstance()->Initialize(information->npc_pool_initial_,
        information->npc_pool_initial_) == false) {
    LOG_ERROR("初始化 SceneItemPool 失败");
    return false;
  }
  // 实体过滤器
  if(EntityFilter::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) EntityFilter 对象不存在",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(EntityFilter::GetInstance()->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 EntityFilter 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 配置文件对象
  if(Configure::GetInstance() == NULL) {
    global::LogError("%s:%d (%s) 场景配置对象创建失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 加载配置文件
  if(Configure::GetInstance()->LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetScenesFile()) == false) {
    global::LogError("%s:%d (%s) 加载场景配置文件失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  const ServerMapConfigure::MapSet *maps = Configure::GetInstance()->GetServerMaps(
      GameServerSingleton::GetInstance().GetSignature().id_);
  if(maps == NULL) {
    LOG_ERROR("获取服务器地图集失败");
    return false;
  }

  ServerMapConfigure::MapSet::const_iterator iterator_map = maps->begin();
  for(; iterator_map != maps->end(); ++iterator_map) {
    Scene *scene = SceneBuilder::GetInstance()->CreateStatic(*iterator_map, 0);
    if(scene == NULL) {
      LOG_ERROR("创建 静态场景地图(%d) 失败", *iterator_map);
      return false;
    }
    LOG_DEBUG("静态场景(map=%d) 建立成功", *iterator_map);
  }

  // 初始化所有事件处理
  if(this->actor_login_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化玩家登陆回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->actor_logout_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化玩家登出回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->actor_load_finish_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化玩家扩展加载完成回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->login_session_event_.Initialize() == false) {
    LOG_ERROR("初始化登陆会话服务器回调失败");
    return false;
  }
  if(this->npc_create_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 npc 创建回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->role_basic_status_change_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化基本状态切换回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->role_killed_event_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化角色被杀死回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->bar_finish_event_.Initialize() == false) {
    LOG_ERROR("初始化 BarFinishEventHandler 失败");
    return false;
  }
  if(this->skill_event_.Initialize() == false) {
    LOG_ERROR("初始化 SkillEventHandler 失败");
    return false;
  }
  if(this->actor_move_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorMoveEventHandler 失败");
    return false;
  }
  if(this->actor_attacked_event_.Initialize() == false) {
    LOG_ERROR("初始化 ActorAttackedEventHandler 失败");
    return false;
  }
  if(this->fighting_status_event_.Initialize() == false) {
    LOG_ERROR("初始化 FightingStatusEventHandler 失败");
    return false;
  }

  // 初始化玩家消息处理回调
  if(this->actor_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化玩家消息处理回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  
  // 初始化玩家世界跳转
  if(this->world_jump_message_handler_.Initialize() == false) {
    LOG_ERROR("初始化 WorldJumpMessageHandler 失败");
    return false;
  }

  // 初始化会话消息处理回调
  if(this->session_message_handler_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化会话消息处理回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }


  // 初始化所有请求处理
  if(this->req_actor_broadcast_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化广播场景所有玩家失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_clear_all_npc_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化清除场景NPC失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_clear_npc_.Initialize() == false) {
    LOG_ERROR("初始化 RequestClearNpc 失败");
    return false;
  }
  if(this->req_get_around_actors_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化获取周围玩家列表失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_role_move_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化角色移动失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_summon_npc_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化召唤 npc 请求回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_scene_create_map_.Initialize() == false) {
    LOG_ERROR("初始化 场景创建地图请求 失败");
    return false;
  }
  if(this->req_scene_jump_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化场景跳转请求回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_scene_pkinfo_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化pk信息请求回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_scene_issafe_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化场景是否安全区请求回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_select_targets_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化选择目标集请求回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->request_router_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化寻路请求回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_scene_pk_check_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化pk检测请求回调失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->req_scene_relocation_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化场景坐标重置回调失败");
    return false;
  }
  if(this->req_scene_revive_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化场景复活请求失败");
    return false;
  }
  if (this->req_scene_forbid_drop_item_.Initialize() == false) {
    LOG_ERROR("初始化 RequestSceneForbidDropItem 失败");
    return false;
  }
  if (this->req_scene_get_npcs_.Initialize() == false) {
    LOG_ERROR("初始化 RequestSceneGetNpcs 失败");
    return false;
  }

  return true;
}

void SceneExtension::Finalize() {
  this->request_router_.Finalize();
  this->req_select_targets_.Finalize();
  this->req_scene_jump_.Finalize();
  this->req_scene_create_map_.Finalize();
  this->req_summon_npc_.Finalize();
  this->req_role_move_.Finalize();
  this->req_get_around_actors_.Finalize();
  this->req_scene_pkinfo_.Finalize();
  this->req_scene_issafe_.Finalize();
  this->req_scene_pk_check_.Finalize();
  this->req_scene_revive_.Finalize();
  this->req_scene_forbid_drop_item_.Finalize();
  this->req_scene_get_npcs_.Finalize();
  this->req_scene_relocation_.Finalize();
  this->req_clear_npc_.Finalize();
  this->req_clear_all_npc_.Finalize();
  this->req_actor_broadcast_.Finalize();

  this->session_message_handler_.Finalize();

  this->actor_message_handler_.Finalize();
  
  this->world_jump_message_handler_.Finalize();
  
  this->skill_event_.Finalize();
  this->bar_finish_event_.Finalize();
  this->role_killed_event_.Finalize();
  this->role_basic_status_change_event_.Finalize();
  this->npc_create_event_.Finalize();
  this->login_session_event_.Finalize();
  this->actor_login_event_.Finalize();
  this->actor_logout_event_.Finalize();
  this->actor_load_finish_event_.Finalize();
  this->actor_attacked_event_.Finalize();
  this->actor_move_event_.Finalize();
  this->fighting_status_event_.Finalize();

  Configure::ReleaseInstance();

  SceneItemPool::GetInstance()->Finalize();
  SceneItemPool::ReleaseInstance();
  SceneItemManager::ReleaseInstance();

  SceneNpcManager::GetInstance()->Finalize();
  SceneNpcManager::ReleaseInstance();

  SceneNpcPool::GetInstance()->Finalize();
  SceneNpcPool::ReleaseInstance();

  SceneActorPool::GetInstance()->Finalize();
  SceneActorPool::ReleaseInstance();

  SceneManager::GetInstance()->Finalize();
  SceneManager::ReleaseInstance();

  ScenePool::GetInstance()->Finalize();
  ScenePool::ReleaseInstance();

  SceneBuilder::ReleaseInstance();
}

void SceneExtension::Start() {}
void SceneExtension::Stop() {}

SceneActor *SceneExtension::GetSceneActor(core::uint64 id) {
  // 玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    return NULL;
  }

  // 场景对象
  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    return NULL;
  }

  // 场景玩家对象
  return scene->GetActor(id);
}

}  // namespace scene

}  // namespace server

}  // namespace game

