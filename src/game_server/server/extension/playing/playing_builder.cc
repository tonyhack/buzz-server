//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-26 09:56:22.
// File name: playing_builder.cc
//
// Description:
// Define class PlayingBuilder.
//

#include "game_server/server/extension/playing/playing_builder.h"

#include "coresh/communicator.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "game_server/server/extension/playing/playing.h"
#include "game_server/server/extension/playing/playing_manager.h"
#include "game_server/server/extension/playing/playing_pool.h"
#include "game_server/server/extension/playing/facade_request.h"
#include "global/logging.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace playing {

Playing *PlayingBuilder::Create(core::uint32 template_id, core::uint64 id) {
  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(template_id);
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", template_id);
    return NULL;
  }

  // 分配对象
  Playing *playing = PlayingPool::GetInstance()->Allocate();
  if(playing == NULL) {
    LOG_ERROR("分配 Playing(%u) 对象失败", template_id);
    return NULL;
  }

  // 初始化
  if(playing->Initialize(template_id, id) == false) {
    LOG_ERROR("初始化 Playing(%u) 失败", template_id);
    PlayingPool::GetInstance()->Deallocate(playing);
    return NULL;
  }

  // 加入管理器
  if(PlayingManager::GetInstance()->Add(playing) == false) {
    LOG_ERROR("Playing(%u) 加入管理器失败", template_id);
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    return NULL;
  }

  // 创建地图场景
  core::uint64 scene = FacadeRequest::GetInstance()->CreateSceneMap(cell->map_);
  if(scene <= 0) {
    LOG_ERROR("创建 场景地图(%u) 失败", cell->map_);
    PlayingManager::GetInstance()->Remove(playing->GetID());
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    return NULL;
  }

  // 加入管理器
  if(PlayingManager::GetInstance()->AddScenePlaying(scene, playing->GetID()) == false) {
    LOG_ERROR("场景(%lu) Playing(%U) 加入管理器失败", scene, template_id);
    FacadeRequest::GetInstance()->DestorySceneMap(scene);
    PlayingManager::GetInstance()->Remove(playing->GetID());
    playing->Finalize();
    PlayingPool::GetInstance()->Deallocate(playing);
    return NULL;
  }

  playing->SetSceneID(scene);

  // 发送副本创建事件
  event::EventPlayingCreate event;
  event.__set_playing_(playing->GetID());
  event.__set_template_(playing->GetTemplateID());
  event.__set_scene_(scene);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_PLAYING_CREATE, playing->GetTemplateID(),
      event::ChannelType::CHANNEL_PLAYING, &event,
      sizeof(event::EventPlayingCreate));

  return playing;
}

void PlayingBuilder::Destory(Playing *playing) {
  if(playing == NULL) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 踢出所有玩家
  playing->KickoutActors();

  // 发送副本销毁事件
  event::EventPlayingDestory event;
  event.__set_playing_(playing->GetID());
  event.__set_template_(playing->GetTemplateID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_PLAYING_DESTORY, playing->GetTemplateID(),
      event::ChannelType::CHANNEL_PLAYING, &event,
      sizeof(event::EventPlayingDestory));

  // 销毁
  core::uint64 scene_id = playing->GetSceneID();
  PlayingManager::GetInstance()->RemoveScenePlaying(scene_id);
  PlayingManager::GetInstance()->Remove(playing->GetID());
  playing->Finalize();
  PlayingPool::GetInstance()->Deallocate(playing);
  if(FacadeRequest::GetInstance()->DestorySceneMap(scene_id) == false) {
    LOG_ERROR("销毁 场景地图(%lu) 失败", scene_id);
    return ;
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

