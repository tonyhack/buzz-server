#include "robot/action/action_modify_variable.h"

#include <cstdlib>
#include <brickred/string_util.h>

#include "robot/logging.h"
#include "robot/actor.h"

namespace robot {

ActionModifyVariable::ActionModifyVariable(const std::string &name,
    const std::string &op, const std::string &value) :
    name_(name), op_(op), value_(value)
{
}

ActionModifyVariable::~ActionModifyVariable()
{
}

Action *ActionModifyVariable::clone()
{
    return new ActionModifyVariable(name_, op_, value_);
}

void ActionModifyVariable::start()
{
    if ("+" == op_ ||
        "-" == op_ ||
        "*" == op_ ||
        "/" == op_ ||
        "%" == op_) {
        int var = ::atoi(actor_->getAutomataVariable(name_).c_str());
        int value = ::atoi(value_.c_str());

        if ("+" == op_) {
            var += value;
        } else if ("-" == op_) {
            var -= value;
        } else if ("*" == op_) {
            var *= value;
        } else if ("/" == op_) {
            var /= value;
        } else if ("%" == op_) {
            var %= value;
        }

        actor_->setAutomataVariable(name_,
            brickred::string_util::toString(var));

    } else {
        LOG_ERROR("invalid op(%s)", op_.c_str());
        return finish(false);
    }

    return finish();
}

void ActionModifyVariable::reset()
{
}

} // namespace robot

