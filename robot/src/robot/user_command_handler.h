#ifndef BUZZ_ROBOT_ROBOT_USER_COMMAND_HANDLER_H
#define BUZZ_ROBOT_ROBOT_USER_COMMAND_HANDLER_H

#include <string>
#include <brickred/class_util.h>

namespace robot {

class UserCommandHandler {
public:
    UserCommandHandler() {}
    ~UserCommandHandler() {}

    bool parse(const std::string &user_command);

private:
    BRICKRED_NONCOPYABLE(UserCommandHandler)
};

} // namespace robot

#endif

