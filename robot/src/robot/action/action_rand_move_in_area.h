#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_RAND_MOVE_IN_AREA_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_RAND_MOVE_IN_AREA_H

#include "robot/action/action.h"
#include "robot/position.h"

namespace robot {

class ActionMove;

class ActionRandMoveInArea : public Action {
public:
    ActionRandMoveInArea(const Position &pos, int width, int height);
    virtual ~ActionRandMoveInArea();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    void onMoveComplete(bool success);

    Position pos_;
    int width_;
    int height_;
    ActionMove *action_move_;
};

} // namespace robot

#endif

