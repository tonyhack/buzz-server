#include "game_server/server/extension/scene/zazen_actor.h"

#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/configure.h"
#include "global/common_functions.h"
#include "global/configure/configure.h"
#include "global/types.h"

namespace game {

namespace server {

namespace scene {

ZazenActor::ZazenActor() {}
ZazenActor::~ZazenActor() {}

bool ZazenActor::Initialize(SceneActor *actor) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 初始化失败，参数为NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->actor_ = actor;

  return true;
}

void ZazenActor::Finalize() {
  // 结束时删除复活定时器
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_IMCOME, this);
}

void ZazenActor::OnTimer(core::uint32 id) {
  if(id == TIMER_IMCOME) {
    // 处理收益
    GameActor *actor = this->GetGameActor();
    if(actor == NULL) {
      LOG_ERROR("玩家对象没有找到");
      return ;
    }
    // 获取玩家是否在防沉迷状态
    if(actor->GetAttribute(entity::ActorClientFields::FCM_STATUS) != entity::FcmStatusType::NORMAL) {
      return ;
    }
    const game::server::AccordingLevelCell *cell = 
      game::server::Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(actor->GetAttribute(
            entity::RoleAoiFields::LEVEL));
    if(cell == NULL) {
      LOG_ERROR("配置 AccordingLevelCell 没有找到(%d)", actor->GetAttribute(entity::RoleAoiFields::LEVEL));
      return ;
    }
    // 得到玩家所在的场景
    Scene *scene = this->actor_->GetScene();
    if(scene == NULL) {
      LOG_ERROR("玩家所在的场景不存在");
      return ;
    }
    const ServerMapConfigure *server_map = Configure::GetInstance()->GetServerMapConfigure();
    const MapInfo *info_cell = server_map->GetMapInfo(scene->GetMapID());
    if(info_cell == NULL) {
      LOG_ERROR("玩家所在的场景地图(%d)配置没有找到", scene->GetMapID());
      return ;
    }
    int exp_value = cell->zazen_add_nimbus_ + cell->zazen_add_nimbus_ * info_cell->zazen_coe_add_/100;
    int nimbus_value = cell->zazen_add_exp_ + cell->zazen_add_exp_ *info_cell->zazen_coe_add_/100;
    exp_value = exp_value*(global::kPercentDenom + actor->GetAttribute(
          entity::ActorServerFields::KILL_MOB_AND_ZAZEN_EXP_ADD_PERCENT))/global::kPercentDenom;
    nimbus_value = nimbus_value*(global::kPercentDenom + actor->GetAttribute(
          entity::ActorServerFields::KILL_MOB_AND_ZAZEN_NIMBUS_ADD_PERCENT))/global::kPercentDenom;
    actor->AddResource(entity::ResourceID::EXP, exp_value);
    actor->AddResource(entity::ResourceID::NIMBUS, nimbus_value);
    // 通知客户端
    gateway::protocol::MessageSceneZazenIncomeNotice notice;
    notice.__set_exp_(exp_value);
    notice.__set_nimbus_(nimbus_value);
    actor->SendMessage(notice,
        gateway::protocol::MessageType::MESSAGE_SCENE_ZAZEN_INCOME_NOTICE);
  }
}

bool ZazenActor::StartTime() {
  // 获取玩家等级
  GameActor *game_actor = this->GetGameActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家没有找到");
    return false;
  }

  // 判断是否处于幻化状态
  if(entity::RoleCommonStatus::SOUL & game_actor->GetAttribute(entity::RoleAoiFields::COMMON_STATUS)) {
    LOG_ERROR("处于幻化状态的玩家不可打坐");
    return false;
  }

  if(game_actor->GetBasicStatus() == entity::RoleBasicStatus::STATUS_ZAZEN) {
    LOG_ERROR("当前处于打坐状态");
    return false;
  }

  if(game_actor->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
    LOG_ERROR("当前处于死亡状态");
    return false;
  }

  game_actor->SetBasicStatus(entity::RoleBasicStatus::STATUS_ZAZEN);

  GameServerSingleton::GetInstance().RemoveTimer(TIMER_IMCOME, this);
  // 获取配置
  GameServerSingleton::GetInstance().AddTimer(TIMER_IMCOME,
      MISC_CONF()->zazen_revenue_time_ * 1000, -1, this, "ZazenActor::OnTime");

  return true;
}

void ZazenActor::StopTime() {
  // 获取玩家等级
  GameActor *game_actor = this->GetGameActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家没有找到");
    return ;
  }

  GameServerSingleton::GetInstance().RemoveTimer(TIMER_IMCOME, this);
}

GameActor* ZazenActor::GetGameActor() {
  if(this->actor_ == NULL) {
    LOG_ERROR("场景玩家没有找到");
    return NULL;
  }
  return this->actor_->GetActor();
}

}  // namespace scene

}  // namespace server

}  // namespace game

