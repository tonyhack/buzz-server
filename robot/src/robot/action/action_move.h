#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_MOVE_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_MOVE_H

#include <stdint.h> 

#include "robot/action/action.h"
#include "robot/position.h"

namespace robot {

class ActionMove : public Action {
public:
    typedef std::vector<Position> PositionVector;

    ActionMove(const Position &dest_pos);
    virtual ~ActionMove();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    void onActorEventSelfMove(const void *event);
    void onActorEventSelfRelocation(const void *event);
    void onMoveTimer(int64_t timer_id);
    void onFinishTimer(int64_t timer_id);
    void move();

    Position dest_pos_;
    int64_t self_move_ev_token_;
    int64_t self_relocation_ev_token_;
    int64_t timer_id_;
    PositionVector search_path_;
    int path_index_;
    int grid_size_;
    int move_ms_;
};

} // namespace robot

#endif

