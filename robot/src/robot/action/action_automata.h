#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_AUTOMATA_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_AUTOMATA_H

#include <stdint.h>
#include <cstddef>
#include <string>
#include <vector>
#include <brickred/class_util.h>
#include <brickred/function.h>

namespace robot {

class Action;
class Actor;

class ActionAutomata {
public:
    typedef brickred::Function<void (bool)> CompleteCallback;

    class ActionStep {
    public:
        Action *action_;
        int success_next_step_;
        int failed_next_step_;
    };

    typedef std::vector<ActionStep> ActionStepVector;

    ActionAutomata(int id);
    ~ActionAutomata();
    ActionAutomata *clone();

    bool load(const std::string &file);
    int getId() const { return id_; }

    void setActor(Actor *actor);
    void setCompleteCallback(CompleteCallback complete_cb);
    void start();
    void suspend();
    void resume();

private:
    BRICKRED_NONCOPYABLE(ActionAutomata)

    // must be last call
    void finish(bool success = true);
    void onActionComplete(bool success);
    void schedule();
    void cancelSchedule();
    void onScheduleTimer(int64_t timer_id);

    int id_;
    Actor *actor_;
    int state_;
    CompleteCallback complete_cb_;
    ActionStepVector action_steps_;
    int current_step_;
    int next_step_;
    int64_t schedule_timer_id_;
};

} // namespace robot

#endif

