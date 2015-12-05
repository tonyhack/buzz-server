#include "robot/action/action_gm_command.h"

#include "robot/logging.h"
#include "robot/actor.h"
#include "protocol/gateway_protocol_types.h"
#include "protocol/gateway_gm_protocol_types.h"

namespace robot {

ActionGmCommand::ActionGmCommand(const std::string &command) :
    command_(command)
{
}

ActionGmCommand::~ActionGmCommand()
{
    reset();
}

Action *ActionGmCommand::clone()
{
    return new ActionGmCommand(command_);
}

void ActionGmCommand::start()
{
    gateway::protocol::MessageGmCommand request;
    request.__set_command_(command_);
    actor_->sendToGateway(request,
        gateway::protocol::MessageType::MESSAGE_GM_COMMAND);

    return finish();
}

void ActionGmCommand::reset()
{
}

} // namespace robot

