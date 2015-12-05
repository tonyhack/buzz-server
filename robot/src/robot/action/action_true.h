#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_TRUE_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_TRUE_H

#include "robot/action/action.h"

namespace robot {

class ActionTrue : public Action {
public:
    ActionTrue() {}
    virtual ~ActionTrue() {}
    virtual Action *clone() { return new ActionTrue(); }

    virtual void start() { finish(); }
    virtual void reset() {}
};

} // namespace robot

#endif

