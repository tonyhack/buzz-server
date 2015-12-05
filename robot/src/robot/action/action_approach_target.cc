#include "robot/action/action_approach_target.h"

#include "robot/logging.h"
#include "robot/client_app.h"
#include "robot/actor.h"
#include "robot/actor_sight.h"
#include "robot/actor_event.h"
#include "robot/aoi_entity.h"
#include "robot/map.h"
#include "robot/map_manager.h"
#include "protocol/gateway_protocol_types.h"
#include "protocol/gateway_entity_protocol_types.h"

namespace robot {

ActionApproachTarget::ActionApproachTarget(int64_t target_id, int distance) :
    target_id_(target_id), distance_(distance),
    self_move_ev_token_(-1), self_relocation_ev_token_(-1),
    timer_id_(-1), path_index_(0), grid_size_(0)
{
    if (distance_ < 0) {
        distance_ = 0;
    }
}

ActionApproachTarget::~ActionApproachTarget()
{
    reset();
}

Action *ActionApproachTarget::clone()
{
    return new ActionApproachTarget(target_id_, distance_);
}

void ActionApproachTarget::start()
{
    self_move_ev_token_ = actor_->addEventListener(
        ActorEventType::SELF_MOVE, BRICKRED_BIND_MEM_FUNC(
            &ActionApproachTarget::onActorEventSelfMove, this));
    self_relocation_ev_token_ = actor_->addEventListener(
        ActorEventType::SELF_RELOCATION, BRICKRED_BIND_MEM_FUNC(
            &ActionApproachTarget::onActorEventSelfRelocation, this));

    approach();
}

void ActionApproachTarget::reset()
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
    search_path_.clear();
    path_index_ = 0;
}

void ActionApproachTarget::onActorEventSelfMove(const void *event)
{
    int ms = grid_size_ * 1000 / actor_->getMoveSpeed();

    timer_id_ = ClientApp::getInstance()->startTimer(ms,
        BRICKRED_BIND_MEM_FUNC(
            &ActionApproachTarget::onApproachTimer, this), 1);
}

void ActionApproachTarget::onActorEventSelfRelocation(const void *event)
{
    return finish(false);
}

void ActionApproachTarget::onApproachTimer(int64_t timer_id)
{
    timer_id_ = -1;
    approach();
}

void ActionApproachTarget::approach()
{
    const AoiEntity *target = actor_->getSight().getAoiEntity(target_id_);
    if (NULL == target) {
        LOG_DEBUG("actor(%ld) get target(%ld) failed",
                  actor_->getActorNo(), target_id_);
        return finish(false);
    }

    if (actor_->getPos().getDistance(target->getPos()) <= distance_) {
        return finish();
    }

    // search path 
    // i.   first time search
    // ii.  target is far away from the dest point
    // iii. at the dest pos and target is not there
    if (search_path_.empty() ||
        target->getPos().getDistance(search_path_.back()) >= 5 ||
        path_index_ == (int)search_path_.size()) {

        Map *map = MapManager::getInstance()->getMap(actor_->getMap());
        if (NULL == map) {
            LOG_ERROR("map(%d) not found", actor_->getMap());
            return finish(false);
        }

        search_path_.clear();
        path_index_ = 0;
        grid_size_ = map->getGridSize();
        map->searchPath(actor_->getPos(), target->getPos(), &search_path_);

        if (search_path_.empty()) {
            LOG_ERROR("search path failed", actor_->getMap());
            return finish(false);
        }
    }

    gateway::protocol::MessageActorMove request;
    request.__set_dir_(actor_->getPos().getDirection(
        search_path_[path_index_++]));
    actor_->sendToGateway(request,
        gateway::protocol::MessageType::MESSAGE_ACTOR_MOVE);
}

} // namespace robot

