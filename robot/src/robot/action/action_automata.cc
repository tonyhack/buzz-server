#include "robot/action/action_automata.h"

#include <cstdlib>
#include <tinyxml2/tinyxml2.h>
#include <brickred/unique_ptr.h>

#include "robot/logging.h"
#include "robot/client_app.h"
#include "robot/actor.h"
#include "robot/action/action.h"
#include "robot/position.h"
#include "robot/ai/monster_selector.h"
#include "robot/ai/melee_skill_selector.h"
#include "robot/action/action_true.h"
#include "robot/action/action_set_variable.h"
#include "robot/action/action_check_variable.h"
#include "robot/action/action_modify_variable.h"
#include "robot/action/action_move.h"
#include "robot/action/action_rand_move.h"
#include "robot/action/action_rand_move_in_area.h"
#include "robot/action/action_approach_target.h"
#include "robot/action/action_sleep.h"
#include "robot/action/action_gm_command.h"
#include "robot/action/action_check_in_area.h"
#include "robot/action/action_attack.h"

namespace robot {

using tinyxml2::XML_NO_ERROR;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using brickred::UniquePtr;

struct SpecialStep {
    enum type {
        EXIT = -1,
        LOOP_BREAK = -2,
        LOOP_CONTINUE = -3,
        RESUME_ACTION = -4
    };
};

struct State {
    enum type {
        NOT_STARTED = 0,
        RUNNING,
        SUSPENDED,
        FINISHED
    };
};

ActionAutomata::ActionAutomata(int id) :
    id_(id), actor_(NULL), state_(State::NOT_STARTED),
    current_step_(SpecialStep::EXIT), next_step_(SpecialStep::EXIT),
    schedule_timer_id_(-1)
{
}

ActionAutomata::~ActionAutomata()
{
    cancelSchedule();

    for (size_t i = 0; i < action_steps_.size(); ++i) {
        delete action_steps_[i].action_;
    }
}

ActionAutomata *ActionAutomata::clone()
{
    UniquePtr<ActionAutomata> new_automata(new ActionAutomata(id_));

    for (size_t i = 0; i < action_steps_.size(); ++i) {
        UniquePtr<Action> action(action_steps_[i].action_->clone());

        ActionStep new_action_step;
        new_action_step.action_ = action.get();
        new_action_step.success_next_step_ =
            action_steps_[i].success_next_step_;
        new_action_step.failed_next_step_=
            action_steps_[i].failed_next_step_;

        new_automata->action_steps_.push_back(new_action_step);
        action.release();
    }
    new_automata->current_step_ = current_step_;

    return new_automata.release();
}

void ActionAutomata::setActor(Actor *actor)
{
    actor_ = actor;
    for (size_t i = 0; i < action_steps_.size(); ++i) {
        action_steps_[i].action_->setActor(actor_);
    }
}

void ActionAutomata::setCompleteCallback(CompleteCallback complete_cb)
{
    complete_cb_ = complete_cb;
}

static Action *buildActionMove(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("x");
    if (NULL == attr) {
        LOG_ERROR("attr x not found");
        return NULL;
    }
    int x = ::atoi(attr);

    attr = node->Attribute("y");
    if (NULL == attr) {
        LOG_ERROR("attr y not found");
        return NULL;
    }
    int y = ::atoi(attr);

    if (x < 0 || y < 0) {
        LOG_ERROR("invalid params");
        return NULL;
    }

    return new ActionMove(Position(x, y));
}

static Action *buildActionSetVariable(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("name");
    if (NULL == attr) {
        LOG_ERROR("attr name not found");
        return NULL;
    }
    std::string name = attr;

    attr = node->Attribute("value");
    if (NULL == attr) {
        LOG_ERROR("attr value not found");
        return NULL;
    }
    std::string value = attr;

    return new ActionSetVariable(name, value);
}

static Action *buildActionCheckVariable(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("name");
    if (NULL == attr) {
        LOG_ERROR("attr name not found");
        return NULL;
    }
    std::string name = attr;

    attr = node->Attribute("op");
    if (NULL == attr) {
        LOG_ERROR("attr op not found");
        return NULL;
    }
    std::string op = attr;

    attr = node->Attribute("value");
    if (NULL == attr) {
        LOG_ERROR("attr value not found");
        return NULL;
    }
    std::string value = attr;

    return new ActionCheckVariable(name, op, value);
}

static Action *buildActionModifyVariable(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("name");
    if (NULL == attr) {
        LOG_ERROR("attr name not found");
        return NULL;
    }
    std::string name = attr;

    attr = node->Attribute("op");
    if (NULL == attr) {
        LOG_ERROR("attr op not found");
        return NULL;
    }
    std::string op = attr;

    attr = node->Attribute("value");
    if (NULL == attr) {
        LOG_ERROR("attr value not found");
        return NULL;
    }
    std::string value = attr;

    return new ActionModifyVariable(name, op, value);
}

static Action *buildActionRandMoveInArea(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("x");
    if (NULL == attr) {
        LOG_ERROR("attr x not found");
        return NULL;
    }
    int x = ::atoi(attr);

    attr = node->Attribute("y");
    if (NULL == attr) {
        LOG_ERROR("attr y not found");
        return NULL;
    }
    int y = ::atoi(attr);

    attr = node->Attribute("width");
    if (NULL == attr) {
        LOG_ERROR("attr width not found");
        return NULL;
    }
    int width = ::atoi(attr);

    attr = node->Attribute("height");
    if (NULL == attr) {
        LOG_ERROR("attr height not found");
        return NULL;
    }
    int height = ::atoi(attr);

    return new ActionRandMoveInArea(Position(x, y), width, height);
}

static Action *buildActionApproachTarget(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("target_id");
    if (NULL == attr) {
        LOG_ERROR("attr target_id not found");
        return NULL;
    }
    int64_t target_id = ::atol(attr);

    attr = node->Attribute("distance");
    if (NULL == attr) {
        LOG_ERROR("attr distance not found");
        return NULL;
    }
    int distance = ::atoi(attr);

    return new ActionApproachTarget(target_id, distance);
}

static Action *buildActionSleep(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("ms");
    if (NULL == attr) {
        LOG_ERROR("attr ms not found");
        return NULL;
    }
    int ms = ::atoi(attr);

    if (ms < 0) {
        LOG_ERROR("invalid params");
        return NULL;
    }

    return new ActionSleep(ms);
}

static Action *buildActionRandSleep(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("min_ms");
    if (NULL == attr) {
        LOG_ERROR("attr min not found");
        return NULL;
    }
    int min_ms = ::atoi(attr);

    attr = node->Attribute("max_ms");
    if (NULL == attr) {
        LOG_ERROR("attr max not found");
        return NULL;
    }
    int max_ms = ::atoi(attr);

    return new ActionSleep(ClientApp::getInstance()->uniformRand(min_ms, max_ms));
}

static Action *buildActionGmCommand(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("command");
    if (NULL == attr) {
        LOG_ERROR("attr command not found");
        return NULL;
    }
    std::string command = attr;

    return new ActionGmCommand(command);
}

static Action *buildActionCheckInArea(XMLElement *node)
{
    const char *attr = NULL;

    attr = node->Attribute("x");
    if (NULL == attr) {
        LOG_ERROR("attr x not found");
        return NULL;
    }
    int x = ::atoi(attr);

    attr = node->Attribute("y");
    if (NULL == attr) {
        LOG_ERROR("attr y not found");
        return NULL;
    }
    int y = ::atoi(attr);

    attr = node->Attribute("width");
    if (NULL == attr) {
        LOG_ERROR("attr width not found");
        return NULL;
    }
    int width = ::atoi(attr);

    attr = node->Attribute("height");
    if (NULL == attr) {
        LOG_ERROR("attr height not found");
        return NULL;
    }
    int height = ::atoi(attr);

    return new ActionCheckInArea(Position(x, y), width, height);
}

static Action *buildActionAttackMonster(XMLElement *node) 
{
    const char *attr = NULL;

    attr = node->Attribute("id");
    if (NULL == attr) {
        LOG_ERROR("attr id not found");
        return NULL;
    }
    int64_t template_id = ::atol(attr);

    UniquePtr<MonsterSelector> target_selector(
        new MonsterSelector(template_id));
    UniquePtr<MeleeSkillSelector> skill_selector(
        new MeleeSkillSelector());
    UniquePtr<ActionAttack> action_attack(
        new ActionAttack(target_selector.get(), skill_selector.get()));
    target_selector.release();
    skill_selector.release();

    ActionAttack *raw_action_attack = action_attack.release();
    return raw_action_attack;
}

static Action *buildAction(XMLElement *node)
{
    std::string node_name(node->Name());

    if ("true" == node_name) {
        return new ActionTrue(); 
    } else if ("set_var" == node_name) {
        return buildActionSetVariable(node);
    } else if ("check_var" == node_name) {
        return buildActionCheckVariable(node);
    } else if ("modify_var" == node_name) {
        return buildActionModifyVariable(node);
    } else if ("move" == node_name) {
        return buildActionMove(node);
    } else if ("rand_move" == node_name) {
        return new ActionRandMove();
    } else if ("rand_move_in_area" == node_name) {
        return buildActionRandMoveInArea(node);
    } else if ("approach" == node_name) {
        return buildActionApproachTarget(node);
    } else if ("sleep" == node_name) {
        return buildActionSleep(node);
    } else if ("rand_sleep" == node_name) {
        return buildActionRandSleep(node);
    } else if ("gm" == node_name) {
        return buildActionGmCommand(node);
    } else if ("in_area" == node_name) {
        return buildActionCheckInArea(node);
    } else if ("attack_monster" == node_name) {
        return buildActionAttackMonster(node);
    } else {
        LOG_ERROR("invalid node %s", node->Name());
        return NULL;
    }
}

// forward declare
static bool buildBlock(ActionAutomata::ActionStepVector &action_steps,
    const std::string &parent_node_path, XMLElement *node_start);

static bool buildIfBlock(ActionAutomata::ActionStepVector &action_steps,
    const std::string &if_node_path, XMLElement *if_node)
{
    // get cond node
    XMLElement *cond_node = if_node->FirstChildElement();
    if (NULL == cond_node) {
        LOG_ERROR("expect <cond> or <not_cond>");
        return false;
    }
    
    bool not_cond = false;
    if (cond_node->Name() == std::string("cond")) {
        not_cond = false;
    } else if (cond_node->Name() == std::string("not_cond")) {
        not_cond = true;
    } else {
        LOG_ERROR("expect <cond> or <not_cond>");
        return false;
    }

    if (cond_node->NoChildren()) {
        LOG_ERROR("expect not empty <cond> or <not_cond>");
        return false;
    }

    int cond_step_index = -1;
    int then_start_step_index = -1;
    int then_end_step_index = -1;
    int else_start_step_index = -1;
    int else_end_step_index = -1;

    // insert cond action
    {
        UniquePtr<Action> action(buildAction(cond_node->FirstChildElement()));
        if (!action) {
            LOG_ERROR("build cond action failed");
            return false;
        }

        ActionAutomata::ActionStep action_step;
        action_step.action_ = action.get();
        action_steps.push_back(action_step);
        action.release();
        cond_step_index = action_steps.size() - 1;
    }

    // get then block
    XMLElement *then_node = cond_node->NextSiblingElement();
    if (NULL == then_node || then_node->Name() != std::string("then")) {
        LOG_ERROR("expect <then>");
        return false;
    }

    {
        if (!then_node->NoChildren()) {
            if (buildBlock(action_steps, if_node_path + "/then",
                           then_node->FirstChildElement()) == false) {
                LOG_ERROR("build then block failed");
                return false;
            }
        }

        // insert dummy jump point
        UniquePtr<Action> action(new ActionTrue());
        ActionAutomata::ActionStep action_step;
        action_step.action_ = action.get();
        action_steps.push_back(action_step);
        action.release();

        then_start_step_index = cond_step_index + 1;
        then_end_step_index = action_steps.size() - 1;
    }

    // get else block
    XMLElement *else_node = then_node->NextSiblingElement();
    if (else_node != NULL) {
        if (std::string("else") != then_node->Name()) {
            LOG_ERROR("except <else>");
            return false;
        }

        if (!else_node->NoChildren()) {
            if (buildBlock(action_steps, if_node_path + "/else",
                           then_node->FirstChildElement()) == false) {
                LOG_ERROR("build else block failed");
                return false;
            }
        } else {
            UniquePtr<Action> action(new ActionTrue());

            ActionAutomata::ActionStep action_step;
            action_step.action_ = action.get();
            action_steps.push_back(action_step);
            action.release();
        }
        else_start_step_index = then_end_step_index + 1;
        else_end_step_index = action_steps.size() - 1;
    } else {
        else_start_step_index = then_end_step_index + 1;
        else_end_step_index = then_end_step_index;
    }

    // set jump point
    if (!not_cond) {
        action_steps[cond_step_index].success_next_step_ =
            then_start_step_index;
        action_steps[cond_step_index].failed_next_step_ =
            else_start_step_index;
    } else {
        action_steps[cond_step_index].success_next_step_ =
            else_start_step_index;
        action_steps[cond_step_index].failed_next_step_ =
            then_start_step_index;
    }

    action_steps[then_end_step_index].success_next_step_ =
        else_end_step_index + 1;
    action_steps[then_end_step_index].failed_next_step_ =
        else_end_step_index + 1;

    return true;
}

static bool buildWhileBlock(ActionAutomata::ActionStepVector &action_steps,
    const std::string &while_node_path, XMLElement *while_node)
{
    // get cond node
    XMLElement *cond_node = while_node->FirstChildElement();
    if (NULL == cond_node) {
        LOG_ERROR("expect <cond> or <not_cond>");
        return false;
    }
    
    bool not_cond = false;
    if (cond_node->Name() == std::string("cond")) {
        not_cond = false;
    } else if (cond_node->Name() == std::string("not_cond")) {
        not_cond = true;
    } else {
        LOG_ERROR("expect <cond> or <not_cond>");
        return false;
    }

    if (cond_node->NoChildren()) {
        LOG_ERROR("expect not empty <cond> or <not_cond>");
        return false;
    }

    int cond_step_index = -1;
    int loop_start_step_index = -1;
    int loop_end_step_index = -1;

    // insert cond action
    {
        UniquePtr<Action> action(buildAction(cond_node->FirstChildElement()));
        if (!action) {
            LOG_ERROR("build cond action failed");
            return false;
        }

        ActionAutomata::ActionStep action_step;
        action_step.action_ = action.get();
        action_steps.push_back(action_step);
        action.release();
        cond_step_index = action_steps.size() - 1;
    }

    // get while block
    {
        if (cond_node->NextSiblingElement() != NULL) {
            if (buildBlock(action_steps, while_node_path,
                           cond_node->NextSiblingElement()) == false) {
                LOG_ERROR("build while block failed");
                return false;
            }
        }

        // insert dummy jump point
        UniquePtr<Action> action(new ActionTrue());
        ActionAutomata::ActionStep action_step;
        action_step.action_ = action.get();
        action_steps.push_back(action_step);
        action.release();

        loop_start_step_index = cond_step_index + 1;
        loop_end_step_index = action_steps.size() - 1;
    }

    // set jump point
    if (!not_cond) {
        action_steps[cond_step_index].success_next_step_ =
            loop_start_step_index;
        action_steps[cond_step_index].failed_next_step_ =
            loop_end_step_index + 1;
    } else {
        action_steps[cond_step_index].success_next_step_ =
            loop_end_step_index + 1;
        action_steps[cond_step_index].failed_next_step_ =
            loop_start_step_index;
    }

    action_steps[loop_end_step_index].success_next_step_ =
        cond_step_index;
    action_steps[loop_end_step_index].failed_next_step_ =
        cond_step_index;

    for (int i = cond_step_index + 1; i < loop_end_step_index; ++i) {
        if (SpecialStep::LOOP_CONTINUE == action_steps[i].success_next_step_) {
            action_steps[i].success_next_step_ = cond_step_index;
        }
        if (SpecialStep::LOOP_CONTINUE == action_steps[i].failed_next_step_) {
            action_steps[i].failed_next_step_= cond_step_index;
        }
        if (SpecialStep::LOOP_BREAK == action_steps[i].success_next_step_) {
            action_steps[i].success_next_step_ = loop_end_step_index + 1;
        }
        if (SpecialStep::LOOP_BREAK == action_steps[i].failed_next_step_) {
            action_steps[i].failed_next_step_ = loop_end_step_index + 1;
        }
    }

    return true;
}

static bool buildBlock(ActionAutomata::ActionStepVector &action_steps,
    const std::string &parent_node_path, XMLElement *node_start)
{

    for (XMLElement *node = node_start;
         node != NULL;
         node = node->NextSiblingElement()) {
        std::string node_name(node->Name());
        std::string node_path = parent_node_path + "/" + node_name;

        if ("while" == node_name) {
            if (buildWhileBlock(action_steps, node_path, node) == false) {
                LOG_ERROR("build %s failed", node_path.c_str());
                return false;
            }
        } else if ("if" == node_name) {
            if (buildIfBlock(action_steps, node_path, node) == false) {
                LOG_ERROR("build %s failed", node_path.c_str());
                return false;
            }
        } else {
            ActionAutomata::ActionStep action_step;
            UniquePtr<Action> action;

            if ("exit" == node_name) {
                action.reset(new ActionTrue());
                action_step.success_next_step_ = SpecialStep::EXIT;
                action_step.failed_next_step_ = SpecialStep::EXIT;
            } else if ("break" == node_name) {
                action.reset(new ActionTrue());
                action_step.success_next_step_ = SpecialStep::LOOP_BREAK;
                action_step.failed_next_step_ = SpecialStep::LOOP_BREAK;
            } else if ("continue" == node_name) {
                action.reset(new ActionTrue());
                action_step.success_next_step_ = SpecialStep::LOOP_CONTINUE;
                action_step.failed_next_step_ = SpecialStep::LOOP_CONTINUE;
            } else {
                action.reset(buildAction(node));
                if (!action) {
                    LOG_ERROR("build %s failed", node_path.c_str());
                    return false;
                }
                action_step.success_next_step_ = action_steps.size() + 1;
                action_step.failed_next_step_ = action_steps.size() + 1;
            }

            action_step.action_ = action.get();
            action_steps.push_back(action_step);
            action.release();
        }
    }

    return true;
}

bool ActionAutomata::load(const std::string &file)
{
    XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != XML_NO_ERROR) {
        LOG_ERROR("load config file(%s) failed", file.c_str());
        return false;
    }

