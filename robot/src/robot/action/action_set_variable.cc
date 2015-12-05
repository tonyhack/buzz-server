#include "robot/action/action_set_variable.h"

#include "robot/actor.h"

namespace robot {

ActionSetVariable::ActionSetVariable(const std::string &name,
                                     const std::string &value) :
    name_(name), value_(value)
{
}

ActionSetVariable::~ActionSetVariable()
{
}

Action *ActionSetVariable::clone()
{
    return new ActionSetVariable(name_, value_);
}

void ActionSetVariable::start()
{
    actor_->setAutomataVariable(name_, value_);
    return finish();
}

void ActionSetVariable::reset()
{
}

} // namespace robot

