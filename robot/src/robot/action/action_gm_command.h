#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_GM_COMMAND_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_GM_COMMAND_H

#include <string>

#include "robot/action/action.h"

namespace robot {

class ActionGmCommand : public Action {
public:
    ActionGmCommand(const std::string &command);
    virtual ~ActionGmCommand();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    std::string command_;
};

} // namespace robot

#endif

