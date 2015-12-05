//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-05 20:02:52.
// File name: world_boss.cc
//
// Description:
//
#include "session_server/server/extension/scene/world_boss.h"

#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "global/configure/configure.h"
#include "global/configure/load_world_boss_configure.h"
#include "global/logging.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/scene/scene.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/session_server.h"

namespace session {

namespace server {

namespace scene {

WorldBoss::WorldBoss() {}
WorldBoss::~WorldBoss() {}

bool WorldBoss::Initialize(Scene *scene) {
  if(scene == NULL) {
    LOG_ERROR("世界BOSS初始化失败");
    return false;
  }
  this->scene_ = scene;

  if(!this->scene_->CheckDynamic()) {
    this->live_ = false;
  }
  this->boss_template_ = 0;

  // 获取场景ID
  core::uint16 map_id = scene->GetMapID();
  const global::configure::WorldBossCell *cell = 
    WORLD_BOSS_CONF()->GetCell((core::int32)map_id);
  if(cell != NULL) {
    this->boss_template_ = cell->template_id_;
  }
  return true;
}

void WorldBoss::Finalize() {
  this->scene_ = NULL;
  this->live_ = false;
  this->boss_template_ = 0;
}

void WorldBoss::OnWorldBossDie(std::string &killer) {
  this->live_ = false;
  this->killer_ = killer;
  this->BroadCastKillBoss();
}

void WorldBoss::BroadCastKillBoss() {
  if(this->live_) {
    LOG_ERROR("世界BOSS还活着，不能广播");
    return ;
  }
  if(this->boss_template_ == 0) {
    return ;
  }
  gateway::protocol::MessageSceneBossKilled message;
  message.__set_killer_(this->killer_);
  message.__set_template_id_(this->boss_template_);
  SessionServerSingleton::GetInstance().BroadcastMessage(message,
      gateway::protocol::MessageType::MESSAGE_SCENE_BOSS_KILLED);
}

void WorldBoss::BroadCastSummonBoss(core::int32 boss_template, core::int32 x, core::int32 y,
    core::int32 width, core::int32 heigth) {
  if(this->live_) {
    return ;
  }
  if(this->scene_ == NULL) {
    LOG_ERROR("场景对象没有找到");
    return ;
  }
  // 得到game_server
  SessionTerminal *game_server = SessionServerSingleton::GetInstance().GetTerminal(
      global::protocol::ServerType::GAME_SERVER, this->scene_->GetGameServer());
  if(game_server == NULL) {
    LOG_ERROR("游戏场景对象没有找到(%lu)", this->scene_->GetGameServer());
    return ;
  }
  this->live_ = true;
  this->boss_template_ = boss_template;
  session::protocol::MessageSceneSummonWorldBoss message;
  message.__set_heigth_(heigth);
  message.__set_width_(width);
  message.__set_x_(x);
  message.__set_y_(y);
  message.__set_id_(this->scene_->GetID());
  message.__set_template_id_(boss_template);
  game_server->SendGameMessage(message,
      session::protocol::GameMessageType::MESSAGE_SCENE_SUMMON_WORLD_BOSS);
}

} // namespace scene

} // namespace server 

} // namespace session

