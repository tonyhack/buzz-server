#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_MODIFY_VARIABLE_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_MODIFY_VARIABLE_H

#include <string>

#include "robot/action/action.h"

namespace robot {

class ActionModifyVariable : public Action {
public:
    ActionModifyVariable(const std::string &name,
                         const std::string &op,
                         const std::string &value);
    virtual ~ActionModifyVariable();
    virtual Action *clone();

    virtual void start();
    virtual void reset();

private:
    std::string name_;
    std::string op_;
    std::string value_;
};

} // namespace robot

#endif

