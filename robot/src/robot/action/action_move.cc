#include "robot/action/action_move.h"

#include <cstddef>

#include "robot/logging.h"
#include "robot/client_app.h"
#include "robot/actor.h"
#include "robot/actor_event.h"
#include "robot/map.h"
#include "robot/map_manager.h"
#include "protocol/constants_constants.h"
#include "protocol/gateway_protocol_types.h"
#include "protocol/gateway_entity_protocol_types.h"

namespace robot {

ActionMove::ActionMove(const Position &dest_pos) :
    dest_pos_(dest_pos),
    self_move_ev_token_(-1), self_relocation_ev_token_(-1),
    timer_id_(-1), path_index_(0), grid_size_(0), move_ms_(0)
{
}

ActionMove::~ActionMove()
{
    reset();
}

Action *ActionMove::clone()
{
    return new ActionMove(dest_pos_);
}

void ActionMove::start()
{
    if (actor_->getPos() == dest_pos_) {
        return finish();
    }

    self_move_ev_token_ = actor_->addEventListener(
        ActorEventType::SELF_MOVE, BRICKRED_BIND_MEM_FUNC(
            &ActionMove::onActorEventSelfMove, this));
    self_relocation_ev_token_ = actor_->addEventListener(
        ActorEventType::SELF_RELOCATION, BRICKRED_BIND_MEM_FUNC(
            &ActionMove::onActorEventSelfRelocation, this));

    Map *map = MapManager::getInstance()->getMap(actor_->getMap());
    if (NULL == map) {
        LOG_ERROR("map(%d) not found", actor_->getMap());
        return finish(false);
    }

    if (map->isWalkable(dest_pos_) == false) {
        LOG_ERROR("map(%d) position(%d, %d) is not walkable",
                  actor_->getMap(),
                  actor_->getPos().x_, actor_->getPos().y_);
        return finish(false);
    }

    search_path_.clear();
    path_index_ = 0;
    grid_size_ = map->getGridSize();
    map->searchPath(actor_->getPos(), dest_pos_, &search_path_);

    if (search_path_.empty()) {
        LOG_ERROR("search path failed", actor_->getMap());
        return finish(false);
    }

    move();
}

void ActionMove::reset()
{
    if (self_move_ev_token_ != -1) {
        actor_->removeEventListener(self_move_ev_token_);
        self_move_ev_token_ = -1;
    }
    if (self_relocation_ev_token_ != -1) {
        actor_->removeEventListener(self_relocation_ev_token_);
        self_relocation_ev_token_ = -1;
    }
    if (timer_id_ != -1) {
        ClientApp::getInstance()->stopTimer(timer_id_);
        timer_id_ = -1;
    }
}

void ActionMove::onActorEventSelfMove(const void *event)
{
    if (actor_->getPos() == dest_pos_) {
        timer_id_ = ClientApp::getInstance()->startTimer(move_ms_,
            BRICKRED_BIND_MEM_FUNC(&ActionMove::onFinishTimer, this), 1);
    } else {
        timer_id_ = ClientApp::getInstance()->startTimer(move_ms_,
            BRICKRED_BIND_MEM_FUNC(&ActionMove::onMoveTimer, this), 1);
    }
}

void ActionMove::onActorEventSelfRelocation(const void *event)
{
    return finish(false);
}

void ActionMove::onMoveTimer(int64_t timer_id)
{
    timer_id_ = -1;
    move();
}

void ActionMove::onFinishTimer(int64_t timer_id)
{
    timer_id_ = -1;
    return finish();
}

void ActionMove::move()
{
    gateway::protocol::MessageActorMove request;
    request.__set_dir_(actor_->getPos().getDirection(
        search_path_[path_index_++]));
    actor_->sendToGateway(request,
        gateway::protocol::MessageType::MESSAGE_ACTOR_MOVE);

    if (request.dir_ != entity::DirectionType::NONE &&
        entity::g_constants_constants.kDirectionOffsetX[request.dir_] != 0 && 
        entity::g_constants_constants.kDirectionOffsetX[request.dir_] != 0) {
      move_ms_ = grid_size_ * 1414 / actor_->getMoveSpeed();
    } else {
      move_ms_ = grid_size_ * 1000 / actor_->getMoveSpeed();
    }
}

} // namespace robot

