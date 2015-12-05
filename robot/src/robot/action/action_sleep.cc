#include "robot/action/action_sleep.h"

#include "robot/logging.h"
#include "robot/client_app.h"

namespace robot {

ActionSleep::ActionSleep(int ms) :
    ms_(ms), timer_id_(-1)
{
}

ActionSleep::~ActionSleep()
{
    reset();
}

Action *ActionSleep::clone()
{
    return new ActionSleep(ms_);
}

void ActionSleep::start()
{
    timer_id_ = ClientApp::getInstance()->startTimer(ms_,
        BRICKRED_BIND_MEM_FUNC(&ActionSleep::onSleepTimer, this), 1);
}

void ActionSleep::reset()
{
    if (timer_id_ != -1) {
        ClientApp::getInstance()->stopTimer(timer_id_);
        timer_id_ = -1;
    }
}

void ActionSleep::onSleepTimer(int64_t timer_id)
{
    timer_id_ = -1;
    return finish();
}

} // namespace robot

