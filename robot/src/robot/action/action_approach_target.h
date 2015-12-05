#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_APPROACH_TARGET_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_APPROACH_TARGET_H

#include <stdint.h> 

#include "robot/action/action.h"
#include "robot/position.h"

namespace robot {

class ActionApproachTarget : public Action {
public:
    typedef std::vector<Position> PositionVector;

    ActionApproachTarget(int64_t target_id, int distance);
    virtual ~ActionApproachTarget();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    void onActorEventSelfMove(const void *event);
    void onActorEventSelfRelocation(const void *event);
    void onApproachTimer(int64_t timer_id);
    void approach();

    int64_t target_id_;
    int distance_;
    int64_t self_move_ev_token_;
    int64_t self_relocation_ev_token_;
    int64_t timer_id_;
    PositionVector search_path_;
    int path_index_;
    int grid_size_;
};

} // namespace robot

#endif

