#include "robot/action/action_rand_move_in_area.h"

#include <cstddef>
#include <brickred/unique_ptr.h>

#include "robot/logging.h"
#include "robot/client_app.h"
#include "robot/actor.h"
#include "robot/position.h"
#include "robot/map.h"
#include "robot/map_manager.h"
#include "robot/action/action_move.h"

namespace robot {

using brickred::UniquePtr;

ActionRandMoveInArea::ActionRandMoveInArea(const Position &pos,
                                           int width, int height) :
    pos_(pos), width_(width), height_(height), action_move_(NULL)
{
    if (width_ < 0)  {
        width_ = 0;
    }
    if (height_ < 0) {
        height_ = 0;
    }
}

ActionRandMoveInArea::~ActionRandMoveInArea()
{
    reset();
}

Action *ActionRandMoveInArea::clone()
{
    return new ActionRandMoveInArea(pos_, width_, height_);
}

void ActionRandMoveInArea::start()
{
    Map *map = MapManager::getInstance()->getMap(actor_->getMap());
    if (NULL == map) {
        LOG_ERROR("map(%d) not found", actor_->getMap());
        return finish(false);
    }

    for (int i = 0; i < 100; ++i) {
        Position rand_pos(ClientApp::getInstance()->uniformRand(pos_.x_, pos_.x_ + width_),
                          ClientApp::getInstance()->uniformRand(pos_.y_, pos_.y_ + height_));

        if (map->isWalkable(rand_pos)) {
            UniquePtr<ActionMove> action_move(new ActionMove(rand_pos));
            action_move->setActor(actor_);
            action_move->setCompleteCallback(BRICKRED_BIND_MEM_FUNC(
                &ActionRandMoveInArea::onMoveComplete, this));
            action_move_ = action_move.release();
            action_move_->start();
            break;
        }
    }
}

void ActionRandMoveInArea::reset()
{
    if (action_move_ != NULL) {
        delete action_move_;
        action_move_ = NULL;
    }
}

void ActionRandMoveInArea::onMoveComplete(bool success)
{
    return finish(success);
}

} // namespace robot

