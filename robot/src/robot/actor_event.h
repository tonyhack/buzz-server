#ifndef BUZZ_ROBOT_ROBOT_ACTOR_EVENT_H
#define BUZZ_ROBOT_ROBOT_ACTOR_EVENT_H

namespace robot {

struct ActorEventType {
    enum type {
        MIN = 0,

        // normal event               // event param type
        LOGIN_START = 0,              /* NULL */
        LOGIN_FINISH,                 /* NULL */
        LOGOUT,                       /* NULL */
        SELF_MOVE,                    /* NULL */
        SELF_RELOCATION,              /* NULL */

        // forward message event
        FORWARD_MESSAGE_ROLE_MOVE_SYNCHRONIZE,
        FORWARD_MESSAGE_SYNCHRONIZE_ACTOR,
        FORWARD_MESSAGE_ACTOR_RELOCATION,

        MAX
    };
};

} // namespace robot

#endif

