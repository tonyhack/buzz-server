#include "robot/action/action.h"

#include <cstddef>

namespace robot {

Action::Action() : actor_(NULL)
{
}

Action::~Action()
{
}

void Action::setActor(Actor *actor)
{
    actor_ = actor;
}

void Action::setCompleteCallback(CompleteCallback complete_cb)
{
    complete_cb_ = complete_cb;
}

void Action::finish(bool success)
{
    if (complete_cb_) {
        complete_cb_(success);
    }
}

} // namespace robot

