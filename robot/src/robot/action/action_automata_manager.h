#ifndef BUZZ_ROBOT_ROBOT_ACTION_ACTION_AUTOMATA_MANAGER_H
#define BUZZ_ROBOT_ROBOT_ACTION_ACTION_AUTOMATA_MANAGER_H

#include <string>
#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace robot {

class ActionAutomata;

class ActionAutomataManager {
public:
    bool loadActionAutomatas(const std::string &file);
    ActionAutomata *getActionAutomata(int id);

private:
    BRICKRED_SINGLETON(ActionAutomataManager)

    class Impl;
    brickred::UniquePtr<Impl> pimpl_;
};

} // namespace robot

#endif

