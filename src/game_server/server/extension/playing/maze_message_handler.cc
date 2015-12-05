#include "game_server/server/extension/playing/maze_message_handler.h"

#include <boost/bind.hpp>

#include "entity/resource_types.h"
#include "global/types.h"
#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "game_server/server/extension/playing/facade_request.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/maze_actor.h"
#include "game_server/server/extension/playing/maze_map_manager.h"

namespace game {

namespace server {

namespace playing {

MazeMessageHandler::MazeMessageHandler() {}
MazeMessageHandler::~MazeMessageHandler() {}

bool MazeMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAZE_MOVE_REQUEST,
      boost::bind(&MazeMessageHandler::OnMessageMazeMoveRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAZE_CONFIRM_MOVE_REQUEST,
      boost::bind(&MazeMessageHandler::OnMessageMazeConfirmMoveRequest, this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAZE_AWARD_REQUEST,
      boost::bind(&MazeMessageHandler::OnMessageMazeAwardRequest, this, _1, _2, _3));

  return true;
}

void MazeMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAZE_MOVE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAZE_CONFIRM_MOVE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAZE_AWARD_REQUEST);
}

static void SendMessageMazeMoveResponseFailed(PlayingActor *playing_actor) {
  gateway::protocol::MessageMazeMoveResponse response;
  response.__set_result_(false);
  playing_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_MAZE_MOVE_RESPONSE);
}

static void SendMessageMazeConfirmMoveResponseFailed(PlayingActor *playing_actor) {
  gateway::protocol::MessageMazeConfirmMoveResponse response;
  response.__set_result_(false);
  playing_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_MAZE_CONFIRM_MOVE_RESPONSE);
}

static void SendMessageMazeAwardResponseFailed(PlayingActor *playing_actor) {
  gateway::protocol::MessageMazeAwardResponse response;
  response.__set_result_(false);
  playing_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_MAZE_AWARD_RESPONSE);
}

void MazeMessageHandler::OnMessageMazeMoveRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取副本玩家对象
  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(id);
  if (NULL == playing_actor) {
    LOG_ERROR("Get PlayingActor(%lu) from PlayingActorManager failed.", id);
    return;
  }
  GameActor *game_actor = playing_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  MazeActor *maze_actor = playing_actor->GetMazeActor();

  // 消息解析
  gateway::protocol::MessageMazeMoveRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageMazeMoveRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_MAZE_MOVE_REQUEST from actor(%lu, %s), dir_=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(), request.dir_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::MAZE_MODULE) == false) {
    LOG_WARNING("MAZE_MODULE is disable.");
    SendMessageMazeMoveResponseFailed(playing_actor);
    return;
  }

  // 检查行动点
  if (maze_actor->GetActionPoint() <= 0) {
    LOG_WARNING("Action point is not enough.");
    SendMessageMazeMoveResponseFailed(playing_actor);
    return;
  }

  core::int32 new_pos = maze_actor->GetNearPosition(request.dir_);
  if (-1 == new_pos) {
    LOG_WARNING("New position is invalid.");
    SendMessageMazeMoveResponseFailed(playing_actor);
    return;
  }

  core::int32 new_pos_grid_id = maze_actor->GetMapGrid(new_pos);
  if (-1 == new_pos_grid_id) {
    LOG_ERROR("New position(%d) grid id is invalid.", new_pos_grid_id);
    SendMessageMazeMoveResponseFailed(playing_actor);
    return;
  }

  // 发送成功回复
  gateway::protocol::MessageMazeMoveResponse response;
  response.__set_result_(true);
  response.__set_grid_id_(new_pos_grid_id);
  playing_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_MAZE_MOVE_RESPONSE);
}

