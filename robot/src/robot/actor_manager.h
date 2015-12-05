#ifndef BUZZ_ROBOT_ROBOT_ACTOR_MANAGER_H
#define BUZZ_ROBOT_ROBOT_ACTOR_MANAGER_H

#include <stdint.h>
#include <string>
#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace robot {

class Actor;

class ActorManager {
public:
    bool loadActors(const std::string &file);
    Actor *getActor(int64_t actor_no);

private:
    BRICKRED_SINGLETON(ActorManager)

    class Impl;
    brickred::UniquePtr<Impl> pimpl_;
};

} // namespace robot

#endif