    XMLElement *action_node = doc.FirstChildElement("action");
    if (NULL == action_node) {
        LOG_ERROR("/action not found");
        return false;
    }

    if (buildBlock(action_steps_, "/action",
                   action_node->FirstChildElement()) == false) {
        LOG_ERROR("build /action failed");
        return false;
    }

    if (action_steps_.empty()) {
        current_step_ = SpecialStep::EXIT;
    } else {
        current_step_ = 0;
    }

    for (size_t i = 0; i < action_steps_.size(); ++i) {
        if (action_steps_[i].success_next_step_ == (int)action_steps_.size()) {
            action_steps_[i].success_next_step_ = SpecialStep::EXIT;
        }
        if (action_steps_[i].failed_next_step_ == (int)action_steps_.size()) {
            action_steps_[i].failed_next_step_ = SpecialStep::EXIT;
        }
    }

    return true;
}

void ActionAutomata::start()
{
    if (state_ != State::NOT_STARTED) {
        return;
    }
    state_ = State::RUNNING;

    if (SpecialStep::EXIT == current_step_) {
        return finish();
    }

    for (size_t i = 0; i < action_steps_.size(); ++i) {
        action_steps_[i].action_->setCompleteCallback(
            BRICKRED_BIND_MEM_FUNC(&ActionAutomata::onActionComplete, this));
    }

    Action *action = action_steps_[current_step_].action_;
    action->start();
}

