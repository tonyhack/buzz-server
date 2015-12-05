//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-24 10:41:47.
// File name: scene_npc.cc
//
// Description:
// Define class SceneNpc.
//

#include "game_server/server/extension/scene/scene_npc.h"

#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_item.h"
#include "game_server/server/extension/scene/scene_item_manager.h"
#include "game_server/server/extension/scene/scene_item_pool.h"
#include "game_server/server/npc_configure.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/configure/configure.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace scene {

SceneNpc::SceneNpc() : scene_(NULL), npc_(NULL),
  need_reborn_(false), reborn_secs_(0), born_dir_(entity::DirectionType::MAX) {}

SceneNpc::~SceneNpc() {}

bool SceneNpc::Initialize(GameNpc *npc, bool need_reborn, core::uint32 reborn_secs) {
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 参数 npc 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  const NpcCell *cell = Configure::GetInstance()->GetNpcConfigure().GetNpc(
      npc->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("找不到 NpcCell(%d)", npc->GetTemplateID());
    return false;
  }

  if(cell->owner_type_ == entity::NpcOwnerType::ALL) {
    this->ownership_ = &ownership_all_;
  }
  else if(cell->owner_type_ == entity::NpcOwnerType::KILL) {
    this->ownership_ = &this->ownership_kill_;
  } else if(cell->owner_type_ == entity::NpcOwnerType::DAMAGE) {
    this->ownership_ = &this->ownership_damage_;
  } else if(cell->owner_type_ == entity::NpcOwnerType::CAMP_COMPETE) {
    this->ownership_ = &this->ownership_camp_compete_; 
  } else if(cell->owner_type_ == entity::NpcOwnerType::CAMP_EACH) {
    this->ownership_ = &this->ownership_camp_each_;
  } else {
    LOG_ERROR("归属类型错误");
    return false;
  }

  if(this->ownership_->Initialize(this) == false) {
    LOG_ERROR("初始化 Ownership 失败");
    return false;
  }

  this->npc_ = npc;

  // 设置出生坐标
  this->npc_->SetBornPosition(npc->GetPosx(), npc->GetPosy());
  //  设置出生面向
  this->born_dir_ = (entity::DirectionType::type)this->npc_->GetAttribute(
      entity::RoleAoiFields::DIRECTION);
  // 重生时间
  this->reborn_secs_ = reborn_secs;
  this->need_reborn_ = need_reborn;

  this->forbid_drop_items_ = false;

  return true;
}

void SceneNpc::Finalize() {
  this->ownership_->Finalize();
  this->npc_ = NULL;
  this->scene_ = NULL;
  this->need_reborn_ = false;
}

static void BuildSceneItem(Scene *scene, core::int32 x, core::int32 y,
    const DropElement &element, entity::NpcOwnerType::type owner_type, core::uint64 owner_id) {
  SceneItem *item = SceneItemPool::GetInstance()->Allocate();
  if(item == NULL) {
    LOG_ERROR("分配 SceneItem 失败");
    return;
  }

  if(item->Initialize(element.resource_id_) == false) {
    LOG_ERROR("初始化 SceneItem 失败");
    SceneItemPool::GetInstance()->Deallocate(item);
    return;
  }

  item->SetScene(scene->GetSceneID());
  item->SetItemScene(scene);
  item->SetPosition(x, y);
  item->SetOwnerType(owner_type);
  if(owner_id > 0) {
    item->AddOwner(owner_id, false);
  }
  item->SetAttribute(entity::SceneItemAoiFields::TYPE, element.resource_type_, false);
  item->SetAttribute(entity::SceneItemAoiFields::TEMPLATE, element.resource_id_, false);
  item->SetAttribute(entity::SceneItemAoiFields::BIND, element.bind_, false);
  item->SetAttribute(entity::SceneItemAoiFields::NUMBER, element.number_, false);

  if(SceneItemManager::GetInstance()->Add(item) == false) {
    LOG_ERROR("SceneItem 加入管理器失败");
    item->Finalize();
    SceneItemPool::GetInstance()->Deallocate(item);
    return;
  }

  if(scene->AddEntity(item) == false) {
    LOG_ERROR("SceneItem 加入场景失败");
    SceneItemManager::GetInstance()->Remove(item->GetID());
    item->Finalize();
    SceneItemPool::GetInstance()->Deallocate(item);
    return;
  }

  return;
}

void SceneNpc::DropItemsOwner(core::uint64 owner) {
  core::int32 npc_template = this->npc_->GetTemplateID();

  const NpcCell *cell =
    game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(npc_template);
  if(cell == NULL) {
    LOG_ERROR("获取 NpcCell(%d) 失败", npc_template);
    return;
  }

  core::int32 x = this->npc_->GetPosx();
  core::int32 y = this->npc_->GetPosy();

  const LoadDropConfigure::DropElementVector &drops =
    game::server::Configure::GetInstance()->Drop(cell->drop_id_);
  LoadDropConfigure::DropElementVector::const_iterator iterator = drops.begin();
  if(iterator == drops.end()) {
    return;
  }

  for(core::uint32 step = 0; step < 20; ++step) {
    for(core::uint32 count = 0; count <= step; ++count) {
      (step % 2) == 0 ? ++y : --y;
      if(this->scene_->CheckAddSceneItem(x, y) == true) {
        BuildSceneItem(this->scene_, x, y, *iterator, this->ownership_->GetType(), owner);
        if(++iterator == drops.end()) {
          return;
        }
      }
    }
    for(core::uint32 count = 0; count <= step; ++count) {
      (step % 2) == 0 ? ++x : --x;
      if(this->scene_->CheckAddSceneItem(x, y) == true) {
        BuildSceneItem(this->scene_, x, y, *iterator, this->ownership_->GetType(), owner);
        if(++iterator == drops.end()) {
          return;
        }
      }
    }
  } 
}

