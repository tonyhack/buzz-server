#include "robot/action/action_rand_move.h"

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

ActionRandMove::ActionRandMove() :
    action_move_(NULL)
{
}

ActionRandMove::~ActionRandMove()
{
    reset();
}

Action *ActionRandMove::clone()
{
    return new ActionRandMove();
}

void ActionRandMove::start()
{
    Map *map = MapManager::getInstance()->getMap(actor_->getMap());
    if (NULL == map) {
        LOG_ERROR("map(%d) not found", actor_->getMap());
        return finish(false);
    }

    for (;;) {
        Position rand_pos(ClientApp::getInstance()->rand(map->getWidth()),
                          ClientApp::getInstance()->rand(map->getHeight()));

        if (map->isWalkable(rand_pos)) {
            UniquePtr<ActionMove> action_move(new ActionMove(rand_pos));
            action_move->setActor(actor_);
            action_move->setCompleteCallback(BRICKRED_BIND_MEM_FUNC(
                &ActionRandMove::onMoveComplete, this));
            action_move_ = action_move.release();
            action_move_->start();
            break;
        }
    }
}

void ActionRandMove::reset()
{
    if (action_move_ != NULL) {
        delete action_move_;
        action_move_ = NULL;
    }
}

void ActionRandMove::onMoveComplete(bool success)
{
    return finish(success);
}

} // namespace robot