void ActionAutomata::finish(bool success)
{
    state_ = State::FINISHED;

    if (complete_cb_) {
        complete_cb_(success);
    }
}

void ActionAutomata::suspend()
{
    if (state_ != State::RUNNING) {
        return;
    }
    state_ = State::SUSPENDED;

    // action is running
    if (SpecialStep::EXIT == next_step_) {
        action_steps_[current_step_].action_->suspend();
        next_step_ = SpecialStep::RESUME_ACTION;
    }

    cancelSchedule();
}

void ActionAutomata::resume()
{
    if (state_ != State::SUSPENDED) {
        return;
    }
    state_ = State::RUNNING;

    schedule();
}

void ActionAutomata::onActionComplete(bool success)
{
    next_step_ = success ? action_steps_[current_step_].success_next_step_ :
                           action_steps_[current_step_].failed_next_step_;
    schedule();
}

void ActionAutomata::schedule()
{
    // use a zero time timer to avoid callback overflow
    schedule_timer_id_ = ClientApp::getInstance()->startTimer(0,
        BRICKRED_BIND_MEM_FUNC(&ActionAutomata::onScheduleTimer, this), 1);
}

void ActionAutomata::cancelSchedule()
{
    if (schedule_timer_id_ != -1) {
        ClientApp::getInstance()->stopTimer(schedule_timer_id_);
        schedule_timer_id_ = -1;
    }
}

void ActionAutomata::onScheduleTimer(int64_t timer_id)
{
    schedule_timer_id_ = -1;

    if (SpecialStep::EXIT == next_step_) {
        action_steps_[current_step_].action_->reset();
        current_step_ = SpecialStep::EXIT;
        return finish();
    } else if (SpecialStep::RESUME_ACTION == next_step_) {
        action_steps_[current_step_].action_->resume();
    } else {
        action_steps_[current_step_].action_->reset();
        current_step_ = next_step_;
        next_step_ = SpecialStep::EXIT;
        action_steps_[current_step_].action_->start();
    }
}

} // namespace robot

