//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-08 16:29:55.
// File name: request_playing_handler.cc
//
// Description:
// Define class RequestPlayingHandler.
//

#include "game_server/server/extension/playing/request_playing_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_server.h"
#include "game_server/server/error_code.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "game_server/server/extension/playing/playing.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/playing_manager.h"
#include "game_server/server/extension/playing/playing_page.h"
#include "game_server/server/extension/playing/playing_page_pool.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_playing_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/logging.h"
#include "global/configure/configure.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace playing {

RequestPlayingHandler::RequestPlayingHandler() {}
RequestPlayingHandler::~RequestPlayingHandler() {}

bool RequestPlayingHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_SYNCHRONIZE_SCORE,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingSynchronizeScore,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_SYNCHRONIZE_RANKING,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingSynchronizeRanking,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_INITIALIZE_ACTOR,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingInitializeActor,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_COMPLETE,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingComplete,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_ACTOR_COMPLETE,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingActorComplete,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_FAILURE,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingFailure,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_UPDATE_RECORD,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingUpdateRecord,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_UPDATE_RESULT,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingUpdateResult,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_CLEAR_COMPLETE,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingClearComplete,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_INCREASE_COMPLETE,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingIncreaseComplete,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_ACTOR_INCREASE_COMPLETE,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingActorIncreaseComplete,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_AWARD_ACTOR,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingAwardActor,
        this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_PLAYING_ACTOR_PLAYING_ID,
      boost::bind(&RequestPlayingHandler::OnRequestPlayingID,
        this, _1, _2));

  return true;
}

void RequestPlayingHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_SYNCHRONIZE_SCORE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_SYNCHRONIZE_RANKING);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_INITIALIZE_ACTOR);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_COMPLETE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_ACTOR_COMPLETE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_FAILURE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_UPDATE_RECORD);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_UPDATE_RESULT);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_CLEAR_COMPLETE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_INCREASE_COMPLETE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_ACTOR_INCREASE_COMPLETE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_AWARD_ACTOR);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_PLAYING_ACTOR_PLAYING_ID);
}

