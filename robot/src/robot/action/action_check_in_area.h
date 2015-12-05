#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_CHECK_IN_AREA_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_CHECK_IN_AREA_H

#include "robot/action/action.h"
#include "robot/position.h"

namespace robot {

class ActionCheckInArea : public Action {
public:
    ActionCheckInArea(const Position &pos, int width, int height);
    virtual ~ActionCheckInArea();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    Position pos_;
    int width_;
    int height_;
};

} // namespace robot

#endif

