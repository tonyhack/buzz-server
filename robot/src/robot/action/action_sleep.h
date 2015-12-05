#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_SLEEP_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_SLEEP_H

#include <stdint.h> 

#include "robot/action/action.h"

namespace robot {

class ActionSleep : public Action {
public:
    ActionSleep(int ms);
    virtual ~ActionSleep();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    void onSleepTimer(int64_t timer_id);

    int ms_;
    int64_t timer_id_;
};

} // namespace robot

#endif

