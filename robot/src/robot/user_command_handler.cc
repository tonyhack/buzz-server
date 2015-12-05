#include "robot/user_command_handler.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <brickred/unique_ptr.h>
#include <brickred/string_util.h>

#include "robot/logging.h"
#include "robot/actor.h"
#include "robot/actor_manager.h"
#include "robot/action/action_automata.h"
#include "robot/action/action_automata_manager.h"
#include "robot/position.h"
#include "robot/map.h"
#include "robot/map_manager.h"

namespace robot {

using brickred::UniquePtr;

///////////////////////////////////////////////////////////////////////////////
static void usage_actor_login()
{
    ::printf("actor_login(al) <actor_no>\n");
    ::printf("actor_login(al) <actor_no_start> <actor_no_end> "
                             "[login_count_once] [login_delay_ms]\n");
}

static void cmd_actor_login(const std::vector<std::string> &args)
{
    if (args.size() == 2) {
        int64_t actor_no = ::atol(args[1].c_str());

        Actor *actor = ActorManager::getInstance()->getActor(actor_no);
        if (NULL == actor) {
            ::printf("arg <actor_no> is invalid\n");
            return;
        }
        if (actor->logined()) {
            ::printf("actor already logined\n");
            return;
        }
        actor->login();

    } else if (args.size() == 3 || args.size() == 5) {
        int64_t actor_no_start = ::atol(args[1].c_str());
        int64_t actor_no_end = ::atol(args[2].c_str());
        int login_count_once = 0;
        int login_delay_ms = 0;

        if (args.size() == 5) {
            login_count_once = ::atoi(args[3].c_str());
            login_delay_ms = ::atoi(args[4].c_str());
        }

        int j = 0;
        int ms = 0;
        for (int64_t i = actor_no_start; i <= actor_no_end; ++i) {
            if (login_count_once && ++j >= login_count_once) {
                ms += login_delay_ms;
                j = 0;
            }
            Actor *actor = ActorManager::getInstance()->getActor(i);
            if (NULL == actor) {
                continue;
            }
            if (actor->logined()) {
                continue;
            }
            actor->login(ms);
        }

    } else {
        usage_actor_login();
    }
}

///////////////////////////////////////////////////////////////////////////////
static void usage_actor_logout()
{
    ::printf("actor_logout(ao) <actor_no>\n");
    ::printf("actor_logout(ao) <actor_no_start> <actor_no_end>\n");
}

static void cmd_actor_logout(const std::vector<std::string> &args)
{
    if (args.size() == 2) {
        int64_t actor_no = ::atol(args[1].c_str());

        Actor *actor = ActorManager::getInstance()->getActor(actor_no);
        if (NULL == actor) {
            ::printf("arg <actor_no> is invalid\n");
            return;
        }
        if (actor->logined() == false) {
            ::printf("actor not logined\n");
            return;
        }
        actor->logout();

    } else if (args.size() == 3) {
        int64_t actor_no_start = ::atol(args[1].c_str());
        int64_t actor_no_end = ::atol(args[2].c_str());

        for (int64_t i = actor_no_start; i <= actor_no_end; ++i) {
            Actor *actor = ActorManager::getInstance()->getActor(i);
            if (NULL == actor) {
                continue;
            }
            if (actor->logined() == false) {
                continue;
            }
            actor->logout();
        }

    } else {
        usage_actor_logout();
    }
}

///////////////////////////////////////////////////////////////////////////////
static void usage_actor_info()
{
    ::printf("actor_info(ai) <actor_no>\n");
}

static void cmd_actor_info(const std::vector<std::string> &args)
{
    if (args.size() != 2) {
        usage_actor_info();
        return;
    }

    int64_t actor_no = ::atol(args[1].c_str());

    Actor *actor = ActorManager::getInstance()->getActor(actor_no);
    if (NULL == actor) {
        ::printf("arg <actor_no> is invalid\n");
        return;
    }
    if (actor->logined() == false) {
        ::printf("actor not logined\n");
        return;
    }

    ::printf("actor_id: %ld\n", actor->getActorId());
    ::printf("actor_name: %s\n", actor->getActorName().c_str());
    ::printf("position: %d(%d, %d)\n", actor->getMap(),
             actor->getPos().x_, actor->getPos().y_);
    ::printf("move_speed: %d\n", actor->getMoveSpeed());
    ::printf("gold: %d\n", actor->getGold());
}

///////////////////////////////////////////////////////////////////////////////
static void usage_actor_run()
{
    ::printf("actor_run(ar) <actor_no> <action_id>\n");
    ::printf("actor_run(ar) <actor_no_start> <actor_no_end> <action_id>\n");
}

static void cmd_actor_run(const std::vector<std::string> &args)
{
    if (args.size() == 3) {
        int64_t actor_no = ::atol(args[1].c_str());
        int64_t action_id = ::atol(args[2].c_str());

        Actor *actor = ActorManager::getInstance()->getActor(actor_no);
        if (NULL == actor) {
            ::printf("arg <actor_no> is invalid\n");
            return;
        }
        if (actor->logined() == false) {
            ::printf("actor not logined\n");
            return;
        }

        ActionAutomata *automata_prototype =
            ActionAutomataManager::getInstance()->getActionAutomata(action_id);
        if (NULL == automata_prototype) {
            ::printf("arg <action_id> is invalid\n");
            return;
        }
        UniquePtr<ActionAutomata> automata(automata_prototype->clone());
        actor->runAutomata(automata.get());
        automata.release();

    } else if (args.size() == 4) {
        int64_t actor_no_start = ::atol(args[1].c_str());
        int64_t actor_no_end = ::atol(args[2].c_str());
        int64_t action_id = ::atol(args[3].c_str());

        ActionAutomata *automata_prototype =
            ActionAutomataManager::getInstance()->getActionAutomata(action_id);
        if (NULL == automata_prototype) {
            ::printf("arg <action_id> is invalid\n");
            return;
        }

        for (int64_t i = actor_no_start; i <= actor_no_end; ++i) {
            Actor *actor = ActorManager::getInstance()->getActor(i);
            if (NULL == actor) {
                continue;
            }
            if (actor->logined() == false) {
                continue;
            }
            UniquePtr<ActionAutomata> automata(automata_prototype->clone());
            actor->runAutomata(automata.get());
            automata.release();
        }

    } else {
        usage_actor_run();
    }
}

///////////////////////////////////////////////////////////////////////////////
static void usage_actor_move()
{
    ::printf("actor_move(am) <actor_no> <x> <y>\n");
}

static void cmd_actor_move(const std::vector<std::string> &args)
{
    if (args.size() == 4) {
        int64_t actor_no = ::atol(args[1].c_str());
        int x = ::atoi(args[2].c_str());
        int y = ::atoi(args[3].c_str());

        Actor *actor = ActorManager::getInstance()->getActor(actor_no);
        if (NULL == actor) {
            ::printf("arg <actor_no> is invalid\n");
            return;
        }
        if (x < 0 || y < 0) {
            ::printf("arg <x> or <y> is invalid\n");
            return;
        }
        if (actor->logined() == false) {
            ::printf("actor not logined\n");
            return;
        }

        actor->move(Position(x, y));

    } else {
        usage_actor_move();
    }
}

///////////////////////////////////////////////////////////////////////////////
static void usage_actor_sight()
{
    ::printf("actor_sight(as) <actor_no>\n");
}

static void cmd_actor_sight(const std::vector<std::string> &args)
{
    if (args.size() != 2) {
        usage_actor_sight();
        return;
    }

    int64_t actor_no = ::atol(args[1].c_str());

    Actor *actor = ActorManager::getInstance()->getActor(actor_no);
    if (NULL == actor) {
        ::printf("arg <actor_no> is invalid\n");
        return;
    }
    if (actor->logined() == false) {
        ::printf("actor not logined\n");
        return;
    }

    actor->printSight();
}

///////////////////////////////////////////////////////////////////////////////
static void usage_map_print()
{
    ::printf("map_print(mp) <map_id>\n");
}

static void cmd_map_print(const std::vector<std::string> &args)
{
    if (args.size() != 2) {
        usage_map_print();
        return;
    }

    int map_id = ::atoi(args[1].c_str());

    Map *map = MapManager::getInstance()->getMap(map_id);
    if (NULL == map) {
        ::printf("arg <map_id> is invalid\n");
        return;
    }

    map->print();
}

///////////////////////////////////////////////////////////////////////////////
static void usage_map_search()
{
    ::printf("map_search(ms) <map_id> <start_x> <start_y> <end_x> <end_y>\n");
}

static void cmd_map_search(const std::vector<std::string> &args)
{
    if (args.size() != 6) {
        usage_map_search();
        return;
    }

    int map_id = ::atoi(args[1].c_str());
    int start_x = ::atoi(args[2].c_str());
    int start_y = ::atoi(args[3].c_str());
    int end_x = ::atoi(args[4].c_str());
    int end_y = ::atoi(args[5].c_str());

    Map *map = MapManager::getInstance()->getMap(map_id);
    if (NULL == map) {
        ::printf("arg <map_id> is invalid\n");
        return;
    }

    map->printSearchPath(Position(start_x, start_y),
                         Position(end_x, end_y));
}

///////////////////////////////////////////////////////////////////////////////
static void cmd_help()
{
    usage_actor_login();
    usage_actor_logout();
    usage_actor_info();
    usage_actor_run();
    usage_actor_move();
    usage_actor_sight();
    usage_map_print();
    usage_map_search();
}

///////////////////////////////////////////////////////////////////////////////
bool UserCommandHandler::parse(const std::string &user_command)
{
    std::vector<std::string> result;
    std::vector<std::string> args;

    brickred::string_util::split(user_command.c_str(), " ", &result); 
    for (size_t i = 0; i < result.size(); ++i) {
        if (result[i].empty()) {
            continue;
        }
        args.push_back(result[i]);
    }

    if (args.empty()) {
        return true;
    }

    if ("actor_login" == args[0] || "al" == args[0]) {
        cmd_actor_login(args);
    } else if ("actor_logout" == args[0] || "ao" == args[0]) {
        cmd_actor_logout(args);
    } else if ("actor_info" == args[0] || "ai" == args[0]) {
        cmd_actor_info(args);
    } else if ("actor_run" == args[0] || "ar" == args[0]) {
        cmd_actor_run(args);
    } else if ("actor_move" == args[0] || "am" == args[0]) {
        cmd_actor_move(args);
    } else if ("actor_sight" == args[0] || "as" == args[0]) {
        cmd_actor_sight(args);
    } else if ("map_print" == args[0] || "mp" == args[0]) {
        cmd_map_print(args);
    } else if ("map_search" == args[0] || "ms" == args[0]) {
        cmd_map_search(args);
    } else if ("help" == args[0] || "h" == args[0]) {
        cmd_help();
    } else {
        return false;
    }

    return true;
}

} // namespace robot

