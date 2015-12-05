//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-17 15:34:30.
// File name: request_add_threat.cc
//
// Description:
// Define RequestAddThreat.
//

#include "game_server/server/extension/ai/request_add_threat.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/extension/ai/black_board_manager.h"
#include "game_server/server/extension/ai/ai_actor.h"
#include "game_server/server/extension/ai/ai_actor_manager.h"
#include "game_server/server/request/game_ai_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

RequestAddThreat::RequestAddThreat() {}
RequestAddThreat::~RequestAddThreat() {}

bool RequestAddThreat::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_AI_ADD_THREAT,
      boost::bind(&RequestAddThreat::OnRequest, this, _1, _2));

  return true;
}

void RequestAddThreat::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_AI_ADD_THREAT);
}

int RequestAddThreat::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestAiAddThreat) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestAiAddThreat *request = (request::RequestAiAddThreat *)message;

  if(entity::EntityType::TYPE_ACTOR == request->target_type_ && 
       entity::EntityType::TYPE_ACTOR == request->src_type_) {
    AiActor *ai_actor = AiActorManager::GetInstance()->Get(request->target_id_);
    if(ai_actor == NULL) {
      LOG_ERROR("玩家 AI 对象没有找到(%d)", request->target_id_);
      return -1;
    }
    const AiActor::NpcSet &npcs = ai_actor->GetNpcs();
    for(AiActor::NpcSet::iterator itr = npcs.begin();
        itr != npcs.end(); ++itr) {
      BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(*itr);
      if(black_board != NULL) {
         black_board->GetThreat().Add(request->src_id_, request->threat_value_);
         black_board->SetTarget(entity::EntityType::TYPE_ACTOR, 
            black_board->GetThreat().GetMaxThreatActor());
      }
    }
  }
  
  if(entity::EntityType::TYPE_NPC == request->target_type_ && 
      entity::EntityType::TYPE_ACTOR == request->src_type_) {
    AiActor *ai_actor = AiActorManager::GetInstance()->Get(request->src_id_);
    if(ai_actor == NULL) {
       LOG_ERROR("玩家 AI 对象没有找到(%d)", request->src_id_);
       return -1;
    }
    GameNpc *game_npc = GameNpcManager::GetInstance()->Get(request->target_id_);
    if (game_npc == NULL) {
      LOG_ERROR("获取 GameNpc(%lu) 失败", request->target_id_);
      return -1;
    }
    // 护送类NPC不加仇恨
    if (game_npc->GetNpcType() == entity::NpcKind::TRANSPORT) {
      return 0;
    }
    if(ai_actor->GetNpc(request->target_id_) == false) {
       ai_actor->AddNpc(request->target_id_);
    }
    BlackBoard *black_board = BlackBoardManager::GetInstance()->GetNpc(request->target_id_);
    if(black_board != NULL && black_board->CheckBackStatus() == false) {
       black_board->GetThreat().Add(request->src_id_, request->threat_value_);
       black_board->SetTarget(entity::EntityType::TYPE_ACTOR,
          black_board->GetThreat().GetMaxThreatActor());
    }
  }

  return 0;
}

}  // namespace ai

}  // namespace server

}  // namespace game

