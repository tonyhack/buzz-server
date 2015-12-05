#include "robot/action/action_check_variable.h"

#include <cstdlib>
#include <brickred/string_util.h>

#include "robot/logging.h"
#include "robot/actor.h"

namespace robot {

ActionCheckVariable::ActionCheckVariable(const std::string &name,
    const std::string &op, const std::string &value) :
    name_(name), op_(op), value_(value)
{
}

ActionCheckVariable::~ActionCheckVariable()
{
}

Action *ActionCheckVariable::clone()
{
    return new ActionCheckVariable(name_, op_, value_);
}

void ActionCheckVariable::start()
{
    if (">" == op_ ||
        "<" == op_ ||
        "==" == op_ ||
        "!=" == op_ ||
        ">=" == op_ ||
        "<=" == op_) {

        int var = 0;
        if ("_actor_current_hp_" == name_) {
            var = actor_->getCurrentHp();
        } else {
            var = ::atoi(actor_->getAutomataVariable(name_).c_str());
        }
        int value = ::atoi(value_.c_str());

        if (">" == op_) {
            return finish(var > value);
        } else if ("<" == op_) {
            return finish(var < value);
        } else if ("==" == op_) {
            return finish(var == value);
        } else if ("!=" == op_) {
            return finish(var != value);
        } else if (">=" == op_) {
            return finish(var >= value);
        } else if ("<=" == op_) {
            return finish(var <= value);
        }

    } else {
        LOG_ERROR("invalid op(%s)", op_.c_str());
        return finish(false);
    }
}

void ActionCheckVariable::reset()
{
}

} // namespace robot