int RequestPlayingHandler::OnRequestPlayingSynchronizeScore(
    void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestPlayingSynchronizeScore) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingSynchronizeScore *request =
    (request::RequestPlayingSynchronizeScore *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(request->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", request->actor_);
    return -1;
  }

  gateway::protocol::MessagePlayingScoreSynchronize synchronize;
  synchronize.__set_score_(request->score_);
  actor->SendMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_SCORE_SYNCHRONIZE);

  LOG_DEBUG("收到请求 RequestPlayingSynchronizeScore");

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingSynchronizeRanking(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestPlayingSynchronizeRanking) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingSynchronizeRanking *request =
    (request::RequestPlayingSynchronizeRanking *)message;

  if (request->actors_.empty() == false) {
    gateway::protocol::MessagePlayingRandkingSynchronize synchronize;
    synchronize.__set_ranking_(request->ranking_);

    GameServerSingleton::GetInstance().MulticastMessage(synchronize,
        gateway::protocol::MessageType::MESSAGE_PLAYING_RANKING_SYNCHRONIZE,
        (core::uint64 *)&request->actors_[0], request->actors_.size());
  }

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingInitializeActor(
    void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestPlayingInitializeActor) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingInitializeActor *request =
    (request::RequestPlayingInitializeActor *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(request->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", request->actor_);
    return -1;
  }

  Playing *playing = PlayingManager::GetInstance()->Get(actor->GetPlaying());
  if(playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", actor->GetPlaying());
    return -1;
  }

  // 发送初始化消息给玩家
  gateway::protocol::MessagePlayingEnterInitialize initialize;
  initialize.__set_template_(playing->GetTemplateID());
  initialize.__set_spend_time_(request->spend_time_);
  initialize.__set_scores_(request->scores_);
  initialize.__set_datas_(request->datas_);
  actor->SendMessage(initialize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_ENTER_INITIALIZE);

  LOG_DEBUG("收到请求 RequestPlayingInitializeActor");

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingComplete(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestPlayingComplete) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingComplete *request = (request::RequestPlayingComplete *)message;

  Playing *playing = PlayingManager::GetInstance()->Get(request->playing_);
  if (playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", request->playing_);
    return -1;
  }

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(
        playing->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%u) 失败", playing->GetTemplateID());
    return -1;
  }

  if (cell->camp_type_ != entity::PlayingCampType::ACTIVITY) {
    const Playing::ActorSet &actors = playing->GetActors();
    for (Playing::ActorSet::const_iterator iterator = actors.begin();
        iterator != actors.end(); ++iterator) {
      PlayingActor *actor = PlayingActorManager::GetInstance()->Get(*iterator);
      if (actor == NULL) {
        continue;
      }
      PlayingPage *page = actor->GetPage(playing->GetTemplateID());
      if (page == NULL) {
        page = PlayingPagePool::GetInstance()->Allocate();
        if (page == NULL) {
          LOG_ERROR("分配 PlayingPage 失败");
          return -1;
        }
        if (page->Initialize(actor, playing->GetTemplateID()) == false) {
          LOG_ERROR("初始化 PlayingPage 失败");
          PlayingPagePool::GetInstance()->Deallocate(page);
          return -1;
        }
        if (actor->AddPage(page) == false) {
          LOG_ERROR("副本加入列表失败");
          page->Finalize();
          PlayingPagePool::GetInstance()->Deallocate(page);
          return -1;
        }
      }

      // 设置结果
      page->SetResult(request->result_);

      // 请求更新记录
      event::EventPlayingRequestUpdateRecord update_request;
      update_request.__set_actor_(actor->GetID());
      update_request.__set_template_(playing->GetTemplateID());
      update_request.__set_result_(request->result_);
      update_request.__set_record_(page->GetRecord());
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_PLAYING_REQUEST_UPDATE_RECORD, actor->GetID(),
          event::ChannelType::CHANNEL_ACTOR, &update_request, sizeof(update_request));

      // 发送副本完成事件
      event::EventPlayingActorFinish finish_event;
      finish_event.__set_actor_(actor->GetID());
      finish_event.__set_template_(playing->GetTemplateID());
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_PLAYING_ACTOR_FINISH, actor->GetID(),
          event::ChannelType::CHANNEL_ACTOR, &finish_event, sizeof(finish_event));

      // 奖励回应
      gateway::protocol::MessagePlayingCompleteSynchronize synchronize;
      synchronize.__set_result_(request->result_);
      if (request->__isset.awards_ == true) {
        synchronize.__set_awards_(request->awards_);
      }
      if (request->__isset.draw_award_ == true) {
        synchronize.__set_draw_award_(request->draw_award_);
      }
      actor->SendMessage(synchronize,
          gateway::protocol::MessageType::MESSAGE_PLAYING_COMPLETE_SYNCHRONIZE);
      // 同步副本数据
      actor->SynchronizePage(playing->GetTemplateID());
    }
  }

  // 请求会话副本结束
  session::protocol::MessagePlayingEndRequest playing_end_session;
  playing_end_session.__set_playing_(playing->GetID());
  SessionChannel::SendMessage(playing_end_session,
      session::protocol::GameMessageType::MESSAGE_PLAYING_END_REQUEST);

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingActorComplete(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestPlayingActorComplete) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingActorComplete *request =
    (request::RequestPlayingActorComplete *)message;

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(request->actor_);
  if (playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", request->actor_);
    return -1;
  }

  if (playing_actor->GetPlaying() != (core::uint64)request->playing_) {
    LOG_ERROR("PlayingActor(%lu) 不在副本(%d)", request->actor_, request->playing_);
    return -1;
  }

  Playing *playing = PlayingManager::GetInstance()->Get(request->playing_);
  if (playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", request->playing_);
    return -1;
  }

  PlayingPage *page = playing_actor->GetPage(playing->GetTemplateID());
  if (page == NULL) {
    page = PlayingPagePool::GetInstance()->Allocate();
    if (page == NULL) {
      LOG_ERROR("分配 PlayingPage 失败");
      return -1;
    }
    if (page->Initialize(playing_actor, playing->GetTemplateID()) == false) {
      LOG_ERROR("初始化 PlayingPage 失败");
      PlayingPagePool::GetInstance()->Deallocate(page);
      return -1;
    }
    if (playing_actor->AddPage(page) == false) {
      LOG_ERROR("加入 PlayingPage 失败");
      page->Finalize();
      PlayingPagePool::GetInstance()->Deallocate(page);
      return -1;
    }
  }

  page->SetResult(request->result_);

  // 请求更新记录
  event::EventPlayingRequestUpdateRecord update_request;
  update_request.__set_actor_(playing_actor->GetID());
  update_request.__set_template_(playing->GetTemplateID());
  update_request.__set_result_(request->result_);
  update_request.__set_record_(page->GetRecord());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_PLAYING_REQUEST_UPDATE_RECORD, playing_actor->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &update_request, sizeof(update_request));

  // 发送副本完成事件
  event::EventPlayingActorFinish finish_event;
  finish_event.__set_actor_(playing_actor->GetID());
  finish_event.__set_template_(playing->GetTemplateID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_PLAYING_ACTOR_FINISH, playing_actor->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &finish_event, sizeof(finish_event));

  // 奖励回应
  gateway::protocol::MessagePlayingCompleteSynchronize synchronize;
  synchronize.__set_result_(request->result_);
  if (request->__isset.awards_ == true) {
    synchronize.__set_awards_(request->awards_);
  }
  if (request->__isset.draw_award_ == true) {
    synchronize.__set_draw_award_(request->draw_award_);
  }
  playing_actor->SendMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_COMPLETE_SYNCHRONIZE);

  LOG_DEBUG("发送 MESSAGE_PLAYING_COMPLETE_SYNCHRONIZE 给玩家(%lu)", playing_actor->GetID());

  // 同步副本数据
  playing_actor->SynchronizePage(playing->GetTemplateID());

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingFailure(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestPlayingFailure) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingFailure *request = (request::RequestPlayingFailure *)message;

  Playing *playing = PlayingManager::GetInstance()->Get(request->playing_);
  if(playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", request->playing_);
    return -1;
  }

  gateway::protocol::MessagePlayingLeaveSynchronize synchronize;
  const Playing::ActorSet &actors = playing->GetActors();
  for (Playing::ActorSet::const_iterator iterator = actors.begin();
      iterator != actors.end(); ++iterator) {
    PlayingActor *actor = PlayingActorManager::GetInstance()->Get(*iterator);
    if(actor) {
      actor->SendMessage(synchronize,
          gateway::protocol::MessageType::MESSAGE_PLAYING_LEAVE_SYNCHRONIZE);
    }
  }

  // 失败直接结束副本
  playing->RequireDestory();

  LOG_DEBUG("收到请求 RequestPlayingFailure");

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingUpdateRecord(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestPlayingUpdateRecord) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingUpdateRecord *request =
    (request::RequestPlayingUpdateRecord *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(request->actor_);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败");
    return -1;
  }

  PlayingPage *page = actor->GetPage(request->playing_template_);
  if (page == NULL) {
    LOG_ERROR("获取 玩家(%d) PlayingPage(%d) 失败",
        request->actor_, request->playing_template_);
    return -1;
  }

  page->SetRecord(request->record_);
  page->Synchronize();

  LOG_DEBUG("收到请求 REQUEST_PLAYING_UPDATE_RECORD, record.values_.size()=%zd",
      request->record_.values_.size());

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingUpdateResult(void *message, size_t size) {
  if (message == NULL || sizeof(request::RequestPlayingUpdateResult) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingUpdateResult *request =
    (request::RequestPlayingUpdateResult *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(request->actor_);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败");
    return -1;
  }

  PlayingPage *page = actor->GetPage(request->playing_template_);
  if (page == NULL) {
    LOG_ERROR("获取 玩家(%d) PlayingPage(%d) 失败",
        request->actor_, request->playing_template_);
    return -1;
  }

  page->SetResult(request->result_);
  page->Synchronize();

  LOG_DEBUG("收到请求 REQUEST_PLAYING_UPDATE_RESULT");

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingClearComplete(
    void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestPlayingClearComplete) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingClearComplete *request =
    (request::RequestPlayingClearComplete *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(request->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", request->actor_);
    return -1;
  }

  PlayingPage *page = actor->GetPage(request->playing_template_);
  if (page == NULL) {
    LOG_ERROR("获取 玩家(%lu) PlayingPage(%d) 失败", request->actor_,
        request->playing_template_);
    return -1;
  }

  entity::PlayingRecordField &record = page->GetRecord();
  record.__set_period_count_(0);

  page->Synchronize();

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingIncreaseComplete(
    void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestPlayingIncreaseComplete) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingIncreaseComplete *request =
    (request::RequestPlayingIncreaseComplete *)message;

  Playing *playing = PlayingManager::GetInstance()->Get(request->playing_);
  if (playing == NULL) {
    LOG_ERROR("获取 Playing(%lu) 失败", request->playing_);
    return -1;
  }

  const Playing::ActorSet &actors = playing->GetActors();
  for (Playing::ActorSet::const_iterator iterator = actors.begin();
      iterator != actors.end(); ++iterator) {
    PlayingActor *actor = PlayingActorManager::GetInstance()->Get(*iterator);
    if (actor == NULL) {
      continue;
    }

    PlayingPage *page = actor->GetPage(playing->GetTemplateID());
    if (page == NULL) {
      LOG_ERROR("获取 玩家(%lu) PlayingPage(%d) 失败", actor->GetID(),
          playing->GetTemplateID());
      return -1;
    }

    entity::PlayingRecordField &record = page->GetRecord();
    ++record.period_count_;

    page->Synchronize();
  }

  LOG_INFO("OnRequestPlayingIncreaseComplete");

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingActorIncreaseComplete(
    void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestPlayingActorIncreaseComplete) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingActorIncreaseComplete *request =
    (request::RequestPlayingActorIncreaseComplete *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(request->actor_);
  if (actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", request->actor_);
    return -1;
  }

  PlayingPage *page = actor->GetPage(request->playing_template_);
  if (page == NULL) {
    page = PlayingPagePool::GetInstance()->Allocate();
    if (page == NULL) {
      LOG_ERROR("分配 PlayingPage 失败");
      return -1;
    }
    if (page->Initialize(actor, request->playing_template_) == false) {
      LOG_ERROR("初始化 PlayingPage 失败");
      PlayingPagePool::GetInstance()->Deallocate(page);
      return -1;
    }
    if (actor->AddPage(page) == false) {
      LOG_ERROR("副本加入列表失败");
      page->Finalize();
      PlayingPagePool::GetInstance()->Deallocate(page);
      return -1;
    }
  }

  entity::PlayingRecordField &record = page->GetRecord();
  ++record.period_count_;

  page->Synchronize();

  LOG_INFO("OnRequestPlayingActorIncreaseComplete");

  return 0;
}

int RequestPlayingHandler::OnRequestPlayingAwardActor(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestPlayingAwardActor) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingAwardActor *request =
    (request::RequestPlayingAwardActor *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(request->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", request->actor_);
    return -1;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", request->actor_);
    return -1;
  }

  // 领取道具奖励
  request::RequestAddItems request_add_items;
  request_add_items.__set_actor_id_(request->actor_);
  entity::AddItemFields field;
  for(size_t pos = 0; pos < request->awards_.size(); ++pos) {
    if(request->awards_[pos].type_ == entity::ResourceType::ITEM) {
      field.__set_item_template_(request->awards_[pos].id_);
      field.__set_number_(request->awards_[pos].number_);
      field.__set_bind_(true);
      request_add_items.items_.push_back(field);
      LOG_DEBUG("玩家(%lu) 副本奖励道具(%d,%d)", request->actor_,
          request->awards_[pos].id_, request->awards_[pos].number_);
    }
  }
  // 发送请求
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEMS,
        &request_add_items, sizeof(request_add_items)) != 0) {
    LOG_ERROR("请求失败 REQUEST_ADD_ITEMS");
    return false;
  }
  // 请求结果
  if(request_add_items.result_ != request::AddItemResult::SUCCESS) {
    if(request_add_items.result_ == request::AddItemResult::PACKET_FULL) {
      LOG_ERROR("增加道具失败，背包已满");
      ErrorCode::Send(game_actor, entity::ErrorType::PACKET_FULL);
      return false;
    } else {
      LOG_ERROR("增加道具失败，错误未知");
      return false;
    }
  }

  // 领取资源奖励
  for(size_t pos = 0; pos < request->awards_.size(); ++pos) {
    if(request->awards_[pos].type_ == entity::ResourceType::RESOURCE) {
      field.__set_item_template_(request->awards_[pos].id_);
      field.__set_number_(request->awards_[pos].number_);
      field.__set_bind_(true);
      if(request->awards_[pos].number_ <= 0) {
        continue;
      }
      game_actor->AddResource((entity::ResourceID::type)request->awards_[pos].id_,
          request->awards_[pos].number_);
      LOG_DEBUG("玩家(%lu) 副本增加资源(%d,%d)",
          request->actor_, request->awards_[pos].id_, request->awards_[pos].number_);
    }
  }

  // 设置已领取奖励
  PlayingPage *page = actor->GetPage(request->playing_template_);
  if(page) {
    if (request->first_pass_award_ == true) {
      page->SetFirstPassAwarded();
    } else {
      page->SetAwarded();
    }
    page->Synchronize();
  }

  LOG_DEBUG("收到请求 REQUEST_PLAYING_AWARD_ACTOR");

  return 0;
}


int RequestPlayingHandler::OnRequestPlayingID(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestPlayingAwardActor) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestPlayingAwardActor *request =
    (request::RequestPlayingAwardActor *)message;

  PlayingActor *actor = PlayingActorManager::GetInstance()->Get(request->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", request->actor_);
    return -1;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", request->actor_);
    return -1;
  }

  // 得到副本ID
  Playing *playing = PlayingManager::GetInstance()->Get(actor->GetPlaying());
  if(playing == NULL) {
    return -1;
  }
  return (core::int32)playing->GetTemplateID();
}

}  // namespace playing

}  // namespace server

}  // namespace game