void MazeMessageHandler::OnMessageMazeConfirmMoveRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取副本玩家对象
  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(id);
  if (NULL == playing_actor) {
    LOG_ERROR("Get PlayingActor(%lu) from PlayingActorManager failed.", id);
    return;
  }
  GameActor *game_actor = playing_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  MazeActor *maze_actor = playing_actor->GetMazeActor();

  // 消息解析
  gateway::protocol::MessageMazeConfirmMoveRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageMazeConfirmMoveRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_MAZE_CONFIRM_MOVE_REQUEST from actor(%lu, %s), dir=[%d].",
           game_actor->GetID(), game_actor->GetName().c_str(), request.dir_);

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::MAZE_MODULE) == false) {
    LOG_WARNING("MAZE_MODULE is disable.");
    SendMessageMazeConfirmMoveResponseFailed(playing_actor);
    return;
  }

  // 检查行动点
  if (maze_actor->GetActionPoint() <= 0) {
    LOG_WARNING("Action point is not enough.");
    SendMessageMazeConfirmMoveResponseFailed(playing_actor);
    return;
  }

  core::int32 new_pos = maze_actor->GetNearPosition(request.dir_);
  if (-1 == new_pos) {
    LOG_WARNING("New position is invalid.");
    SendMessageMazeConfirmMoveResponseFailed(playing_actor);
    return;
  }

  core::int32 new_pos_grid_id = maze_actor->GetMapGrid(new_pos);
  if (-1 == new_pos_grid_id) {
    LOG_ERROR("New position(%d) grid id is invalid.", new_pos_grid_id);
    SendMessageMazeConfirmMoveResponseFailed(playing_actor);
    return;
  }

  bool pos_explored = maze_actor->CheckPositionExlored(new_pos);
  core::int32 bonus_id = -1;
  core::int32 random_number = 0;

  // 扣除行动点
  maze_actor->DecreaseActionPoint(1);
  // 移动
  maze_actor->Move(new_pos);

  // 奖励和惩罚
  if (pos_explored == false) {
    // 随机奖励
    core::uint32 random =
      ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);
    const global::configure::MazeGridBonusCell *cell =
      MAZE_CONF()->GetRandomMazeGridBonus(new_pos_grid_id, random);
    if (cell != NULL) {
      bonus_id = cell->bonus_id_;
      random_number =
        ExtensionManager::GetInstance()->GetRandom().Uniform(cell->min_number_, cell->max_number_);

      if (global::configure::MazeGridBonusCell::BonusType::RESOURCE == cell->type_) {
        // 增加资源
        game_actor->AddResource((entity::ResourceID::type)cell->id_, random_number);

      } else if (global::configure::MazeGridBonusCell::BonusType::ITEM == cell->type_) {
        // 增加道具
        FacadeRequest::GetInstance()->AddPacketItem(id, cell->id_, random_number,
            entity::BindRequireType::BIND);

      } else if (global::configure::MazeGridBonusCell::BonusType::BUFF == cell->type_) {
        // 增加BUFF
        FacadeRequest::GetInstance()->AddBuff(id, cell->id_);

      } else if (global::configure::MazeGridBonusCell::BonusType::ADD_ACTION_POINT == cell->type_) {
        // 增加行动点
        maze_actor->AddActionPoint(random_number);

      } else if (global::configure::MazeGridBonusCell::BonusType::DECREASE_ACTION_POINT == cell->type_) {
        // 减少行动点
        maze_actor->DecreaseActionPoint(random_number);

      } else if (global::configure::MazeGridBonusCell::BonusType::MOVE_BACK1 == cell->type_) {
        // 倒退1
        maze_actor->MoveBack(1);

      } else if (global::configure::MazeGridBonusCell::BonusType::MOVE_BACK2 == cell->type_) {
        // 倒退2
        maze_actor->MoveBack(2);

      } else if (global::configure::MazeGridBonusCell::BonusType::MOVE_TO_START_GRID == cell->type_) {
        // 回到起点
        maze_actor->MoveToStartPosition();
      }
    }
  }

  // 发送成功回复
  gateway::protocol::MessageMazeConfirmMoveResponse response;
  response.__set_result_(true);
  if (pos_explored == false) {
    response.__set_grid_id_(new_pos_grid_id);
    response.__set_bonus_id_(bonus_id);
    response.__set_number_(random_number);
  }
  playing_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_MAZE_CONFIRM_MOVE_RESPONSE);

  // 判断是否移动到下一层
  if (maze_actor->GetX() == MISC_CONF()->maze_width_ - 1 &&
      maze_actor->GetY() == MISC_CONF()->maze_height_ - 1) {
    if (MazeMapManager::GetInstance()->GetMap(maze_actor->GetCurrentFloor() + 1) != NULL) {
      maze_actor->EnterNextFloor(maze_actor->GetCurrentFloor() + 1);
    } else{
      maze_actor->AddGiftNumber();
      maze_actor->EnterNextFloor(1);
    }
  }
}

void MazeMessageHandler::OnMessageMazeAwardRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取副本玩家对象
  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(id);
  if (NULL == playing_actor) {
    LOG_ERROR("Get PlayingActor(%lu) from PlayingActorManager failed.", id);
    return;
  }
  GameActor *game_actor = playing_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  MazeActor *maze_actor = playing_actor->GetMazeActor();

  // 消息解析
  gateway::protocol::MessageMazeAwardRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageMazeAwardRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_MAZE_AWARD_REQUEST from actor(%lu, %s).",
           game_actor->GetID(), game_actor->GetName().c_str());

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::MAZE_MODULE) == false) {
    LOG_WARNING("MAZE_MODULE is disable.");
    SendMessageMazeAwardResponseFailed(playing_actor);
    return;
  }

  // 检查礼包数量
  if (maze_actor->GetGiftNumber() <= 0) {
    LOG_WARNING("Gift number is not enough.");
    SendMessageMazeAwardResponseFailed(playing_actor);
    return;
  }

  // 检查是否可以添加道具
  if (FacadeRequest::GetInstance()->CheckAddPacketItem(id,
          MISC_CONF()->maze_gift_item_id_, 1, entity::BindRequireType::BIND) == false) {
    LOG_WARNING("Can not add item.");
    SendMessageMazeAwardResponseFailed(playing_actor);
    return;
  }

  // 扣除消耗
  maze_actor->DecreaseGiftNumber();

  // 增加道具
  FacadeRequest::GetInstance()->AddPacketItem(id,
      MISC_CONF()->maze_gift_item_id_, 1, entity::BindRequireType::BIND);
}

}  // namespace playing

}  // namespace server

}  // namespace game

