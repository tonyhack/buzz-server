#include "robot/action/action_check_in_area.h"

#include "robot/actor.h"

namespace robot {

ActionCheckInArea::ActionCheckInArea(const Position &pos,
                                     int width, int height) :
    pos_(pos), width_(width), height_(height)
{
    if (width_ < 0)  {
        width_ = 0;
    }
    if (height_ < 0) {
        height_ = 0;
    }
}

ActionCheckInArea::~ActionCheckInArea()
{
}

Action *ActionCheckInArea::clone()
{
    return new ActionCheckInArea(pos_, width_, height_);
}

void ActionCheckInArea::start()
{
    const Position &actor_pos = actor_->getPos();

    if (actor_pos.x_ >= pos_.x_ && actor_pos.x_ <= pos_.x_ + width_ &&
        actor_pos.y_ >= pos_.y_ && actor_pos.y_ <= pos_.y_ + height_) {
        return finish(true);
    } else {
        return finish(false);
    }
}

void ActionCheckInArea::reset()
{
}

} // namespace robot

