#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_RAND_MOVE_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_RAND_MOVE_H

#include "robot/action/action.h"

namespace robot {

class ActionMove;

class ActionRandMove : public Action {
public:
    ActionRandMove();
    virtual ~ActionRandMove();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    void onMoveComplete(bool success);

    ActionMove *action_move_;
};

} // namespace robot

#endif

