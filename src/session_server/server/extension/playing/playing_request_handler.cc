//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-11 11:01:45.
// File name: playing_request_handler.cc
//
// Description:
// Define class PlayingRequestHandler.
//

#include <boost/bind.hpp>

#include "session_server/server/extension_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_playing_request_types.h"
#include "session_server/server/extension/playing/playing.h"
#include "session_server/server/extension/playing/playing_manager.h"

namespace session {

namespace server {

namespace playing {

PlayingRequestHandler::PlayingRequestHandler() {}
PlayingRequestHandler::~PlayingRequestHandler() {}

bool PlayingRequestHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_ACTOR_CONNECT_INFO,
      boost::bind(&PlayingRequestHandler::OnRequestPlayingActorConnectInfo, this, _1, _2));

  return true;
}

void PlayingRequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_ACTOR_CONNECT_INFO);
}

int PlayingRequestHandler::OnRequestPlayingActorConnectInfo(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestPlayingActorConnectInfo) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingActorConnectInfo *request =
    (request::RequestPlayingActorConnectInfo *)message;

  request->__set_ret_playing_(PlayingManager::GetInstance()->GetActorPlaying(
      request->req_actor_));
  if(request->ret_playing_ == 0) {
    return 0;
  }

  // TODO: 这里如果玩家进入活动副本(PlayingCampType::ACTIVITY)，下线，副本结束后上线，
  //       按原来的处理，这里为NULL后，返回错误，会导致玩家登陆失败，暂时处理为找不到
  //       则返回不在副本中，并清除记录，因为副本ID是自增的，所以不会出问题。
  Playing *playing = PlayingManager::GetInstance()->Get(request->ret_playing_);
  if(playing == NULL) {
    PlayingManager::GetInstance()->RemoveActorPlaying(request->req_actor_);
    request->ret_playing_ = 0;
    return 0;
  }

  // 副本状态如果是结束或销毁时，不返回关联
  if(playing->GetStatus() == StatusType::END ||
    playing->GetStatus() == StatusType::DESTORY) {
    request->ret_playing_ = 0;
    return 0;
  }

  request->__set_ret_playing_template_(playing->GetTemplateID());
  request->__set_ret_scene_(playing->GetScene());

  // TODO: 暂时设置为上线回到副本中
  request->__set_ret_online_back_to_(true);

  return 0;
}

}  // namespace playing

}  // namespace server

}  // namespace session

