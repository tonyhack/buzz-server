#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_SET_VARIABLE_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_SET_VARIABLE_H

#include <string>

#include "robot/action/action.h"

namespace robot {

class ActionSetVariable : public Action {
public:
    ActionSetVariable(const std::string &name, const std::string &value);
    virtual ~ActionSetVariable();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    std::string name_;
    std::string value_;
};

} // namespace robot

#endif