void SceneNpc::DropItems(GameRole *owner) {
  if(owner == NULL || this->npc_ == NULL || this->scene_ == NULL) {
    LOG_ERROR("参数错误");
    return;
  }

  // 禁止掉落
  if (this->GetForbidDropItems() == true) {
    return;
  }

  // 防沉迷惩罚
  if(owner->GetType() == entity::EntityType::TYPE_ACTOR) {
    GameActor *game_killer = (GameActor *)owner;

    if(game_killer->GetAttribute(entity::ActorClientFields::FCM_STATUS) ==
           entity::FcmStatusType::HALF_INCOME) {
      if (ExtensionManager::GetInstance()->GetRandom().Random() % 2 == 0) {
        return;
      }
    } else if(game_killer->GetAttribute(entity::ActorClientFields::FCM_STATUS) == 
                  entity::FcmStatusType::ZERO_INCOME) {
      return;
    }
  }

  core::uint64 value = this->ownership_->GetValue();
  core::uint64 owner_actor = 0;
  if(this->ownership_->GetType() == entity::NpcOwnerType::ALL) {
    this->DropItemsOwner(0);
  } else if(this->ownership_->GetType() == entity::NpcOwnerType::KILL) {
    this->DropItemsOwner(value);
  } else if(this->ownership_->GetType() == entity::NpcOwnerType::DAMAGE) {
    this->DropItemsOwner(value);
  } else if(this->ownership_->GetType() == entity::NpcOwnerType::CAMP_COMPETE) {
    this->DropItemsOwner(value);
  } else if(this->ownership_->GetType() == entity::NpcOwnerType::CAMP_EACH) {
    // 获取同阵营的玩家
    request::RequestSceneGetAroundActors request;
    request.__set_type_(entity::EntityType::TYPE_NPC);
    request.__set_id_(this->npc_->GetID());
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_SCENE_GET_AROUND_ACTORS,
          &request, sizeof(request)) == -1) {
      LOG_ERROR("获取NPC(%d)周围玩家列表失败", this->npc_->GetTemplateID());
      return ;
    }
    core::int32 max_drop = 0;
    for(size_t i = 0; i < request.actors_.size(); ++i) {
      GameActor *game_actor = GameActorManager::GetInstance()->GetActor(request.actors_[i]);
      if(game_actor != NULL && game_actor->GetAttribute(entity::RoleAoiFields::CAMP) == (core::int32)value) {
        this->DropItemsOwner(request.actors_[i]);
        if(max_drop > MISC_CONF()->drop_item_owner_max_) {
          break;
        }
        ++max_drop;
      }
    }
  } else {
    LOG_ERROR("NPC(%d)掉落拥有者类型错误", this->npc_->GetTemplateID());
    return ;
  }

}

void SceneNpc::LoseExp(GameRole *owner) {
  // 加玩家经验
  if(owner->GetType() != entity::EntityType::TYPE_ACTOR) {
    LOG_ERROR("加成经验失败");
    return ;
  }
  GameActor *game_killer = (GameActor *)owner;
  if(game_killer == NULL || this->npc_ == NULL) {
    LOG_ERROR("对象没有找到");
    return ;
  }
  // 获取经验配置
  const NpcCell *cell = game::server::Configure::GetInstance()->GetNpcConfigure().GetNpc(
      this->npc_->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("NPC 配置没有找到[%d]",this->npc_->GetTemplateID());
    return ;
  }
  core::int32 level_span = this->npc_->GetAttribute(entity::RoleAoiFields::LEVEL) - 
    game_killer->GetAttribute(entity::RoleAoiFields::LEVEL);

  core::int32 vip_exp_percent = 0;
  // 得到玩家VIP配置
  const global::configure::VipCell *vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)game_killer->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::SCENE_KILL_NPC_UP_EXP);
  if (vip_cell != NULL) {
    vip_exp_percent = vip_cell->value1_;
  }

  core::int32 exp = 0;
  if(level_span <= -10) {
  } else if(level_span >= -9 && level_span <= -4) {
    exp = cell->exp_ - cell->exp_*(-level_span)*10/100;
  } else if(level_span >= -3 && level_span <= 3) {
    exp = cell->exp_;
  } else if(level_span >= 4 && level_span <= 49) {
    exp = cell->exp_ - cell->exp_*level_span*2/100;
  } else {

  }
  exp = exp + exp * vip_exp_percent/100;

  // 防沉迷惩罚
  if(game_killer->GetAttribute(entity::ActorClientFields::FCM_STATUS) ==
      entity::FcmStatusType::HALF_INCOME) {
    exp /= 2;
  } else if(game_killer->GetAttribute(entity::ActorClientFields::FCM_STATUS) == 
      entity::FcmStatusType::ZERO_INCOME) {
    exp = 0;
  }

  // 额外加成
  exp = exp*(global::kPercentDenom + game_killer->GetAttribute(
        entity::ActorServerFields::KILL_MOB_AND_ZAZEN_EXP_ADD_PERCENT))/global::kPercentDenom;

  game_killer->AddResource(entity::ResourceID::EXP, exp);
}

void SceneNpc::OnEvent(event::EventType::type type, const void *message, size_t size) {
  this->ownership_->OnEvent(type, message, size);
}

}  // namespace scene

}  // namespace server

}  // namespace game

